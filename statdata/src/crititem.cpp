/*
 * crititem.cpp
 *
 *  Created on: 9 mai 2016
 *      Author: boubad
 */
#include "../include/crititem.h"
#include "../include/indiv.h"
////////////////////////////////
#include <boost/assert.hpp>
///////////////////////////////
namespace info {
/////////////////////////////////////
extern void info_global_compute_distances(IIndivProvider *pProvider,
		IndivDistanceMap &oDistances) {
	BOOST_ASSERT(pProvider != nullptr);
	BOOST_ASSERT(pProvider->is_valid());
	size_t nCount = 0;
	bool bRet = pProvider->indivs_count(nCount);
	BOOST_ASSERT(bRet);
	oDistances.clear();
	VariableMode mode = VariableMode::modeNumeric;
#pragma omp parallel for
	for (size_t i = 0; i < nCount; ++i) {
		Indiv oInd1;
		if (pProvider->find_indiv_at(i, oInd1, mode)) {
			const IntType aIndex1 = oInd1.id();
			for (size_t j = 0; j < i; ++j) {
				Indiv oInd2;
				if (pProvider->find_indiv_at(j, oInd2, mode)) {
					const IntType aIndex2 = oInd2.id();
					double v = oInd1.distance(oInd2);
					oDistances.add(aIndex1, aIndex2, v);
				}
			} // j
		} //
	} // i
} //info_global_compute_distances
//////////////////////////////////////
IndivDistanceMap::IndivDistanceMap() {

}
IndivDistanceMap::~IndivDistanceMap() {

}
void IndivDistanceMap::clear(void) {
#if defined(__CYGWIN__)
	std::lock_guard<std::mutex> oLock(this->_mutex);
#else
	boost::mutex::scoped_lock oLock(this->_mutex);
#endif // __CYGWIN__
	this->m_data.clear();
	this->m_set.clear();
} // clear
void IndivDistanceMap::add(const IntType i1, const IntType i2, const double v) {
	BOOST_ASSERT(v >= 0);
	IntType aIndex1 = i1, aIndex2 = i2;
	if (i1 > i2) {
		const IntType t = aIndex1;
		aIndex1 = aIndex2;
		aIndex2 = t;
	}
	{
#if defined(__CYGWIN__)
		std::lock_guard<std::mutex> oLock(this->_mutex);
#else
		boost::mutex::scoped_lock oLock(this->_mutex);
#endif // __CYGWIN__
		intdoubles_map_map &data = this->m_data;
		ints_set &oSet = this->m_set;
		auto it = data.find(aIndex1);
		if (it == data.end()) {
			intdoubles_map o;
			o[aIndex2] = v;
			data[aIndex1] = o;
		} else {
			intdoubles_map &m = (*it).second;
			m[aIndex2] = v;
		}
		if (oSet.find(aIndex1) == oSet.end()) {
			oSet.insert(aIndex1);
		}
		if (oSet.find(aIndex2) == oSet.end()) {
			oSet.insert(aIndex2);
		}
	} // sync
} // add
const ints_set & IndivDistanceMap::indexes(void) const {
	return (this->m_set);
} // indexes
void IndivDistanceMap::add(const CritItem &item) {
	size_t i1, i2;
	double v;
	item.get(i1, i2, v);
	this->add(i1, i2, v);
}
bool IndivDistanceMap::get(const IntType i1, const IntType i2,
		double &v) const {
	IntType aIndex1 = i1, aIndex2 = i2;
	if (i1 > i2) {
		const IntType t = aIndex1;
		aIndex1 = aIndex2;
		aIndex2 = t;
	}
	const intdoubles_map_map &data = this->m_data;
	auto it = data.find(aIndex1);
	if (it != data.end()) {
		const intdoubles_map &m = (*it).second;
		auto jt = m.find(aIndex2);
		if (jt != m.end()) {
			v = (*jt).second;
			return (true);
		}
	}
	return (false);
} // get
bool IndivDistanceMap::get(CritItem &item) const {
	size_t i1, i2;
	double v;
	item.get(i1, i2, v);
	if (this->get(i1, i2, v)) {
		item.value(v);
		return (true);
	}
	return (false);
} // get
////////////////////////////////
extern size_t info_global_generate_crititems(const size_t n,
		crititems_vector &oRes) {
	BOOST_ASSERT(n > 1);
	oRes.clear();
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < i; ++j) {
			oRes.push_back(CritItem(j, i));
		} // j
	} // i
	return (oRes.size());
} //info_global_generate_crititems
/////////////////////////////////
CritItem::CritItem() :
		m_first(0), m_second(0), m_crit(0) {
}
CritItem::CritItem(const size_t i1, const size_t i2, const double v /*= 0*/) :
		m_first(i1), m_second(i2), m_crit(v) {
	BOOST_ASSERT(this->m_crit >= 0);
	if (this->m_first > this->m_second) {
		const IntType t = this->m_first;
		this->m_first = this->m_second;
		this->m_second = t;
	}
}
CritItem::CritItem(const CritItem & other) :
		m_first(other.m_first), m_second(other.m_second), m_crit(other.m_crit) {
}
CritItem & CritItem::operator=(const CritItem &other) {
	if (this != &other) {
		this->m_first = other.m_first;
		this->m_second = other.m_second;
		this->m_crit = other.m_crit;
	}
	return (*this);
}
CritItem::~CritItem() {
}
double CritItem::value(void) const {
	return (this->m_crit);
}
void CritItem::value(const double d) {
	if (d >= 0) {
		this->m_crit = d;
	}
}
void CritItem::get(std::pair<size_t, size_t> &oPair) const {
	oPair.first = this->m_first;
	oPair.second = this->m_second;
}
void CritItem::get(size_t &i1, size_t &i2, double &v) const {
	i1 = this->m_first;
	i2 = this->m_second;
	v = this->m_crit;
}
bool CritItem::is_valid(void) const {
	return (this->m_first < this->m_second);
}
bool CritItem::operator==(const CritItem &other) const {
	return ((this->m_first == other.m_first)
			&& (this->m_second == other.m_second));
}
bool CritItem::operator<(const CritItem &other) const {
	if (this->m_first < other.m_first) {
		return (true);
	} else if (this->m_first > other.m_first) {
		return (false);
	}
	return (this->m_second < other.m_second);
}
CritItem & CritItem::operator+=(const CritItem &other) {
	if ((!this->is_valid()) && other.is_valid()) {
		*this = other;
	} else if (this->is_valid() && other.is_valid()) {
		if (other.m_crit < this->m_crit) {
			*this = other;
		}
	}
	return (*this);
} // operator+=
const CritItem CritItem::operator+(const CritItem &other) const {
	CritItem r(*this);
	r += other;
	return r;
}
void CritItem::swap(CritItem &other) {
	CritItem t(*this);
	*this = other;
	other = *this;
}
//////////////////////////////////
}// namespace info

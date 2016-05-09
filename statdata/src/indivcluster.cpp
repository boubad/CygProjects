/*
 * indivcluster.cpp
 *
 *  Created on: 9 mai 2016
 *      Author: boubad
 */
#include "../include/indivcluster.h"
////////////////////////////////
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
///////////////////////////////
namespace info {
/*
 * size_t	   m_index;
 IIndivProvider *m_provider;
 ints_deque m_individs;
 DbValueMap m_center;
 */
///////////////////////////////////
IndivCluster::IndivCluster(IIndivProvider *pProvider, const size_t aIndex) :
		m_index(aIndex), m_provider(pProvider) {
}
IndivCluster::IndivCluster(const IndivCluster &other) :
		m_index(other.m_index), m_provider(other.m_provider), m_individs(
				other.m_individs), m_center(other.m_center) {

}
IndivCluster & IndivCluster::operator=(const IndivCluster &other) {
	if (this != &other) {
		this->m_index = other.m_index;
		this->m_provider = other.m_provider;
		this->m_individs = other.m_individs;
		this->m_center = other.m_center;
	}
	return (*this);
}
IndivCluster::~IndivCluster() {
}
bool IndivCluster::is_valid(void) const {
	return ((this->m_provider != nullptr) && (this->m_provider->is_valid()));
}
bool IndivCluster::is_empty(void) const {
	return (this->m_individs.empty());
}
IIndivProvider *IndivCluster::provider(void) const {
	return (this->m_provider);
}
void IndivCluster::provider(IIndivProvider *pProvider) {
	this->m_provider = pProvider;
}
void IndivCluster::index(const size_t n) {
	this->m_index = n;
}
size_t IndivCluster::index(void) const {
	return (this->m_index);
}
const ints_deque & IndivCluster::members(void) const {
	return (this->m_individs);
}
const DbValueMap &IndivCluster::center(void) const {
	return (this->m_center);
}
double IndivCluster::distance(const Indiv &oInd) const {
	const DbValueMap &m1 = this->m_center;
	const DbValueMap &m2 = oInd.data();
	double dRet = 0;
	size_t nc = 0;
	typedef std::pair<IntType, DbValue> MyPair;
	BOOST_FOREACH(const MyPair &oPair, m1){
	const IntType key = oPair.first;
	auto jt = m2.find(key);
	if (jt != m2.end()) {
		const DbValue &v1 = oPair.second;
		const DbValue &v2 = (*jt).second;
		double t = v1.double_value() - v2.double_value();
		dRet += t * t;
		++nc;
	} // found
} // oPair
	if (nc > 1) {
		dRet /= nc;
	}
	return dRet;
}
void IndivCluster::add(const Indiv &oInd) {
	const IntType aIndex = oInd.id();
	ints_deque &vv = this->m_individs;
	for (auto it = vv.begin(); it != vv.end(); ++it) {
		if ((*it) == aIndex) {
			return;
		}
	} // it
	vv.push_back(aIndex);
} // add
double IndivCluster::distance(const IntType aIndex, ClusterDistanceMode &mode) {
	BOOST_ASSERT(this->is_valid());
	IIndivProvider *pProvider = this->m_provider;
	Indiv oInd;
	if (!pProvider->find_indiv(aIndex, oInd, VariableMode::modeNumeric)) {
		BOOST_ASSERT(false);
		return (false);
	}
	return this->distance(oInd, mode);
} // distance
double IndivCluster::distance(const Indiv &oInd, ClusterDistanceMode &mode) {
	BOOST_ASSERT(this->is_valid());
	IIndivProvider *pProvider = this->m_provider;
	ints_deque &vv = this->m_individs;
	const size_t n = vv.size();
	if (n < 1) {
		return (this->distance(oInd));
	}
	if (n == 1) {
		Indiv oInd2;
		if (!pProvider->find_indiv(vv[0], oInd2, VariableMode::modeNumeric)) {
			mode = ClusterDistanceMode::modeInvalid;
			BOOST_ASSERT(false);
			return (false);
		}
		return (oInd.distance(oInd2));
	}
	if (mode == ClusterDistanceMode::modeTop) {
		Indiv oInd2;
		if (!pProvider->find_indiv(vv[0], oInd2, VariableMode::modeNumeric)) {
			mode = ClusterDistanceMode::modeInvalid;
			BOOST_ASSERT(false);
			return (false);
		}
		return (oInd.distance(oInd2));
	} else if (mode == ClusterDistanceMode::modeBottom) {
		Indiv oInd2;
		if (!pProvider->find_indiv(vv[n - 1], oInd2,
				VariableMode::modeNumeric)) {
			mode = ClusterDistanceMode::modeInvalid;
			BOOST_ASSERT(false);
			return (false);
		}
		return (oInd.distance(oInd2));
	}
	mode = ClusterDistanceMode::modeInvalid;
	return (0);
} // distance
double IndivCluster::min_distance(const IndivCluster &other,
		ClusterAppendMode &mode) {

} // min_distance
void IndivCluster::add(const IndivCluster &other,
	const ClusterAppendMode mode) {

}
void IndivCluster::clear_members(void) {

}
void IndivCluster::update_center(void) {

}

///////////////////////////////////////
}// namespace info

/*
 * indivcluster.cpp
 *
 *  Created on: 9 mai 2016
 *      Author: boubad
 */
#include "../include/indivcluster.h"
////////////////////////////////
namespace info {
///////////////////////////////////
IndivCluster::IndivCluster() :
		m_mustdelete(false), m_index(0), m_provider(nullptr), m_pdist(nullptr) {

}
IndivCluster::IndivCluster(IndivDistanceMap *pDist, const size_t aIndex) :
		m_mustdelete(false), m_index(aIndex), m_provider(nullptr), m_pdist(
				pDist) {

}
IndivCluster::IndivCluster(IIndivProvider *pProvider, const size_t aIndex) :
		m_mustdelete(false), m_index(aIndex), m_provider(pProvider), m_pdist(
				nullptr) {
}
IndivCluster::IndivCluster(const IndivCluster &other) :
		m_mustdelete(false), m_index(other.m_index), m_provider(
				other.m_provider), m_pdist(nullptr), m_individs(
				other.m_individs), m_center(other.m_center) {
		if (!other.m_mustdelete){
			this->m_pdist = other.m_pdist;
		}
}
IndivCluster & IndivCluster::operator=(const IndivCluster &other) {
	if (this != &other) {
		if (this->m_mustdelete) {
			delete this->m_pdist;
			this->m_pdist = nullptr;
			this->m_mustdelete = false;
		}
		this->m_index = other.m_index;
		this->m_provider = other.m_provider;
		if (!other.m_mustdelete) {
			this->m_pdist = other.m_pdist;
		}
		this->m_individs = other.m_individs;
		this->m_center = other.m_center;
	}
	return (*this);
}
void IndivCluster::swap(IndivCluster &other){
	IndivCluster t(*this);
	*this = other;
	other = t;
}// swap
IndivCluster::~IndivCluster() {
	if (this->m_mustdelete) {
		delete this->m_pdist;
		this->m_pdist = nullptr;
		this->m_mustdelete = false;
	}
}
bool IndivCluster::is_valid(void) const {
	if (m_pdist != nullptr) {
		return (true);
	}
	return ((this->m_provider != nullptr) && (this->m_provider->is_valid()));
}
bool IndivCluster::get_distance(const IntType aIndex1, const IntType &aIndex2,
		double &dRes) {
	if (this->m_pdist != nullptr) {
		if (this->m_pdist->get(aIndex1, aIndex2, dRes)) {
			return (true);
		}
	}
	IIndivProvider *pProvider = this->m_provider;
	if (pProvider == nullptr) {
		return (false);
	}
	Indiv oInd1;
	if (!pProvider->find_indiv(aIndex1, oInd1, VariableMode::modeNumeric)) {
		return (false);
	}
	Indiv oInd2;
	if (!pProvider->find_indiv(aIndex2, oInd2, VariableMode::modeNumeric)) {
		return (false);
	}
	dRes = oInd1.distance(oInd2);
	if (this->m_pdist == nullptr) {
		this->m_pdist = new IndivDistanceMap();
		BOOST_ASSERT(this->m_pdist != nullptr);
		this->m_mustdelete = true;
	}
	this->m_pdist->add(aIndex1, aIndex2, dRes);
	return (true);
} //get_distanc
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
	BOOST_FOREACH(const MyPair &oPair, m1) {
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
bool IndivCluster::min_distance(const IndivCluster &other, double &dRes,
		ClusterAppendMode &mode) {
	BOOST_ASSERT(this->is_valid());
	ints_deque &vv1 = this->m_individs;
	const size_t n1 = vv1.size();
	const ints_deque &vv2 = other.m_individs;
	const size_t n2 = vv2.size();
	if ((n1 < 1) || (n2 < 1)) {
		return (false);
	} else if ((n1 == 1) && (n2 == 1)) {
		mode = ClusterAppendMode::modeTopBottom;
		return (this->get_distance(vv1[0], vv2[0], dRes));
	} else if ((n1 == 1) && (n2 > 1)) {
		double d1, d2;
		if (!this->get_distance(vv1[0], vv2[0], d1)) {
			return (false);
		}
		if (!this->get_distance(vv1[0], vv2[n2 - 1], d2)) {
			return (false);
		}
		if (d1 < d2) {
			mode = ClusterAppendMode::modeTopTop;
			dRes = d1;
		} else {
			mode = ClusterAppendMode::modeTopBottom;
			dRes = d2;
		}
		return (true);
	} else if ((n1 > 1) && (n2 == 1)) {
		double d1, d2;
		if (!this->get_distance(vv1[0], vv2[0], d1)) {
			return (false);
		}
		if (!this->get_distance(vv1[n1 - 1], vv2[0], d2)) {
			return (false);
		}
		if (d1 < d2) {
			mode = ClusterAppendMode::modeTopTop;
			dRes = d1;
		} else {
			mode = ClusterAppendMode::modeBottomTop;
			dRes = d2;
		}
		return (true);
	} else {
		if (!this->get_distance(vv1[0], vv2[0], dRes)) {
			return (false);
		}
		mode = ClusterAppendMode::modeTopTop;
		double d;
		if (!this->get_distance(vv1[0], vv2[n2 - 1], d)) {
			return (false);
		}
		if (d < dRes) {
			dRes = d;
			mode = ClusterAppendMode::modeTopBottom;
		}
		if (!this->get_distance(vv1[n1 - 1], vv2[0], d)) {
			return (false);
		}
		if (d < dRes) {
			dRes = d;
			mode = ClusterAppendMode::modeBottomTop;
		}
		if (!this->get_distance(vv1[n1 - 1], vv2[n2 - 1], d)) {
			return (false);
		}
		if (d < dRes) {
			dRes = d;
			mode = ClusterAppendMode::modeBottomBottom;
		}
		return (true);
	}
} // min_distance
void IndivCluster::add(const IndivCluster &other,
		const ClusterAppendMode mode) {
	ints_deque &dest = this->m_individs;
	const size_t n1 = dest.size();
	const ints_deque &src = other.m_individs;
	const size_t n2 = src.size();
	if ((n1 < 1) || (n2 < 1)) {
		return;
	}
	switch (mode) {
	case ClusterAppendMode::modeTopBottom: {
		for (size_t i = (size_t)(n2-1); i >= 0; i--) {
			dest.push_front(src[i]);
			if (i == 0) {
				break;
			}
		}
	}
		break;
	case ClusterAppendMode::modeTopTop: {
		for (size_t i = 0; i < n2; ++i) {
			dest.push_front(src[i]);
		}
	}
		break;
	case ClusterAppendMode::modeBottomTop: {
		for (size_t i = 0; i < n2; ++i) {
			dest.push_back(src[i]);
		}
	}
		break;
	case ClusterAppendMode::modeBottomBottom: {
		for (size_t i = (size_t)(n2 - 1); i >= 0; i--) {
			dest.push_back(src[i]);
			if (i == 0) {
				break;
			}
		}
	}
		break;
	default:
		break;
	} //mode
} //add
void IndivCluster::clear_members(void) {
	this->m_individs.clear();
} //clear_members
void IndivCluster::update_center(void) {
	IIndivProvider *pProvider = this->m_provider;
	if (pProvider == nullptr) {
		return;
	}
	typedef std::pair<IntType, DbValue> MyPair;
	typedef std::pair<IntType, size_t> MyPair2;
	boost::container::flat_map<IntType, size_t> counts;
	boost::container::flat_map<IntType, double> sommes;
	const ints_deque &vv = this->m_individs;
	BOOST_FOREACH(IntType aIndex, vv) {
		Indiv oInd;
		if (pProvider->find_indiv(aIndex, oInd, VariableMode::modeNumeric)) {
			const DbValueMap &oMap = oInd.data();
			BOOST_FOREACH(const MyPair &p, oMap) {
				const DbValue &v = p.second;
				if (!v.empty()) {
					double x = v.double_value();
					const IntType key = p.first;
					if (counts.find(key) == counts.end()) {
						counts[key] = 1;
						sommes[key] = x;
					} else {
						size_t nx = counts[key];
						++nx;
						counts[key] = nx;
						double s = sommes[key];
						s += x;
						sommes[key] = s;
					}
				} // not empty
			} // p
		} // indiv
	} // aIndex
	DbValueMap &oRes = this->m_center;
	oRes.clear();
	BOOST_FOREACH(const MyPair2 &p,counts) {
		const IntType key = p.first;
		size_t n = p.second;
		BOOST_ASSERT(n > 0);
		double x = (sommes[key]) / n;
		oRes[key] = x;
	} //p
} //update_center

///////////////////////////////////////
} // namespace info

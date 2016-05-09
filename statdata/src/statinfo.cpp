/*
 * statinfo.cpp
 *
 *  Created on: 8 mai 2016
 *      Author: boubad
 */
#include "../include/statinfo.h"
#include "../include/indiv.h"
//////////////////////////////
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>
//////////////////////////////////
namespace info {
///////////////////////////////////////////////////
extern size_t info_global_compute_stats(IIndivProvider *pProvider,
		statinfos_map &oRes) {
	BOOST_ASSERT(pProvider != nullptr);
	BOOST_ASSERT(pProvider->is_valid());
	variables_map oMapVars;
	bool bRet = pProvider->get_variables_map(oMapVars);
	BOOST_ASSERT(bRet);
	oRes.clear();
	typedef std::pair<IntType, StatInfo> StatInfoPair;
	size_t nTotal = 0;
	bRet = pProvider->indivs_count(nTotal);
	BOOST_ASSERT(bRet);
	VariableMode mode = VariableMode::modeNumeric;
	StatInfoMap infoMap;
#pragma omp parallel for
	for (size_t i = 0; i < nTotal; ++i) {
		Indiv oInd;
		if (pProvider->find_indiv_at(i, oInd, mode)) {
			const DbValueMap &oMap = oInd.data();
			infoMap.add(oMap);
		} // indiv
	} // i
	statinfos_map oStats = infoMap.map();
	BOOST_FOREACH(StatInfoPair &oPair, oStats){
	StatInfo oInfo = oPair.second;
	oInfo.compute();
	double vm=0, vv = 0, vs=0;
	oInfo.get_mean_var_std(vm,vv,vs);
	if (vv > 0) {
		oRes[oPair.first] = oInfo;
	}
} // oPair
	return (oRes.size());
} //info_global_compute_stats
//////////////////////////////////////////
StatInfoMap::StatInfoMap() {

}
StatInfoMap::~StatInfoMap() {

}
const statinfos_map & StatInfoMap::map(void) const {
	return (this->m_map);
}
void StatInfoMap::add(const DbValueMap &oMap) {
	typedef std::pair<IntType, DbValue> ValuePair;
#if defined(__CYGWIN__)
	std::lock_guard<std::mutex> oLock(this->_mutex);
#else
	boost::mutex::scoped_lock oLock(this->_mutex);
#endif // __CYGWIN__
	statinfos_map &oStats = this->m_map;
	BOOST_FOREACH(const ValuePair &oPair, oMap){
	DbValue vv = oPair.second;
	if (!vv.empty()) {
		const IntType key = oPair.first;
		double vx = vv.double_value();
		auto it = oStats.find(key);
		if (it == oStats.end()) {
			StatInfo oInfo(key);
			oStats[key] = oInfo;
		}
		StatInfo & info = oStats[key];
		info.add(vx);
	} // notEmpty
} // oPair
}
 // add
//////////////////////////////////////////
#ifdef MYTOTO
extern size_t info_global_compute_stats(IIndivProvider *pProvider,
	statinfos_map &oRes) {
BOOST_ASSERT(pProvider != nullptr);
BOOST_ASSERT(pProvider->is_valid());
variables_map oMapVars;
bool bRet = pProvider->get_variables_map(oMapVars);
BOOST_ASSERT(bRet);
oRes.clear();
typedef std::pair<IntType, DbValue> ValuePair;
typedef std::pair<IntType, StatInfo> StatInfoPair;
size_t nTotal = 0;
bRet = pProvider->indivs_count(nTotal);
BOOST_ASSERT(bRet);
boost::container::flat_map<IntType, StatInfo> oStats;
VariableMode mode = VariableMode::modeNumeric;
for (size_t i = 0; i < nTotal; ++i) {
	Indiv oInd;
	if (pProvider->find_indiv_at(i, oInd, mode)) {
		const DbValueMap &oMap = oInd.data();
		BOOST_FOREACH(const ValuePair &p,oMap) {
			const DbValue &v = p.second;
			if (!v.empty()) {
				const double x = v.double_value();
				const double x2 = x * x;
				const IntType key = p.first;
				auto it = oStats.find(key);
				if (it == oStats.end()) {
					StatInfo oInfo;
					oInfo.index = key;
					oInfo.count = 1;
					oInfo.somme1 = x;
					oInfo.somme2 = x2;
					oStats[key] = oInfo;
				} else {
					StatInfo &oInfo = (*it).second;
					oInfo.count++;
					oInfo.somme1 += x;
					oInfo.somme2 += x2;
				}
			} // not empty
		} // p
	} // indiv
} // i
BOOST_FOREACH(const StatInfoPair &oPair, oStats) {
	const StatInfo &oInfo = oPair.second;
	const size_t nc = oInfo.count;
	if (nc > 0) {
		double s1 = oInfo.somme1 / nc;
		double s2 = (oInfo.somme2 / nc) - (s1 * s1);
		if (s2 > 0.0) {
			const IntType key = oPair.first;
			StatInfo xx(oInfo);
			xx.deviation = std::sqrt(s2);
			xx.variance = s2;
			xx.mean = s1;
			oRes[key] = xx;
		} // s2
	} // nc
} // oPair
return (oRes.size());
} //info_global_compute_stats
#endif // MYTOTO
///////////////////////////////////////////
StatInfo::StatInfo(IntType aIndex) :
		index(aIndex), count(0), min(0), max(0), mean(0), median(0), variance(
				0), deviation(0), somme1(0), somme2(0) {
}
StatInfo::StatInfo(const StatInfo &other) :
		index(0), count(0), min(0), max(0), mean(0), median(0), variance(0), deviation(
				0), somme1(0), somme2(0) {
	this->index = other.index;
	this->count = other.count;
	this->min = other.min;
	this->max = other.max;
	this->mean = other.mean;
	this->median = other.median;
	this->variance = other.variance;
	this->deviation = other.deviation;
	this->somme1 = other.somme1;
	this->somme2 = other.somme2;
}
StatInfo & StatInfo::operator=(const StatInfo &other) {
	if (this != &other) {
		this->index = other.index;
		this->count = other.count;
		this->min = other.min;
		this->max = other.max;
		this->mean = other.mean;
		this->median = other.median;
		this->variance = other.variance;
		this->deviation = other.deviation;
		this->somme1 = other.somme1;
		this->somme2 = other.somme2;
	}
	return (*this);
}
StatInfo::~StatInfo() {
}
void StatInfo::add(const double v) {
	this->count++;
	this->somme1 += v;
	this->somme2 += v * v;
	if (this->count < 2) {
		this->min = v;
		this->max = v;
	} else {
		if (v < this->min) {
			this->min = v;
		} else if (v > this->max) {
			this->max = v;
		}
	}
} // add
void StatInfo::compute(void) {
	const size_t n = this->count;
	if (n > 0) {
		const double m = this->somme1 / n;
		this->mean = m;
		double s = (this->somme2 / n) - (m * m);
		if (s >= 0) {
			this->variance = s;
			this->deviation = std::sqrt(s);
		}
	}
} // compute
bool StatInfo::operator==(const StatInfo &other) const {
	return (this->index == other.index);
}
bool StatInfo::operator<(const StatInfo &other) const {
	return (this->index == other.index);
}
IntType StatInfo::operator()(void) const {
	return (this->index);
}
void StatInfo::swap(StatInfo &other) {
	StatInfo t(*this);
	*this = other;
	other = t;
} // swap
IntType StatInfo::key(void) const {
	return (this->index);
}
void StatInfo::reset(void) {
	this->count = 0;
	this->min = 0;
	this->max = 0;
	this->mean = 0;
	this->median = 0;
	this->variance = 0;
	this->deviation = 0;
	this->somme1 = 0;
	this->somme2 = 0;
}
void StatInfo::get_count(size_t &nCount) {
	nCount = this->count;
}
void StatInfo::get_mean_var_std(double &vm, double &vv, double &vs) {
	const size_t n = this->count;
	if (n > 0) {
		const double m = this->somme1 / n;
		this->mean = m;
		double s = (this->somme2 / n) - (m * m);
		if (s >= 0) {
			this->variance = s;
			this->deviation = std::sqrt(s);
		}
	}
	vm = this->mean;
	vv = this->variance;
	vs = this->deviation;
}
void StatInfo::get_min_max(double &vmin, double &vmax) {
	vmin = this->min;
	vmax = this->max;
}
///////////////////////////////////////////
}// namespace info

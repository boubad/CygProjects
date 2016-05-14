/*
 * statinfo.cpp
 *
 *  Created on: 8 mai 2016
 *      Author: boubad
 */
#include "../include/statinfo.h"
#include "../include/indiv.h"
 //////////////////////////////////
namespace info {
	///////////////////////////////////////////////////
	extern size_t info_global_compute_stats(IIndivProvider *pProvider,
		statinfos_map &oRes) {
		assert(pProvider != nullptr);
		assert(pProvider->is_valid());
		variables_map oMapVars;
		bool bRet = pProvider->get_variables_map(oMapVars);
		assert(bRet);
		oRes.clear();
		typedef std::pair<IntType, StatInfo> StatInfoPair;
		size_t nTotal = 0;
		bRet = pProvider->indivs_count(nTotal);
		assert(bRet);
		VariableMode mode = VariableMode::modeNumeric;
		StatInfoMap infoMap;
		for (size_t i = 0; i < nTotal; ++i) {
			Indiv oInd;
			if (pProvider->find_indiv_at(i, oInd, mode)) {
				const DbValueMap &oMap = oInd.data();
				infoMap.add(oMap);
			} // indiv
		} // i
		statinfos_map oStats = infoMap.map();
		std::for_each(oStats.begin(), oStats.end(), [&](const StatInfoPair &oPair) {
			const StatInfo &oInfo = oPair.second;
			StatInfo &o = const_cast<StatInfo &>(oInfo);
			o.compute();
			double vm = 0, vv = 0, vs = 0;
			oInfo.get_mean_var_std(vm, vv, vs);
			if (vv > 0) {
				oRes[oPair.first] = oInfo;
			}
		});
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
		statinfos_map &oStats = this->m_map;
		std::for_each(oMap.begin(), oMap.end(), [&](const ValuePair &oPair) {
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
		});
	}
	// add
   //////////////////////////////////////////
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
		}
		else {
			if (v < this->min) {
				this->min = v;
			}
			else if (v > this->max) {
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
	void StatInfo::get_count(size_t &nCount) const {
		nCount = this->count;
	}
	void StatInfo::get_mean_var_std(double &vm, double &vv, double &vs) const {
		const size_t n = this->count;
		vm = this->mean;
		vv = this->variance;
		vs = this->deviation;
	}
	void StatInfo::get_min_max(double &vmin, double &vmax) const {
		vmin = this->min;
		vmax = this->max;
	}
	///////////////////////////////////////////
}// namespace info

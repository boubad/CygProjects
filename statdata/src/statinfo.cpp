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
//////////////////////////////////////////
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
///////////////////////////////////////////
} // namespace info

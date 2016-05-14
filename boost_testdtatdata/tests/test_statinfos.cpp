/*
 * test_statinfos.cpp
 *
 *  Created on: 13 mai 2016
 *      Author: boubad
 */
///////////////////////////////////
#include <boost/test/unit_test.hpp>
///////////////////////////
#include "indivproviderfixture.h"
/////////////////////////
#include <statinfo.h>
#include <indivcluster.h>
//////////////////////
using namespace std;
using namespace info;
/////////////////////////////
BOOST_FIXTURE_TEST_SUITE(MiscTestSuite, IndivProviderFixture)
;
BOOST_AUTO_TEST_CASE(testStatInfo) {
	IIndivProvider *pProvider = m_provider.get();
	statinfos_map oRes;
	size_t n = info_global_compute_stats(pProvider, oRes);
	BOOST_CHECK(n == this->m_nbcols);
} //testStatInfo
BOOST_AUTO_TEST_CASE(testComputeDistances) {
	IIndivProvider *pProvider = m_provider.get();
	//
	size_t nc = 0;
	bool bRet = pProvider->indivs_count(nc);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == nc);
	//
	IndivDistanceMap oDistances;
	info_global_compute_distances(pProvider, oDistances);
	//
	const ints_set &oSetIndivs = oDistances.indexes();
	BOOST_CHECK(oSetIndivs.size() == nc);
	//
	for (size_t i = 0; i < nc; ++i) {
		Indiv oInd1;
		bRet = pProvider->find_indiv_at(i, oInd1);
		BOOST_CHECK(bRet);
		IntType aIndex1 = oInd1.id();
		BOOST_CHECK(aIndex1 != 0);
		for (size_t j = 0; j < i; ++j) {
			Indiv oInd2;
			bRet = pProvider->find_indiv_at(j, oInd2);
			BOOST_CHECK(bRet);
			IntType aIndex2 = oInd2.id();
			BOOST_CHECK(aIndex2 != 0);
			double dRes = 0;
			bRet = oDistances.get(aIndex1, aIndex2, dRes);
			BOOST_CHECK(bRet);
			BOOST_CHECK(dRes > 0);
		} // j
	} // i
} //testComputeDistances

BOOST_AUTO_TEST_SUITE_END();


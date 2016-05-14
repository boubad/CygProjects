///////////////////////////////////
#include <boost/test/unit_test.hpp>
///////////////////////////
#include "distancefixture.h"
/////////////////////////
#include <indivcluster.h>
//////////////////////
using namespace std;
using namespace info;
/////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IndivClusterTestSuite, DistanceFixture)
;
BOOST_AUTO_TEST_CASE(testIndivCluster) {
	IIndivProvider *pProvider = m_provider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	IndivDistanceMap *pMap = m_odistances.get();
	BOOST_REQUIRE(pMap != nullptr);
	IntType aIndex1 = 100, aIndex2 = 200;
	IndivCluster oCluster1(pProvider, aIndex1);
	IndivCluster oCluster2(pProvider, aIndex2);
	size_t nc = 0;
	bool bRet = pProvider->indivs_count(nc);
	BOOST_REQUIRE(bRet);
	BOOST_REQUIRE(nc > 1);
	for (size_t i = 0; i < nc; i += 2) {
		Indiv oInd1;
		if (!pProvider->find_indiv_at(i, oInd1)) {
			break;
		}
		oCluster1.add(oInd1);
		Indiv oInd2;
		if (!pProvider->find_indiv_at(i + 1, oInd2)) {
			break;
		}
		oCluster2.add(oInd2);
	}// i
	oCluster1.update_center();
	oCluster2.update_center();
	//
	double fintra = 0, finter = 0;
	bRet = oCluster1.intra_inertia(fintra);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fintra > 0);
	bRet = oCluster1.inter_inertia(oCluster2, finter);
	BOOST_CHECK(bRet);
	BOOST_CHECK(finter > 0);
	//
	double dd = 0;
	ClusterAppendMode mode = ClusterAppendMode::modeInvalid;
	bRet = oCluster1.min_distance(oCluster2,dd, mode);
	BOOST_CHECK(bRet);
	BOOST_CHECK(mode != ClusterAppendMode::modeInvalid);
	BOOST_CHECK(dd > 0);
	oCluster1.add(oCluster2, mode);
	oCluster1.update_center();
	//
	for (size_t i = 0; i < nc; i += 2) {
		Indiv oInd;
		if (!pProvider->find_indiv_at(i, oInd)) {
			break;
		}
		double d1 = oCluster1.distance(oInd);
		BOOST_CHECK(d1 > 0);
		double d2 = oCluster2.distance(oInd);
		BOOST_CHECK(d2 > 0);
	}// i
} //testStatInfo


BOOST_AUTO_TEST_SUITE_END();

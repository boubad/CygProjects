/*
 * test_clusterize.cpp
 *
 *  Created on: 20 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include <clusterize.h>
#include "storeindivprovider_fixture.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
//////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(ClusterizeTestSuite, StoreIndivSorceFixture<>)
;
BOOST_AUTO_TEST_CASE(testClusterizeRandom) {
	using ClusterizeKMeansType = ClusterizeKMeans<>;
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	size_t nbClusters = 6;
	//LinkMode mode = LinkMode::linkMean;
	size_t nbIters = 20;
	//
	ClusterizeKMeansType oMan;
	size_t nx = oMan.compute_random(nbClusters,pMan,nbIters);
	BOOST_CHECK(nx > 0);
	std::stringstream os;
	os << oMan;
	std::string ss = os.str();
	BOOST_TEST_MESSAGE("CLUSTERIZE KMEANS RANDOM\n" << ss);
} //testClusterizeMean
BOOST_AUTO_TEST_SUITE_END();






/*
 * test_cluterize.cpp
 *
 *  Created on: 28 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <clusterize.h>
/////////////////////////////
#include "mytestfixture.h"
////////////////////////////////////
#include "global_defs.h"
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using MyFixture = MyTestFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
/////////////////////////////////////
using IndivType = typename MyFixture::IndivType;
using DataMap = typename MyFixture::DataMap;
using IndivTypePtr = typename MyFixture::IndivTypePtr;
using SourceType = typename MyFixture::SourceType;
////////////////////////////////
using ClusterizeKMeansType = ClusterizeKMeans<IDTYPE, STRINGTYPE, DISTANCETYPE>;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(ClusterizeTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalClusterize) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	ClusterizeKMeansType oMan;
	size_t nbClusters = 5;
	size_t nbIters = 20;
	size_t nIters = oMan.process(pProvider, nbClusters, nbIters);
	BOOST_CHECK(nIters > 0);
	STRINGTYPE ss;
	oMan.to_string(ss);
	BOOST_TEST_MESSAGE("");
	BOOST_TEST_MESSAGE("CLUSTERIZE KMEANS MORTAL");
	BOOST_TEST_MESSAGE(ss);
} //testMortalClusterize
BOOST_AUTO_TEST_CASE(testConsoClusterize) {
	SourceType *pProvider = this->conso_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	ClusterizeKMeansType oMan;
	size_t nbClusters = 3;
	size_t nbIters = 20;
	size_t nIters = oMan.process(pProvider, nbClusters, nbIters);
	BOOST_CHECK(nIters > 0);
	STRINGTYPE ss;
	oMan.to_string(ss);
	BOOST_TEST_MESSAGE("");
	BOOST_TEST_MESSAGE("CLUSTERIZE KMEANS CONSO");
	BOOST_TEST_MESSAGE(ss);
} //testConsoClusterize
BOOST_AUTO_TEST_CASE(testTestClusterize) {
	SourceType *pProvider = this->test_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	ClusterizeKMeansType oMan;
	size_t nbClusters = 5;
	size_t nbIters = 20;
	size_t nIters = oMan.process(pProvider, nbClusters, nbIters);
	BOOST_CHECK(nIters > 0);
	STRINGTYPE ss;
	oMan.to_string(ss);
	BOOST_TEST_MESSAGE("");
	BOOST_TEST_MESSAGE("CLUSTERIZE KMEANS TEST");
	BOOST_TEST_MESSAGE(ss);
} //testTestClusterize
BOOST_AUTO_TEST_SUITE_END();


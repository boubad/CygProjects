/*
 * test_treeelem.cpp
 *
 *  Created on: 28 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <treecollection.h>
/////////////////////////////
#include <mytestfixture.h>
////////////////////////////////////
#include <global_defs.h>
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
using IndivsTreeType = IndivsTree<IDTYPE,STRINGTYPE,DISTANCETYPE>;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(HierarTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalHierar) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	IndivsTreeType oMan;
	size_t nbClusters = 5;
	size_t nbIters = 20;
	size_t nIters = oMan.process(pProvider, nbClusters, nbIters);
	BOOST_CHECK(nIters > 0);
	/*
	STRINGTYPE ss;
	oMan.to_string(ss);
	BOOST_TEST_MESSAGE("");
	BOOST_TEST_MESSAGE("CLUSTERIZE HIERAR MORTAL");
	BOOST_TEST_MESSAGE("Nb iterations: " << nIters);
	BOOST_TEST_MESSAGE("Nb clusters: " << oMan.clusters_count() << "/" << nbClusters);
	BOOST_TEST_MESSAGE(ss);
	*/
} //testMortalHierar
BOOST_AUTO_TEST_CASE(testConsoHierar) {
	SourceType *pProvider = this->conso_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	IndivsTreeType oMan;
	size_t nbClusters = 3;
	size_t nbIters = 20;
	size_t nIters = oMan.process(pProvider, nbClusters, nbIters);
	BOOST_CHECK(nIters > 0);
	/*
	STRINGTYPE ss;
	oMan.to_string(ss);
	BOOST_TEST_MESSAGE("");
	BOOST_TEST_MESSAGE("CLUSTERIZE HIERAR CONSO");
	BOOST_TEST_MESSAGE("Nb iterations: " << nIters);
	BOOST_TEST_MESSAGE("Nb clusters: " << oMan.clusters_count() << "/" << nbClusters);
	BOOST_TEST_MESSAGE(ss);
	*/
} //testConsoHierar
/*
BOOST_AUTO_TEST_CASE(testTestHierar) {
	SourceType *pProvider = this->test_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	IndivsTreeType oMan;
	size_t nbClusters = 5;
	size_t nbIters = 20;
	size_t nIters = oMan.process(pProvider, nbClusters, nbIters);
	BOOST_CHECK(nIters > 0);
	STRINGTYPE ss;
	oMan.to_string(ss);
	BOOST_TEST_MESSAGE("");
	BOOST_TEST_MESSAGE("CLUSTERIZE HIERAR TEST");
	BOOST_TEST_MESSAGE("Nb iterations: " << nIters);
	BOOST_TEST_MESSAGE("Nb clusters: " << oMan.clusters_count() << "/" << nbClusters);
	BOOST_TEST_MESSAGE(ss);
} //testTestHierar
*/
BOOST_AUTO_TEST_SUITE_END();






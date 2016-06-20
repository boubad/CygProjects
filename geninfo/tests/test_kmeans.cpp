/*
 * test_kmeans.cpp
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <clusterize.h>
/////////////////////////////
#include "datafixture.h"
#include "global_defs.h"
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using MyFixture =InfoDataFixture<IDTYPE, STRINGTYPE, DATATYPE>;
using SourceType = typename MyFixture::SourceType;
using MatriceDataType = typename MyFixture::MatriceDataType;
using strings_vector = typename MyFixture::strings_vector;
using pcancelflag = typename MyFixture::pcancelflag;
using PBackgrounder =typename MyFixture::PBackgrounder;
////////////////////////////////
using ints_sizet_map = std::map<IDTYPE, size_t>;
using IndivClusterType = IndivCluster<IDTYPE,STRINGTYPE>;
using SourceType = IIndivSource<IDTYPE,STRINGTYPE>;
using clusters_vector = std::vector<IndivClusterType>;
using ints_vector = std::vector<IDTYPE>;
using sizet_intsvector_map = std::map<size_t, ints_vector>;
using ClustersCollectionType = ClustersCollection<IDTYPE,STRINGTYPE>;
using cancelflag = std::atomic<bool>;
using pcancelflag = cancelflag *;
using PBackgrounder = Backgrounder *;
using ClusterizeResultType = ClusterizeResult<IDTYPE>;
using ClusterizeResultPtr = std::shared_ptr< ClusterizeResultType>;
using ClusterizeKMeansType = ClusterizeKMeans<IDTYPE,STRINGTYPE,DISTANCETYPE>;
//////////////////////////
BOOST_FIXTURE_TEST_SUITE(KMeansTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalKmeans) {
	auto infologger = [&](ClusterizeResultPtr oRes) {
		ClusterizeResultType *p = oRes.get();
		if (p != nullptr) {
			STRINGTYPE sr;
			p->to_string(sr);
			BOOST_TEST_MESSAGE(sr);
		} // p
		};
	SourceType *pProvider = this->mortal_indiv_provider();
	BOOST_CHECK(pProvider != nullptr);
	//
	pcancelflag pCancel = this->get_cancelflag();
	BOOST_CHECK(pCancel != nullptr);
	PBackgrounder pBack = this->get_backgrounder();
	BOOST_CHECK(pBack != nullptr);
	//
	ClusterizeKMeansType oMat(pCancel, pBack, infologger);
	oMat.process(pProvider, 5, 10);
} //testMortalKmeans

BOOST_AUTO_TEST_SUITE_END();

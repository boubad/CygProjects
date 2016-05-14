///////////////////////////////////
#include <boost/test/unit_test.hpp>
///////////////////////////
#include "distancefixture.h"
/////////////////////////
#include <clusterize_kmeans.h>
#include <boost/foreach.hpp>
//////////////////////
using namespace std;
using namespace info;
/////////////////////////////
BOOST_FIXTURE_TEST_SUITE(ClusterizeKMeansTestSuite, DistanceFixture)
;
BOOST_AUTO_TEST_CASE(testInfoGlobalClusterize) {
	BOOST_TEST_MESSAGE("InfoGlobalClusterizeKMeans");
	IIndivProvider *pProvider = m_provider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	size_t nbClusters = 5;
	size_t nbMaxIters = 100;
	indivclusters_vector oClusters;
	size_t  nbIters = info_global_clusterize_kmeans(pProvider, nbClusters, oClusters, nbMaxIters);
	BOOST_REQUIRE(nbIters > 0);
	BOOST_CHECK(nbIters < nbMaxIters);
	BOOST_TEST_MESSAGE("NbIterations: " << nbIters);
	BOOST_FOREACH(const IndivCluster &oCluster, oClusters)
	{
		std::string ss;
		this->write_point(oCluster.center(), ss);
		BOOST_TEST_MESSAGE("Cluster: " << oCluster.index() << "\t" << ss);
	}// oCluster
} //testInfoGlobalClusterize
BOOST_AUTO_TEST_CASE(testClusterizeKMeans) {
	BOOST_TEST_MESSAGE("ClusterizeKMeans");
	IIndivProvider *pProvider = m_provider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	size_t nbClusters = 5;
	size_t nbMaxIters = 100;
	ClusterizeKMeans oMeans(pProvider, nbClusters);
	BOOST_REQUIRE(oMeans.is_valid());
	bool bRet = oMeans.compute(nbMaxIters);
	BOOST_REQUIRE(bRet);
	double fIntra = 0, fInter = 0, fCrit = 0;
	bRet = oMeans.intra_inertia(fIntra);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fIntra > 0);
	bRet = oMeans.inter_inertia(fInter);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fInter > 0);
	bRet = oMeans.criteria(fCrit);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fCrit > 0);
	BOOST_TEST_MESSAGE("Criteria: " << fCrit << "\tIntra: " << fIntra << "\tInter: " << fInter);
	const indivclusters_vector & oClusters = oMeans.clusters();
	BOOST_FOREACH(const IndivCluster &oCluster, oClusters)
	{
		std::string ss;
		this->write_point(oCluster.center(), ss);
		BOOST_TEST_MESSAGE("Cluster: " << oCluster.index() << "\t" << ss);
	}// oCluster
} //testClusterizeKMeans
BOOST_AUTO_TEST_CASE(testClusterizeKMeansCollector) {
	BOOST_TEST_MESSAGE("ClusterizeKMeansCollector");
	IIndivProvider *pProvider = m_provider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	size_t nbClusters = 5;
	size_t nbMaxIters = 100;
	size_t nbRuns = 10;
	ClusterizeKMeansCollector oCol;
	oCol.process(pProvider, nbClusters, nbMaxIters, nbRuns);
	const ClusterizeKMeans &oMeans = oCol.object();
	double fIntra = 0, fInter = 0, fCrit = 0;
	bool bRet = oMeans.intra_inertia(fIntra);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fIntra > 0);
	bRet = oMeans.inter_inertia(fInter);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fInter > 0);
	bRet = oMeans.criteria(fCrit);
	BOOST_CHECK(bRet);
	BOOST_CHECK(fCrit > 0);
	BOOST_TEST_MESSAGE("Criteria: " << fCrit << "\tIntra: " << fIntra << "\tInter: " << fInter);
	const indivclusters_vector & oClusters = oMeans.clusters();
	BOOST_FOREACH(const IndivCluster &oCluster, oClusters)
	{
		std::string ss;
		this->write_point(oCluster.center(), ss);
		BOOST_TEST_MESSAGE("Cluster: " << oCluster.index() << "\t" << ss);
	}// oCluster
} //testClusterizeKMeansCollector
BOOST_AUTO_TEST_SUITE_END();

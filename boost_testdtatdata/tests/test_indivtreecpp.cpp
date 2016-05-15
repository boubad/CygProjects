///////////////////////////////////
#include <boost/test/unit_test.hpp>
///////////////////////////
#include "distancefixture.h"
/////////////////////////
#include <indivtree.h>
#include <clusterize_kmeans.h>
/////////////////////////////////
#include <boost/foreach.hpp>
//////////////////////
using namespace std;
using namespace info;
/////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IndivTreeTestSuite, DistanceFixture)
;
BOOST_AUTO_TEST_CASE(testIndivTreeMean) {
	INumIndivProvider *pProvider = m_pnumprovider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	size_t nbClusters = 5;
	LinkMode mode = LinkMode::linkMean;
	IndivsTree oTree;
	oTree.process(pProvider, nbClusters, mode);
	ints_size_t_map xMap;
	oTree.get_map(xMap);
	size_t n = xMap.size();
	BOOST_CHECK(n == m_nbrows);
	IndivsTree::valuemaps_vector oCenters;
	oTree.get_centers(oCenters);
	BOOST_CHECK(oCenters.size() == nbClusters);
	int nx = 0;
	BOOST_TEST_MESSAGE("LINK MEAN");
	BOOST_FOREACH(const DbValueMap &oMap, oCenters)
	{
		std::string ss;
		this->write_point(oMap, ss);
		BOOST_TEST_MESSAGE("Cluster " << nx++ << ":\t" << ss);
	}
	std::string sx;
	this->write_indis_map(xMap,sx);
	BOOST_TEST_MESSAGE(sx);
} //testInfoGlobalClusterize
BOOST_AUTO_TEST_CASE(testIndivTreeMin) {
	INumIndivProvider *pProvider = m_pnumprovider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	size_t nbClusters = 5;
	LinkMode mode = LinkMode::linkMin;
	IndivsTree oTree;
	oTree.process(pProvider, nbClusters, mode);
	ints_size_t_map xMap;
	oTree.get_map(xMap);
	size_t n = xMap.size();
	BOOST_CHECK(n == m_nbrows);
	IndivsTree::valuemaps_vector oCenters;
	oTree.get_centers(oCenters);
	BOOST_CHECK(oCenters.size() == nbClusters);
	int nx = 0;
	BOOST_TEST_MESSAGE("LINK MAX");
	BOOST_FOREACH(const DbValueMap &oMap, oCenters)
	{
		std::string ss;
		this->write_point(oMap, ss);
		BOOST_TEST_MESSAGE("Cluster " << nx++ << ":\t" << ss);
	}
	std::string sx;
	this->write_indis_map(xMap, sx);
	BOOST_TEST_MESSAGE(sx);
} //testInfoGlobalClusterize
BOOST_AUTO_TEST_CASE(testIndivTreeMax) {
	INumIndivProvider *pProvider = m_pnumprovider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	size_t nbClusters = 5;
	LinkMode mode = LinkMode::linkMax;
	IndivsTree oTree;
	oTree.process(pProvider, nbClusters, mode);
	ints_size_t_map xMap;
	oTree.get_map(xMap);
	size_t n = xMap.size();
	BOOST_CHECK(n == m_nbrows);
	IndivsTree::valuemaps_vector oCenters;
	oTree.get_centers(oCenters);
	BOOST_CHECK(oCenters.size() == nbClusters);
	int nx = 0;
	BOOST_TEST_MESSAGE("LINK MIN");
	BOOST_FOREACH(const DbValueMap &oMap, oCenters)
	{
		std::string ss;
		this->write_point(oMap, ss);
		BOOST_TEST_MESSAGE("Cluster " << nx++ << ":\t" << ss);
	}
	std::string sx;
	this->write_indis_map(xMap, sx);
	BOOST_TEST_MESSAGE(sx);
} //testInfoGlobalClusterize
BOOST_AUTO_TEST_CASE(testClusterizeKMeansWithSeed) {
	BOOST_TEST_MESSAGE("ClusterizeKMeansSeed");
	INumIndivProvider *pProvider = m_pnumprovider.get();
	BOOST_REQUIRE(pProvider != nullptr);
	LinkMode mode = LinkMode::linkMin;
	size_t nbClusters = 5;
	IndivsTree oTree;
	oTree.process(pProvider, nbClusters, mode);
	IndivsTree::valuemaps_vector oCenters;
	oTree.get_centers(oCenters);
	BOOST_CHECK(oCenters.size() == nbClusters);
	size_t nbMaxIters = 100;
	ClusterizeKMeans oMeans(pProvider, nbClusters);
	BOOST_REQUIRE(oMeans.is_valid());
	bool bRet = oMeans.compute(oCenters,nbMaxIters);
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
	ints_size_t_map xMap = oMeans.get_map();
	std::string sx;
	this->write_indis_map(xMap, sx);
	BOOST_TEST_MESSAGE(sx);
} //testClusterizeKMeans
BOOST_AUTO_TEST_SUITE_END();

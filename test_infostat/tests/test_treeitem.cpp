/*
 * test_treeitem.cpp
 *
 *  Created on: 20 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include <treeitem.h>
#include "storeindivprovider_fixture.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
//////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(TreeItemTestSuite, StoreIndivSorceFixture)
;
BOOST_AUTO_TEST_CASE(testTreeItemMean) {
	using IndivsTreeType = IndivsTree<>;
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	size_t nbClusters = 5;
	LinkMode mode = LinkMode::linkMean;
	//
	IndivsTreeType oTree;
	oTree.process(pMan, nbClusters, mode);
	double fIntra = 0, fInter = 0, ff = 0;
	bool bRet = oTree.get_criterias(fIntra, fInter, ff);
	BOOST_CHECK(bRet);
	//BOOST_TEST_MESSAGE("TREE LINK MEAN");
	//BOOST_TEST_MESSAGE("fIntra: " << fIntra << " ,fInter:" << fInter << " ,ff:" << ff);
	//
	IndivsTreeType::datamaps_vector oVec;
	oTree.get_centers(oVec);
	std::for_each(oVec.begin(), oVec.end(),
			[&](const IndivsTreeType::DataMap &oMap) {
				std::string s;
				test_write_map(oMap,s);
				//BOOST_TEST_MESSAGE(s);
			});
} //testTreeItemMean
BOOST_AUTO_TEST_CASE(testTreeItemMin) {
	using IndivsTreeType = IndivsTree<>;
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	size_t nbClusters = 5;
	LinkMode mode = LinkMode::linkMin;
	//
	IndivsTreeType oTree;
	oTree.process(pMan, nbClusters, mode);
	double fIntra = 0, fInter = 0, ff = 0;
	bool bRet = oTree.get_criterias(fIntra, fInter, ff);
	BOOST_CHECK(bRet);
	//BOOST_TEST_MESSAGE("TREE LINK MIN");
	//BOOST_TEST_MESSAGE("fIntra: " << fIntra << " ,fInter:" << fInter << " ,ff:" << ff);
	//
	IndivsTreeType::datamaps_vector oVec;
	oTree.get_centers(oVec);
	std::for_each(oVec.begin(), oVec.end(),
			[&](const IndivsTreeType::DataMap &oMap) {
				std::string s;
				test_write_map(oMap,s);
				//BOOST_TEST_MESSAGE(s);
			});
} //testTreeItemMin
BOOST_AUTO_TEST_CASE(testTreeItemMax) {
	using IndivsTreeType = IndivsTree<>;
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	size_t nbClusters = 5;
	LinkMode mode = LinkMode::linkMax;
	//
	IndivsTreeType oTree;
	oTree.process(pMan, nbClusters, mode);
	double fIntra = 0, fInter = 0, ff = 0;
	bool bRet = oTree.get_criterias(fIntra, fInter, ff);
	BOOST_CHECK(bRet);
	//BOOST_TEST_MESSAGE("TREE LINK MAX");
	//BOOST_TEST_MESSAGE("fIntra: " << fIntra << " ,fInter:" << fInter << " ,ff:" << ff);
	//
	IndivsTreeType::datamaps_vector oVec;
	oTree.get_centers(oVec);
	std::for_each(oVec.begin(), oVec.end(),
			[&](const IndivsTreeType::DataMap &oMap) {
				std::string s;
				test_write_map(oMap,s);
				//BOOST_TEST_MESSAGE(s);
			});
} //testTreeItemMax
BOOST_AUTO_TEST_SUITE_END();


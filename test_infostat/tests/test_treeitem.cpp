/*
 * test_treeitem.cpp
 *
 *  Created on: 20 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include <treecollection.h>
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
	size_t nbClusters = 6;
	LinkMode mode = LinkMode::linkMean;
	//
	IndivsTreeType oTree;
	oTree.link_mode(mode);
	oTree.process(pMan, nbClusters);
	std::stringstream os;
	os << oTree;
	std::string ss = os.str();
	BOOST_TEST_MESSAGE("TREE LINK MEAN\n" << ss);
} //testTreeItemMean
BOOST_AUTO_TEST_CASE(testTreeItemMeanRecode) {
	using IndivsTreeType = IndivsTree<>;
	StoreIndivSourceType *pMan = m_source.get();
	BOOST_CHECK(pMan != nullptr);
	pMan->transformation(TransformationType::recode);
	size_t nbClusters = 6;
	LinkMode mode = LinkMode::linkMean;
	//
	IndivsTreeType oTree;
	oTree.link_mode(mode);
	oTree.process(pMan, nbClusters);
	std::stringstream os;
	os << oTree;
	std::string ss = os.str();
	BOOST_TEST_MESSAGE("TREE LINK MEAN RECODE\n" << ss);
} //testTreeItemMean
BOOST_AUTO_TEST_CASE(testTreeItemMin) {
	using IndivsTreeType = IndivsTree<>;
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	size_t nbClusters = 6;
	LinkMode mode = LinkMode::linkMin;
	//
	IndivsTreeType oTree;
	oTree.link_mode(mode);
	oTree.process(pMan, nbClusters);
	std::stringstream os;
	os << oTree;
	std::string ss = os.str();
	BOOST_TEST_MESSAGE("TREE LINK MIN\n" << ss);
} //testTreeItemMin
BOOST_AUTO_TEST_CASE(testTreeItemMax) {
	using IndivsTreeType = IndivsTree<>;
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	size_t nbClusters = 6;
	LinkMode mode = LinkMode::linkMax;
	//
	IndivsTreeType oTree;
	oTree.link_mode(mode);
	oTree.process(pMan, nbClusters);
	std::stringstream os;
	os << oTree;
	std::string ss = os.str();
	BOOST_TEST_MESSAGE("TREE LINK MAX\n" << ss);
} //testTreeItemMax
BOOST_AUTO_TEST_SUITE_END();


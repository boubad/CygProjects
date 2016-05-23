/*
 * test_matitem.cpp
 *
 *  Created on: 23 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include <matitem.h>
#include "storeindivprovider_fixture.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
//////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(MatItemTestSuite, StoreIndivSorceFixture<>)
;
BOOST_AUTO_TEST_CASE(testMatItem) {
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	MatComputeParams<unsigned long,long,std::string> oParams(pMan);
	MatItem<unsigned long,long,std::string> oMat(&oParams);
	oMat.permute();
} //testTreeItemMean
BOOST_AUTO_TEST_SUITE_END();






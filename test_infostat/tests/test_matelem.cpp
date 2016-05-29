/*
 * test_matelem.cpp
 *
 *  Created on: 28 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <matelem.h>
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
using MatElemType = MatElem<IDTYPE,DISTANCETYPE,STRINGTYPE>;
using DistanceMapType = typename MatElemType::DistanceMapType;
using ints_vector = typename MatElemType::ints_vector;
using sizets_vector = typename MatElemType::sizets_vector;
using MatElemResultType = typename MatElemType::MatElemResultType;
using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(MatElemTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalMatElem) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	MatElemType::DistanceMapType oDist(pProvider);
	MatElemType::ints_vector oIds;
	oDist.indexes(oIds);
	//BOOST_TEST_MESSAGE("\nTEST MATELEM MORTAL");
	MatElemType oMat(&oDist, &oIds);
	//BOOST_TEST_MESSAGE("Start criteria: " << oMat.criteria());
	oMat.process();
	//BOOST_TEST_MESSAGE("End criteria: " << oMat.criteria());
	MatElemType::ints_vector ids;
	oMat.ids(ids);
	for (auto aIndex : ids) {
		MatElemType::IndivTypePtr oInd = pProvider->find(aIndex);
		MatElemType::IndivType *pInd = oInd.get();
		BOOST_CHECK(pInd != nullptr);
		STRINGTYPE s = pInd->sigle();
		//BOOST_TEST_MESSAGE(s);
	}
} //testMortalMatElem
BOOST_AUTO_TEST_CASE(testConsoMatElem) {
	SourceType *pProvider = this->conso_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	MatElemType::DistanceMapType oDist(pProvider);
	MatElemType::ints_vector oIds;
	oDist.indexes(oIds);
	//BOOST_TEST_MESSAGE("\nTEST MATELEM CONSO");
	MatElemType oMat(&oDist, &oIds);
	//BOOST_TEST_MESSAGE("Start criteria: " << oMat.criteria());
	oMat.process();
	//BOOST_TEST_MESSAGE("End criteria: " << oMat.criteria());
	MatElemType::ints_vector ids;
	oMat.ids(ids);
	for (auto aIndex : ids) {
		MatElemType::IndivTypePtr oInd = pProvider->find(aIndex);
		MatElemType::IndivType *pInd = oInd.get();
		BOOST_CHECK(pInd != nullptr);
		STRINGTYPE s = pInd->sigle();
		//BOOST_TEST_MESSAGE(s);
	}
} //testConsoMatElem
BOOST_AUTO_TEST_CASE(testTestMatElem) {
	SourceType *pProvider = this->test_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	MatElemType::DistanceMapType oDist(pProvider);
	MatElemType::ints_vector oIds;
	oDist.indexes(oIds);
	BOOST_TEST_MESSAGE("\nTEST MATELEM TEST");
	MatElemType oMat(&oDist, &oIds);
	BOOST_TEST_MESSAGE("Start criteria: " << oMat.criteria());
	size_t nIter = 0;
	DISTANCETYPE oldCrit = 0;
	oMat.process_interm([&](const MatElemResultPtr &oCrit) {
		if (oCrit.get() != nullptr) {
			++nIter;
			DISTANCETYPE crit = oCrit->first;
			if (oldCrit == 0){
				BOOST_TEST_MESSAGE("Iteration: " << nIter << ",\t" << crit);
				oldCrit = crit;
			} else {
				DISTANCETYPE delta = oldCrit - crit;
				double f = 10000.0 * (delta/(double)oldCrit);
				BOOST_TEST_MESSAGE("Iteration: " << nIter << ",\t" << crit << ",\t" << delta << ",\t" << f);
				oldCrit = crit;
			}

		}//ptr
	});
	BOOST_TEST_MESSAGE("End criteria: " << oMat.criteria());
	MatElemType::ints_vector ids;
	oMat.ids(ids);
	for (auto aIndex : ids) {
		MatElemType::IndivTypePtr oInd = pProvider->find(aIndex);
		MatElemType::IndivType *pInd = oInd.get();
		BOOST_CHECK(pInd != nullptr);
		STRINGTYPE s = pInd->sigle();
		BOOST_TEST_MESSAGE(s);
	}
} //testTestMatElem
BOOST_AUTO_TEST_SUITE_END();


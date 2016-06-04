/*
 * test_intramatelem.cpp
 *
 *  Created on: 31 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <intramat.h>
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
using MatElemType = IntraMatElem<IDTYPE,DISTANCETYPE,STRINGTYPE>;
using DistanceMapType = typename MatElemType::DistanceMapType;
using ints_vector = typename MatElemType::ints_vector;
using sizets_vector = typename MatElemType::sizets_vector;
using MatElemResultType = typename MatElemType::IntraMatElemResultType;
using MatElemResultPtr = typename MatElemType::IntraMatElemResultPtr;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IntraMatElemTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalMatElemIntra) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	MatElemType oMat;
	auto conn = oMat.connect([&](MatElemResultPtr oCrit) {
		MatElemResultType *pCrit = oCrit.get();
		if (pCrit != nullptr) {
			DISTANCETYPE varCrit = pCrit->first;
			STRINGTYPE s;
			write_vector(pCrit->second, s);
			BOOST_TEST_MESSAGE("Criteria: " << varCrit << ", \t" << s);
		} // pCrit
		});
	//
	oMat.arrange(pProvider);
	sizets_vector oIndexes;
	oMat.indexes(oIndexes);
	BOOST_TEST_MESSAGE("============ RESULTAT MORTAL =============");
	STRINGTYPE ss;
	DISTANCETYPE crit = oMat.criteria();
	write_vector(oIndexes, ss);
	BOOST_TEST_MESSAGE("Criteria: " << crit << ", \t" << ss);
} //testMortalMatElem

BOOST_AUTO_TEST_SUITE_END();


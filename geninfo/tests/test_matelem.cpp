/*
 * test_matelem.cpp
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <info_matelem.h>
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
using MatElemType = MatElem<IDTYPE,DISTANCETYPE,STRINGTYPE>;
using DistanceMapType = typename MatElemType::DistanceMapType;
using ints_vector = typename MatElemType::ints_vector;
using sizets_vector = typename MatElemType::sizets_vector;
using MatElemResultType = typename MatElemType::MatElemResultType;
using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
using MatElemFunctionType = typename MatElemType::MatElemFunctionType;
//////////////////////////
MatElemFunctionType infologger = [&](MatElemResultPtr oRes) {
	MatElemResultType *p = oRes.get();
	if (p != nullptr) {
		STRINGTYPE ss;
		p->to_string(ss);
		BOOST_TEST_MESSAGE(ss);
	} // p
	};
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(MatElemTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalMatElem) {
	SourceType *pIndProvider = this->mortal_indiv_provider();
	BOOST_CHECK(pIndProvider != nullptr);
	SourceType *pVarProvider = this->mortal_variable_provider();
	BOOST_CHECK(pVarProvider != nullptr);
	MatriceDataType *pData = this->get_mortal_data();
	STRINGTYPE name = pData->name();
	//
	pcancelflag pCancel = this->get_cancelflag();
	BOOST_CHECK(pCancel != nullptr);
	PBackgrounder pBack = this->get_backgrounder();
	BOOST_CHECK(pBack != nullptr);
	//
	std::thread t1([&]() {
		MatElemType oInd(DispositionType::indiv,pCancel,pBack,infologger);
		oInd.sigle(name);
		oInd.arrange(pIndProvider);
	});
	//
	std::thread t2([&]() {
		MatElemType oVar(DispositionType::variable,pCancel,pBack,infologger);
		oVar.sigle(name);
		oVar.arrange(pVarProvider);
	});
	t1.join();
	t2.join();
} //testMortalMatElem

BOOST_AUTO_TEST_SUITE_END();


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
BOOST_FIXTURE_TEST_SUITE(MatElemTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalMatElem) {
	MatElemFunctionType infologger = [&](MatElemResultPtr oRes) {
		MatElemResultType *p = oRes.get();
		if (p != nullptr) {
			STRINGTYPE sr;
			p->to_string(sr,true);
			BOOST_TEST_MESSAGE(sr);
		} // p
		};
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
	MatElemType oInd(DispositionType::indiv, pCancel, pBack, infologger);
	oInd.sigle(name);
	oInd.arrange(pIndProvider);
	//
	MatElemType oVar(DispositionType::variable, pCancel, pBack, infologger);
	oVar.sigle(name);
	oVar.arrange(pVarProvider);
} //testMortalMatElem
#ifdef MYTOTO
BOOST_AUTO_TEST_CASE(testConsoMatElem) {
	MatElemFunctionType infologger = [&](MatElemResultPtr oRes) {
		MatElemResultType *p = oRes.get();
		if (p != nullptr) {
			STRINGTYPE sr;
			p->to_string(sr,true);
			BOOST_TEST_MESSAGE(sr);
		} // p
	};
	SourceType *pIndProvider = this->conso_indiv_provider();
	BOOST_CHECK(pIndProvider != nullptr);
	SourceType *pVarProvider = this->conso_variable_provider();
	BOOST_CHECK(pVarProvider != nullptr);
	MatriceDataType *pData = this->get_conso_data();
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
} //testConsoMatElem
BOOST_AUTO_TEST_CASE(testTestMatElem) {
	MatElemFunctionType infologger2 = [&](MatElemResultPtr oRes) {
		MatElemResultType *p = oRes.get();
		if (p != nullptr) {
			STRINGTYPE sr;
			p->to_string(sr,false);
			BOOST_TEST_MESSAGE(sr);
		} // p
	};
	SourceType *pIndProvider = this->test_indiv_provider();
	BOOST_CHECK(pIndProvider != nullptr);
	SourceType *pVarProvider = this->test_variable_provider();
	BOOST_CHECK(pVarProvider != nullptr);
	MatriceDataType *pData = this->get_test_data();
	STRINGTYPE name = pData->name();
	//
	pcancelflag pCancel = this->get_cancelflag();
	BOOST_CHECK(pCancel != nullptr);
	PBackgrounder pBack = this->get_backgrounder();
	BOOST_CHECK(pBack != nullptr);
	//
	std::thread t1([&]() {
				MatElemType oInd(DispositionType::indiv,pCancel,pBack,infologger2);
				oInd.sigle(name);
				oInd.arrange(pIndProvider);
			});
	//
	std::thread t2([&]() {
				MatElemType oVar(DispositionType::variable,pCancel,pBack,infologger2);
				oVar.sigle(name);
				oVar.arrange(pVarProvider);
			});
	t1.join();
	t2.join();
} //testTestMatElem
#endif
BOOST_AUTO_TEST_SUITE_END();


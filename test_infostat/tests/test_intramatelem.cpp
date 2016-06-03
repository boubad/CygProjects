/*
 * test_intramatelem.cpp
 *
 *  Created on: 31 mai 2016
 *      Author: boubad
 */
#include "stdafx.h"
#include <boost/test/unit_test.hpp>
 /////////////////////////
#include <intramat.h>
#include <activeobject.h>
/////////////////////////////
#include <mytestfixture.h>
#include <mytestvariablefixture.h>
////////////////////////////////////
#include <global_defs.h>
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using MyFixture = MyTestFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
using MyVariableFixture = MyTestVariableFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
/////////////////////////////////////
using IndivType = typename MyFixture::IndivType;
using DataMap = typename MyFixture::DataMap;
using IndivTypePtr = typename MyFixture::IndivTypePtr;
using SourceType = typename MyFixture::SourceType;
////////////////////////////////
using MatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
using DistanceMapType = typename MatElemType::DistanceMapType;
using ints_vector = typename MatElemType::ints_vector;
using sizets_vector = typename MatElemType::sizets_vector;
using MatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE>;
using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
using MatOrdType = IntraMatOrd<IDTYPE, DISTANCETYPE, STRINGTYPE>;
using IntraMatOrdResultType = IntraMatOrdResult<IDTYPE, DISTANCETYPE>;
using IntraMatOrdResultTypePtr = std::shared_ptr<IntraMatOrdResultType>;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IntraMatElemTestSuite, MyFixture)
BOOST_AUTO_TEST_CASE(testMortalMatElemIntra) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	MatElemType oMat;
	auto conn = oMat.connect([&](MatElemResultPtr oCrit) {
		MatElemResultType *pCrit = oCrit.get();
		if (pCrit != nullptr) {
			//DISTANCETYPE varCrit = pCrit->first;
			STRINGTYPE s;
			write_vector(pCrit->second, s);
			//BOOST_TEST_MESSAGE("Criteria: " << varCrit << ", \t" << s);
		} // pCrit
	});
	//
	auto t1 = std::thread([&]() {
		oMat.arrange(pProvider);
	});
	t1.join();
	//
	//oMat.arrange(pProvider);
	sizets_vector oIndexes;
	oMat.indexes(oIndexes);
	//BOOST_TEST_MESSAGE("============ RESULTAT MORTAL =============");
	STRINGTYPE ss;
	//DISTANCETYPE crit = oMat.criteria();
	write_vector(oIndexes, ss);
	//BOOST_TEST_MESSAGE("Criteria: " << crit << ", \t" << ss);
	//

} //testMortalMatElem
BOOST_AUTO_TEST_CASE(testMortalMatElemIntraIntraMatOrd) {
	//
	SourceType *pIndsProvider = this->mortal_source();
	BOOST_CHECK(pIndsProvider != nullptr);
	//
	MyVariableFixture oVarFixture;
	SourceType *pVarProvider = oVarFixture.mortal_source();
	BOOST_CHECK(pVarProvider != nullptr);
	//
	//
	MatOrdType oMat;
	auto conn =
		oMat.connect(
			[&](IntraMatOrdResultTypePtr oCrit) {
		IntraMatOrdResultType *px = oCrit.get();
		if (px != nullptr) {
			MatElemResultPtr oVarCrit = px->variables_results();
			MatElemResultPtr oIndCrit = px->indivs_results();
			MatElemResultType *pVarCrit = oVarCrit.get();
			MatElemResultType *pIndCrit = oIndCrit.get();
			if ((pIndCrit != nullptr) && (pVarCrit != nullptr)) {
				DISTANCETYPE varCrit = pVarCrit->first;
				DISTANCETYPE indCrit = pIndCrit->first;
				STRINGTYPE s;
				STRINGTYPE ss;
				write_vector(pVarCrit->second, ss);
				write_vector(pIndCrit->second, s);
				BOOST_TEST_MESSAGE("Ind: " << indCrit << ",\t" << s << ",\tVar: " << varCrit << ", " << ss);
			}
			else if (pVarCrit != nullptr) {
				DISTANCETYPE varCrit = pVarCrit->first;
				STRINGTYPE s;
				write_vector(pVarCrit->second, s);
				BOOST_TEST_MESSAGE("Var: " << varCrit << ", \t" << s);
			}
			else if (pIndCrit != nullptr) {
				DISTANCETYPE indCrit = pIndCrit->first;
				STRINGTYPE s;
				write_vector(pIndCrit->second, s);
				BOOST_TEST_MESSAGE("Ind: " << indCrit << ", \t" << s);
			}
		}// px
	});
	oMat.arrange(pIndsProvider, pVarProvider);
} //testMortalMatElemIntraIntraMatOrd
//
#ifdef MYTOTO
BOOST_AUTO_TEST_CASE(testTesMatElemIntraIntraMatOrd) {
	//
	SourceType *pIndsProvider = this->test_source();
	BOOST_CHECK(pIndsProvider != nullptr);
	//
	MyVariableFixture oVarFixture;
	SourceType *pVarProvider = oVarFixture.test_source();
	BOOST_CHECK(pVarProvider != nullptr);
	//
	//
	Backgrounder oBack;
	MatOrdType oMat(&oBack);
	auto conn =
		oMat.connect(
			[&](MatElemResultPtr oIndCrit, MatElemResultPtr oVarCrit) {
		MatElemResultType *pVarCrit = oVarCrit.get();
		MatElemResultType *pIndCrit = oIndCrit.get();
		if ((pIndCrit != nullptr) && (pVarCrit != nullptr)) {
			DISTANCETYPE varCrit = pVarCrit->first;
			DISTANCETYPE indCrit = pIndCrit->first;
			BOOST_TEST_MESSAGE("Ind: " << indCrit << ",\tVar: " << varCrit);
		}
		else if (pVarCrit != nullptr) {
			DISTANCETYPE varCrit = pVarCrit->first;
			BOOST_TEST_MESSAGE("Var: " << varCrit);
		}
		else if (pIndCrit != nullptr) {
			DISTANCETYPE indCrit = pIndCrit->first;
			BOOST_TEST_MESSAGE("Ind: " << indCrit);
		}
	});
	oMat.arrange(pIndsProvider, pVarProvider);
} //testMortalMatElemIntraIntraMatOrd
#endif
//
BOOST_AUTO_TEST_SUITE_END();


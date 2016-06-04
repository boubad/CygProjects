/*
 * test_matord.cpp
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <matrice.h>
/////////////////////////////
#include "mytestfixture.h"
#include "mytestvariablefixture.h"
////////////////////////////////////
#include "global_defs.h"
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using MyIndivFixture = MyTestFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
using MyVariableFixture = MyTestVariableFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
/////////////////////////////////////
using MatOrdType = MatOrd<IDTYPE,DISTANCETYPE,STRINGTYPE>;
////////////////////////////////
using MatElemResultType = typename MatOrdType::MatElemResultType;
using MatElemResultPtr = typename MatOrdType::MatElemResultPtr;
using ConnectionType =typename MatOrdType::ConnectionType;
using SourceType = typename MatOrdType::SourceType;
using ints_vector = typename MatOrdType::ints_vector;
using IndivType = typename MatOrdType::IndivType;
using IndivTypePtr = typename MatOrdType::IndivTypePtr;
//////////////////////////////////////////
BOOST_AUTO_TEST_SUITE(MatOrdTestSuite)
BOOST_AUTO_TEST_CASE(testMortalMatOrd) {
	//BOOST_TEST_MESSAGE("================= MORTAL =======================");
	//
	MyIndivFixture oIndFixture;
	SourceType *pIndivProvider = oIndFixture.mortal_source();
	BOOST_CHECK(pIndivProvider != nullptr);
	//
	MyVariableFixture oVarFixture;
	SourceType *pVarProvider = oVarFixture.mortal_source();
	BOOST_CHECK(pVarProvider != nullptr);
	//
	MatOrdType oMat;
	auto conn =
			oMat.connect(
					[&](MatElemResultPtr oIndCrit, MatElemResultPtr oVarCrit) {
						MatElemResultType *pVarCrit = oVarCrit.get();
						MatElemResultType *pIndCrit = oIndCrit.get();
						if ((pIndCrit != nullptr) && (pVarCrit != nullptr)) {
							DISTANCETYPE varCrit = pVarCrit->first;
							DISTANCETYPE indCrit = pIndCrit->first;
							STRINGTYPE s;
							STRINGTYPE ss;
							write_vector(pVarCrit->second, ss);
							write_vector(pIndCrit->second, s);
						//	BOOST_TEST_MESSAGE("Ind: " << indCrit << ",\t" << s << ",\tVar: " << varCrit << ", " << ss);
						} else if (pVarCrit != nullptr) {
							DISTANCETYPE varCrit = pVarCrit->first;
							STRINGTYPE s;
							write_vector(pVarCrit->second, s);
						//	BOOST_TEST_MESSAGE("Var: " << varCrit << ", \t" << s);
						} else if (pIndCrit != nullptr) {
							DISTANCETYPE indCrit = pIndCrit->first;
							STRINGTYPE s;
							write_vector(pIndCrit->second, s);
							//BOOST_TEST_MESSAGE("Ind: " << indCrit << ", \t" << s);
						}
					});
	bool bRet = oMat.process(pIndivProvider, pVarProvider);
	BOOST_CHECK(bRet);
	ints_vector varids, indids;
	oMat.get_ids(indids, varids);
//	BOOST_TEST_MESSAGE("========================================");
//	BOOST_TEST_MESSAGE("VARIABLES");
	for (auto &aIndex : varids) {
		IndivTypePtr oInd = pVarProvider->find(aIndex);
		IndivType *p = oInd.get();
		if (p != nullptr) {
			STRINGTYPE s = p->sigle();
			//BOOST_TEST_MESSAGE(s);
		} // p
	} // varIndex
	//BOOST_TEST_MESSAGE("========================================");
	//BOOST_TEST_MESSAGE("INDIVS");
	for (auto &aIndex : indids) {
		IndivTypePtr oInd = pIndivProvider->find(aIndex);
		IndivType *p = oInd.get();
		if (p != nullptr) {
			STRINGTYPE s = p->sigle();
			//BOOST_TEST_MESSAGE(s);
		} // p
	} // varIndex
} //testMortalMatElem
BOOST_AUTO_TEST_CASE(testCONSOMatOrd) {
	//BOOST_TEST_MESSAGE("================= CONSO =======================");
	//
	MyIndivFixture oIndFixture;
	SourceType *pIndivProvider = oIndFixture.conso_source();
	BOOST_CHECK(pIndivProvider != nullptr);
	//
	MyVariableFixture oVarFixture;
	SourceType *pVarProvider = oVarFixture.conso_source();
	BOOST_CHECK(pVarProvider != nullptr);
	//
	MatOrdType oMat;
	auto conn =
			oMat.connect(
					[&](MatElemResultPtr oIndCrit, MatElemResultPtr oVarCrit) {
						MatElemResultType *pVarCrit = oVarCrit.get();
						MatElemResultType *pIndCrit = oIndCrit.get();
						if ((pIndCrit != nullptr) && (pVarCrit != nullptr)) {
							DISTANCETYPE varCrit = pVarCrit->first;
							DISTANCETYPE indCrit = pIndCrit->first;
							STRINGTYPE s;
							STRINGTYPE ss;
							write_vector(pVarCrit->second, ss);
							write_vector(pIndCrit->second, s);
						//	BOOST_TEST_MESSAGE("Ind: " << indCrit << ",\t" << s << ",\tVar: " << varCrit << ", " << ss);
						} else if (pVarCrit != nullptr) {
							DISTANCETYPE varCrit = pVarCrit->first;
							STRINGTYPE s;
							write_vector(pVarCrit->second, s);
							//BOOST_TEST_MESSAGE("Var: " << varCrit << ", \t" << s);
						} else if (pIndCrit != nullptr) {
							DISTANCETYPE indCrit = pIndCrit->first;
							STRINGTYPE s;
							write_vector(pIndCrit->second, s);
							//BOOST_TEST_MESSAGE("Ind: " << indCrit << ", \t" << s);
						}
					});
	bool bRet = oMat.process(pIndivProvider, pVarProvider);
	BOOST_CHECK(bRet);
	ints_vector varids, indids;
	oMat.get_ids(indids, varids);
	//BOOST_TEST_MESSAGE("========================================");
	//BOOST_TEST_MESSAGE("VARIABLES");
	for (auto &aIndex : varids) {
		IndivTypePtr oInd = pVarProvider->find(aIndex);
		IndivType *p = oInd.get();
		if (p != nullptr) {
			STRINGTYPE s = p->sigle();
		//	BOOST_TEST_MESSAGE(s);
		} // p
	} // varIndex
	//BOOST_TEST_MESSAGE("========================================");
	//BOOST_TEST_MESSAGE("INDIVS");
	for (auto &aIndex : indids) {
		IndivTypePtr oInd = pIndivProvider->find(aIndex);
		IndivType *p = oInd.get();
		if (p != nullptr) {
			STRINGTYPE s = p->sigle();
		//	BOOST_TEST_MESSAGE(s);
		} // p
	} // varIndex
} //testMortalMatElem
BOOST_AUTO_TEST_SUITE_END();


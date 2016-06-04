/*
* test_intramatelem.cpp
*
*  Created on: 31 mai 2016
*      Author: boubad
*/
#include "stdafx.h"
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <anacompoindivprovider.h>
/////////////////////////////
#include <mytestfixture.h>
#include <mytestvariablefixture.h>
#include <infotestdata.h>
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
using CreatorType = AnaCompoIndivSourceCreator<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
using DataVectorIndivSourceType = DataVectorIndivSource<IDTYPE, STRINGTYPE>;
using DataVectorIndivSourcePtr = std::shared_ptr<DataVectorIndivSourceType>;
using IStoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
using DataVectorIndivSourceTuple = std::tuple<DataVectorIndivSourcePtr, DataVectorIndivSourcePtr>;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(AnaCompoTestSuite, MyFixture)
BOOST_AUTO_TEST_CASE(testCReate) {
	//
	SourceType *pIndsProvider = this->mortal_source();
	BOOST_CHECK(pIndsProvider != nullptr);
	//
	MyVariableFixture oVarFixture;
	SourceType *pVarProvider = oVarFixture.mortal_source();
	BOOST_CHECK(pVarProvider != nullptr);
	//
	STRINGTYPE sigle;
	InfoTestData::get_mortal_name(sigle);
	//
	IStoreType *pStore = this->get_store();
	DataVectorIndivSourceTuple r = CreatorType::create(pStore, sigle);
	DataVectorIndivSourcePtr oInd = std::get<0>(r);
	DataVectorIndivSourcePtr oVar = std::get<1>(r);
	DataVectorIndivSourceType *pSourceInd = oInd.get();
	BOOST_CHECK(pSourceInd != nullptr);
	DataVectorIndivSourceType *pSourceVar = oVar.get();
	BOOST_CHECK(pSourceVar != nullptr);
	//
	SourceType *pProvider = pSourceInd;
	size_t nCount = pProvider->count();
	BOOST_CHECK(nCount > 0);
	size_t nbVars = 0;
	for (size_t i = 0; i < nCount; ++i) {
		IndivTypePtr oInd = pProvider->get(i);
		IndivType *pIndiv = oInd.get();
		BOOST_CHECK(pIndiv != nullptr);
		IDTYPE aIndex = pIndiv->id();
		BOOST_CHECK(aIndex != 0);
		IndivTypePtr xInd = pProvider->find(aIndex);
		IndivType *pIndiv2 = xInd.get();
		BOOST_CHECK(pIndiv2 != nullptr);
		const DataMap &oCenter = pIndiv->center();
		BOOST_CHECK(oCenter.size() > 0);
		BOOST_CHECK(pIndiv->has_numeric_fields());
		if (nbVars == 0) {
			nbVars = oCenter.size();
		}
		BOOST_CHECK(nbVars == oCenter.size());
	} //i
	pProvider->reset();
	size_t nc = 0;
	do {
		IndivTypePtr oInd = pProvider->next();
		IndivType *pIndiv = oInd.get();
		if (pIndiv == nullptr) {
			break;
		}
		++nc;
	} while (true);
	BOOST_CHECK(nc == nCount);
	//
	pProvider = pSourceVar;
	nCount = pProvider->count();
	BOOST_CHECK(nCount > 0);
	nbVars = 0;
	for (size_t i = 0; i < nCount; ++i) {
		IndivTypePtr oInd = pProvider->get(i);
		IndivType *pIndiv = oInd.get();
		BOOST_CHECK(pIndiv != nullptr);
		IDTYPE aIndex = pIndiv->id();
		BOOST_CHECK(aIndex != 0);
		IndivTypePtr xInd = pProvider->find(aIndex);
		IndivType *pIndiv2 = xInd.get();
		BOOST_CHECK(pIndiv2 != nullptr);
		const DataMap &oCenter = pIndiv->center();
		BOOST_CHECK(oCenter.size() > 0);
		BOOST_CHECK(pIndiv->has_numeric_fields());
		if (nbVars == 0) {
			nbVars = oCenter.size();
		}
		BOOST_CHECK(nbVars == oCenter.size());
	} //i
	pProvider->reset();
	nc = 0;
	do {
		IndivTypePtr oInd = pProvider->next();
		IndivType *pIndiv = oInd.get();
		if (pIndiv == nullptr) {
			break;
		}
		++nc;
	} while (true);
	BOOST_CHECK(nc == nCount);
	//
} //testMortalMatElem

BOOST_AUTO_TEST_SUITE_END();


/*
 * test_variableprovider.cpp
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include "mytestvariablefixture.h"
////////////////////////////////////
#include "global_defs.h"
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using MyFixture = MyTestVariableFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
/////////////////////////////////////
using IndivType = typename MyFixture::IndivType;
using DataMap = typename MyFixture::DataMap;
using IndivTypePtr = typename MyFixture::IndivTypePtr;
using SourceType = typename MyFixture::SourceType;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(VariableProviderTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalProviderVariable) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
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
} //testMortalProvider
BOOST_AUTO_TEST_CASE(testConsoProviderVariable) {
	SourceType *pProvider = this->conso_source();
	BOOST_CHECK(pProvider != nullptr);
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
} //testConsoProvider
BOOST_AUTO_TEST_CASE(testTestProviderVariable) {
	SourceType *pProvider = this->test_source();
	BOOST_CHECK(pProvider != nullptr);
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
} //testTestProvider
BOOST_AUTO_TEST_SUITE_END();






#include <boost/test/unit_test.hpp>
/////////////////////////////
#include "storeindivprovider_fixture.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
//////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IndivProviderTestSuite, StoreIndivSorceFixture<>);
BOOST_AUTO_TEST_CASE(testRandomAccess) {
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	//
	size_t count = pMan->count();
	BOOST_CHECK(count == m_nbrows);
	for (size_t i = 0; i < count; ++i) {
		IndivTypePtr oInd = pMan->get(i);
		IndivType *p = oInd.get();
		BOOST_CHECK(p != nullptr);
		IndexType aIndex = p->id();
		BOOST_CHECK(aIndex != 0);
		DataMap oCenter = p->center();
		BOOST_CHECK(oCenter.size() == m_nbcols);
	}// i
} //testRandomAccess
BOOST_AUTO_TEST_CASE(testSerialAccess) {
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	//
	size_t count = 0;
	pMan->reset();
	do {
		IndivTypePtr oInd = pMan->next();
		IndivType *p = oInd.get();
		if (p == nullptr) {
			break;
		}
		++count;
		IndexType aIndex = p->id();
		BOOST_CHECK(aIndex != 0);
		DataMap oCenter = p->center();
		BOOST_CHECK(oCenter.size() == m_nbcols);
	} while (true);
	BOOST_CHECK(count == m_nbrows);
} // testSerialAccess
BOOST_AUTO_TEST_CASE(testDistances) {
	IndivSourceType *pMan = get_source();
	BOOST_CHECK(pMan != nullptr);
	//
	size_t count = pMan->count();
	BOOST_CHECK(count == m_nbrows);
	for (size_t i = 0; i < count; ++i) {
		IndivTypePtr oInd1 = pMan->get(i);
		IndivType *p1 = oInd1.get();
		BOOST_CHECK(p1 != nullptr);
		IndexType aIndex1 = p1->id();
		BOOST_CHECK(aIndex1 != 0);
		DataMap oCenter1 = p1->center();
		BOOST_CHECK(oCenter1.size() == m_nbcols);
		for (size_t j = 0; j < i; ++j) {
			IndivTypePtr oInd2 = pMan->get(j);
			IndivType *p2 = oInd2.get();
			BOOST_CHECK(p2 != nullptr);
			IndexType aIndex2 = p2->id();
			BOOST_CHECK(aIndex2 != 0);
			DataMap oCenter2 = p2->center();
			BOOST_CHECK(oCenter2.size() == m_nbcols);
			double d = 0.0;
			bool bRet = p1->distance(*p2, d);
			BOOST_CHECK(bRet);
			BOOST_CHECK(d > 0);
			//BOOST_TEST_MESSAGE("Ind" << aIndex1 << " --> Ind" << aIndex2 << " \t" << d);
		}// j
	}// i
} // testDistances
BOOST_AUTO_TEST_SUITE_END();

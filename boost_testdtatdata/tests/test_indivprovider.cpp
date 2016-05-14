#include <boost/test/unit_test.hpp>
/////////////////////////////
#include "databasefixture.h"
//////////////////////////////////
#include <storeindivprovider.h>
#include <numericindivprovider.h>
////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
using namespace std;
////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IndivProviderTestSuite, DatabaseFixture);
BOOST_AUTO_TEST_CASE(StoreIndivProviderTest)
{
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	StoreIndivProvider oProvider(pMan, oSet);
	BOOST_CHECK(oProvider.is_valid());
	//
	size_t nc = 0;
	bool bRet = oProvider.indivs_count(nc);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == nc);
	//
	variables_map vars;
	bRet = oProvider.get_variables_map(vars);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == vars.size());
	//
	for (size_t i = 0; i < nc; ++i) {
		Indiv oInd;
		bRet = oProvider.find_indiv_at(i, oInd);
		BOOST_CHECK(bRet);
		IntType aIndex = oInd.id();
		BOOST_CHECK(aIndex != 0);
	} // i
}//StoreIndivProviderTest
BOOST_AUTO_TEST_CASE(testSerialIndivProvider) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	SerialStoreIndivProvider oProvider(pMan, oSet);
	BOOST_CHECK(oProvider.is_valid());
	//
	bool bRet = oProvider.reset();
	BOOST_CHECK(bRet);
	size_t nc = 0;
	do {
		Indiv oInd1;
		bRet = oProvider.next(oInd1);
		if (!bRet) {
			BOOST_CHECK(m_nbrows == nc);
			BOOST_CHECK(oInd1.id() == 0);
			break;
		}
		++nc;
		IntType aIndex1 = oInd1.id();
		BOOST_CHECK(aIndex1 != 0);
		Indiv oInd2;
		bRet = oProvider.next(oInd2);
		if (!bRet) {
			BOOST_CHECK(m_nbrows == nc);
			BOOST_CHECK(oInd2.id() == 0);
			break;
		}
		++nc;
		IntType aIndex2 = oInd1.id();
		BOOST_CHECK(aIndex2 != 0);
		double d1 = oInd1.distance(oInd2);
		BOOST_CHECK(d1 >= 0);
		double d2 = oInd2.distance(oInd1);
		BOOST_CHECK(d2 >= 0);
		BOOST_CHECK(d1 == d2);
	} while (true);
} //testSerialIndivProvider
BOOST_AUTO_TEST_CASE(testNumIndivProvider) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	StoreIndivProvider oProviderBase(pMan, oSet);
	BOOST_CHECK(oProviderBase.is_valid());
	NumericIndivProvider oProvider(&oProviderBase);
	BOOST_CHECK(oProvider.is_valid());
	//
	size_t nc = 0;
	bool bRet = oProvider.indivs_count(nc);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == nc);
	//
	variables_map vars;
	bRet = oProvider.get_variables_map(vars);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == vars.size());
	ints_vector oIds;
	bRet = oProvider.get_variables_ids(oIds);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == oIds.size());
	for (size_t i = 0; i < m_nbcols; ++i) {
		IntType key = oIds[i];
		StatInfo oInfo;
		bRet = oProvider.get_statinfo(key, oInfo);
		BOOST_CHECK(bRet);
	} // i
	  //
	for (size_t i = 0; i < nc; ++i) {
		Indiv oInd;
		bRet = oProvider.find_indiv_at(i, oInd);
		BOOST_CHECK(bRet);
		IntType aIndex = oInd.id();
		BOOST_CHECK(aIndex != 0);
		doubles_vector vv1, vv2;
		bRet = oProvider.find_indiv(aIndex, vv1);
		BOOST_CHECK(aIndex != 0);
		bRet = oProvider.find_indiv_at(i, aIndex, vv1);
		BOOST_CHECK(aIndex != 0);
	} // i
} //testNumIndivProvider
BOOST_AUTO_TEST_SUITE_END();

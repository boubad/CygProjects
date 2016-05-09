/*
 * test_sqliteprovider.cpp
 *
 *  Created on: 7 mai 2016
 *      Author: boubad
 */

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
/////////////////////////////////////
#include <boost/foreach.hpp>
//////////////////////////////////////////
#include <sqlitestathelper.h>
#include <storeindivprovider.h>
#include <statinfo.h>
#include <numericindivprovider.h>
#include <crititem.h>
///////////////////////////////////
#include <memory>
//////////////////////////////////
#include "infotestdata.h"
////////////////////////////////
using namespace info;
using namespace CppUnit;
////////////////////////////
class TestSQLiteProvider: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(TestSQLiteProvider);
	CPPUNIT_TEST(testDatasets);
	CPPUNIT_TEST(testVariables);
	CPPUNIT_TEST(testIndivs);
	CPPUNIT_TEST(testValues);
	CPPUNIT_TEST(testIndivProvider);
	CPPUNIT_TEST(testSerialIndivProvider);
	CPPUNIT_TEST(testStatInfo);
	CPPUNIT_TEST(testNumIndivProvider);
	CPPUNIT_TEST(testComputeDistances);CPPUNIT_TEST_SUITE_END()
	;
public:
	TestSQLiteProvider();
	~TestSQLiteProvider();
	void setUp(void);
	void tearDown(void);
protected:
	void testDatasets(void);
	void testVariables(void);
	void testIndivs(void);
	void testValues(void);
	void testIndivProvider(void);
	void testSerialIndivProvider(void);
	void testStatInfo(void);
	void testNumIndivProvider(void);
	void testComputeDistances(void);
private:
	size_t m_nbcols;
	size_t m_nbrows;
	std::unique_ptr<SQLiteStatHelper> m_man;
	DBStatDataset m_oset;
	std::string m_filename;
	std::string m_name;
	std::vector<std::string> m_colnames;
	std::vector<std::string> m_rownames;
	std::vector<int> m_data;
};
//class TestSQLiteProvider
/////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(TestSQLiteProvider);
//////////////////////////////////////
TestSQLiteProvider::TestSQLiteProvider() :
		m_nbcols(0), m_nbrows(0) {
	InfoTestData::get_mortal_data(m_name, m_nbrows, m_nbcols, m_data,
			m_rownames, m_colnames);
	InfoTestData::get_database_filename(m_filename);
	CPPUNIT_ASSERT(!m_name.empty());
	CPPUNIT_ASSERT(!m_filename.empty());
	CPPUNIT_ASSERT(m_nbrows > 2);
	CPPUNIT_ASSERT(m_nbcols > 2);
	CPPUNIT_ASSERT(m_colnames.size() >= m_nbcols);
	CPPUNIT_ASSERT(m_rownames.size() >= m_nbrows);
	CPPUNIT_ASSERT(m_data.size() >= (size_t )(m_nbcols * m_nbrows));
}
TestSQLiteProvider::~TestSQLiteProvider() {

}
void TestSQLiteProvider::setUp(void) {
	bool bRet = false;
	this->m_man = std::make_unique < SQLiteStatHelper > (this->m_filename);
	//this->m_man.reset(new SQLiteStatHelper(m_filename));
	IStoreHelper *p = this->m_man.get();
	CPPUNIT_ASSERT(p != nullptr);
	CPPUNIT_ASSERT(p->is_valid());
	DBStatDataset &oSet = this->m_oset;
	oSet.set_sigle(this->m_name);
	if (!p->find_dataset(oSet)) {
		bRet = p->maintains_dataset(oSet);
		CPPUNIT_ASSERT(bRet);
	}
	CPPUNIT_ASSERT(oSet.id() != 0);
	variables_vector oVars;
	for (size_t i = 0; i < m_nbcols; ++i) {
		std::string sigle = m_colnames[i];
		DBStatVariable v(oSet, sigle);
		if (!p->find_variable(v)) {
			oVars.push_back(v);
		}
	} // i
	if (!oVars.empty()) {
		bRet = p->maintains_variables(oVars);
		CPPUNIT_ASSERT(bRet);
	}
	indivs_vector oInds;
	for (size_t i = 0; i < m_nbrows; ++i) {
		std::string sigle = m_rownames[i];
		DBStatIndiv v(oSet, sigle);
		if (!p->find_indiv(v)) {
			oInds.push_back(v);
		}
	} // i
	if (!oInds.empty()) {
		bRet = p->maintains_indivs(oInds);
		CPPUNIT_ASSERT(bRet);
	}
	oInds.clear();
	bRet = p->find_dataset_indivs(oSet, oInds, 0, m_nbrows);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbrows == oInds.size());
	oVars.clear();
	bRet = p->find_dataset_variables(oSet, oVars, 0, m_nbcols);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbcols == oVars.size());
	//
	boost::container::flat_map<std::string, DBStatVariable *> pVars;
	BOOST_FOREACH(const std::string &s, this->m_colnames){
	std::string sigle = s;
	std::string rsigle;
	DBStatVariable ovar(this->m_oset, sigle);
	ovar.get_sigle(rsigle);
	DBStatVariable *p = nullptr;
	for (size_t i = 0; i < oVars.size(); ++i) {
		DBStatVariable &vv = oVars[i];
		std::string sx;
		vv.get_sigle(sx);
		if (sx == rsigle) {
			p = &vv;
			break;
		}
	} // i
	CPPUNIT_ASSERT(p != nullptr);
	pVars[sigle] = p;
}
	boost::container::flat_map<std::string, DBStatIndiv *> pInds;
	BOOST_FOREACH(const std::string &s, this->m_rownames){
	std::string sigle = s;
	std::string rsigle;
	DBStatIndiv ovar(this->m_oset, sigle);
	ovar.get_sigle(rsigle);
	DBStatIndiv *p = nullptr;
	for (size_t i = 0; i < oInds.size(); ++i) {
		DBStatIndiv &vv = oInds[i];
		std::string sx;
		vv.get_sigle(sx);
		if (sx == rsigle) {
			p = &vv;
			break;
		}
	} // i
	CPPUNIT_ASSERT(p != nullptr);
	pInds[sigle] = p;
}
	values_vector oVals;
	for (size_t i = 0; i < m_nbrows; ++i) {
		std::string sigleind = m_rownames[i];
		DBStatIndiv *pInd = pInds[sigleind];
		CPPUNIT_ASSERT(pInd != nullptr);
		for (size_t j = 0; j < m_nbcols; ++j) {
			std::string siglevar = m_colnames[j];
			DBStatVariable *pVar = pVars[siglevar];
			CPPUNIT_ASSERT(pVar != nullptr);
			DBStatValue val(*pVar, *pInd);
			if (!p->find_value(val)) {
				float f = (float) m_data[i * m_nbcols + j];
				DbValue vv(f);
				val.set_value(vv);
				oVals.push_back(val);
			}
		} // j
	} // i
	if (!oVals.empty()) {
		bRet = p->maintains_values(oVals);
		CPPUNIT_ASSERT(bRet);
	}
} // setuPu
void TestSQLiteProvider::tearDown(void) {

}
void TestSQLiteProvider::testDatasets(void) {
	IStoreHelper *pMan = m_man.get();
	//
	int count = 0;
	bool bRet = pMan->find_all_datasets_count(count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(count > 0);
	datasets_vector oSets;
	bRet = pMan->find_all_datasets(oSets, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oSets.size());
	//
	ints_vector oIds;
	bRet = pMan->find_all_datasets_ids(oIds, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oIds.size());
	//
	std::string sigle;
	this->m_oset.get_sigle(sigle);
	DBStatDataset xSet(sigle);
	bRet = pMan->find_dataset(xSet);
	CPPUNIT_ASSERT(bRet);
	bRet = pMan->remove_dataset(xSet);
	CPPUNIT_ASSERT(bRet);
	bRet = pMan->find_dataset(xSet);
	CPPUNIT_ASSERT(!bRet);
} // testDatasets
void TestSQLiteProvider::testVariables(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	int count = 0;
	bool bRet = pMan->find_dataset_variables_count(oSet, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbcols == (size_t )count);
	//
	ints_vector oIds;
	bRet = pMan->find_dataset_variables_ids(oSet, oIds, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oIds.size());
	//
	variables_vector oVars;
	bRet = pMan->find_dataset_variables(oSet, oVars, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oVars.size());
	//
	inttype_string_map oMap;
	bRet = pMan->find_dataset_variables_types(oSet, oMap);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oMap.size());
	//
	DBStatVariable &vv = oVars[0];
	bRet = pMan->find_variable(vv);
	CPPUNIT_ASSERT(bRet);
	bRet = pMan->remove_variable(vv);
	CPPUNIT_ASSERT(bRet);
	bRet = pMan->find_variable(vv);
	CPPUNIT_ASSERT(!bRet);
} // testVariables
void TestSQLiteProvider::testIndivs(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	int count = 0;
	bool bRet = pMan->find_dataset_indivs_count(oSet, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbrows == (size_t )count);
	//
	ints_vector oIds;
	bRet = pMan->find_dataset_indivs_ids(oSet, oIds, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oIds.size());
	//
	indivs_vector oInds;
	bRet = pMan->find_dataset_indivs(oSet, oInds, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oInds.size());
	//
	DBStatIndiv &vv = oInds[0];
	bRet = pMan->find_indiv(vv);
	CPPUNIT_ASSERT(bRet);
	bRet = pMan->remove_indiv(vv);
	CPPUNIT_ASSERT(bRet);
	bRet = pMan->find_indiv(vv);
	CPPUNIT_ASSERT(!bRet);
} // testIndivs
void TestSQLiteProvider::testValues(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	int count = 0;
	int nTotal = (int) (m_nbcols * m_nbrows);
	bool bRet = pMan->find_dataset_values_count(oSet, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(nTotal == count);
	//
	values_vector oVals;
	bRet = pMan->find_dataset_values(oSet, oVals, 0, count);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT((size_t )count == oVals.size());
	//
	variables_vector oListVars;
	bRet = pMan->find_dataset_variables(oSet, oListVars, 0, 1);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(!oListVars.empty());
	DBStatVariable &vv = oListVars[0];
	values_vector vals;
	bRet = pMan->find_variable_values(vv, vals, 0, 1000);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbrows == vals.size());
	strings_vector ss;
	bRet = pMan->find_variable_distinct_values(vv, ss);
	CPPUNIT_ASSERT(bRet);
	//
	indivs_vector oListInds;
	bRet = pMan->find_dataset_indivs(oSet, oListInds, 0, 1);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(!oListInds.empty());
	DBStatIndiv &vi = oListInds[0];
	vals.clear();
	bRet = pMan->find_indiv_values(vi, vals, 0, 1000);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbcols == vals.size());
	//
	size_t nb = oVals.size();
	DbValue vempty;
	for (size_t i = 0; i < nb; ++i) {
		DBStatValue &v = oVals[i];
		int nx = (int) i % 2;
		if (nx == 0) {
			v.set_value(vempty);
		}
	} // i
	bRet = pMan->maintains_values(oVals);
	CPPUNIT_ASSERT(bRet);
	int nz = 0;
	bRet = pMan->find_dataset_values_count(oSet, nz);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(nz < count);
} // testValues
void TestSQLiteProvider::testIndivProvider(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	StoreIndivProvider oProvider(pMan, oSet);
	CPPUNIT_ASSERT(oProvider.is_valid());
	//
	size_t nc = 0;
	bool bRet = oProvider.indivs_count(nc);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbrows == nc);
	//
	variables_map vars;
	bRet = oProvider.get_variables_map(vars);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbcols == vars.size());
	//
	for (size_t i = 0; i < nc; ++i) {
		Indiv oInd;
		bRet = oProvider.find_indiv_at(i, oInd);
		CPPUNIT_ASSERT(bRet);
		IntType aIndex = oInd.id();
		CPPUNIT_ASSERT(aIndex != 0);
	} // i
} // testIndivProvider
void TestSQLiteProvider::testSerialIndivProvider(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	SerialStoreIndivProvider oProvider(pMan, oSet);
	CPPUNIT_ASSERT(oProvider.is_valid());
	//
	bool bRet = oProvider.reset();
	CPPUNIT_ASSERT(bRet);
	size_t nc = 0;
	do {
		Indiv oInd1;
		bRet = oProvider.next(oInd1);
		if (!bRet) {
			CPPUNIT_ASSERT(m_nbrows == nc);
			CPPUNIT_ASSERT(oInd1.id() == 0);
			break;
		}
		++nc;
		IntType aIndex1 = oInd1.id();
		CPPUNIT_ASSERT(aIndex1 != 0);
		Indiv oInd2;
		bRet = oProvider.next(oInd2);
		if (!bRet) {
			CPPUNIT_ASSERT(m_nbrows == nc);
			CPPUNIT_ASSERT(oInd2.id() == 0);
			break;
		}
		++nc;
		IntType aIndex2 = oInd1.id();
		CPPUNIT_ASSERT(aIndex2 != 0);
		double d1 = oInd1.distance(oInd2);
		CPPUNIT_ASSERT(d1 >= 0);
		double d2 = oInd2.distance(oInd1);
		CPPUNIT_ASSERT(d2 >= 0);
		CPPUNIT_ASSERT(d1 == d2);
	} while (true);
} //testSerialIndivProvider
void TestSQLiteProvider::testStatInfo(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	SerialStoreIndivProvider oProvider(pMan, oSet);
	CPPUNIT_ASSERT(oProvider.is_valid());
	//
	statinfos_map oRes;
	size_t n = info_global_compute_stats(&oProvider, oRes);
	CPPUNIT_ASSERT(n == this->m_nbcols);
	//
} //testStatInfo
void TestSQLiteProvider::testNumIndivProvider(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	StoreIndivProvider oProviderBase(pMan, oSet);
	CPPUNIT_ASSERT(oProviderBase.is_valid());
	NumericIndivProvider oProvider(&oProviderBase);
	CPPUNIT_ASSERT(oProvider.is_valid());
	//
	size_t nc = 0;
	bool bRet = oProvider.indivs_count(nc);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbrows == nc);
	//
	variables_map vars;
	bRet = oProvider.get_variables_map(vars);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbcols == vars.size());
	ints_vector oIds;
	bRet = oProvider.get_variables_ids(oIds);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbcols == oIds.size());
	for (size_t i = 0; i < m_nbcols; ++i) {
		IntType key = oIds[i];
		StatInfo oInfo;
		bRet = oProvider.get_statinfo(key, oInfo);
		CPPUNIT_ASSERT(bRet);
	} // i
	  //
	for (size_t i = 0; i < nc; ++i) {
		Indiv oInd;
		bRet = oProvider.find_indiv_at(i, oInd);
		CPPUNIT_ASSERT(bRet);
		IntType aIndex = oInd.id();
		CPPUNIT_ASSERT(aIndex != 0);
		doubles_vector vv1, vv2;
		bRet = oProvider.find_indiv(aIndex, vv1);
		CPPUNIT_ASSERT(aIndex != 0);
		bRet = oProvider.find_indiv_at(i, aIndex, vv1);
		CPPUNIT_ASSERT(aIndex != 0);
	} // i
} //testNumIndivProvider
void TestSQLiteProvider::testComputeDistances(void) {
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	StoreIndivProvider oProviderBase(pMan, oSet);
	CPPUNIT_ASSERT(oProviderBase.is_valid());
	NumericIndivProvider oProvider(&oProviderBase);
	CPPUNIT_ASSERT(oProvider.is_valid());
	//
	size_t nc = 0;
	bool bRet = oProvider.indivs_count(nc);
	CPPUNIT_ASSERT(bRet);
	CPPUNIT_ASSERT(m_nbrows == nc);
	//
	IndivDistanceMap oDistances;
	info_global_compute_distances(&oProvider, oDistances);
	//
	const ints_set &oSetIndivs = oDistances.indexes();
	CPPUNIT_ASSERT(oSetIndivs.size() == nc);
	//
	for (size_t i = 0; i < nc; ++i) {
		Indiv oInd1;
		bRet = oProvider.find_indiv_at(i, oInd1);
		CPPUNIT_ASSERT(bRet);
		IntType aIndex1 = oInd1.id();
		CPPUNIT_ASSERT(aIndex1 != 0);
		for (size_t j = 0; j < i; ++j) {
			Indiv oInd2;
			bRet = oProvider.find_indiv_at(j, oInd2);
			CPPUNIT_ASSERT(bRet);
			IntType aIndex2 = oInd2.id();
			CPPUNIT_ASSERT(aIndex2 != 0);
			double dRes = 0;
			bRet = oDistances.get(aIndex1, aIndex2, dRes);
			CPPUNIT_ASSERT(bRet);
			CPPUNIT_ASSERT(dRes > 0);
		} // j
	} // i
} //testComputeDistances

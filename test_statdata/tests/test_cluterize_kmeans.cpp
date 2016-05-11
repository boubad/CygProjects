/*
 * test_cluterize_kmeans.cpp
 *
 *  Created on: 10 mai 2016
 *      Author: boubad
 */
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
/////////////////////////////////////
#include <sqlitestathelper.h>
#include <storeindivprovider.h>
#include <indivcluster.h>
//////////////////////////////////
#include <boost/foreach.hpp>
///////////////////////////////////
#include "infotestdata.h"
////////////////////////////////
using namespace info;
using namespace CppUnit;
////////////////////////////
class TestClusterizeKMeans: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(TestClusterizeKMeans);

CPPUNIT_TEST_SUITE_END()
	;
public:
	TestClusterizeKMeans();
	~TestClusterizeKMeans();
	void setUp(void);
	void tearDown(void);
protected:
	void testClusterizeKMeans(void);
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
//class TestClusterizeKMeans
/////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(TestClusterizeKMeans);
//////////////////////////////////////
TestClusterizeKMeans::TestClusterizeKMeans() :
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
TestClusterizeKMeans::~TestClusterizeKMeans() {

}
void TestClusterizeKMeans::setUp(void) {
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
	BOOST_FOREACH(const std::string &s, this->m_colnames) {
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
	BOOST_FOREACH(const std::string &s, this->m_rownames) {
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
void TestClusterizeKMeans::tearDown(void) {
}
void TestClusterizeKMeans::testClusterizeKMeans(void){
	IStoreHelper *pMan = m_man.get();
		DBStatDataset &oSet = this->m_oset;
		//
		StoreIndivProvider oProvider(pMan, oSet);
		CPPUNIT_ASSERT(oProvider.is_valid());
		//
		indivclusters_vector oRes;
		size_t nbClusters = 5;
		size_t nbIters = info_global_clusterize_kmeans(&oProvider,
			nbClusters,oRes);
		CPPUNIT_ASSERT(nbIters > 0);
}//testClusterizeKMeans

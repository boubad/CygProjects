#include <boost/test/unit_test.hpp>
/////////////////////////////
#include "infoglobaldata.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
//////////////////////////////////
class StoreFixture : public InfoGlobalInit {
protected:
	size_t	 m_nbcols;
	size_t	 m_nbrows;
	SQLiteStatHelper::DatasetType m_oset;
public:
	StoreFixture() :m_nbcols(0), m_nbrows(0) {
		SQLiteStatHelper *p = this->m_man.get();
		assert(p != nullptr);
		assert(p->is_valid());
		std::string name;
		InfoTestData::get_mortal_name(name, m_nbrows, m_nbcols);
		assert(!name.empty());
		this->m_oset.sigle(name);
		bool bRet = p->find_dataset(this->m_oset);
		assert(bRet);
		assert(this->m_oset.id() != 0);
	}
	virtual ~StoreFixture() {}
};// class  DatabaseFixture
  ////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IStoreHelperTestSuite, StoreFixture);
BOOST_AUTO_TEST_CASE(testDatasets) {
	SQLiteStatHelper *pMan = m_man.get();
	//
	size_t count = 0;
	bool bRet = pMan->find_all_datasets_count(count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(count > 0);
	SQLiteStatHelper::datasets_vector oSets;
	bRet = pMan->find_all_datasets(oSets, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oSets.size());
	//
	SQLiteStatHelper::ints_vector oIds;
	bRet = pMan->find_all_datasets_ids(oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	std::string sigle = this->m_oset.sigle();
	SQLiteStatHelper::DatasetType xSet(sigle);
	bRet = pMan->find_dataset(xSet);
	BOOST_CHECK(bRet);
	/*
	bRet = pMan->remove_dataset(xSet);
	BOOST_CHECK(bRet);
	bRet = pMan->find_dataset(xSet);
	BOOST_CHECK(!bRet);
	*/
} // testDatasets
BOOST_AUTO_TEST_CASE(testVariables) {
	SQLiteStatHelper *pMan = m_man.get();
	SQLiteStatHelper::DatasetType &oSet = this->m_oset;
	//
	size_t count = 0;
	bool bRet = pMan->find_dataset_variables_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == (size_t)count);
	//
	SQLiteStatHelper::ints_vector oIds;
	bRet = pMan->find_dataset_variables_ids(oSet, oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	SQLiteStatHelper::variables_vector oVars;
	bRet = pMan->find_dataset_variables(oSet, oVars, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oVars.size());
	//
	SQLiteStatHelper::ints_string_map oMap;
	bRet = pMan->find_dataset_variables_types(oSet, oMap);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oMap.size());
	//
	SQLiteStatHelper::VariableType &vv = oVars[0];
	bRet = pMan->find_variable(vv);
	BOOST_CHECK(bRet);
	/*
	bRet = pMan->remove_variable(vv);
	BOOST_CHECK(bRet);
	bRet = pMan->find_variable(vv);
	BOOST_CHECK(!bRet);
	*/
} // testVariables
BOOST_AUTO_TEST_CASE(testIndivs) {
	SQLiteStatHelper *pMan = m_man.get();
	SQLiteStatHelper::DatasetType &oSet = this->m_oset;
	//
	size_t count = 0;
	bool bRet = pMan->find_dataset_indivs_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == (size_t)count);
	//
	SQLiteStatHelper::ints_vector oIds;
	bRet = pMan->find_dataset_indivs_ids(oSet, oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	SQLiteStatHelper::indivs_vector oInds;
	bRet = pMan->find_dataset_indivs(oSet, oInds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oInds.size());
	//
	SQLiteStatHelper::IndivType &vv = oInds[0];
	bRet = pMan->find_indiv(vv);
	BOOST_CHECK(bRet);
	/*
	bRet = pMan->remove_indiv(vv);
	BOOST_CHECK(bRet);
	bRet = pMan->find_indiv(vv);
	BOOST_CHECK(!bRet);
	*/
} // testIndivs
BOOST_AUTO_TEST_CASE(testValues) {
	SQLiteStatHelper *pMan = m_man.get();
	SQLiteStatHelper::DatasetType &oSet = this->m_oset;
	//
	size_t count = 0;
	size_t nTotal = (size_t)(m_nbcols * m_nbrows);
	bool bRet = pMan->find_dataset_values_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(nTotal == count);
	//
	SQLiteStatHelper::values_vector oVals;
	bRet = pMan->find_dataset_values(oSet, oVals, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oVals.size());
	//
	SQLiteStatHelper::variables_vector oListVars;
	bRet = pMan->find_dataset_variables(oSet, oListVars, 0, 1);
	BOOST_CHECK(bRet);
	BOOST_CHECK(!oListVars.empty());
	SQLiteStatHelper::VariableType &vv = oListVars[0];
	SQLiteStatHelper::values_vector vals;
	bRet = pMan->find_variable_values(vv, vals, 0, 1000);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == vals.size());
	SQLiteStatHelper::strings_vector ss;
	bRet = pMan->find_variable_distinct_values(vv, ss);
	BOOST_CHECK(bRet);
	//
	SQLiteStatHelper::indivs_vector oListInds;
	bRet = pMan->find_dataset_indivs(oSet, oListInds, 0, 1);
	BOOST_CHECK(bRet);
	BOOST_CHECK(!oListInds.empty());
	SQLiteStatHelper::IndivType &vi = oListInds[0];
	vals.clear();
	bRet = pMan->find_indiv_values(vi, vals, 0, 1000);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == vals.size());
	/*
	//
	size_t nb = oVals.size();
	DbValue vempty;
	for (size_t i = 0; i < nb; ++i) {
	DBStatValue &v = oVals[i];
	int nx = (int)i % 2;
	if (nx == 0) {
	v.set_value(vempty);
	}
	} // i
	bRet = pMan->maintains_values(oVals);
	BOOST_CHECK(bRet);
	int nz = 0;
	bRet = pMan->find_dataset_values_count(oSet, nz);
	BOOST_CHECK(bRet);
	BOOST_CHECK(nz < count);
	*/
} // testValues
BOOST_AUTO_TEST_SUITE_END();

#include <boost/test/unit_test.hpp>
/////////////////////////////
#include "memorystorefixture.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
//////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(MemoryStoreTestSuite, MemoryStoreFixture<>);
BOOST_AUTO_TEST_CASE(testDatasets) {
	IStoreType *pMan = this->get_store();
	//
	size_t count = 0;
	bool bRet = pMan->find_all_datasets_count(count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(count > 0);
	IStoreType::datasets_vector oSets;
	bRet = pMan->find_all_datasets(oSets, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oSets.size());
	//
	IStoreType::ints_vector oIds;
	bRet = pMan->find_all_datasets_ids(oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	std::string sigle;
	InfoTestData::get_mortal_name(sigle);
	IStoreType::DatasetType xSet(sigle);
	bRet = pMan->find_dataset(xSet);
	BOOST_CHECK(bRet);
} // testDatasets
BOOST_AUTO_TEST_CASE(testVariables) {
	IStoreType *pMan = this->get_store();
	IStoreType::DatasetType &oSet = this->m_oset;
	//
	size_t count = 0;
	bool bRet = pMan->find_dataset_variables_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == (size_t)count);
	//
	IStoreType::ints_vector oIds;
	bRet = pMan->find_dataset_variables_ids(oSet, oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	IStoreType::variables_vector oVars;
	bRet = pMan->find_dataset_variables(oSet, oVars, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oVars.size());
	//
	IStoreType::ints_string_map oMap;
	bRet = pMan->find_dataset_variables_types(oSet, oMap);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oMap.size());
	//
	IStoreType::VariableType &vv = oVars[0];
	bRet = pMan->find_variable(vv);
	BOOST_CHECK(bRet);
} // testVariables
BOOST_AUTO_TEST_CASE(testIndivs) {
	IStoreType *pMan = this->get_store();
	IStoreType::DatasetType &oSet = this->m_oset;
	//
	size_t count = 0;
	bool bRet = pMan->find_dataset_indivs_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == (size_t)count);
	//
	IStoreType::ints_vector oIds;
	bRet = pMan->find_dataset_indivs_ids(oSet, oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	IStoreType::indivs_vector oInds;
	bRet = pMan->find_dataset_indivs(oSet, oInds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oInds.size());
	//
	IStoreType::IndivType &vv = oInds[0];
	bRet = pMan->find_indiv(vv);
	BOOST_CHECK(bRet);
} // testIndivs
BOOST_AUTO_TEST_CASE(testValues) {
	IStoreType *pMan = this->get_store();
	IStoreType::DatasetType &oSet = this->m_oset;
	//
	size_t count = 0;
	size_t nTotal = (size_t)(m_nbcols * m_nbrows);
	bool bRet = pMan->find_dataset_values_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(nTotal == count);
	//
	IStoreType::values_vector oVals;
	bRet = pMan->find_dataset_values(oSet, oVals, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oVals.size());
	//
	IStoreType::variables_vector oListVars;
	bRet = pMan->find_dataset_variables(oSet, oListVars, 0, 1);
	BOOST_CHECK(bRet);
	BOOST_CHECK(!oListVars.empty());
	IStoreType::VariableType &vv = oListVars[0];
	IStoreType::values_vector vals;
	bRet = pMan->find_variable_values(vv, vals, 0, 1000);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == vals.size());
	IStoreType::strings_vector ss;
	bRet = pMan->find_variable_distinct_values(vv, ss);
	BOOST_CHECK(bRet);
	//
	IStoreType::indivs_vector oListInds;
	bRet = pMan->find_dataset_indivs(oSet, oListInds, 0, 1);
	BOOST_CHECK(bRet);
	BOOST_CHECK(!oListInds.empty());
	IStoreType::IndivType &vi = oListInds[0];
	vals.clear();
	bRet = pMan->find_indiv_values(vi, vals, 0, 1000);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == vals.size());
} // testValues
BOOST_AUTO_TEST_SUITE_END();

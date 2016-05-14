#include <boost/test/unit_test.hpp>
/////////////////////////////
#include "infoglobaldata.h"
#include "infotestdata.h"
//////////////////////////////////
#include <boost/foreach.hpp>
/////////////////////////////
using namespace info;
using namespace std;
//////////////////////////////////
class StoreFixture : public InfoGlobalInit {
protected:
	size_t	 m_nbcols;
	size_t	 m_nbrows;
	DBStatDataset m_oset;
public:
	StoreFixture():m_nbcols(0),m_nbrows(0) {
		IStoreHelper *p = this->m_man.get();
		assert(p != nullptr);
		assert(p->is_valid());
		string name;
		InfoTestData::get_mortal_name(name, m_nbrows, m_nbcols);
		assert(!name.empty());
		this->m_oset.set_sigle(name);
		bool bRet = p->find_dataset(this->m_oset);
		assert(bRet);
		assert(this->m_oset.id() != 0);
	}
	virtual ~StoreFixture(){}
};// class  DatabaseFixture
////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(IStoreHelperTestSuite, StoreFixture);
BOOST_AUTO_TEST_CASE(testDatasets) {
	IStoreHelper *pMan = m_man.get();
	//
	int count = 0;
	bool bRet = pMan->find_all_datasets_count(count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(count > 0);
	datasets_vector oSets;
	bRet = pMan->find_all_datasets(oSets, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oSets.size());
	//
	ints_vector oIds;
	bRet = pMan->find_all_datasets_ids(oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	std::string sigle;
	this->m_oset.get_sigle(sigle);
	DBStatDataset xSet(sigle);
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
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	int count = 0;
	bool bRet = pMan->find_dataset_variables_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbcols == (size_t)count);
	//
	ints_vector oIds;
	bRet = pMan->find_dataset_variables_ids(oSet, oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	variables_vector oVars;
	bRet = pMan->find_dataset_variables(oSet, oVars, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oVars.size());
	//
	inttype_string_map oMap;
	bRet = pMan->find_dataset_variables_types(oSet, oMap);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oMap.size());
	//
	DBStatVariable &vv = oVars[0];
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
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	int count = 0;
	bool bRet = pMan->find_dataset_indivs_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == (size_t)count);
	//
	ints_vector oIds;
	bRet = pMan->find_dataset_indivs_ids(oSet, oIds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oIds.size());
	//
	indivs_vector oInds;
	bRet = pMan->find_dataset_indivs(oSet, oInds, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oInds.size());
	//
	DBStatIndiv &vv = oInds[0];
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
	IStoreHelper *pMan = m_man.get();
	DBStatDataset &oSet = this->m_oset;
	//
	int count = 0;
	int nTotal = (int)(m_nbcols * m_nbrows);
	bool bRet = pMan->find_dataset_values_count(oSet, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK(nTotal == count);
	//
	values_vector oVals;
	bRet = pMan->find_dataset_values(oSet, oVals, 0, count);
	BOOST_CHECK(bRet);
	BOOST_CHECK((size_t)count == oVals.size());
	//
	variables_vector oListVars;
	bRet = pMan->find_dataset_variables(oSet, oListVars, 0, 1);
	BOOST_CHECK(bRet);
	BOOST_CHECK(!oListVars.empty());
	DBStatVariable &vv = oListVars[0];
	values_vector vals;
	bRet = pMan->find_variable_values(vv, vals, 0, 1000);
	BOOST_CHECK(bRet);
	BOOST_CHECK(m_nbrows == vals.size());
	strings_vector ss;
	bRet = pMan->find_variable_distinct_values(vv, ss);
	BOOST_CHECK(bRet);
	//
	indivs_vector oListInds;
	bRet = pMan->find_dataset_indivs(oSet, oListInds, 0, 1);
	BOOST_CHECK(bRet);
	BOOST_CHECK(!oListInds.empty());
	DBStatIndiv &vi = oListInds[0];
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

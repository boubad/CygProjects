#pragma once
#ifndef __MEMORYSTOREFIXTURE_H__
#define __MEMORYSTOREFIXTURE_H__
////////////////////////////////////
#include <memorystatstore.h>
#include <iostream>
#include <sstream>
/////////////////////////////////
#include "infotestdata.h"
////////////////////////////////
template<typename U>
void test_write_map(const std::map<U, info::InfoValue> &oMap, std::string &ss) {
	info::info_global_write_map(oMap, ss);
} // test_write_map
/////////////////////////////
template<typename IDTYPE = unsigned long, typename INTTYPE = int,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = float>
class MemoryStoreFixture {
protected:
	using MemoryStoreType = info::MemoryStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using IStoreType = info::IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using DatasetType = typename IStoreType::DatasetType;
	//
	size_t m_nbrows;
	size_t m_nbcols;
	std::unique_ptr<MemoryStoreType> m_man;
	IStoreType *get_store(void) {
		IStoreType *p = this->m_man.get();
		assert(p != nullptr);
		assert(p->is_valid());
		return (p);
	}
	DatasetType m_oset;
public:
	MemoryStoreFixture() :
			m_nbrows(0), m_nbcols(0) {
		this->init_data();
		this->fill_mortal_data();
		this->fill_conso_data();
		STRINGTYPE sigle;
		info::InfoTestData::get_mortal_name(sigle);
		this->m_oset.sigle(sigle);
		IStoreType *p = this->m_man.get();
		assert(p != nullptr);
		bool bRet = p->find_dataset(this->m_oset);
		assert(bRet);
		assert(this->m_oset.id() != 0);
	} // init
	virtual ~MemoryStoreFixture() {
		this->data_teardown();
	}
private:
	void data_teardown(void) {
		this->m_man.reset();
	} // data_teardown
	void fill_mortal_data(void) {
		STRINGTYPE name;
		size_t nRows = 0, nCols = 0;
		std::vector<int> gdata;
		std::vector<STRINGTYPE> rowNames, colNames;
		info::InfoTestData::get_mortal_data(name, nRows, nCols, gdata, rowNames,
				colNames);
		assert(!name.empty());
		assert(nRows > 2);
		assert(nCols > 2);
		this->m_nbrows = nRows;
		this->m_nbcols = nCols;
		assert(colNames.size() >= nCols);
		assert(rowNames.size() >= nRows);
		assert(gdata.size() >= (size_t )(nCols * nRows));
		this->import(name, nRows, nCols, gdata, rowNames, colNames);
	} // fill_mortal_data
	void fill_conso_data(void) {
		STRINGTYPE name;
		size_t nRows = 0, nCols = 0;
		std::vector<int> gdata;
		std::vector<STRINGTYPE> rowNames, colNames;
		info::InfoTestData::get_conso_data(name, nRows, nCols, gdata, rowNames,
				colNames);
		assert(!name.empty());
		assert(nRows > 2);
		assert(nCols > 2);
		assert(colNames.size() >= nCols);
		assert(rowNames.size() >= nRows);
		assert(gdata.size() >= (size_t )(nCols * nRows));
		this->import(name, nRows, nCols, gdata, rowNames, colNames);
	} // fill_mortal_data
	void init_data(void) {
		m_man.reset(new MemoryStoreType());
		MemoryStoreType *p = this->m_man.get();
		assert(p != nullptr);
		assert(p->is_valid());
	}
	void import(const std::string &name, size_t nRows, size_t nCols,
			const std::vector<int> &data,
			const std::vector<STRINGTYPE> &rowNames,
			const std::vector<STRINGTYPE> &colNames) {
		MemoryStoreType *p = this->m_man.get();
		assert(p != nullptr);
		assert(p->is_valid());
		std::string stype("float");
		bool bRet = p->import(name, nRows, nCols, data, rowNames, colNames,
				stype);
		assert(bRet);
	} // import
};
/////////////////////////////////////
#endif // !__MEMORYSTOREFIXTURE_H__

#pragma once
#ifndef __MEMORYSTOREFIXTURE_H__
#define __MEMORYSTOREFIXTURE_H__
////////////////////////////////////
#include <memorystatstore.h>
#include <iostream>
#include <sstream>
////////////////////////////////
template <typename U>
void test_write_map(const std::map<U,info::InfoValue> &oMap,std::string &ss){
	std::stringstream os;
	for (auto it = oMap.begin(); it != oMap.end(); ++it){
		if (it != oMap.begin()){
			os << ", ";
		}
		std::string s;
		U key = (*it).first;
		const info::InfoValue &v = (*it).second;
		v.get_value(s);
		os << "(" << key << "," << s << ")";
	}// it
	ss = os.str();
}// test_write_map
/////////////////////////////
class MemoryStoreFixture {
protected:
	using MemoryStoreType = info::MemoryStatStore<>;
	using IStoreType = info::IStatStore<>;
	using DatasetType = IStoreType::DatasetType;
	//
	size_t m_nbrows;
	size_t m_nbcols;
	std::unique_ptr<MemoryStoreType> m_man;
	IStoreType *get_store(void);
	DatasetType m_oset;
public:
	MemoryStoreFixture();
	virtual ~MemoryStoreFixture();
private:
	void  data_teardown(void);
	void fill_mortal_data(void);
	void fill_conso_data(void);
	void init_data(void);
	void import(const std::string &name, size_t nRows, size_t nCols,
		const std::vector<int> &data,
		const std::vector<std::string> &rowNames,
		const std::vector<std::string> &colNames);
};

/////////////////////////////////////
#endif // !__MEMORYSTOREFIXTURE_H__

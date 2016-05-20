#pragma once
#ifndef __ISTATSTORE_H__
#define __ISTATSTORE_H__
////////////////////////////////
#include "baseitem.h"
//////////////////////////////////
#include <vector>
#include <map>
//////////////////////////////////////////
namespace info {
	//////////////////////////////////////////////
	template <typename IDTYPE = unsigned long, typename INTTYPE = int, typename STRINGTYPE = std::string, typename WEIGHTYPE = float>
	class IStatStore {
	public:
		using strings_vector = std::vector<STRINGTYPE>;
		using ints_vector = std::vector<IDTYPE>;
		using ints_string_map = std::map<IDTYPE, STRINGTYPE>;
		//
		using DatasetType = StatDataset<IDTYPE, INTTYPE, STRINGTYPE>;
		using datasets_vector = std::vector<DatasetType>;
		//
		using VariableType = StatVariable<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using variables_vector = std::vector<VariableType>;
		//
		using IndivType = StatIndiv<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using indivs_vector = std::vector<IndivType>;
		//
		using ValueType = StatValue<IDTYPE, INTTYPE, STRINGTYPE>;
		using values_vector = std::vector<ValueType>;
		//
	public:
		static const size_t  DATATRANSFER_CHUNK_SIZE;
	public:
		virtual bool is_valid(void) = 0;
		virtual void begin_transaction(void) = 0;
		virtual void commit_transaction(void) = 0;
		virtual void rollback_transaction(void) = 0;
		//
		virtual bool find_all_datasets_count(size_t &nCount) = 0;
		virtual bool find_all_datasets(datasets_vector &oList, size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_all_datasets_ids(ints_vector &oList, size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_dataset(DatasetType &cur) = 0;
		virtual bool maintains_dataset(DatasetType &cur, bool bCommit = true) = 0;
		virtual bool remove_dataset(const DatasetType &cur, bool bCommit = true) = 0;
		//
		virtual bool find_dataset_variables_types(const DatasetType &oSet, ints_string_map &oMap) = 0;
		virtual bool find_dataset_variables_count(const DatasetType &oSet, size_t &nCount) = 0;
		virtual bool find_dataset_variables_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_dataset_variables(const DatasetType &oSet, variables_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_variable(VariableType &cur) = 0;
		virtual bool maintains_variables(const variables_vector &oVars, bool bCommit = true,
			bool bRemove = false) = 0;
		virtual bool remove_variable(const VariableType &oVar, bool bCommit = true) = 0;
		//
		virtual bool find_dataset_indivs_count(const DatasetType &oSet, size_t &nCount) = 0;
		virtual bool find_dataset_indivs_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_dataset_indivs(const DatasetType &oSet, indivs_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_indiv(IndivType &cur) = 0;
		virtual bool maintains_indivs(const indivs_vector &oInds, bool bCommit = true,
			bool bRemove = false) = 0;
		virtual bool remove_indiv(const IndivType &oInd, bool bCommit = true) = 0;
		//
		virtual bool find_value(ValueType &cur) = 0;
		virtual bool maintains_values(const values_vector &oVals, bool bCommit = true,
			bool bRemove = false) = 0;
		virtual bool find_dataset_values_count(const DatasetType &oSet, size_t &nCount) = 0;
		virtual bool find_dataset_values(const DatasetType &oSet, values_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_variable_values(VariableType &oVar, values_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_variable_distinct_values(VariableType &oVar, strings_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_indiv_values(IndivType &oInd, values_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_indiv_values_count(IndivType &oInd, size_t &nc) = 0;
		virtual bool find_variable_values_count(VariableType &oVar, size_t &nc) = 0;
	public:
		virtual ~IStatStore() {}
	}; // class IStatStore<IDTYPE,INTTYPE,STRINGTYPE,WEIGHTTYPE>
	///////////////////////////////////////////////
	template <typename IDTYPE, typename INTTYPE, typename STRINGTYPE, typename WEIGHTYPE>
	const size_t IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>::DATATRANSFER_CHUNK_SIZE(100);
}// namespace info
////////////////////////////////////
#endif // !__ISTATSTORE_H__
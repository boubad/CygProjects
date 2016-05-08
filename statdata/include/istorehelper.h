#pragma once
#ifndef __ISTOREHELPER__H__
#define __ISTOREHELPER__H__
////////////////////////////////////
#include "dbstatitems.h"
////////////////////////////////////////////
namespace info {
	///////////////////////////////////////////
	class IStoreHelper {
	public:
		virtual void begin_transaction(void) = 0;
		virtual void commit_transaction(void) = 0;
		virtual void rollback_transaction(void) = 0;
		virtual bool is_valid(void) = 0;
		virtual bool find_all_datasets_count(int &nCount) = 0;
		virtual bool find_all_datasets(datasets_vector &oList, int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_all_datasets_ids(ints_vector &oList, int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_dataset(DBStatDataset &cur) = 0;
		virtual bool maintains_dataset(DBStatDataset &cur, bool bCommit = true) = 0;
		virtual bool remove_dataset(const DBStatDataset &cur, bool bCommit = true) = 0;
		//
		virtual bool find_dataset_variables_types(const DBStatDataset &oSet, inttype_string_map &oMap) = 0;
		virtual bool find_dataset_variables_count(const DBStatDataset &oSet, int &nCount) = 0;
		virtual bool find_dataset_variables_ids(const DBStatDataset &oSet, ints_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_dataset_variables(const DBStatDataset &oSet,variables_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_variable(DBStatVariable &cur) = 0;
		virtual bool maintains_variables(const variables_vector &oVars, bool bCommit = true,
			bool bRemove = false) = 0;
		virtual bool remove_variable(const DBStatVariable &oVar, bool bCommit = true) = 0;
		//
		virtual bool find_dataset_indivs_count(const DBStatDataset &oSet, int &nCount) = 0;
		virtual bool find_dataset_indivs_ids(const DBStatDataset &oSet, ints_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_dataset_indivs(const DBStatDataset &oSet,indivs_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_indiv(DBStatIndiv &cur) = 0;
		virtual bool maintains_indivs(const indivs_vector &oInds, bool bCommit = true,
			bool bRemove = false) = 0;
		virtual bool remove_indiv(const DBStatIndiv &oInd, bool bCommit = true) = 0;
		//
		virtual bool find_value(DBStatValue &cur) = 0;
		virtual bool maintains_values(const values_vector &oVals, bool bCommit = true,
			bool bRemove = false) = 0;
		virtual bool find_dataset_values_count(const DBStatDataset &oSet, int &nCount) = 0;
		virtual bool find_dataset_values(const DBStatDataset &oSet, values_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_variable_values(DBStatVariable &oVar, values_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_variable_distinct_values(DBStatVariable &oVar, strings_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
		virtual bool find_indiv_values(DBStatIndiv &oInd, values_vector &oList,
			int skip = 0, int count = DATATRANSFER_CHUNK_SIZE) = 0;
	public:
		virtual ~IStoreHelper() {}
	};// class IStoreHelper
	//////////////////////////////////////////
}// namespace info
//////////////////////////////////
#endif // !__IDATABASEHELPER__H__

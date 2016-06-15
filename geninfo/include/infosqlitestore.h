#pragma once
#ifndef __INFOSQLITESTORE_H__
#define __INFOSQLITESTORE_H__
////////////////////////////////////////
#include <boost/noncopyable.hpp>
/////////////////////////////////////
#include "istatstore.h"
////////////////////////////////////////////
namespace info_sqlite {
	class SQLite_Database;
	class SQLite_Statement;
}
//////////////////////////////////////////
namespace info {
	//////////////////////////////////////
	class SQLiteStatHelper : public IStatStore<unsigned long, unsigned long, std::string, double>, private boost::noncopyable {
		using mutex_type = std::recursive_mutex;
		using lock_type = std::unique_lock<mutex_type>;
	public:
		using IDTYPE = unsigned long;
		using INTTYPE = unsigned long;
		using STRINGTYPE = std::string;
		using WEIGHTYPE = double;
		//
		using StatStoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using strings_vector = std::vector<STRINGTYPE>;
		using ints_vector = std::vector<IDTYPE>;
		using ints_string_map = std::map<IDTYPE, STRINGTYPE>;
		using DatasetType = StatDataset<IDTYPE, INTTYPE, STRINGTYPE>;
		using datasets_vector = std::vector<DatasetType>;
		using VariableType = StatVariable<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using variables_vector = std::vector<VariableType>;
		using IndivType = StatIndiv<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using indivs_vector = std::vector<IndivType>;
		using ValueType = StatValue<IDTYPE, INTTYPE, STRINGTYPE>;
		using values_vector = std::vector<ValueType>;
	private:
		bool m_intransaction;
		std::unique_ptr<info_sqlite::SQLite_Database> m_base;
		mutex_type _mutex;
		//
		void check_schema(void);
		void read_dataset(info_sqlite::SQLite_Statement &q, DatasetType &cur);
		void read_variable(info_sqlite::SQLite_Statement &q, VariableType &cur);
		void read_indiv(info_sqlite::SQLite_Statement &q, IndivType &cur);
		void read_value(info_sqlite::SQLite_Statement &q, ValueType &cur);
		//
		bool find_variable_type(IDTYPE nVarId, STRINGTYPE stype);
	public:
		static const STRINGTYPE DEFAULT_DATABASE_NAME;
	public:
		SQLiteStatHelper(const STRINGTYPE &sDatabaseName = DEFAULT_DATABASE_NAME);
		virtual ~SQLiteStatHelper();
	public:
		// overrides
		virtual bool is_valid(void);
		virtual void begin_transaction(void);
		virtual void commit_transaction(void);
		virtual void rollback_transaction(void);
		//
		virtual bool find_all_datasets_count(size_t &nCount);
		virtual bool find_all_datasets(datasets_vector &oList, size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_all_datasets_ids(ints_vector &oList, size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_dataset(DatasetType &cur);
		virtual bool maintains_dataset(DatasetType &cur, bool bCommit = true);
		virtual bool remove_dataset(const DatasetType &cur, bool bCommit = true);
		//
		virtual bool find_dataset_variables_types(const DatasetType &oSet, ints_string_map &oMap);
		virtual bool find_dataset_variables_count(const DatasetType &oSet, size_t &nCount);
		virtual bool find_dataset_variables_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_dataset_variables(const DatasetType &oSet, variables_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_variable(VariableType &cur);
		virtual bool maintains_variables(const variables_vector &oVars, bool bCommit = true,
			bool bRemove = false);
		virtual bool remove_variable(const VariableType &oVar, bool bCommit = true);
		//
		virtual bool find_dataset_indivs_count(const DatasetType &oSet, size_t &nCount);
		virtual bool find_dataset_indivs_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_dataset_indivs(const DatasetType &oSet, indivs_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_indiv(IndivType &cur);
		virtual bool maintains_indivs(const indivs_vector &oInds, bool bCommit = true,
			bool bRemove = false);
		virtual bool remove_indiv(const IndivType &oInd, bool bCommit = true);
		//
		virtual bool find_value(ValueType &cur);
		virtual bool maintains_values(const values_vector &oVals, bool bCommit = true,
			bool bRemove = false);
		virtual bool find_dataset_values_count(const DatasetType &oSet, size_t &nCount);
		virtual bool find_dataset_values(const DatasetType &oSet, values_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_variable_values(VariableType &oVar, values_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_variable_distinct_values(VariableType &oVar, strings_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_indiv_values(IndivType &oInd, values_vector &oList,
			size_t skip = 0, size_t count = DATATRANSFER_CHUNK_SIZE);
		virtual bool find_indiv_values_count(IndivType &oInd, size_t &nc);
		virtual bool find_variable_values_count(VariableType &oVar, size_t &nc);
	}; // class SQLiteStatHelper
	//////////////////////////////////////////
}// namespace info
////////////////////////////////////////
#endif // !__INFOSQLITESTORE_H__

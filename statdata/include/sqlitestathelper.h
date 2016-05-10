#pragma once
#ifndef __SQLITESTATHELPER_H__
#define __SQLITESTATHELPER_H__
////////////////////////////////
#include "istorehelper.h"
//////////////////////////////////////
namespace info {
	////////////////////////////////
	class SQLite_Database;
	class SQLite_Statement;
	///////////////////////////////
	class SQLiteStatHelper : boost::noncopyable, public IStoreHelper {
	private:
		bool m_intransaction;
		std::unique_ptr<SQLite_Database> m_base;
		//
		void check_schema(void);
		void read_dataset(SQLite_Statement &q, DBStatDataset &cur);
		void read_variable(SQLite_Statement &q, DBStatVariable &cur);
		void read_indiv(SQLite_Statement &q, DBStatIndiv &cur);
		void read_value(SQLite_Statement &q, DBStatValue &cur);
	public:
		static const std::string DEFAULT_DATABASE_NAME;
	public:
		SQLiteStatHelper(const std::string &sDatabaseName = DEFAULT_DATABASE_NAME);
		SQLiteStatHelper(const std::wstring &sDatabaseName);
		virtual ~SQLiteStatHelper();
		//
		virtual void begin_transaction(void);
		virtual void commit_transaction(void);
		virtual void rollback_transaction(void);
		virtual bool is_valid(void) ;
		virtual bool find_all_datasets_count(int &nCount) ;
		virtual bool find_all_datasets(datasets_vector &oList, int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_all_datasets_ids(ints_vector &oList, int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_dataset(DBStatDataset &cur) ;
		virtual bool maintains_dataset(DBStatDataset &cur, bool bCommit = true) ;
		virtual bool remove_dataset(const DBStatDataset &cur, bool bCommit = true) ;
		//
		virtual bool find_dataset_variables_types(const DBStatDataset &oSet, inttype_string_map &oMap) ;
		virtual bool find_dataset_variables_count(const DBStatDataset &oSet, int &nCount) ;
		virtual bool find_dataset_variables_ids(const DBStatDataset &oSet, ints_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_dataset_variables(const DBStatDataset &oSet, variables_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_variable(DBStatVariable &cur) ;
		virtual bool maintains_variables(const variables_vector &oVars, bool bCommit = true,
			bool bRemove = false) ;
		virtual bool remove_variable(const DBStatVariable &oVar, bool bCommit = true) ;
		//
		virtual bool find_dataset_indivs_count(const DBStatDataset &oSet, int &nCount) ;
		virtual bool find_dataset_indivs_ids(const DBStatDataset &oSet, ints_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_dataset_indivs(const DBStatDataset &oSet, indivs_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_indiv(DBStatIndiv &cur) ;
		virtual bool maintains_indivs(const indivs_vector &oInds, bool bCommit = true,
			bool bRemove = false) ;
		virtual bool remove_indiv(const DBStatIndiv &oInd, bool bCommit = true) ;
		//
		virtual bool find_value(DBStatValue &cur) ;
		virtual bool maintains_values(const values_vector &oVals, bool bCommit = true,
			bool bRemove = false) ;
		virtual bool find_dataset_values_count(const DBStatDataset &oSet, int &nCount) ;
		virtual bool find_dataset_values(const DBStatDataset &oSet, values_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_variable_values(DBStatVariable &oVar, values_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_variable_distinct_values(DBStatVariable &oVar, strings_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
		virtual bool find_indiv_values(DBStatIndiv &oInd, values_vector &oList,
			int skip = 0 , int count = DATATRANSFER_CHUNK_SIZE) ;
	};// class SQLiteStatHelper
	/////////////////////////////////
}// namespace info
////////////////////////////////
#endif // !__DBSTATHELPER_H

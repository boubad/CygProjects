#pragma once
#ifndef __MEMORYSTOREFIXTURE_H__
#define __MEMORYSTOREFIXTURE_H__
////////////////////////////////////
#include <memorystatstore.h>
#include <sqlitestore.h>
/////////////////////////////////
#include "infotestdata.h"
////////////////////////////////
namespace info {
	////////////////////////////////
	template<typename IDTYPE, typename INTTYPE,typename STRINGTYPE, typename WEIGHTYPE>
		class TestStoreFixture {
		public:
			using SQLiteStoreType = SQLiteStore<IDTYPE,INTTYPE,STRINGTYPE,WEIGHTYPE>;
			using MemoryStoreType = MemoryStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using IStoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = typename IStoreType::DatasetType;
			using TestStoreType = TestStoreFixture<IDTYPE,INTTYPE,STRINGTYPE,WEIGHTYPE>;
			//
			std::unique_ptr<MemoryStoreType> m_man;
			std::unique_ptr<SQLiteStoreType> m_sqlite;
		public:
			TestStoreFixture() {
				(void)this->get_sql_store();
				(void)this->get_memory_store();
			} // init
			virtual ~TestStoreFixture() {
			}
			IStoreType *get_store(bool bMemory = true) {
				if (bMemory) {
					return (this->get_memory_store());
				}
				else {
					return (this->get_sql_store());
				}
				//return (this->get_sql_store());
			}
			IStoreType *get_memory_store(void) {
				IStoreType *p = this->m_man.get();
				if (p == nullptr) {
					m_man.reset(new MemoryStoreType());
					p = m_man.get();
					assert(p != nullptr);
					assert(p->is_valid());
					this->init_data(p);
				}
				assert(p != nullptr);
				assert(p->is_valid());
				return (p);
			}// get_memory_store
			IStoreType *get_sql_store(void) {
				IStoreType *p = this->m_sqlite.get();
				if (p == nullptr) {
					m_sqlite.reset(new SQLiteStoreType());
					p = m_sqlite.get();
					assert(p != nullptr);
					assert(p->is_valid());
					this->init_data(p);
				}
				assert(p != nullptr);
				assert(p->is_valid());
				return (p);
			}// get_sqlite_store
		private:
			void init_data(IStoreType *p) {
				this->fill_test_data(p);
				this->fill_mortal_data(p);
				this->fill_conso_data(p);
			}
			void fill_test_data(IStoreType *p) {
				STRINGTYPE name;
				/*
				InfoTestData::get_test_name(name);
				{
					DatasetType oSet(name);
					if (p->find_dataset(oSet)) {
						return;
					}
				}
				*/
				size_t nRows = 0, nCols = 0;
				std::vector<int> gdata;
				std::vector<STRINGTYPE> rowNames, colNames;
				info::InfoTestData::get_test_data(name, nRows, nCols, gdata, rowNames,
					colNames);
				assert(!name.empty());
				assert(nRows > 2);
				assert(nCols > 2);
				assert(colNames.size() >= nCols);
				assert(rowNames.size() >= nRows);
				assert(gdata.size() >= (size_t)(nCols * nRows));
				this->import(p, name, nRows, nCols, gdata, rowNames, colNames);
			} // fill_test_data
			void fill_mortal_data(IStoreType *p) {
				STRINGTYPE name;
				/*
				InfoTestData::get_mortal_name(name);
				{
					DatasetType oSet(name);
					if (p->find_dataset(oSet)) {
						return;
					}
				}
				*/
				size_t nRows = 0, nCols = 0;
				std::vector<int> gdata;
				std::vector<STRINGTYPE> rowNames, colNames;
				info::InfoTestData::get_mortal_data(name, nRows, nCols, gdata, rowNames,
					colNames);
				assert(!name.empty());
				assert(nRows > 2);
				assert(nCols > 2);
				assert(colNames.size() >= nCols);
				assert(rowNames.size() >= nRows);
				assert(gdata.size() >= (size_t)(nCols * nRows));
				this->import(p, name, nRows, nCols, gdata, rowNames, colNames);
			} // fill_mortal_data
			void fill_conso_data(IStoreType *p) {
				STRINGTYPE name;
				/**
				InfoTestData::get_conso_name(name);
				{
					DatasetType oSet(name);
					if (p->find_dataset(oSet)) {
						return;
					}
				}
				*/
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
				assert(gdata.size() >= (size_t)(nCols * nRows));
				this->import(p, name, nRows, nCols, gdata, rowNames, colNames);
			} // fill_mortal_data
			void import(IStoreType *p, const STRINGTYPE &name, 
				size_t nRows, size_t nCols,
				const std::vector<int> &data,
				const std::vector<STRINGTYPE> &rowNames,
				const std::vector<STRINGTYPE> &colNames) {
				assert(p != nullptr);
				assert(p->is_valid());
				DatasetType oSet(name);
				bool bRet = p->find_dataset(oSet);
				if (!bRet) {
					bRet = p->maintains_dataset(oSet);
					assert(bRet);
				}
				p->import(oSet, nRows, nCols, data, rowNames, colNames);
			} // import
	};
}// namespace info
 /////////////////////////////////////
#endif // !__MEMORYSTOREFIXTURE_H__

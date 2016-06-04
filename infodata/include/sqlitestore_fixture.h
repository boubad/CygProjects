#pragma once
#ifndef __SQLITESTORE_FIXTURE_H__
#define __SQLITESTORE_FIXTURE_H__
////////////////////////////////////
#include <infosqlitestore.h>
/////////////////////////////////
#include "infotestdata.h"
////////////////////////////////
namespace info {
	////////////////////////////////
	template<typename IDTYPE = unsigned long, typename INTTYPE = unsigned long,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
		class SQLiteStoreFixture {
		public:
			using ImplStoreType = SQLiteStatHelper;
			using IStoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = typename IStoreType::DatasetType;
			//
			std::unique_ptr<ImplStoreType> m_man;
		public:
			MemoryStoreFixture() {
				this->init_data();
				this->fill_mortal_data();
				this->fill_conso_data();
				this->fill_test_data();
			} // init
			virtual ~MemoryStoreFixture() {
				this->data_teardown();
			}
			IStoreType *get_store(void) {
				IStoreType *p = this->m_man.get();
				assert(p != nullptr);
				assert(p->is_valid());
				return (p);
			}
		private:
			void data_teardown(void) {
				this->m_man.reset();
			} // data_teardown
			void fill_test_data(void) {
				STRINGTYPE name;
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
				this->import(name, nRows, nCols, gdata, rowNames, colNames);
			} // fill_test_data
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
				assert(colNames.size() >= nCols);
				assert(rowNames.size() >= nRows);
				assert(gdata.size() >= (size_t)(nCols * nRows));
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
				assert(gdata.size() >= (size_t)(nCols * nRows));
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
				DatasetType oSet(name);
				bool bRet = p->find_dataset(oSet);
				if (!bRet) {
					bRet = p->maintains_dataset(oSet);
					assert(bRet);
				}
				bRet = p->import(oSet, nRows, nCols, data, rowNames, colNames);
				assert(bRet);
			} // import
	};
}// namespace info
 /////////////////////////////////////
#endif // !__MEMORYSTOREFIXTURE_H__

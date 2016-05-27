#pragma once
#ifndef __MEMORYSTATSTORE_H__
#define __MEMORYSTATSTORE_H__
/////////////////////////////////////////
#include "istatstore.h"
#include "memorydataset.h"
#include "indiv.h"
/////////////////////////////
namespace info {
	////////////////////////////////
	template <typename IDTYPE = unsigned long, typename INTTYPE = unsigned long, typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
	class MemoryStatStore : public IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>, private boost::noncopyable {
	public:
		using MemoryDatasetType = MemoryDataset<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		using PMemoryDatasetType = std::shared_ptr<MemoryDatasetType>;
		using pmemorydatasets_vector = std::vector<PMemoryDatasetType>;
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
		//
		using datasets_map = std::map<IDTYPE, DatasetType>;
		using variables_map = std::map<IDTYPE, VariableType>;
		using indivs_map = std::map<IDTYPE, IndivType>;
		using values_map = std::map<IDTYPE, ValueType>;
		using strings_pdataset_map = std::map<STRINGTYPE, PMemoryDatasetType>;
		using ints_pdataset_map = std::map<IDTYPE, PMemoryDatasetType>;
	private:
		//
		IDTYPE m_lastid;
		strings_pdataset_map m_sigle_map;
		ints_pdataset_map m_id_map;
		//
	public:
		MemoryStatStore() :m_lastid(1) {}
		virtual ~MemoryStatStore() {}
	public:
		//
		template <typename T>
		bool import(const STRINGTYPE &name, size_t nRows, size_t nCols,
			const std::vector<T> &data,
			const strings_vector &rowNames,
			const strings_vector &colNames,
			const STRINGTYPE &stype) {
			DatasetType oSet(name);
			if (!this->maintains_dataset(oSet, true)) {
				return (false);
			}
			MemoryDatasetType *p = this->get_dataset(oSet);
			if (p != nullptr) {
				p->import(nRows, nCols, data, rowNames, colNames, stype);
				return (true);
			}
			return (false);
		}// import
		// overrides
		virtual bool is_valid(void) {
			return (true);
		}
		virtual void begin_transaction(void) {
		}
		virtual void commit_transaction(void) {
		}
		virtual void rollback_transaction(void) {
		}
		//
		virtual bool find_all_datasets_count(size_t &nCount) {
			nCount = this->m_id_map.size();
			return (true);
		}
		virtual bool find_all_datasets(datasets_vector &oList, size_t skip = 0, size_t count = 100) {
			oList.clear();
			if (count < 1) {
				count = 100;
			}
			size_t nStart = skip;
			size_t nMax = this->m_id_map.size();
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			size_t nc = nEnd - nStart;
			if (nc > 0) {
				ints_pdataset_map &oMap = this->m_id_map;
				size_t offset = 0;
				for (auto it = oMap.begin(); it != oMap.end(); ++it) {
					if (offset >= nStart) {
						PMemoryDatasetType &o = (*it).second;
						DatasetType oSet;
						o->get_dataset(oSet);
						if (oList.size() >= nc) {
							break;
						}
					}
					++offset;
				}// it
			}// nc
			return (true);
		}//find_all_datasets
		virtual bool find_all_datasets_ids(ints_vector &oList, size_t skip = 0, size_t count = 100) {
			oList.clear();
			if (count < 1) {
				count = 100;
			}
			size_t nStart = skip;
			size_t nMax = this->m_id_map.size();
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			size_t nc = nEnd - nStart;
			if (nc > 0) {
				ints_pdataset_map &oMap = this->m_id_map;
				size_t offset = 0;
				for (auto it = oMap.begin(); it != oMap.end(); ++it) {
					if (offset >= nStart) {
						oList.push_back((*it).first);
						if (oList.size() >= nc) {
							break;
						}
					}
					++offset;
				}// it
			}// nc
			return (true);
		}//find_all_datasets_ids
		MemoryDatasetType *get_dataset(DatasetType &cur) {
			MemoryDatasetType *pRet = nullptr;
			IDTYPE nId = cur.id();
			if (nId != 0) {
				auto it = this->m_id_map.find(nId);
				if (it != this->m_id_map.end()) {
					PMemoryDatasetType o = (*it).second;
					pRet = o.get();
					return (pRet);
				}
			}
			STRINGTYPE sigle = cur.sigle();
			if (!sigle.empty()) {
				auto it = this->m_sigle_map.find(sigle);
				if (it != this->m_sigle_map.end()) {
					PMemoryDatasetType o = (*it).second;
					pRet = o.get();
					return (pRet);
				}
			}
			return (pRet);
		}// get_dataset
		virtual bool find_dataset(DatasetType &cur) {
			MemoryDatasetType *pf = this->get_dataset(cur);
			if (pf == nullptr) {
				return (false);
			}
			return (pf->find_dataset(cur));
		}//find_dataset
		virtual bool maintains_dataset(DatasetType &cur, bool/* bCommit = true*/) {
			if (!cur.is_writeable()) {
				return (false);
			}
			MemoryDatasetType *pf = this->get_dataset(cur);
			if (pf != nullptr) {
				return pf->update_dataset(cur);
			}
			IDTYPE nx = cur.id();
			if (nx != 0) {
				if (nx >= this->m_lastid) {
					this->m_lastid = (IDTYPE)(nx + 1);
				}
			}
			else {
				cur.id(this->m_lastid++);
			}
			cur.version(1);
			PMemoryDatasetType o = std::make_shared<MemoryDatasetType>(cur);
			IDTYPE key1 = cur.id();
			this->m_id_map[key1] = o;
			STRINGTYPE key2 = cur.sigle();
			this->m_sigle_map[key2] = o;
			return (true);
		}//maintains_dataset
		virtual bool remove_dataset(const DatasetType &cur, bool /*bCommit = true*/) {
			DatasetType xSet(cur);
			MemoryDatasetType *pf = this->get_dataset(xSet);
			if (pf == nullptr) {
				return (false);
			}
			if (!pf->find_dataset(xSet)) {
				return (false);
			}
			{
				STRINGTYPE key = xSet.sigle();
				auto it = this->m_sigle_map.find(key);
				if (it != this->m_sigle_map.end()) {
					this->m_sigle_map.erase(it);
				}
			}
			{
				IDTYPE key = xSet.id();
				auto it = this->m_id_map.find(key);
				if (it != this->m_id_map.end()) {
					this->m_id_map.erase(it);
				}
			}
			return (true);
		}//remove_dataset
		//
		virtual bool find_dataset_variables_types(const DatasetType &oSet, ints_string_map &oMap) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_dataset_variables_types(oMap);
			}
			return (false);
		}//find_dataset_variables_types
		virtual bool find_dataset_variables_count(const DatasetType &oSet, size_t &nCount) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->get_variables_count(nCount);
			}
			return (false);
		}
		virtual bool find_dataset_variables_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = 100) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->get_variables_ids(oList, skip, count);
			}
			return (false);
		}
		virtual bool find_dataset_variables(const DatasetType &oSet, variables_vector &oList,
			size_t skip = 0, size_t count = 100) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->get_variables(oList, skip, count);
			}
			return (false);
		}
		virtual bool find_variable(VariableType &cur) {
			IDTYPE nDatasetId = cur.dataset_id();
			if (nDatasetId != 0) {
				DatasetType xSet(cur.dataset_id());
				MemoryDatasetType *p = this->get_dataset(xSet);
				if (p != nullptr) {
					return p->find_variable(cur);
				}
				return (false);
			}
			return (false);
		}
		virtual bool maintains_variables(const variables_vector &oVars, bool bCommit = true,
			bool bRemove = false) {
			if (oVars.empty()) {
				return (false);
			}
			DatasetType xSet((oVars[0]).dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				if (bRemove) {
					return p->remove_variables(oVars);
				}
				else {
					return p->maintains_variables(oVars);
				}
			}
			return (false);
		}
		virtual bool remove_variable(const VariableType &oVar, bool bCommit = true) {
			variables_vector vv{ oVar };
			return this->maintains_variables(vv, true, true);
		}
		//
		virtual bool find_dataset_indivs_count(const DatasetType &oSet, size_t &nCount) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->get_indivs_count(nCount);
			}
			return (false);
		}
		virtual bool find_dataset_indivs_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = 100) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->get_indivs_ids(oList, skip, count);
			}
			return (false);
		}
		virtual bool find_dataset_indivs(const DatasetType &oSet, indivs_vector &oList,
			size_t skip = 0, size_t count = 100) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->get_indivs(oList, skip, count);
			}
			return (false);
		}
		virtual bool find_indiv(IndivType &cur) {
			IDTYPE nDatasetId = cur.dataset_id();
			if (nDatasetId != 0) {
				DatasetType xSet(cur.dataset_id());
				MemoryDatasetType *p = this->get_dataset(xSet);
				if (p != nullptr) {
					return p->find_indiv(cur);
				}
				return (false);
			}
			return (false);
		}
		virtual bool maintains_indivs(const indivs_vector &oInds, bool bCommit = true,
			bool bRemove = false) {
			if (oInds.empty()) {
				return (false);
			}
			DatasetType xSet((oInds[0]).dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				if (bRemove) {
					return p->remove_indivs(oInds);
				}
				else {
					return p->maintains_indivs(oInds);
				}
			}
			return (false);
		}
		virtual bool remove_indiv(const IndivType &oInd, bool bCommit = true) {
			indivs_vector vv{ oInd };
			return this->maintains_indivs(vv, true, true);
		}
		//
		virtual bool find_value(ValueType &cur) {
			VariableType oVar(cur.variable_id());
			if (!this->find_variable(oVar)) {
				return (false);
			}
			DatasetType xSet(oVar.dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_value(cur);
			}
			return (false);
		}
		virtual bool maintains_values(const values_vector &oVals, bool bCommit = true,
			bool bRemove = false) {
			if (oVals.empty()) {
				return (false);
			}
			IDTYPE nDatasetId = (oVals[0]).dataset_id();
			DatasetType xSet(nDatasetId);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->maintains_values(oVals, bRemove);
			}
			return (false);
		}
		virtual bool find_dataset_values_count(const DatasetType &oSet, size_t &nCount) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_dataset_values_count(nCount);
			}
			return (false);
		}
		virtual bool find_dataset_values(const DatasetType &oSet, values_vector &oList,
			size_t skip = 0, size_t count = 100) {
			DatasetType  xSet(oSet);
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_dataset_values(oList, skip, count);
			}
			return (false);
		}
		virtual bool find_variable_values(VariableType &oVar, values_vector &oList,
			size_t skip = 0, size_t count = 100) {
			if (!this->find_variable(oVar)) {
				return (false);
			}
			DatasetType xSet(oVar.dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_variable_values(oVar, oList, skip, count);
			}
			return (false);
		}
		virtual bool find_variable_distinct_values(VariableType &oVar, strings_vector &oList,
			size_t skip = 0, size_t count = 100) {
			if (!this->find_variable(oVar)) {
				return (false);
			}
			DatasetType xSet(oVar.dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_variable_distinct_values(oVar, oList, skip, count);
			}
			return (false);
		}
		virtual bool find_indiv_values(IndivType &oInd, values_vector &oList,
			size_t skip = 0, size_t count = 100) {
			if (!this->find_indiv(oInd)) {
				return (false);
			}
			DatasetType xSet(oInd.dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_indiv_values(oInd, oList, skip, count);
			}
			return (false);
		}
		virtual bool find_indiv_values_count(IndivType &oInd, size_t &nc) {
			if (!this->find_indiv(oInd)) {
				return (false);
			}
			DatasetType xSet(oInd.dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_indiv_values_count(oInd, nc);
			}
			return (false);
		}
		virtual bool find_variable_values_count(VariableType &oVar, size_t &nc) {
			if (!this->find_variable(oVar)) {
				return (false);
			}
			DatasetType xSet(oVar.dataset_id());
			MemoryDatasetType *p = this->get_dataset(xSet);
			if (p != nullptr) {
				return p->find_variable_values_count(oVar, nc);
			}
			return (false);
		}
	};// class MemoryStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>
	/////////////////////////////////////
}// namespace info
/////////////////////////////////////////
#endif // !__MEMORYSTATSTORE_H__

#pragma once
#ifndef __ISTATSTORE_H__
#define __ISTATSTORE_H__
////////////////////////////////
#include "baseitem.h"
//////////////////////////////////
namespace info {
	//////////////////////////////////////////////
	template <typename IDTYPE, typename INTTYPE, typename STRINGTYPE, typename WEIGHTYPE>
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
	public:
		template <typename T>
		void fetch_data_type(T &d, std::string &stype) {
			boost::any v(d);
			if ((v.type() == typeid(double)) || (v.type() == typeid(float)) || (v.type() == typeid(long double))) {
				stype = "real";
			}
			else if ((v.type() == typeid(short)) || (v.type() == typeid(unsigned short)) || (v.type() == typeid(int)) ||
				(v.type() == typeid(unsigned int)) || (v.type() == typeid(long)) || (v.type() == typeid(unsigned long)) ||
				(v.type() == typeid(long long))) {
				stype = "integer";
			}
			else if (v.type() == typeid(bool)) {
				stype = "boolean";
			}
			else {
				stype = "text";
			}
		}//fetch_data_type
		template <typename T>
		void fetch_data_type(T &d, std::wstring &stype) {
			boost::any v(d);
			if ((v.type() == typeid(double)) || (v.type() == typeid(float)) || (v.type() == typeid(long double))) {
				stype = L"real";
			}
			else if ((v.type() == typeid(short)) || (v.type() == typeid(unsigned short)) || (v.type() == typeid(int)) ||
				(v.type() == typeid(unsigned int)) || (v.type() == typeid(long)) || (v.type() == typeid(unsigned long)) ||
				(v.type() == typeid(long long))) {
				stype = L"integer";
			}
			else if (v.type() == typeid(bool)) {
				stype = L"boolean";
			}
			else {
				stype = L"text";
			}
		}//fetch_data_type
	public:
		template <typename T>
		void import(const DatasetType &oSet, size_t nRows, size_t nCols,
			const std::vector<T> &data,
			const strings_vector &rowNames,
			const strings_vector &colNames)  {
			STRINGTYPE stype;
			T dummy = 0;
			fetch_data_type(dummy, stype);
			assert(oSet.id() != 0);
			variables_vector oVars;
			for (size_t i = 0; i < nCols; ++i) {
				STRINGTYPE sigle = colNames[i];
				VariableType v(oSet, sigle);
				if (!this->find_variable(v)) {
					v.sigle(sigle);
					v.dataset_id(oSet.id());
					v.vartype(stype);
					oVars.push_back(v);
				}
			} // i
			if (!oVars.empty()) {
				if (!this->maintains_variables(oVars)) {
					throw info_error("maintains variables error");
				}
			}
			indivs_vector oInds;
			for (size_t i = 0; i < nRows; ++i) {
				STRINGTYPE sigle = rowNames[i];
				IndivType v(oSet, sigle);
				if (!this->find_indiv(v)) {
					v.sigle(sigle);
					v.dataset_id(oSet.id());
					oInds.push_back(v);
				}
			} // i
			if (!oInds.empty()) {
				if (!this->maintains_indivs(oInds)){
					throw info_error("maintains indivs error");
				}

			}
			if ((nRows > 0) && (nCols > 0) && (colNames.size() >= nCols) && (rowNames.size() >= nRows)) {
				size_t nn = (size_t)(nRows * nCols);
				if (data.size() >= nn) {
					oInds.clear();
					if (!this->find_dataset_indivs(oSet,oInds, 0, nRows)){
						throw info_error("find  dataset indivs error");
					}
					assert(nRows == oInds.size());
					oVars.clear();
					if (!this->find_dataset_variables(oSet,oVars, 0, nCols)){
						throw info_error("find dataset variables error");
					}
					assert(nCols == oVars.size());
					//
					std::map<STRINGTYPE, VariableType *> pVars;
					for (auto &s : colNames) {
						STRINGTYPE sigle = s;
						STRINGTYPE rsigle;
						VariableType ovar(oSet, sigle);
						rsigle = ovar.sigle();
						VariableType *p = nullptr;
						for (size_t i = 0; i < oVars.size(); ++i) {
							VariableType &vv = oVars[i];
							STRINGTYPE  sx = vv.sigle();
							if (sx == rsigle) {
								p = &vv;
								break;
							}
						} // i
						assert(p != nullptr);
						pVars[sigle] = p;
					}// s
					std::map<STRINGTYPE, IndivType *> pInds;
					for (auto &s : rowNames) {
						STRINGTYPE  sigle = s;
						STRINGTYPE  rsigle;
						IndivType ovar(oSet, sigle);
						rsigle = ovar.sigle();
						IndivType *p = nullptr;
						for (size_t i = 0; i < oInds.size(); ++i) {
							IndivType &vv = oInds[i];
							STRINGTYPE  sx = vv.sigle();
							if (sx == rsigle) {
								p = &vv;
								break;
							}
						} // i
						assert(p != nullptr);
						pInds[sigle] = p;
					}// s
					values_vector oVals;
					for (size_t i = 0; i < nRows; ++i) {
						STRINGTYPE  sigleind = rowNames[i];
						IndivType *pInd = pInds[sigleind];
						assert(pInd != nullptr);
						for (size_t j = 0; j < nCols; ++j) {
							STRINGTYPE  siglevar = colNames[j];
							VariableType *pVar = pVars[siglevar];
							assert(pVar != nullptr);
							ValueType val(*pVar, *pInd);
							if (!this->find_value(val)) {
								T f = data[i * nCols + j];
								InfoValue vv(f);
								val.value(vv);
								val.variable_id(pVar->id());
								val.indiv_id(pInd->id());
								oVals.push_back(val);
							}
						} // j
					} // i
					if (!oVals.empty()) {
						if (!this->maintains_values(oVals)){
							throw info_error("maintains values error");
						}
						//assert(bRet);
					}
				}// ok import data
			}
		} // import
	}; // class IStatStore<IDTYPE,INTTYPE,STRINGTYPE,WEIGHTTYPE>
	///////////////////////////////////////////////
	template <typename IDTYPE, typename INTTYPE, typename STRINGTYPE, typename WEIGHTYPE>
	const size_t IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>::DATATRANSFER_CHUNK_SIZE(100);
}// namespace info
////////////////////////////////////
#endif // !__ISTATSTORE_H__

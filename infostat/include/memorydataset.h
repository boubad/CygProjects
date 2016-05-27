#pragma once
#ifndef __MEMORYDATASET_H__
#define __MEMORYDATASET_H__
/////////////////////////////////
#include "baseitem.h"
/////////////////////////////////////
namespace info {
	////////////////////////////////
	template <typename IDTYPE = unsigned long, typename INTTYPE = unsigned long, typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
	class MemoryDataset : private boost::noncopyable {
	public:
		using ints_set = std::set<IDTYPE>;
		using strings_set = std::set<STRINGTYPE>;
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
		IDTYPE m_lastid;
		DatasetType m_oset;
		variables_vector m_variables;
		indivs_vector m_indivs;
		values_vector m_values;
		//
		IDTYPE next_id(void) {
			return (this->m_lastid++);
		}
		static void convert_to_string(const boost::any &vx, std::wstring &s) {
			s.clear();
			if (vx.empty()) {
				return;
			}
			boost::any v(vx);
			if (v.type() == typeid(boost::any)) {
				v = boost::any_cast<boost::any>(v);
			}
			if (!v.empty())
			{
				const std::type_info & t = v.type();
				if (t == typeid(bool))
				{
					bool b = boost::any_cast<bool>(v);
					s = (b) ? L"T" : L"F";
				}
				else if (t == typeid(char))
				{
					char x = boost::any_cast<char>(v);
					std::stringstream os;
					os << x;
					std::string ss = os.str();
					s = StringConvert::s2ws(ss);
				}
				else if (t == typeid(unsigned char))
				{
					unsigned char x = boost::any_cast<unsigned char>(v);
					std::stringstream os;
					os << x;
					std::string ss = os.str();
					s = StringConvert::s2ws(ss);
				}
				else if (t == typeid(wchar_t))
				{
					wchar_t x = boost::any_cast<wchar_t>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(short))
				{
					short x = boost::any_cast<short>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned short))
				{
					unsigned short x = boost::any_cast<unsigned short>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(int))
				{
					int x = boost::any_cast<int>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned int))
				{
					unsigned int x = boost::any_cast<unsigned int>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(long))
				{
					long x = boost::any_cast<long>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned long))
				{
					unsigned long x = boost::any_cast<unsigned long>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(float))
				{
					float x = boost::any_cast<float>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(double))
				{
					double x = boost::any_cast<double>(v);
					std::wstringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(std::string))
				{
					std::string x = boost::any_cast<std::string>(v);
					s = StringConvert::s2ws(x);
				}
				else if (t == typeid(std::wstring))
				{
					s = boost::any_cast<std::wstring>(v);
				}
			} // not empty
		}
		static void convert_to_string(const boost::any &vx, std::string &s) {
			s.clear();
			if (vx.empty()) {
				return;
			}
			boost::any v(vx);
			if (v.type() == typeid(boost::any)) {
				v = boost::any_cast<boost::any>(v);
			}
			if (!v.empty())
			{
				const std::type_info & t = v.type();
				if (t == typeid(bool))
				{
					bool b = boost::any_cast<bool>(v);
					s = (b) ? "T" : "F";
				}
				else if (t == typeid(char))
				{
					char x = boost::any_cast<char>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned char))
				{
					unsigned char x = boost::any_cast<unsigned char>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(wchar_t))
				{
					wchar_t x = boost::any_cast<wchar_t>(v);
					std::wstringstream os;
					os << x;
					std::wstring ss = os.str();
					s = StringConvert::ws2s(ss);
				}
				else if (t == typeid(short))
				{
					short x = boost::any_cast<short>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned short))
				{
					unsigned short x = boost::any_cast<unsigned short>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(int))
				{
					int x = boost::any_cast<int>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned int))
				{
					unsigned int x = boost::any_cast<unsigned int>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(long))
				{
					long x = boost::any_cast<long>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(unsigned long))
				{
					unsigned long x = boost::any_cast<unsigned long>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(float))
				{
					float x = boost::any_cast<float>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(double))
				{
					double x = boost::any_cast<double>(v);
					std::stringstream os;
					os << x;
					s = os.str();
				}
				else if (t == typeid(std::string))
				{
					std::string x = boost::any_cast<std::string>(v);
					s = x;
				}
				else if (t == typeid(std::wstring))
				{
					std::wstring x = boost::any_cast<std::wstring>(v);
					s = StringConvert::ws2s(x);
				}
			} // not empty
		}// convert_to_string;
	public:
		MemoryDataset() :m_lastid(1) {}
		MemoryDataset(const DatasetType &oSet) :m_lastid(1), m_oset(oSet) {
			size_t nMax = this->m_oset.id();
			if (nMax >= this->m_lastid) {
				this->m_lastid = (IDTYPE)(nMax + 1);
			}
		}
		~MemoryDataset() {}
	public:
		//
		bool find_indiv_values_count(IndivType &oInd, size_t &nc) {
			if (!this->find_indiv(oInd, false)) {
				return (false);
			}
			const IDTYPE nVarId = oInd.id();
			const values_vector &values = this->m_values;
			nc = 0;
			for (auto it = values.begin(); it != values.end(); ++it) {
				const ValueType &v = *it;
				if (v.indiv_id() == nVarId) {
					++nc;
				}// found
			}// it
			return (true);
		}// find_indiv_values_count
		bool find_variable_values_count(VariableType &oVar, size_t &nc) {
			if (!this->find_variable(oVar, false)) {
				return (false);
			}
			const IDTYPE nVarId = oVar.id();
			const values_vector &values = this->m_values;
			nc = 0;
			for (auto it = values.begin(); it != values.end(); ++it) {
				const ValueType &v = *it;
				if (v.variable_id() == nVarId) {
					++nc;
				}// found
			}// it
			return (true);
		}// find_variable_values_count
		bool find_indiv_values(IndivType &oInd, values_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			if (!this->find_indiv(oInd, false)) {
				return (false);
			}
			const IDTYPE nVarId = oInd.id();
			const values_vector &values = this->m_values;
			oList.clear();
			size_t offset = 0;
			for (auto it = values.begin(); it != values.end(); ++it) {
				const ValueType &v = *it;
				if (v.indiv_id() == nVarId) {
					if (offset >= skip) {
						ValueType oo(v);
						oList.push_back(oo);
						++offset;
						if (oList.size() == count) {
							break;
						}
					}
				}// found
			}// it
			return (true);
		}// find_indiv_values
		bool find_variable_distinct_values(VariableType &oVar, strings_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			if (!this->find_variable(oVar, false)) {
				return (false);
			}
			const IDTYPE nVarId = oVar.id();
			const values_vector &values = this->m_values;
			oList.clear();
			size_t offset = 0;
			strings_set oSet;
			for (auto it = values.begin(); it != values.end(); ++it) {
				const ValueType &v = *it;
				if (v.variable_id() == nVarId) {
					if (offset >= skip) {
						boost::any val = v.value();
						STRINGTYPE sval;
						convert_to_string(val, sval);
						if (!sval.empty()) {
							if (oSet.find(sval) == oSet.end()) {
								oSet.insert(sval);
								oList.push_back(sval);
								++offset;
								if (oList.size() == count) {
									break;
								}
							}
						}// not empty
					}
				}// found
			}// it
			return (true);
		}//find_variable_distinct_values
		bool find_variable_values(VariableType &oVar, values_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			if (!this->find_variable(oVar, false)) {
				return (false);
			}
			const IDTYPE nVarId = oVar.id();
			const values_vector &values = this->m_values;
			oList.clear();
			size_t offset = 0;
			for (auto it = values.begin(); it != values.end(); ++it) {
				const ValueType &v = *it;
				if (v.variable_id() == nVarId) {
					if (offset >= skip) {
						ValueType oo(v);
						oList.push_back(oo);
						++offset;
						if (oList.size() == count) {
							break;
						}
					}
				}// found
			}// it
			return (true);
		}// find_variables_values
		bool find_dataset_values(values_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			const values_vector &vv = this->m_values;
			const size_t nMax = vv.size();
			size_t nStart = skip;
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			if (nEnd > nStart) {
				const size_t nc = nEnd - nStart;
				oList.resize(nc);
				for (size_t i = 0; i < nc; ++i) {
					oList[i] = vv[nStart + i];
				}// i
			}
			else {
				oList.clear();
			}
			return (true);
		}// find_dataset_values
		bool find_dataset_values_count(size_t &nCount) {
			nCount = this->m_values.size();
			return (true);
		}//find_dataset_values_count
		bool maintains_values(const values_vector &oList, bool bRemove = false) {
			values_vector &values = this->m_values;
			IDTYPE nMax = 0;
			for (auto &v : oList) {
				IDTYPE nx = v.id();
				if (nx > nMax) {
					nMax = nx;
				}
			}// v
			if (nMax > this->m_lastid) {
				this->m_lastid = (IDTYPE) (nMax + 1);
			}
			const IDTYPE nDatasetId = this->m_oset.id();
			for (auto &oVal : oList) {
				const InfoValue &vx = oVal.value();
				ValueType xVal(oVal);
				this->find_value(xVal, false);
				IDTYPE nId = xVal.id();
				bool bMustRemove = bRemove && (nId != 0);
				if ((nId != 0) && vx.empty()) {
					bMustRemove = true;
				}
				if (bMustRemove) {
					auto it = std::find_if(values.begin(), values.end(), [&](ValueType &v)->bool {
						return (v.id() == nId);
					});
					if (it != values.end()) {
						values.erase(it);
					}
				}
				else if (oVal.is_writeable()) {
					if (vx.empty() && (nId != 0)) {
						auto it = std::find_if(values.begin(), values.end(), [&](ValueType &v)->bool {
							return (v.id() == nId);
						});
						if (it != values.end()) {
							values.erase(it);
						}
					}
					else {
						STRINGTYPE status = oVal.status();
						IDTYPE nVarId = oVal.variable_id();
						IDTYPE nIndId = oVal.indiv_id();
						if (nId != 0) {
							const size_t n = values.size();
							for (size_t i = 0; i < n; ++i) {
								ValueType &vz = values[i];
								if (vz.id() == nId) {
									vz.value(vx);
									vz.status(status);
									break;
								}
							}// i
						}
						else {
							VariableType xx(nVarId);
							IndivType yy(nIndId);
							xx.dataset_id(nDatasetId);
							yy.dataset_id(nDatasetId);
							if (this->find_variable(xx, false) && this->find_indiv(yy, false)) {
								ValueType oo(oVal);
								oo.variable_id(nVarId);
								oo.indiv_id(nIndId);
								oo.value(vx);
								IDTYPE nx = oo.id();
								if (nx == 0) {
									oo.id(this->next_id());
								}
								values.push_back(oo);
							}
						}// insert
					}
				}// write
			}// oVal
			return (true);
		}// maintains_values
		bool find_value(ValueType &cur, bool bLock = true) {
			IDTYPE nId = cur.id();
			IDTYPE nVarId = cur.variable_id();
			IDTYPE nIndId = cur.indiv_id();
			cur.clear();
			if (!bLock) {
				const values_vector &vv = this->m_values;
				if (nId != 0) {
					auto it = std::find_if(vv.begin(), vv.end(), [nId](const ValueType &p)->bool {
						return (p.id() == nId);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				else if ((nVarId != 0) && (nIndId != 0)) {
					auto it = std::find_if(vv.begin(), vv.end(), [nVarId,nIndId](const ValueType &p)->bool {
						return ((p.variable_id() == nVarId) && (p.indiv_id() == nIndId));
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				return (false);
			}
			else {
				const values_vector &vv = this->m_values;
				if (nId != 0) {
					auto it = std::find_if(vv.begin(), vv.end(), [nId](const ValueType &p)->bool {
						return (p.id() == nId);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				else if ((nVarId != 0) && (nIndId != 0)) {
					auto it = std::find_if(vv.begin(), vv.end(), [nVarId,nIndId](const ValueType &p)->bool {
						return ((p.variable_id() == nVarId) && (p.indiv_id() == nIndId));
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
			}
			return (false);
		}// find_value
		//
		bool remove_indivs(const indivs_vector &oList) {
			indivs_vector &variables = this->m_indivs;
			values_vector &values = this->m_values;
			for (auto &p : oList) {
				IndivType xVar(p);
				if (this->find_indiv(xVar, false)) {
					IDTYPE nVarId = xVar.id();
					ints_set oVals;
					for (auto &pv : values) {
						if (pv.indiv_id() == nVarId) {
							oVals.insert(pv.id());
						}
					}
					for (auto &nvalid : oVals) {
						auto jt = std::find_if(values.begin(), values.end(), [&](ValueType &v)->bool {
							return (v.id() == nvalid);
						});
						if (jt != values.end()) {
							values.erase(jt);
						}
					}
					auto it = std::find_if(variables.begin(), variables.end(), [&](IndivType &px)->bool {
						return (px.id() == nVarId);
					});
					if (it != variables.end()) {
						variables.erase(it);
					}
				}// found
			}// p
			return (true);
		}// remove_indivs
		bool maintains_indivs(const indivs_vector &oList) {
			IDTYPE nCurrentDatasetId = this->get_datasetid();
			indivs_vector &vv = this->m_indivs;
			IDTYPE nMax = 0;
			for (auto &v : oList) {
				IDTYPE nx = v.id();
				if (nx > nMax) {
					nMax = nx;
				}
			}
			if (nMax > this->m_lastid) {
				this->m_lastid = (IDTYPE)(nMax + 1);
			}
			for (auto &oVar : oList) {
				if (oVar.is_writeable()) {
					IndivType xVar(oVar);
					this->find_indiv(xVar, false);
					IDTYPE nId = xVar.id();
					if (nId == 0) {
						xVar = oVar;
						xVar.version(1);
						xVar.dataset_id(nCurrentDatasetId);
						IDTYPE xId = xVar.id();
						if (xId == 0) {
							xVar.id(this->next_id());
						}
						vv.push_back(xVar);
					}
					else {
						const size_t n = vv.size();
						for (size_t i = 0; i < n; ++i) {
							IndivType &v0 = vv[i];
							if (v0.id() == nId) {
								INTTYPE old = v0.version();
								v0 = oVar;
								v0.id(nId);
								v0.version(old + 1);
								break;
							}
						}// i
					}
				}// writeable
			}// oVar
			return (true);
		}// maintains_indivs
		bool find_indiv(IndivType &cur, bool bLock = true) {
			IDTYPE nCurrentDatasetId = this->get_datasetid();
			IDTYPE nId = cur.id();
			IDTYPE nDatasetId = cur.dataset_id();
			STRINGTYPE sigle = cur.sigle();
			cur.clear();
			if (!bLock) {
				const indivs_vector &vv = this->m_indivs;
				if (nId != 0) {
					auto it = std::find_if(vv.begin(), vv.end(), [nId](const IndivType &p)->bool {
						return (p.id() == nId);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				else if ((nDatasetId == nCurrentDatasetId) && (!sigle.empty())) {
					auto it = std::find_if(vv.begin(), vv.end(), [sigle](const IndivType &p)->bool {
						STRINGTYPE s = p.sigle();
						return (s == sigle);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				return (false);
			}
			else {
				const indivs_vector &vv = this->m_indivs;
				if (nId != 0) {
					auto it = std::find_if(vv.begin(), vv.end(), [nId](const IndivType &p)->bool {
						return (p.id() == nId);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				else if ((nDatasetId == nCurrentDatasetId) && (!sigle.empty())) {
					auto it = std::find_if(vv.begin(), vv.end(), [sigle](const IndivType &p)->bool {
						STRINGTYPE s = p.sigle();
						return (s == sigle);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
			}
			return (false);
		}
		bool get_indivs_ids(ints_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			const indivs_vector &vv = this->m_indivs;
			const size_t nMax = vv.size();
			size_t nStart = skip;
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			if (nEnd > nStart) {
				const size_t nc = nEnd - nStart;
				oList.resize(nc);
				for (size_t i = 0; i < nc; ++i) {
					oList[i] = (vv[nStart + i]).id();
				}// i
			}
			else {
				oList.clear();
			}
			return (true);
		}// get_indivs_ids
		bool get_indivs(indivs_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			const indivs_vector &vv = this->m_indivs;
			const size_t nMax = vv.size();
			size_t nStart = skip;
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			if (nEnd > nStart) {
				const size_t nc = nEnd - nStart;
				oList.resize(nc);
				for (size_t i = 0; i < nc; ++i) {
					oList[i] = vv[nStart + i];
				}// i
			}
			else {
				oList.clear();
			}
			return (true);
		}// get_indivs
		bool get_indivs_count(size_t &nCount) {
			nCount = this->m_indivs.size();
			return (true);
		}
		//
		bool find_dataset_variables_types(ints_string_map &oMap) {
			oMap.clear();
			variables_vector &vv = this->m_variables;
			std::for_each(vv.begin(), vv.end(), [&](VariableType &v) {
				STRINGTYPE s = v.vartype();
				IDTYPE key = v.id();
				oMap[key] = s;
			});
			return (true);
		}//find_dataset_variables_types
		bool remove_variables(const variables_vector &oList) {
			variables_vector &variables = this->m_variables;
			values_vector &values = this->m_values;
			for (auto &p : oList) {
				VariableType xVar(p);
				if (this->find_variable(xVar, false)) {
					IDTYPE nVarId = xVar.id();
					ints_set oVals;
					for (auto &pv : values) {
						if (pv.variable_id() == nVarId) {
							oVals.insert(pv.id());
						}
					}
					for (auto & nvalid : oVals) {
						auto jt = std::find_if(values.begin(), values.end(), [&](ValueType &v)->bool {
							return (v.id() == nvalid);
						});
						if (jt != values.end()) {
							values.erase(jt);
						}
					}
					auto it = std::find_if(variables.begin(), variables.end(), [&](VariableType &px)->bool {
						return (px.id() == nVarId);
					});
					if (it != variables.end()) {
						variables.erase(it);
					}
				}// found
			}
			return (true);
		}// remove_variables
		bool maintains_variables(const variables_vector &oList) {
			IDTYPE nCurrentDatasetId = this->get_datasetid();
			variables_vector &vv = this->m_variables;
			IDTYPE nMax = 0;
			for (auto &v : oList) {
				IDTYPE nx = v.id();
				if (nx > nMax) {
					nMax = nx;
				}
			}
			if (nMax > this->m_lastid) {
				this->m_lastid = (IDTYPE)(nMax + 1);
			}
			for (auto &oVar : oList) {
				if (oVar.is_writeable()) {
					VariableType xVar(oVar);
					this->find_variable(xVar, false);
					IDTYPE nId = xVar.id();
					if (nId == 0) {
						xVar = oVar;
						xVar.version(1);
						xVar.dataset_id(nCurrentDatasetId);
						IDTYPE xId = xVar.id();
						if (xId == 0) {
							xVar.id(this->next_id());
						}
						vv.push_back(xVar);
					}
					else {
						const size_t n = vv.size();
						for (size_t i = 0; i < n; ++i) {
							VariableType &v0 = vv[i];
							if (v0.id() == nId) {
								INTTYPE old = v0.version();
								v0 = oVar;
								v0.id(nId);
								v0.version(old + 1);
								break;
							}
						}// i
					}
				}// writeable
			}// oVar
			return (true);
		}// maintains_variables
		bool find_variable(VariableType &cur, bool bLock = true) {
			IDTYPE nCurrentDatasetId = this->get_datasetid();
			IDTYPE nId = cur.id();
			IDTYPE nDatasetId = cur.dataset_id();
			STRINGTYPE sigle = cur.sigle();
			cur.clear();
			if (!bLock) {
				const variables_vector &vv = this->m_variables;
				if (nId != 0) {
					auto it = std::find_if(vv.begin(), vv.end(), [nId](const VariableType &p)->bool {
						return (p.id() == nId);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				else if ((nDatasetId == nCurrentDatasetId) && (!sigle.empty())) {
					auto it = std::find_if(vv.begin(), vv.end(), [sigle](const VariableType &p)->bool {
						STRINGTYPE s = p.sigle();
						return (s == sigle);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				return (false);
			}
			else {
				const variables_vector &vv = this->m_variables;
				if (nId != 0) {
					auto it = std::find_if(vv.begin(), vv.end(), [nId](const VariableType &p)->bool {
						return (p.id() == nId);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
				else if ((nDatasetId == nCurrentDatasetId) && (!sigle.empty())) {
					auto it = std::find_if(vv.begin(), vv.end(), [sigle](const VariableType &p)->bool {
						STRINGTYPE s = p.sigle();
						return (s == sigle);
					});
					if (it != vv.end()) {
						cur = *it;
						return (true);
					}
				}
			}
			return (false);
		}
		bool get_variables_ids(ints_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			const variables_vector &vv = this->m_variables;
			const size_t nMax = vv.size();
			size_t nStart = skip;
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			if (nEnd > nStart) {
				const size_t nc = nEnd - nStart;
				oList.resize(nc);
				for (size_t i = 0; i < nc; ++i) {
					oList[i] = (vv[nStart + i]).id();
				}// i
			}
			else {
				oList.clear();
			}
			return (true);
		}// get_variables_ids
		bool get_variables(variables_vector &oList, size_t skip, size_t count) {
			if (count < 1) {
				count = 100;
			}
			const variables_vector &vv = this->m_variables;
			const size_t nMax = vv.size();
			size_t nStart = skip;
			if (nStart > nMax) {
				nStart = nMax;
			}
			size_t nEnd = nStart + count;
			if (nEnd > nMax) {
				nEnd = nMax;
			}
			if (nEnd > nStart) {
				const size_t nc = nEnd - nStart;
				oList.resize(nc);
				for (size_t i = 0; i < nc; ++i) {
					oList[i] = vv[nStart + i];
				}// i
			}
			else {
				oList.clear();
			}
			return (true);
		}// get_variables
		bool get_variables_count(size_t &nCount) {
			nCount = this->m_variables.size();
			return (true);
		}
		//
		bool update_dataset(DatasetType &cur) {
			IDTYPE nId = cur.id();
			STRINGTYPE sigle = cur.sigle();
			if ((nId == 0) || sigle.empty()) {
				return (false);
			}
			{
				DatasetType &vv = this->m_oset;
				if (vv.id() == nId) {
					vv.version(vv.version() + 1);
					vv.sigle(cur.sigle());
					vv.name(cur.name());
					vv.status(cur.status());
					vv.description(cur.description());
					return (true);
				}

			}
			return (false);
		}// update_dataset
		bool find_dataset(IDTYPE nId, const STRINGTYPE &sigle) {
			DatasetType &vv = this->m_oset;
			if ((nId != 0) && (vv.id() == nId)) {
				return (true);
			}
			if (!sigle.empty()) {
				STRINGTYPE ss = vv.sigle();
				return (ss == sigle);
			}
			return (false);
		}//find_dataset
		bool find_dataset(DatasetType &cur) {
			DatasetType &vv = this->m_oset;
			if (cur.id() == vv.id()) {
				cur = vv;
				return (true);
			}
			else {
				STRINGTYPE s1 = cur.sigle();
				STRINGTYPE s2 = vv.sigle();
				if (s1 == s2) {
					cur = vv;
					return (true);
				}
			}
			return (false);
		}// find_dataset
		void get_dataset(DatasetType &oCur) {
			oCur = this->m_oset;
		}
		IDTYPE get_datasetid(void) {
			IDTYPE nRet = this->m_oset.id();
			return (nRet);
		}//get_datasetid
		//
	}; // class MemoryDataset<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>
	////////////////////////////////////
}// namespâce info
//////////////////////////////////
#endif // !__MEMORYDATASET_H__

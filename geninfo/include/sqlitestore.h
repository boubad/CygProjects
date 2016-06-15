#pragma once
#ifndef __SQLITESTORE_H__
#define __SQLITESTORE_H__
/////////////////////////////////////
#include "istatstore.h"
#include "stringconvert.h"
#include "infosqlitestore.h"
/////////////////////////////////////////
namespace info {
	/////////////////////////////////
	template <typename IDTYPE, typename INTTYPE, typename STRINGTYPE, typename WEIGHTYPE>
	class SQLiteStore : public IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>, private boost::noncopyable {
		using IDTYPE_BASE = unsigned long;
		using INTTYPE_BASE = unsigned long;
		using STRINGTYPE_BASE = std::string;
		using WEIGHTYPE_BASE = double;
		//
		using strings_vector_base = std::vector<STRINGTYPE_BASE>;
		using ints_vector_base = std::vector<IDTYPE_BASE>;
		using ints_string_map_base = std::map<IDTYPE_BASE, STRINGTYPE_BASE>;
		using DatasetType_base = StatDataset<IDTYPE_BASE, INTTYPE_BASE, STRINGTYPE_BASE>;
		using datasets_vector_base = std::vector<DatasetType_base>;
		using VariableType_base = StatVariable<IDTYPE_BASE, INTTYPE_BASE, STRINGTYPE_BASE, WEIGHTYPE_BASE>;
		using variables_vector_base = std::vector<VariableType_base>;
		using IndivType_base = StatIndiv<IDTYPE_BASE, INTTYPE_BASE, STRINGTYPE_BASE, WEIGHTYPE_BASE>;
		using indivs_vector_base = std::vector<IndivType_base>;
		using ValueType_base = StatValue<IDTYPE_BASE, INTTYPE_BASE, STRINGTYPE_BASE>;
		using values_vector_base = std::vector<ValueType_base>;
	public:
		using StoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
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
		std::unique_ptr<SQLiteStatHelper> m_helper;
	public:
		SQLiteStore() {
			this->m_helper.reset(new SQLiteStatHelper());
			assert(this->m_helper.get() != nullptr);
			assert(this->m_helper->is_valid());
		}
		SQLiteStore(const STRINGTYPE &sDatabaseName) {
			this->m_helper.reset(new SQLiteStatHelper(sDatabaseName));
			assert(this->m_helper.get() != nullptr);
			assert(this->m_helper->is_valid());
		}
		virtual ~SQLiteStore() {}
	private:
		template <typename SOURCE, typename DEST>
		static void convert_value(const SOURCE &src, DEST &dest, bool bIn = false) {
			if (bIn) {
				dest.id((IDTYPE_BASE)src.id());
				dest.version((INTTYPE_BASE)src.version());
				dest.variable_id((IDTYPE_BASE)src.variable_id());
				dest.indiv_id((IDTYPE_BASE)src.indiv_id());
				dest.dataset_id((IDTYPE_BASE)src.dataset_id());
				STRINGTYPE_BASE s;
				info_convert_string(src.status(), s);
				dest.status(s);
			}
			else {
				dest.id((IDTYPE)src.id());
				dest.version((INTTYPE)src.version());
				dest.variable_id((IDTYPE)src.variable_id());
				dest.indiv_id((IDTYPE)src.indiv_id());
				dest.dataset_id((IDTYPE)src.dataset_id());
				STRINGTYPE s;
				info_convert_string(src.status(), s);
				dest.status(s);
			}
			dest.value(src.value());
		}// convert_value
		template <typename SOURCE, typename DEST>
		static void convert_variable(const SOURCE &src, DEST &dest, bool bIn = false) {
			if (bIn) {
				dest.id((IDTYPE_BASE)src.id());
				dest.version((INTTYPE_BASE)src.version());
				dest.dataset_id((IDTYPE_BASE)src.dataset_id());
				dest.weight((WEIGHTYPE_BASE)src.weight());
				STRINGTYPE_BASE s;
				info_convert_string(src.status(), s);
				dest.status(s);
				info_convert_string(src.sigle(), s);
				dest.sigle(s);
				info_convert_string(src.name(), s);
				dest.name(s);
				info_convert_string(src.description(), s);
				dest.description(s);
				info_convert_string(src.vartype(), s);
				dest.vartype(s);
				info_convert_string(src.genre(), s);
				dest.genre(s);
			}
			else {
				dest.id((IDTYPE)src.id());
				dest.version((INTTYPE)src.version());
				dest.dataset_id((IDTYPE)src.dataset_id());
				dest.weight((WEIGHTYPE)src.weight());
				STRINGTYPE s;
				std::string sx = src.status();
				info_convert_string(sx, s);
				dest.status(s);
				info_convert_string(src.sigle(), s);
				dest.sigle(s);
				info_convert_string(src.name(), s);
				dest.name(s);
				info_convert_string(src.description(), s);
				dest.description(s);
				info_convert_string(src.vartype(), s);
				dest.vartype(s);
				info_convert_string(src.genre(), s);
				dest.genre(s);
			}
			dest.is_categ(src.is_categ());
			dest.modalites_count(src.modalites_count());

		}// convert_variable
		template <typename SOURCE, typename DEST>
		static void convert_indiv(const SOURCE &src, DEST &dest, bool bIn = false) {
			if (bIn) {
				dest.id((IDTYPE_BASE)src.id());
				dest.version((INTTYPE_BASE)src.version());
				dest.dataset_id((IDTYPE_BASE)src.dataset_id());
				dest.weight((WEIGHTYPE_BASE)src.weight());
				STRINGTYPE_BASE s;
				info_convert_string(src.status(), s);
				dest.status(s);
				info_convert_string(src.sigle(), s);
				dest.sigle(s);
				info_convert_string(src.name(), s);
				dest.name(s);
				info_convert_string(src.description(), s);
				dest.description(s);
			}
			else {
				dest.id((IDTYPE)src.id());
				dest.version((INTTYPE)src.version());
				dest.dataset_id((IDTYPE)src.dataset_id());
				dest.weight((WEIGHTYPE)src.weight());
				STRINGTYPE s;
				info_convert_string(src.status(), s);
				dest.status(s);
				info_convert_string(src.sigle(), s);
				dest.sigle(s);
				info_convert_string(src.name(), s);
				dest.name(s);
				info_convert_string(src.description(), s);
				dest.description(s);
			}
		}// convert_indiv
		template <typename SOURCE, typename DEST>
		static void convert_dataset(const SOURCE &src, DEST &dest, bool bIn = false) {
			if (bIn) {
				dest.id((IDTYPE_BASE)src.id());
				dest.version((INTTYPE_BASE)src.version());
				STRINGTYPE_BASE s;
				info_convert_string(src.status(), s);
				dest.status(s);
				info_convert_string(src.sigle(), s);
				dest.sigle(s);
				info_convert_string(src.name(), s);
				dest.name(s);
				info_convert_string(src.description(), s);
				dest.description(s);
			}
			else {
				dest.id((IDTYPE)src.id());
				dest.version((INTTYPE)src.version());
				STRINGTYPE s;
				info_convert_string(src.status(), s);
				dest.status(s);
				info_convert_string(src.sigle(), s);
				dest.sigle(s);
				info_convert_string(src.name(), s);
				dest.name(s);
				info_convert_string(src.description(), s);
				dest.description(s);
			}
		}// convert_dataset
		//
		static void convert_v(const std::vector<IDTYPE_BASE> &src, std::vector<IDTYPE> &dest) {
			const size_t n = src.size();
			dest.resize(n);
			for (size_t i = 0; i < n; ++i) {
				dest[i] = (IDTYPE)src[i];
			}
		}
		static void convert_v(const ints_string_map_base &src, ints_string_map &dest) {
			dest.clear();
			for (auto &p : src) {
				IDTYPE key = (IDTYPE)p.first;
				STRINGTYPE s;
				info_convert_string(p.second, s);
				dest[key] = s;
			}// p
		}
		static void convert_v(const strings_vector_base &src, strings_vector &dest) {
			const size_t n = src.size();
			dest.resize(n);
			for (size_t i = 0; i < n; ++i) {
				STRINGTYPE s;
				info_convert_string(src[i], s);
				dest[i] = s;
			}
		}
		template <typename SOURCE, typename DEST>
		static void convert_values(const std::vector<SOURCE> &src, std::vector<DEST> &dest, bool bIn = false) {
			dest.clear();
			for (auto &p : src) {
				DEST r;
				convert_value(p, r, bIn);
				dest.push_back(r);
			}
		}// convert_v
		template <typename SOURCE, typename DEST>
		static void convert_variables(const std::vector<SOURCE> &src, std::vector<DEST> &dest, bool bIn = false) {
			dest.clear();
			for (auto &p : src) {
				DEST r;
				convert_variable(p, r, bIn);
				dest.push_back(r);
			}
		}// convert_v
		template <typename SOURCE, typename DEST>
		static void convert_indivs(const std::vector<SOURCE> &src, std::vector<DEST> &dest, bool bIn = false) {
			dest.clear();
			for (auto &p : src) {
				DEST r;
				convert_indiv(p, r, bIn);
				dest.push_back(r);
			}
		}// convert_v
		template <typename SOURCE, typename DEST>
		static void convert_datasets(const std::vector<SOURCE> &src, std::vector<DEST> &dest, bool bIn = false) {
			dest.clear();
			for (auto &p : src) {
				DEST r;
				convert_dataset(p, r, bIn);
				dest.push_back(r);
			}
		}// convert_v
	public:
		// overrides
		virtual bool is_valid(void) {
			return (this->m_helper->is_valid());
		}
		virtual void begin_transaction(void) {
			this->m_helper->begin_transaction();
		}
		virtual void commit_transaction(void) {
			this->m_helper->commit_transaction();
		}
		virtual void rollback_transaction(void) {
			this->m_helper->rollback_transaction();
		}
		//
		virtual bool find_all_datasets_count(size_t &nCount) {
			return (this->m_helper->find_all_datasets_count(nCount));
		}
		virtual bool find_all_datasets(datasets_vector &oList, size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			datasets_vector_base dest;
			if (!this->m_helper->find_all_datasets(dest, skip, count)) {
				return (false);
			}
			convert_datasets(dest, oList);
			return (true);
		}
		virtual bool find_all_datasets_ids(ints_vector &oList, size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			ints_vector_base dest;
			if (!this->m_helper->find_all_datasets_ids(dest, skip, count)) {
				return (false);
			}
			convert_v(dest, oList);
			return (true);
		}
		virtual bool find_dataset(DatasetType &cur) {
			DatasetType_base dest;
			convert_dataset(cur, dest, true);
			if (!this->m_helper->find_dataset(dest)) {
				return (false);
			}
			convert_dataset(dest, cur);
			return (true);
		}
		virtual bool maintains_dataset(DatasetType &cur, bool bCommit = true) {
			DatasetType_base dest;
			convert_dataset(cur, dest, true);
			if (!this->m_helper->maintains_dataset(dest, bCommit)) {
				return (false);
			}
			convert_dataset(dest, cur);
			return (true);
		}
		virtual bool remove_dataset(const DatasetType &cur, bool bCommit = true) {
			DatasetType_base dest;
			convert_dataset(cur, dest, true);
			if (!this->m_helper->remove_dataset(dest, bCommit)) {
				return (false);
			}
			return (true);
		}
		//
		virtual bool find_dataset_variables_types(const DatasetType &oSet, ints_string_map &oMap) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			ints_string_map_base temp;
			if (!this->m_helper->find_dataset_variables_types(dest, temp)) {
				return (false);
			}
			convert_v(temp, oMap);
			return (true);
		}
		virtual bool find_dataset_variables_count(const DatasetType &oSet, size_t &nCount) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			if (!this->m_helper->find_dataset_variables_count(dest, nCount)) {
				return (false);
			}
			return (true);
		}
		virtual bool find_dataset_variables_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			ints_vector_base temp;
			if (!this->m_helper->find_dataset_variables_ids(dest, temp, skip, count)) {
				return (false);
			}
			convert_v(temp, oList);
			return (true);
		}
		virtual bool find_dataset_variables(const DatasetType &oSet, variables_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			variables_vector_base temp;
			if (!this->m_helper->find_dataset_variables(dest, temp, skip, count)) {
				return (false);
			}
			convert_variables(temp, oList);
			return (true);
		}
		virtual bool find_variable(VariableType &cur) {
			VariableType_base dest;
			convert_variable(cur, dest, true);
			if (!this->m_helper->find_variable(dest)) {
				return (false);
			}
			convert_variable(dest, cur);
			return (true);
		}
		virtual bool maintains_variables(const variables_vector &oVars, bool bCommit = true,
			bool bRemove = false) {
			variables_vector_base dest;
			convert_variables(oVars, dest, true);
			if (!this->m_helper->maintains_variables(dest, bCommit, bRemove)) {
				return (false);
			}
			return (true);
		}
		virtual bool remove_variable(const VariableType &oVar, bool bCommit = true) {
			VariableType_base dest;
			convert_variable(oVar, dest, true);
			if (!this->m_helper->remove_variable(dest, bCommit)) {
				return (false);
			}
			return (true);
		}
		//
		virtual bool find_dataset_indivs_count(const DatasetType &oSet, size_t &nCount) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			if (!this->m_helper->find_dataset_indivs_count(dest, nCount)) {
				return (false);
			}
			return (true);
		}
		virtual bool find_dataset_indivs_ids(const DatasetType &oSet, ints_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			DatasetType_base dest;
			convert_dataset(oSet, dest);
			ints_vector_base temp;
			if (!this->m_helper->find_dataset_indivs_ids(dest, temp, skip, count)) {
				return (false);
			}
			convert_v(temp, oList);
			return (true);
		}
		virtual bool find_dataset_indivs(const DatasetType &oSet, indivs_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			indivs_vector_base temp;
			if (!this->m_helper->find_dataset_indivs(dest, temp, skip, count)) {
				return (false);
			}
			convert_indivs(temp, oList);
			return (true);
		}
		virtual bool find_indiv(IndivType &cur) {
			IndivType_base dest;
			convert_indiv(cur, dest);
			if (!this->m_helper->find_indiv(dest)) {
				return (false);
			}
			convert_indiv(dest, cur);
			return (true);
		}
		virtual bool maintains_indivs(const indivs_vector &oInds, bool bCommit = true,
			bool bRemove = false) {
			indivs_vector_base dest;
			convert_indivs(oInds, dest, true);
			if (!this->m_helper->maintains_indivs(dest, bCommit, bRemove)) {
				return (false);
			}
			return (true);
		}
		virtual bool remove_indiv(const IndivType &oInd, bool bCommit = true) {
			IndivType_base dest;
			convert_indiv(oInd, dest);
			if (!this->m_helper->remove_indiv(dest, bCommit)) {
				return (false);
			}
			return (true);
		}
		//
		virtual bool find_value(ValueType &cur) {
			ValueType_base dest;
			convert_value(cur, dest, true);
			if (!this->m_helper->find_value(dest)) {
				return (false);
			}
			convert_value(dest, cur);
			return (true);
		}
		virtual bool maintains_values(const values_vector &oVals, bool bCommit = true,
			bool bRemove = false) {
			values_vector_base dest;
			convert_values(oVals, dest, true);
			if (!this->m_helper->maintains_values(dest, bCommit, bRemove)) {
				return (false);
			}
			return (true);
		}
		virtual bool find_dataset_values_count(const DatasetType &oSet, size_t &nCount) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			if (!this->m_helper->find_dataset_values_count(dest, nCount)) {
				return (false);
			}
			return (true);
		}
		virtual bool find_dataset_values(const DatasetType &oSet, values_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			DatasetType_base dest;
			convert_dataset(oSet, dest, true);
			values_vector_base temp;
			if (!this->m_helper->find_dataset_values(dest, temp, skip, count)) {
				return (false);
			}
			convert_values(temp, oList);
			return (true);
		}
		virtual bool find_variable_values(VariableType &oVar, values_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			VariableType_base dest;
			convert_variable(oVar, dest, true);
			values_vector_base temp;
			if (!this->m_helper->find_variable_values(dest, temp, skip, count)) {
				return (false);
			}
			convert_variable(dest, oVar);
			convert_values(temp, oList);
			return (true);
		}
		virtual bool find_variable_distinct_values(VariableType &oVar, strings_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			VariableType_base dest;
			convert_variable(oVar, dest, true);
			strings_vector_base temp;
			if (!this->m_helper->find_variable_distinct_values(dest, temp, skip, count)) {
				return (false);
			}
			convert_variable(dest, oVar);
			convert_v(temp, oList);
			return (true);
		}
		virtual bool find_indiv_values(IndivType &oInd, values_vector &oList,
			size_t skip = 0, size_t count = StoreType::DATATRANSFER_CHUNK_SIZE) {
			IndivType_base dest;
			convert_indiv(oInd, dest, true);
			values_vector_base temp;
			if (!this->m_helper->find_indiv_values(dest, temp, skip, count)) {
				return (false);
			}
			convert_indiv(dest, oInd);
			convert_values(temp, oList);
			return (true);
		}
		virtual bool find_indiv_values_count(IndivType &oInd, size_t &nc) {
			IndivType_base dest;
			convert_indiv(oInd, dest, true);
			if (!this->m_helper->find_indiv_values_count(dest, nc)) {
				return (false);
			}
			convert_indiv(dest, oInd);
			return (true);
		}
		virtual bool find_variable_values_count(VariableType &oVar, size_t &nc) {
			VariableType_base dest;
			convert_variable(oVar, dest, true);
			if (!this->m_helper->find_variable_values_count(dest, nc)) {
				return (false);
			}
			convert_variable(dest, oVar);
			return (true);
		}
	};// class SQLiteStore<IDTYPE,INTTYPE,STRINGTYPE,WEIGHTYPE>
	///////////////////////////////////
}// namespace info
////////////////////////////////////////
#endif // !__SQLITESTORE_H__

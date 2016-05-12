#include "../include/workindivprovider.h"
////////////////
namespace info {
	///////////////////////////////////////////
	WorkerIndivProvider::WorkerIndivProvider(IIndivProvider *pProvider) :m_provider(pProvider)
	{
		assert(this->m_provider != nullptr);
		assert(this->m_provider->is_valid());
		variables_map &ovars = this->m_vars;
		bool bRet = this->m_provider->get_variables_map(ovars);
		assert(bRet);
		ints_set &oSet = this->m_filter;
		oSet.clear();
		typedef std::pair<IntType, DBStatVariable> MyPair;
		std::for_each(ovars.begin(), ovars.end(), [&oSet](const MyPair &p) {
			oSet.insert(p.first);
		});
	}
	WorkerIndivProvider::~WorkerIndivProvider() {
		this->m_provider = nullptr;
	}
	void WorkerIndivProvider::filter_indiv(Indiv &oInd) const {
		DbValueMap oRes;
		typedef std::pair<IntType, DbValue> MyPair;
		const DbValueMap &oSrc = oInd.data();
		const ints_set &oSet = this->m_filter;
		std::for_each(oSrc.begin(), oSrc.end(), [&](const MyPair &oPair) {
			const IntType key = oPair.first;
			DbValue val = oPair.second;
			if (!val.empty()) {
				if (oSet.find(key) != oSet.end()) {
					oRes[key] = oPair.second;
				}
			}// not empty
		});
		oInd.set_data(oRes);
	}

	bool WorkerIndivProvider::find_indiv_filtered(const IntType aIndex, Indiv &oInd, const VariableMode mode /*= VariableMode::modeAll*/) {
		assert(this->is_valid());
		IIndivProvider *p = this->m_provider;
		assert(p != nullptr);
		if (!p->find_indiv(aIndex, oInd, mode)) {
			return (false);
		}
		this->filter_indiv(oInd);
		return (true);
	}
	bool WorkerIndivProvider::find_indiv_at_filtered(const size_t pos, Indiv &oInd, const VariableMode mode /*= VariableMode::modeAll*/) {
		assert(this->is_valid());
		IIndivProvider *p = this->m_provider;
		assert(p != nullptr);
		if (!p->find_indiv_at(pos, oInd, mode)) {
			return (false);
		}
		this->filter_indiv(oInd);
		return (true);
	}
	void WorkerIndivProvider::get_filter(variables_vector &oVars) const {
		oVars.clear();
		const ints_set &oSet = this->m_filter;
		const variables_map &xmap = this->m_vars;
		std::for_each(oSet.begin(), oSet.end(), [&](const IntType &key) {
			auto it = xmap.find(key);
			if (it != xmap.end()) {
				DBStatVariable v = (*it).second;
				oVars.push_back(v);
			}
		});
	}
	void WorkerIndivProvider::set_filter(const variables_vector &oVars) {
		ints_set &oSet = this->m_filter;
		oSet.clear();
		std::for_each(oVars.begin(), oVars.end(), [&](const DBStatVariable &oVar) {
			oSet.insert(oVar.id());
		});
	}
	bool WorkerIndivProvider::reset(void) {
		IIndivProvider *p = this->m_provider;
		if (p == nullptr) {
			return (false);
		}
		if (!p->is_valid()) {
			return (false);
		}
		this->m_current = 0;
		return (true);
	}
	bool WorkerIndivProvider::next(Indiv &oInd, const VariableMode mode /*= VariableMode::modeAll*/) {
		IIndivProvider *p = this->m_provider;
		if (p == nullptr) {
			return (false);
		}
		if (!p->is_valid()) {
			return (false);
		}
		size_t nMax = 0;
		if (!p->indivs_count(nMax)) {
			return (false);
		}
		bool bRet = false;
		size_t n = this->m_current;
		if (n < nMax) {
			bRet = p->find_indiv_at(n, oInd, mode);
		}
		if (bRet) {
			this->m_current = n + 1;
		}
		this->filter_indiv(oInd);
		return (bRet);
	} // next
	bool WorkerIndivProvider::is_valid(void) {
		bool bRet = (this->m_provider != nullptr) && (this->m_provider->is_valid());
		return (bRet);
	}
	bool WorkerIndivProvider::indivs_count(size_t &nCount) {
		assert(this->is_valid());
		return (this->m_provider->indivs_count(nCount));
	}
	bool WorkerIndivProvider::find_indiv(const IntType aIndex, Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeA*/) {
		assert(this->is_valid());
		return (this->find_indiv_filtered(aIndex, oInd, mode));
	}
	bool WorkerIndivProvider::find_indiv_at(const size_t pos, Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeA*/) {
		assert(this->is_valid());
		return (this->find_indiv_at_filtered(pos, oInd, mode));
	}

	bool WorkerIndivProvider::get_variables_map(variables_map &ovars) {
		assert(this->is_valid());
		const variables_map &srcMap = this->m_vars;
		const ints_set &oSet = this->m_filter;
		ovars.clear();
		std::for_each(oSet.begin(), oSet.end(), [&](const IntType &key) {
			auto it = srcMap.find(key);
			if (it != srcMap.end()) {
				DBStatVariable v = (*it).second;
				ovars[key] = v;
			}
		});
		return (true);
	}
	///////////////////////////////////////////////
}// namespace info

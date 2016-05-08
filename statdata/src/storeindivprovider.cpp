#include "../include/storeindivprovider.h"
/////////////////////////////////
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
/////////////////////////////////
namespace info {
////////////////////////////////////////
SerialStoreIndivProvider::SerialStoreIndivProvider(IStoreHelper *pHelper,
		const DBStatDataset &oSet) :
		StoreIndivProvider(pHelper, oSet), m_current(0) {

}
SerialStoreIndivProvider::SerialStoreIndivProvider(IStoreHelper *pHelper,
		const std::string &setName) :
		StoreIndivProvider(pHelper, setName), m_current(0) {

}
SerialStoreIndivProvider::SerialStoreIndivProvider(IStoreHelper *pHelper,
		const std::wstring &setName) :
		StoreIndivProvider(pHelper, setName), m_current(0) {

}
bool SerialStoreIndivProvider::reset(void) {
#if defined(__CYGWIN__)
	std::lock_guard < std::mutex > oLock(this->_mutex);
#else
	boost::mutex::scoped_lock oLock(this->_mutex);
#endif // __CYGWIN__
	this->m_current = 0;
	return (true);
}
bool SerialStoreIndivProvider::next(Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeAll*/) {
#if defined(__CYGWIN__)
	std::lock_guard < std::mutex > oLock(this->_mutex);
#else
	boost::mutex::scoped_lock oLock(this->_mutex);
#endif // __CYGWIN__
	size_t nCur = this->m_current;
	ints_vector &v = this->m_ids;
	if (nCur >= v.size()) {
		oInd.clear();
		return (false);
	}
	IntType aIndex = v[nCur];
	bool bRet = this->find_indiv(aIndex, oInd, mode);
	this->m_current = nCur + 1;
	return (bRet);
}
//////////////////////////////
StoreIndivProvider::StoreIndivProvider(IStoreHelper *pHelper,
		const DBStatDataset &oSet) :
		m_phelper(pHelper), m_oset(oSet) {
	BOOST_ASSERT(this->m_phelper != nullptr);
	this->init();
}
StoreIndivProvider::StoreIndivProvider(IStoreHelper *pHelper,
		const std::string &setName) :
		m_phelper(pHelper), m_oset(setName) {
	BOOST_ASSERT(this->m_phelper != nullptr);
	this->init();
}
StoreIndivProvider::StoreIndivProvider(IStoreHelper *pHelper,
		const std::wstring &setName) :
		m_phelper(pHelper), m_oset(setName) {
	BOOST_ASSERT(this->m_phelper != nullptr);
	this->init();
}
StoreIndivProvider::StoreIndivProvider(const StoreIndivProvider &other) :
		m_phelper(other.m_phelper), m_oset(other.m_oset), m_ids(other.m_ids), m_vars(
				other.m_vars) {
}
StoreIndivProvider & StoreIndivProvider::operator=(
		const StoreIndivProvider &other) {
	if (this != &other) {
		this->m_phelper = other.m_phelper;
		this->m_oset = other.m_oset;
		this->m_ids = other.m_ids;
		this->m_vars = other.m_vars;
	}
	return (*this);
}
StoreIndivProvider::~StoreIndivProvider() {
}
void StoreIndivProvider::init(void) {
	IStoreHelper *pHelper = this->m_phelper;
	BOOST_ASSERT(pHelper != nullptr);
	DBStatDataset &oSet = this->m_oset;
	pHelper->find_dataset(oSet);
	BOOST_ASSERT(oSet.id() != 0);
	int nc = 0;
	ints_vector &oRes = this->m_ids;
	oRes.clear();
	pHelper->find_dataset_indivs_count(oSet, nc);
	if (nc > 0) {
		int offset = 0;
		while (offset < nc) {
			ints_vector vv;
			pHelper->find_dataset_indivs_ids(oSet, vv, offset);
			if (vv.empty()) {
				break;
			}
			BOOST_FOREACH(IntType aId, vv) {
				oRes.push_back(aId);
			}
			offset += vv.size();
		} // while
	} // nc
	variables_map &vv = this->m_vars;
	vv.clear();
	nc = 0;
	pHelper->find_dataset_variables_count(oSet, nc);
	if (nc > 0) {
		int offset = 0;
		while (offset < nc) {
			variables_vector v;
			pHelper->find_dataset_variables(oSet, v, offset);
			if (v.empty()) {
				break;
			}
			BOOST_FOREACH(DBStatVariable aId, v) {
				vv[aId.id()] = aId;
			}
			offset += v.size();
		} // while
	} // nc
} // init
bool StoreIndivProvider::get_variables_map(variables_map &oMap) {
	BOOST_ASSERT(this->is_valid());
	oMap = this->m_vars;
	return (true);
} //get_variables_map
bool StoreIndivProvider::is_valid(void) {
	return ((this->m_phelper != nullptr) && (this->m_phelper->is_valid())
			&& (this->m_oset.id() != 0));
}
bool StoreIndivProvider::indivs_count(size_t &nCount) {
	BOOST_ASSERT(this->is_valid());
	nCount = this->m_ids.size();
	return (true);
}
bool StoreIndivProvider::find_indiv(const IntType aIndex, Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeAll ,*/) {
	BOOST_ASSERT(this->is_valid());
	IStoreHelper *pMan = this->m_phelper;
	oInd.clear();
	oInd.id(aIndex);
	bool bRet = pMan->find_indiv(oInd);
	if (!bRet) {
		return (false);
	}
	values_vector vv;
	if ((bRet = pMan->find_indiv_values(oInd, vv)) != true) {
		return (false);
	}
	DbValueMap oMap;
	variables_map &vars = this->m_vars;
	BOOST_FOREACH(const DBStatValue &v, vv) {
		DbValue vx = v.get_value();
		if (!vx.empty()) {
			IntType key = v.get_variable_id();
			auto jt = vars.find(key);
			if (jt != vars.end()) {
				const DBStatVariable &vz = (*jt).second;
				switch (mode) {
				case VariableMode::modeNumeric:
					if (vz.is_numeric()) {
						oMap[key] = vx;
					}
					break;
				case VariableMode::modeNominal:
					if (vz.is_categ()) {
						oMap[key] = vx;
					}
					break;
				case VariableMode::modeAll:
					oMap[key] = vx;
					break;
				default:
					break;
				} // mode
			} // found
		} // not empty
	} // v
	oInd.set_data(oMap);
	return (true);
} // find_indiv
bool StoreIndivProvider::find_indiv_at(const size_t pos, Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeAll*/) {
	BOOST_ASSERT(this->is_valid());
	//bool bRet = false;
	ints_vector &v = this->m_ids;
	if (pos >= v.size()) {
		return (false);
	}
	IntType aIndex = v[pos];
	return this->find_indiv(aIndex, oInd, mode);
} //find_indiv_at
////////////////////////////////
}	// namespace info
///////////////////////////////////

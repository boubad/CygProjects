#pragma once
#ifndef __STOREINDIVPROVIDER_H__
#define __STOREINDIVPROVIDER_H__
/////////////////////////////////////
#include "istorehelper.h"
#include "indiv.h"
///////////////////////////////////////////
namespace info {
////////////////////////////////
class StoreIndivProvider: public IIndivProvider {
private:
	IStoreHelper *m_phelper;
	DBStatDataset m_oset;
	void init(void);
protected:
	ints_vector m_ids;
	variables_map m_vars;
public:
	StoreIndivProvider(IStoreHelper *pHelper, const DBStatDataset &oSet);
	StoreIndivProvider(IStoreHelper *pHelper, const std::string &setName);
	StoreIndivProvider(IStoreHelper *pHelper, const std::wstring &setName);
	StoreIndivProvider(const StoreIndivProvider &other);
	StoreIndivProvider & operator=(const StoreIndivProvider &other);
	virtual ~StoreIndivProvider();
public:
	virtual bool is_valid(void);
	virtual bool indivs_count(size_t &nCount);
	virtual bool get_variables_map(variables_map &oMap);
	virtual bool find_indiv(const IntType aIndex, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll);
	virtual bool find_indiv_at(const size_t pos, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll);
};
// class StoreIndivProvider
/////////////////////////////////////////
class SerialStoreIndivProvider: public StoreIndivProvider,
		public ISerialIndivProvider,
		private boost::noncopyable {
private:
	size_t m_current;
public:
	SerialStoreIndivProvider(IStoreHelper *pHelper, const DBStatDataset &oSet);
	SerialStoreIndivProvider(IStoreHelper *pHelper, const std::string &setName);
	SerialStoreIndivProvider(IStoreHelper *pHelper,
			const std::wstring &setName);
public:
	virtual bool reset(void);
	virtual bool next(Indiv &oInd, const VariableMode mode =
			VariableMode::modeAll);
};
// class
////////////////////////////////////
}// namespace info
////////////////////////////////////////
#endif // !__STOREINDIVPROVIDER_H__

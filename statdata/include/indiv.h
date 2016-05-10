#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
///////////////////////////////////////
#include "dbstatitems.h"
//////////////////////////////////
namespace info {
////////////////////////////////////////////////
class Indiv: public DBStatIndiv {
private:
	DbValueMap m_map;
public:
	Indiv();
	Indiv(const IntType aIndex);
	Indiv(const IntType aIndex, const DbValueMap &oMap);
	Indiv(const DBStatIndiv &oBaseInd);
	Indiv(const Indiv &other);
	Indiv & operator=(const Indiv &other);
	virtual ~Indiv();
public:
	bool empty(void) const;
	bool has_variable(const IntType key) const;
	void set_variable(const IntType key, const DbValue &v);
	const DbValueMap &data(void) const;
	void swap(Indiv &other);
	double distance(const Indiv &other) const;
public:
	virtual void get_data(DbValueMap &oMap) const;
	virtual void set_data(const DbValueMap &oMap);
};
// class Indiv
////////////////////////////////////////////////
class IIndivProvider {
public:
	virtual bool is_valid(void) = 0;
	virtual bool indivs_count(size_t &nCount) = 0;
	virtual bool get_variables_map(variables_map &oMap) = 0;
	virtual bool find_indiv(const IntType aIndex, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll) = 0;
	virtual bool find_indiv_at(const size_t pos, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll) = 0;
public:
	virtual ~IIndivProvider() {
	}
};
// class IIndivProvider
/////////////////////////////////////////////////
class StatInfo;
class INumIndivProvider : public IIndivProvider {
public:
	virtual bool  get_statinfo(const IntType key, StatInfo &oInfo) = 0;
	virtual bool get_variables_ids(ints_vector &oVec) = 0;
	virtual bool find_indiv_at(const size_t pos, IntType &aIndex,
			doubles_vector &data) = 0;
	virtual bool find_indiv(const IntType aIndex,doubles_vector &data) = 0;
	//
	virtual ~INumIndivProvider(){}
};
////////////////////////////////////////////////
class ISerialIndivProvider {
public:
	virtual bool reset(void) = 0;
	virtual bool next(Indiv &oInd, const VariableMode mode =
			VariableMode::modeAll) = 0;
public:
	virtual ~ISerialIndivProvider() {
	}
};
/////////////////////////////////////////////
class IFilteredIndivProvider : public IIndivProvider, public ISerialIndivProvider {
public:
	virtual void get_filter(variables_vector &oVars) const = 0;
	virtual void set_filter(const variables_vector &oVars) = 0;
	~IFilteredIndivProvider(){}
};
////////////////////////////////////////////////
}// namespace info
///////////////////////////////////////////////
inline void swap(info::Indiv &v1, info::Indiv &v2) {
	v1.swap(v2);
}
/////////////////////////////////////////
#endif // !__INDIV_H__

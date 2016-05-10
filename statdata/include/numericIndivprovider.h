/*
 * numericIndivprovider.h
 *
 *  Created on: 8 mai 2016
 *      Author: boubad
 */

#ifndef NUMERICINDIVPROVIDER_H_
#define NUMERICINDIVPROVIDER_H_

#include "indiv.h"
#include "statinfo.h"
////////////////////////////
namespace info {
///////////////////////////
class NumericIndivProvider: public INumIndivProvider, private boost::noncopyable {
private:
	IIndivProvider *m_provider;
	statinfos_map m_stats;
	ints_vector m_ids;
public:
	NumericIndivProvider(IIndivProvider *pProvider);
	virtual ~NumericIndivProvider();
public:
	virtual bool is_valid(void);
	virtual bool indivs_count(size_t &nCount);
	virtual bool get_variables_map(variables_map &oMap);
	virtual bool find_indiv(const IntType aIndex, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll);
	virtual bool find_indiv_at(const size_t pos, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll);
	virtual bool get_statinfo(const IntType key, StatInfo &oInfo);
	virtual bool get_variables_ids(ints_vector &oVec);
	virtual bool find_indiv_at(const size_t pos, IntType &aIndex,
			doubles_vector &data);
	virtual bool find_indiv(const IntType aIndex, doubles_vector &data);
};
/////////////////////////////////
} /* namespace info */

#endif /* NUMERICINDIVPROVIDER_H_ */

#pragma once
#ifndef __WORKERINDIVPROVIDER_H__
#define __WORKERINDIVPROVIDER_H__
//////////////////////////////////////////////
#include "indiv.h"
/////////////////////////////////////////////////
namespace info {
	///////////////////////////////////////////////
	class WorkerIndivProvider : public IFilteredIndivProvider, private boost::noncopyable {
	public:
		IIndivProvider *m_provider;
		size_t m_current;
		variables_map	m_vars;
		ints_set	m_filter;
	protected:
		bool find_indiv_filtered(const IntType aIndex, Indiv &oInd, const VariableMode mode = VariableMode::modeAll);
		bool find_indiv_at_filtered(const size_t pos, Indiv &oInd, const VariableMode mode = VariableMode::modeAll);
		void filter_indiv(Indiv &oInd) const;
	public:
		WorkerIndivProvider(IIndivProvider *pProvider);
		~WorkerIndivProvider();
	public:
		virtual bool is_valid(void);
		virtual bool indivs_count(size_t &nCount);
		virtual bool find_indiv(const IntType aIndex, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll);
		virtual bool find_indiv_at(const size_t pos, Indiv &oInd,
			const VariableMode mode = VariableMode::modeAll);
		virtual bool get_variables_map(variables_map &ovars);
		//
		virtual bool reset(void);
		virtual bool next(Indiv &oInd, const VariableMode mode = VariableMode::modeAll);
		//
		virtual void get_filter(variables_vector &oVars) const;
		virtual void set_filter(const variables_vector &oVars);
	};// class WorkerIndivProvider
	  /////////////////////////////////////////////////
}// namespace info
 ////////////////////////////////////////////////
#endif // !__WORKERINDIVPROVIDER_H__


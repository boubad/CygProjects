#pragma once
#ifndef MATRESULT_H_
#define MATRESULT_H_
/////////////////////////////
#include "info_includes.h"
//////////////////////////////////
namespace info {
	///////////////////////////////////////////
	enum class DispositionType {invalid,indiv,variable};
	///////////////////////////////////////////
	template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long>
	class IntraMatElemResult {
	public:
		using ints_vector = std::vector<IDTYPE>;
		using sizets_vector = std::vector<size_t>;
		using IntraMatElemResultType = IntraMatElemResult<IDTYPE,DISTANCETYPE>;
		using IntraMatElemResultPtr = std::shared_ptr<IntraMatElemResultType>;
		//
		DispositionType disposition;
		DISTANCETYPE first;
		sizets_vector second;
		ints_vector third;
	public:
		IntraMatElemResult() : disposition(DispositionType::invalid),first(0) {
		}
		IntraMatElemResult(const DISTANCETYPE c, const sizets_vector &v,DispositionType disp = DispositionType::invalid) :
			disposition(disp),first(c), second(v) {
		}
		IntraMatElemResult(const IntraMatElemResultType &other) :
			disposition(other.disposition),first(other.first), second(other.second), third(other.third) {
		}
		IntraMatElemResultType & operator=(const IntraMatElemResultType &other) {
			if (this != &other) {
				this->disposition = other.disposition;
				this->first = other.first;
				this->second = other.second;
				this->third = other.third;
			}
			return (*this);
		}
		virtual ~IntraMatElemResult() {
		}
		void clear(void) {
			this->disposition = DispositionType::invalid;
			this->first = 0;
			this->second.clear();
			this->third.clear();
		}// clear
	};
	// class IntraMatElemResult
	////////////////////////////////////////
	template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long>
	class IntraMatOrdResult {
	public:
		using IntraMatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE>;
		using IntraMatElemResultPtr = std::shared_ptr<IntraMatElemResultType>;
		using IntraMatOrdResultType = IntraMatOrdResult<IDTYPE, DISTANCETYPE>;
		using IntraMatOrdResultTypePtr = std::shared_ptr<IntraMatOrdResultType>;
	private:
		IntraMatElemResultPtr m_indsResults;
		IntraMatElemResultPtr m_varsResults;
	public:
		IntraMatOrdResult(){}
		IntraMatOrdResult(IntraMatElemResultPtr rInds, IntraMatElemResultPtr rVars) :
			m_indsResults(rInds), m_varsResults(rVars) {}
		IntraMatOrdResult(const IntraMatOrdResultType &other):m_indsResults(other.m_indsResults),m_varsResults(other.m_varsResults){}
		IntraMatOrdResultType & operator=(const IntraMatOrdResultType &other) {
			if (this != &other) {
				this->m_indsResults = other.m_indsResults;
				this->m_varsResults = other.m_varsResults;
			}
			return (*this);
		}
		virtual ~IntraMatOrdResult(){}
	public:
		IntraMatElemResultPtr indivs_results(void) const {
			return (this->m_indsResults);
		}
		void indivs_result(IntraMatElemResultPtr o) {
			this->m_indivsResults = o;
		}
		IntraMatElemResultPtr variables_results(void) const {
			return (this->m_varsResults);
		}
		void variables_result(IntraMatElemResultPtr o) {
			this->m_varsResults = o;
		}
		void clear(void) {
			this->m_indsResults.reset();
			this->m_varsResults.reset();
		}// clear
	};
	/////////////////////////////////////
}// namespace infi
 ///////////////////////////////////
#endif /* INTRAMAT_H_ */

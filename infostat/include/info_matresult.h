/*
 * info_matresult.h
 *
 *  Created on: 14 juin 2016
 *      Author: boubad
 */

#ifndef INFO_MATRESULT_H_
#define INFO_MATRESULT_H_
///////////////////////////////
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
////////////////////////////////
#include "stringconvert.h"
/////////////////////////////////
namespace info {
////////////////////////////////
enum class DispositionType {
	invalid, indiv, variable, unknown
};
enum class StageType {
	started, finished, aborted, current, unkown
};
////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
class MatResult {
public:
	using sizets_vector = std::vector<size_t>;
	using ints_vector = std::vector<IDTYPE>;
	using MatResultType = MatResult<IDTYPE,DISTANCETYPE,STRINGTYPE>;
	using MatResultPtr = std::shared_ptr<MatResultType>;
private:
	StageType m_stage;
	DispositionType m_disp;
	DISTANCETYPE m_first;
	sizets_vector m_second;
	ints_vector m_third;
	STRINGTYPE m_sigle;
private:
	MatResult(StageType st = StageType::unkown, DispositionType disp =
			DispositionType::unknown, DISTANCETYPE crit = 0,
			const sizets_vector &inds = sizets_vector(),
			const ints_vector &ids = ints_vector(), const STRINGTYPE &sSigle =
					STRINGTYPE()) :
			m_stage(st), m_disp(disp), m_first(crit) {
		const size_t n = inds.size();
		this->m_second.resize(n);
		this->m_third.resize(n);
		sizets_vector &s1 = this->m_second;
		for (size_t i = 0; i < n; ++i) {
			s1[i] = inds[i];
		}
		ints_vector &s2 = this->m_third;
		size_t nx = ids.size();
		if (nx >= n) {
			for (size_t i = 0; i < n; ++i) {
				s2[i] = ids[i];
			}
		}
		this->m_sigle = STRINGTYPE(sSigle.vegin(), sSigle.end());
	}
public:
	virtual ~MatResult() {
	}
public:
	StageType stage(void) const {
		return (this->m_stage);
	}
	DispositionType disposition(void) const {
		return (this->m_disp);
	}
	DISTANCETYPE criteria(void) const {
		return (this->m_first);
	}
	const sizets_vector & indexes(void) const {
		return (this->m_second);
	}
	const ints_vector & ids(void) const {
		return (this->m_third);
	}
	const STRINGTYPE & sigle(void) const {
		return (this->m_sigle);
	}
	void to_string(std::string &ss) const {
		std::stringstream os;
		std::string s = info_2s(this->m_sigle);
		if (s.empty()) {
			os << s << " ";
		}
		if (this->m_stage == StageType::started) {
			os << "STARTED... ";
		} else if (this->m_stage == StageType::finished) {
			os << "FINISHED!!! ";
		}
		if (this->m_disp == DispositionType::indiv) {
			os << "INDIVS ";
		} else if (this->m_disp == DispositionType::variable) {
			os << "VARS ";
		}
		os << this->m_first;
		ss = os.str();
	} // to_string
	void to_string(std::wstring &ss) const {
		std::wstringstream os;
		std::wstring s = info_2ws(this->m_sigle);
		if (s.empty()) {
			os << s << L" ";
		}
		if (this->m_stage == StageType::started) {
			os << L"STARTED... ";
		} else if (this->m_stage == StageType::finished) {
			os << L"FINISHED!!! ";
		}
		if (this->m_disp == DispositionType::indiv) {
			os << L"INDIVS ";
		} else if (this->m_disp == DispositionType::variable) {
			os << L"VARS ";
		}
		os << this->m_first;
		ss = os.str();
	} // to_string
public:
	MatResultPtr create(StageType st, DispositionType disp, DISTANCETYPE crit,
			const sizets_vector &inds, const ints_vector &ids,
			const STRINGTYPE &sSigle) {
		MatResultPtr oRes = std::make_shared<MatResultType>(st, disp, crit,
				inds, ids, sSigle);
		assert(oRes.get() != nullptr);
		return (oRes);
	} // create
};
///////////////////////////////////
}// namespace info
//////////////////////////////////
#endif /* INFO_MATRESULT_H_ */

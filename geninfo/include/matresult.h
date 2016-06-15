/*
 * matresult.h
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */

#ifndef MATRESULT_H_
#define MATRESULT_H_
//////////////////////
#include "info_global.h"
////////////////////////////
#include "inforunner.h"
/////////////////////////////
namespace info {
///////////////////////////////////////////
enum class DispositionType {
	invalid, indiv, variable, undefined
};
enum class StageType {
	started, finished, aborted, current
};
///////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
class MatElemResult {
public:
	using sizets_vector = std::vector<size_t>;
	using MatElemResultType = MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
private:
	StageType m_stage;
	DispositionType m_disp;
	STRINGTYPE m_sigle;
	DISTANCETYPE m_first;
	sizets_vector m_second;
public:
	MatElemResult() :
			m_stage(StageType::current), m_disp(DispositionType::invalid), m_first(
					0) {
	}
	virtual ~MatElemResult() {
	}
public:
	StageType stage(void) const {
		return (this->m_stage);
	}
	void stage(StageType s) {
		this->m_stage = s;
	}
	DispositionType disposition(void) const {
		return (this->m_disp);
	}
	void disposition(DispositionType d) {
		this->m_disp = d;
	}
	STRINGTYPE sigle(void) const {
		return (this->m_sigle);
	}
	void sigle(const STRINGTYPE &s) {
		this->m_sigle = STRINGTYPE(s.begin(), s.end());
	}
	DISTANCETYPE criteria(void) const {
		return (this->m_first);
	}
	void criteria(DISTANCETYPE c) {
		this->m_first = c;
	}
	sizets_vector indexes(void) const {
		return (this->m_second);
	}
	void indexes(const sizets_vector &src) {
		const size_t n = src.size();
		sizets_vector &dest = this->m_second;
		dest.resize(n);
		for (size_t i = 0; i < n; ++i) {
			dest[i] = src[i];
		}
	}
public:
	void to_string(std::string &ss, bool bIndexes = true) {
		std::stringstream os;
		std::string sx = info_2s(this->m_sigle);
		if (!sx.empty()) {
			os << sx << " ";
		}
		if (this->m_stage == StageType::started) {
			os << "STARTED... ";
		} else if (this->m_stage == StageType::finished) {
			os << "FINISHED!! ";
		} else if (this->m_stage == StageType::aborted) {
			os << "ABORTED. ";
		}
		if (this->m_disp == DispositionType::indiv) {
			os << "INDIVS ";
		} else if (this->m_disp == DispositionType::variable) {
			os << "VARS ";
		}
		os << this->m_first;
		if (bIndexes) {
			std::string sk;
			info_write_vector(this->m_second, sk);
			os << " " << sk;
		}
		ss = os.str();
	} // to_string
	void to_string(std::wstring &ss, bool bIndexes = true) {
		std::wstringstream os;
		std::wstring sx = info_w2s(this->m_sigle);
		if (!sx.empty()) {
			os << sx << L" ";
		}
		if (this->m_stage == StageType::started) {
			os << L"STARTED... ";
		} else if (this->m_stage == StageType::finished) {
			os << L"FINISHED!! ";
		} else if (this->m_stage == StageType::aborted) {
			os << L"ABORTED. ";
		}
		if (this->m_disp == DispositionType::indiv) {
			os << L"INDIVS ";
		} else if (this->m_disp == DispositionType::variable) {
			os << L"VARS ";
		}
		os << this->m_first;
		if (bIndexes) {
			std::wstring sk;
			info_write_vector(this->m_second, sk);
			os << " " << sk;
		}
		ss = os.str();
	} // to_string
};
// class IntraMatElemResult
////////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
class MatElemObject: public CancellableObject<
		std::shared_ptr<MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE> > > {
public:
	using cancelflag = std::atomic<bool>;
	using pcancelflag = cancelflag *;
	using PBackgrounder = Backgrounder *;
	using MatElemResultType = MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
	using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
	using BaseType = CancellableObject<std::shared_ptr<MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE> > >;
public:
	MatElemObject(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			MatElemFunctionType f = [](MatElemResultPtr o) {}) :
			BaseType(pFlag, pq, f) {
	}
	virtual ~MatElemObject() {
	}
};
////////////////////////////////////////////
}// namespace info
///////////////////////////////////

#endif /* MATRESULT_H_ */

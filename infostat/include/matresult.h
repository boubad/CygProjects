#pragma once
#ifndef MATRESULT_H_
#define MATRESULT_H_
////////////////////////////////
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>
////////////////////////////
#include "stringconvert.h"
#include "sharedqueue.h"
#include "inforunner.h"
/////////////////////////////
namespace info {
	///////////////////////////////////////////
	enum class DispositionType { invalid, indiv, variable, undefined };
	enum class StageType { started, finished, aborted, current };
	///////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
	class IntraMatElemResult {
	public:
		using sizets_vector = std::vector<size_t>;
		using IntraMatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using IntraMatElemResultPtr = std::shared_ptr<IntraMatElemResultType>;
		//
		StageType stage;
		DispositionType disposition;
		DISTANCETYPE first;
		sizets_vector second;
		STRINGTYPE sigle;
	public:
		IntraMatElemResult() : stage(StageType::current), disposition(DispositionType::invalid), first(0) {
		}
		IntraMatElemResult(const DISTANCETYPE c, const sizets_vector &v,
			DispositionType disp = DispositionType::invalid,
			StageType st = StageType::current, const STRINGTYPE sSigle = STRINGTYPE()) :
			stage(st), disposition(disp), first(c), second(v), sigle(sSigle) {
		}
		IntraMatElemResult(const IntraMatElemResultType &other) :
			stage(other.stage), disposition(other.disposition), first(other.first), second(other.second), sigle(other.sigle) {
		}
		IntraMatElemResultType & operator=(const IntraMatElemResultType &other) {
			if (this != &other) {
				this->stage = other.stage;
				this->disposition = other.disposition;
				this->first = other.first;
				this->second = other.second;
				this->sigle = other.sigle;
			}
			return (*this);
		}
		virtual ~IntraMatElemResult() {
		}
		void clear(void) {
			this->stage = StageType::current;
			this->disposition = DispositionType::invalid;
			this->first = 0;
			this->second.clear();
			this->sigle.clear();
		}// clear
		void to_string(std::string &ss) {
			std::stringstream os;
			if (this->stage == StageType::started) {
				os << "STARTED... ";
			}
			else if (this->stage == StageType::finished) {
				os << "FINISHED!! ";
			}
			else if (this->stage == StageType::aborted) {
				os << "ABORTED. ";
			}
			os << info_2s(this->sigle) << " ";
			if (this->disposition == DispositionType::indiv) {
				os << "INDIVS ";
			}
			else if (this->disposition == DispositionType::variable) {
				os << "VARS ";
			}
			os << this->first;
			ss = os.str();
		}// to_string
		void to_string(std::wstring &ss) {
			std::wstringstream os;
			if (this->stage = StageType::started) {
				os << L"STARTED... ";
			}
			else if (this->stage == StageType::finished) {
				os << L"FINISHED!! ";
			}
			else if (this->stage == StageType::aborted) {
				os << L"ABORTED. ";
			}
			os << info_2ws(this->sigle) << L" ";
			if (this->disposition == DispositionType::indiv) {
				os << L"INDIVS ";
			}
			else if (this->disposition == DispositionType::variable) {
				os << L"VARS ";
			}
			os << this->first;
			ss = os.str();
		}// to_string
	};
	// class IntraMatElemResult
	////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
	class MatElemResultClient {
	public:
		using MatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
		using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
		using MatElemResultClientType = MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	private:
		MatElemFunctionType m_f;
	public:
		MatElemResultClient() :m_f([](MatElemResultPtr o) {}) {
		}
		MatElemResultClient(MatElemFunctionType f) :m_f(f) {
		}
		virtual ~MatElemResultClient() {
		}
		void set_function(MatElemFunctionType f) {
			this->m_f = f;
		}
		MatElemFunctionType get_function(void) {
			return (this->m_f);
		}
		void operator()(MatElemResultPtr oRes) {
			this->process_result(oRes);
		}
	protected:
		virtual void process_result(MatElemResultPtr oRes) {
			MatElemFunctionType &f = this->m_f;
			f(oRes);
		}//process_result
	public:
		virtual void put(MatElemResultPtr oRes) {
			this->process_result(oRes);
		}// put
	}; // class IntraMatElemBackgrounder<IDTYPE,DISTANCETYPE,STRINGTYPE>
	////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
	class MatElemObject : public CancellableObject, public MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE> {
	public:
		using cancelflag = std::atomic<bool>;
		using pcancelflag = cancelflag *;
		using PBackgrounder = Backgrounder *;
		using MatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
		using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
		using BaseType = MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	public:
		MatElemObject(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			MatElemFunctionType f = [](MatElemResultPtr o) {}) :CancellableObject(pFlag, pq), BaseType(f) {
		}
		virtual ~MatElemObject() {
		}
	public:
		virtual void put(MatElemResultPtr oRes) {
			if (this->is_cancelled()) {
				return;
			}
			if (this->get_backgrounder() != nullptr) {
				this->send_result([this, oRes]() {
					this->process_result(oRes);
				});
			}
			else {
				this->process_result(oRes);
			}
		}// put
	}; // class MatElemObject<IDTYPE,DISTANCETYPE,STRINGTYPE>
	///////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
	class MatElemResultBackgounder : public MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE> {
	public:
		using MatElemResultType = IntraMatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
		using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
		using BaseType = MatElemResultClient<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatElemBackgrounder = MatElemResultBackgounder<IDTYPE, DISTANCETYPE, STRINGTYPE >;
	private:
		std::atomic<bool> done;
		SharedQueue<MatElemResultPtr> dispatchQueue;
		std::unique_ptr<std::thread> runnable;
		//
		void init(void) {
			this->runnable.reset(new std::thread([this]() {
				while (!this->done.load()) {
					MatElemResultPtr o = this->dispatchQueue.take();
					if (o.get() == nullptr) {
						this->done.store(true);
						break;
					}
					this->process_result(o);
				} // while
			}));
		}// init
	public:
		MatElemResultBackgounder() : done(false) {
			this->init();
		}
		MatElemResultBackgounder(MatElemFunctionType f) : BaseType(f), done(false) {
			this->init();
		}
		virtual ~MatElemResultBackgounder() {
			MatElemResultPtr o;
			this->done.store(true);
			this->dispatchQueue.put(o);
			this->runnable->join();
		}
	public:
		virtual void put(MatElemResultPtr oRes) {
			this->dispatchQueue.put(oRes);
		}// put
	}; // class IntraMatElemBackgrounder<IDTYPE,DISTANCETYPE,STRINGTYPE>
	////////////////////////////////////////////
}// namespace info
 ///////////////////////////////////
#endif /* INTRAMAT_H_ */

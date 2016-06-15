#pragma once
#ifndef __INFORUNNER_H__
#define __INFORUNNER_H__
////////////////////////////////
#include "activeobject.h"
///////////////////////////////////
namespace info {
////////////////////////////////////////
template<typename T>
class CancellableObject {
public:
	using cancelflag = std::atomic<bool>;
	using pcancelflag = cancelflag *;
	using PBackgrounder = Backgrounder *;
	using Operation_result = std::function<void()>;
	using function_type = std::function<void (T)>;
private:
	pcancelflag m_pcancel;
	PBackgrounder m_pq;
	function_type m_f;
public:
	CancellableObject(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			function_type f = [](T arg) {}) :
			m_pcancel(pFlag), m_pq(pq), m_f(f) {
	}
	CancellableObject(const CancellableObject &other) :
			m_pcancel(other.m_pcancel), m_pq(other.m_pq), m_f(other.m_f) {
	}
	CancellableObject & operator=(const CancellableObject &other) {
		if (this != &other) {
			this->m_pcancel = other.m_pcancel;
			this->m_pq = other.m_pq;
			this->m_f = other.m_f;
		}
		return (*this);
	}
	virtual ~CancellableObject() {
	}
	pcancelflag get_cancelflag(void) {
		return (this->m_pcancel);
	}
	void set_cancelflag(pcancelflag f) {
		this->m_pcancel = f;
	}
	PBackgrounder get_backgrounder(void) {
		return (this->m_pq);
	}
	void set_backgrounder(PBackgrounder b) {
		this->m_pq = b;
	}
	void set_function(function_type f) {
		this->m_f = f;
	}
	function_type get_function(void) {
		return (this->m_f);
	}
	void send(T arg) {
		PBackgrounder pq = this->m_pq;
		if (pq != nullptr) {
			pq->send([this,arg]() {
				(this->m_f)(arg);
			});
		} else {
			(this->m_f)(arg);
		}
	}
	bool is_cancelled(void) {
		if (this->m_pcancel == nullptr){
			return (false);
		}
		bool bRet = this->m_pcancel->load();
		return (bRet);
	} // is_cancelled
};
// class CancellableObject
///////////////////////////////////////
class InfoRunner {
public:
	using cancelflag = std::atomic<bool>;
	using pcancelflag = cancelflag *;
	using PBackgrounder = Backgrounder *;
	using Operation = std::function<void(pcancelflag, PBackgrounder)>;
private:
	std::atomic<bool> m_cancel;
	std::unique_ptr<Backgrounder> _active;
public:
	InfoRunner() :m_cancel(false),_active(new Backgrounder()) {
	}
	virtual ~InfoRunner() {
	}
	PBackgrounder get_backgrounder(void) {
		return (this->_active.get());
	}
	pcancelflag get_cancelflag(void) {
		return (&(this->m_cancel));
	}
	void reset_cancel(void) {
		this->m_cancel.store(false);
	}
	void cancel(void) {
		this->m_cancel.store(true);
	}
};
// class InfoRunner
////////////////////////////////////
}// namespace info
///////////////////////////////////
#endif // !__INFORUNNER_H__


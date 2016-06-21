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
	bool m_bnotify;
private:
	CancellableObject(const CancellableObject<T> &) = delete;
	CancellableObject<T> & operator=(const CancellableObject<T> &) = delete;
public:
	CancellableObject(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			function_type f = [](T arg) {}, bool bNotify = true) :
			m_pcancel(pFlag), m_pq(pq), m_f(f),m_bnotify(bNotify) {
	}
	virtual ~CancellableObject() {
	}
	bool is_notify(void) const {
		return (this->m_bnotify);
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
		if (this->m_bnotify) {
			PBackgrounder pq = this->m_pq;
			if (pq != nullptr) {
				pq->send([this, arg]() {
					try {
						(this->m_f)(arg);
					}
					catch (...) {}
				});
			}
			else {
				try {
					(this->m_f)(arg);
				}
				catch (...) {}
			}
		}
	}// send
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
	using Operation = std::function<void()>;
private:
	std::atomic<bool> m_cancel;
	std::unique_ptr<Backgrounder> _active;
	std::unique_ptr<Backgrounder> _dispatcher;
public:
	InfoRunner() :m_cancel(false),_active(new Backgrounder()),_dispatcher(new Backgrounder()) {
	}
	virtual ~InfoRunner() {
	}
	PBackgrounder get_dispatcher(void) {
		return (this->_dispatcher.get());
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
	void send_dispatch(Operation op) {
		this->_dispatcher->send(op);
	}
	void send_result(Operation op) {
		this->_active->send(op);
	}
};
// class InfoRunner
////////////////////////////////////
}// namespace info
///////////////////////////////////
#endif // !__INFORUNNER_H__


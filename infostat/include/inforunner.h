#pragma once
#ifndef __INFORUNNER_H__
#define __INFORUNNER_H__
////////////////////////////////
#include "activeobject.h"
///////////////////////////////////
namespace info {
	////////////////////////////////////////
	class CancellableObject {
	public:
		using cancelflag = std::atomic<bool>;
		using pcancelflag = cancelflag *;
		using PBackgrounder = Backgrounder *;
		using Operation_result = std::function<void()>;
	private:
		pcancelflag m_pcancel;
		PBackgrounder m_pq;
	public:
		CancellableObject(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr) :
			m_pcancel(pFlag), m_pq(pq) {
		}
		CancellableObject(const CancellableObject &other) :m_pcancel(other.m_pcancel), m_pq(other.m_pq) {

		}
		CancellableObject & operator=(const CancellableObject &other) {
			if (this != &other) {
				this->m_pcancel = other.m_pcancel;
				this->m_pq = other.m_pq;
			}
			return (*this);
		}
		virtual ~CancellableObject() {
		}
	protected:
		pcancelflag get_cancelflag(void) {
			return (this->m_pcancel);
		}
		PBackgrounder get_backgrounder(void) {
			return (this->m_pq);
		}
		void send_result(Operation_result rf) {
			PBackgrounder pq = this->m_pq;
			if (pq != nullptr) {
				pq->send(rf);
			}
		}
		bool is_cancelled(void) {
			return ((this->m_pcancel != nullptr) && this->m_pcancel->load());
		}// is_cancelled
	};// class CancellableObject
	///////////////////////////////////////
	class CancellableActive {
	public:
		using cancelflag = std::atomic<bool>;
		using pcancelflag = cancelflag *;
		using PBackgrounder = Backgrounder *;
		using Operation = std::function<void(pcancelflag, PBackgrounder)>;
		using Operation_result = std::function<void()>;
	private:
		std::atomic<bool> done;
		std::atomic<bool> m_cancel;
		std::unique_ptr<Backgrounder> m_backgrounder;
		DispatchQueue<Operation> dispatchQueue;
		std::thread runnable;
	private:
		CancellableActive() : done(false), m_cancel(false), m_backgrounder(new Backgrounder()) {
		} // Active
		CancellableActive(const CancellableActive&) = delete;
		CancellableActive& operator=(const Active&) = delete;
		void run() {
			pcancelflag pCancel = &(this->m_cancel);
			PBackgrounder pq = this->m_backgrounder.get();
			std::atomic<bool> &oDone = this->done;
			DispatchQueue<Operation> &q = this->dispatchQueue;
			while (!oDone.load()) {
				Operation op = q.take();
				if (!pCancel->load()) {
					op(pCancel, pq);
				}
			} // while
		} // run
	public:
		virtual ~CancellableActive() {
			this->m_cancel.store(true);
			this->done.store(true);
			dispatchQueue.put([this](pcancelflag /*pCancel*/, PBackgrounder /*pq*/) {
				this->done.store(true);
			});
			this->m_backgrounder.reset();
			runnable.join();
		} // run
		pcancelflag get_cancelflag(void) {
			return (&(this->m_cancel));
		}
		PBackgrounder get_backgrounder(void) {
			return (this->m_backgrounder.get());
		}
		bool is_cancelled(void) {
			return (this->m_cancel.load());
		}
		void reset_cancel(void) {
			this->m_cancel.store(false);
		}
		void cancel(void) {
			this->m_cancel.store(true);
		}
		void send(Operation msg_) {
			this->dispatchQueue.put(msg_);
		} // send
		void send_result(Operation_result rf) {
			PBackgrounder pq = this->m_backgrounder.get();
			if (pq != nullptr) {
				pq->send(rf);
			}
		}
		void clear(void) {
			this->dispatchQueue.clear();
		}
		// Factory: safe construction of object before thread start
		static CancellableActive *createActive(void) {
			CancellableActive *p = new CancellableActive();
			assert(p != nullptr);
			p->runnable = std::thread(&CancellableActive::run, p);
			return p;
		}	 // create
	};
	// class CancellableActive
	///////////////////////////////////
	class InfoRunner {
	public:
		using cancelflag = std::atomic<bool>;
		using pcancelflag = cancelflag *;
		using PBackgrounder = Backgrounder *;
		using Operation = std::function<void(pcancelflag, PBackgrounder)>;
	private:
		std::unique_ptr<CancellableActive> _active;
	protected:
		void clear() {
			this->_active->clear();
		}// clear
	public:
		InfoRunner() {
			this->_active.reset(CancellableActive::createActive());
		}
		virtual ~InfoRunner() {
			this->_active->cancel();
			this->_active.reset();
		}
		PBackgrounder get_backgrounder(void) {
			return (this->_active->get_backgrounder());
		}
		pcancelflag get_cancelflag(void) {
			return (this->_active->get_cancelflag());
		}
		void send(Operation op) {
			this->_active->send(op);
		}	 // send
		bool is_cancelled(void) {
			return (this->_active->is_cancelled());
		}
		void reset_cancel(void) {
			this->_active->reset_cancel();
		}
		void cancel(void) {
			this->_active->cancel();
		}
	};// class InfoRunner
	////////////////////////////////////
}// namespace info
///////////////////////////////////
#endif // !__INFORUNNER_H__


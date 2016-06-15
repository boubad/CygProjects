/*
 * info_activeobject.h
 *
 *  Created on: 14 juin 2016
 *      Author: boubad
 */

#ifndef INFO_ACTIVEOBJECT_H_
#define INFO_ACTIVEOBJECT_H_
////////////////////////////
#include "info_sharedqueue.h"
///////////////////////////////
#include <memory>
#include <thread>
#include <functional>
//////////////////////////////
namespace info {
////////////////////////////////////
class InfoActive {
public:
	using Operation = std::function<void()>;
	using queue_type = InfoSharedQueue<Operation>;
private:
	std::atomic<bool> m_done;
	std::unique_ptr<queue_type> m_dispatchQueue;
	std::thread m_runnable;
	//
	InfoActive(const InfoActive &) = delete;
	InfoActive & operator=(const InfoActive &) = delete;
	//
	void run(void) {
		while (!this->m_done.load()) {
			Operation f = this->m_dispatchQueue->take();
			if (!this->m_dispatchQueue->is_aborted()) {
				f();
			} else {
				break;
			}
		} // while
	} // run
public:
	InfoActive(Operation f = []() {}) :
			m_done(false), m_dispatchQueue(new queue_type(f)) {
		this->m_runnable = std::thread([this]() {
			this->run();
		});
	}
	virtual ~InfoActive() {
		this->m_dispatchQueue->abort();
		this->m_dispatchQueue->put([this]() {
			this->m_done.store(true);
		});
		this->m_runnable.join();
	}
	void send(Operation op) {
		this->m_dispatchQueue->put(op);
	}
};
// class InfoActive<FUNC>
////////////////////////////////////
}// namespace info
#endif /* INFO_ACTIVEOBJECT_H_ */

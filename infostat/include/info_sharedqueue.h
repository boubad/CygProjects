/*
 * info_sharedqueue.h
 *
 *  Created on: 14 juin 2016
 *      Author: boubad
 */

#ifndef INFO_SHAREDQUEUE_H_
#define INFO_SHAREDQUEUE_H_
///////////////////////////
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
//////////////////////////////
namespace info {
///////////////////////////////
template<typename T>
class InfoSharedQueue {
public:
	using queue_type = InfoSharedQueue<T>;
private:
	std::atomic<bool> m_abort;
	T m_dummy;
	std::mutex m_qlock;
	std::queue<T> m_ops_queue;
	std::condition_variable m_empty;
	//
	InfoSharedQueue(const queue_type &) = delete;
	queue_type & operator=(const queue_type &) = delete;
public:
	InfoSharedQueue(T dummy) :
			m_abort(false), m_dummy(dummy) {
	}
	virtual ~InfoSharedQueue() {
	}
public:
	void abort(void) {
		this->m_abort.store(true);
	}
	bool is_aborted(void){
		return (this->m_abort.load());
	}
	void put(T op) {
		std::lock_guard<std::mutex> guard(this->m_qlock);
		this->m_ops_queue.push(op);
		this->m_empty.notify_all();
	} // put
	T take() {
		std::unique_lock<std::mutex> lock(this->m_qlock);
		this->m_empty.wait(lock, [this] {return !this->m_ops_queue.empty();});
		if (this->m_abort.load()) {
			while (!this->m_ops_queue.empty()) {
				this->m_ops_queue.pop();
			}
			return (this->m_dummy);
		} else {
			T op = this->m_ops_queue.front();
			this->m_ops_queue.pop();
			return op;
		}
	} // take
};
///////////////////////////////
}// namespace info
#endif /* INFO_SHAREDQUEUE_H_ */

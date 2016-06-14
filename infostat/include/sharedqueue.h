#pragma once
#ifndef __SHAREDQUEUE_H__
#define __SHAREDQUEUE_H__
////////////////////////////////
#include <queue>
#include <mutex>
#include <condition_variable>
/////////////////////////////////////
namespace info {
	///////////////////////////////////
	template <typename T>
	class SharedQueue: boost::noncopyable {
		std::mutex qlock;
		std::queue<T> ops_queue;
		std::condition_variable empty;
	public:
		SharedQueue() {}
		virtual ~SharedQueue() {}
	public:
		size_t size(void) {
			std::lock_guard<std::mutex> guard(qlock);
			return (this->ops_queue.size());
		}
		void clear(void) {
			std::lock_guard<std::mutex> guard(qlock);
			while (!this->ops_queue.empty()) {
				this->ops_queue.pop();
			}
		}// clear
		bool is_empty(void) {
			std::lock_guard<std::mutex> guard(qlock);
			return (this->ops_queue.empty())
		}
		void put(T op) {
			std::lock_guard<std::mutex> guard(qlock);
			ops_queue.push(op);
			empty.notify_all();
		} // put
		T take() {
			std::unique_lock<std::mutex> lock(qlock);
			empty.wait(lock, [&] {return !ops_queue.empty(); });
			T op = ops_queue.front();
			ops_queue.pop();
			return op;
		} // take
		T try_take(bool &bOk) {
			T op;
			bRet = false;
			std::unique_lock<std::mutex> lock(qlock);
			if (!this->ops_queue.empty()) {
				 op = ops_queue.front();
				ops_queue.pop();
				bOk = true;
			}
			return (op);
		}// try_take
	};
	////////////////////////////////////
	
}// namespace info
////////////////////////////////////
#endif // !__SHAREDQUEUE_H__

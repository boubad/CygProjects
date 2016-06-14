#pragma once
#ifndef __SHAREDQUEUE_H__
#define __SHAREDQUEUE_H__
////////////////////////////////
#include "info_includes.h"
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
	};
	////////////////////////////////////
	template <typename T>
	class DispatchQueue {
		std::mutex qlock;
		std::queue<T> ops_queue;
		std::condition_variable empty;
	public:
		DispatchQueue() {}
		virtual ~DispatchQueue() {}
	public:
		void put(T op) {
			std::lock_guard<std::mutex> guard(qlock);
			ops_queue.push(op);
			empty.notify_one();
		} // put
		T take() {
			std::unique_lock<std::mutex> lock(qlock);
			empty.wait(lock, [&] {return !ops_queue.empty(); });
			T op = ops_queue.front();
			ops_queue.pop();
			return op;
		} // take
	};
	////////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // !__SHAREDQUEUE_H__

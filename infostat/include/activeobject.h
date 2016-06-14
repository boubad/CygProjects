/*
 * activeobject.h
 *
 *  Created on: 31 mai 2016
 *      Author: boubad
 */

#ifndef ACTIVEOBJECT_H_
#define ACTIVEOBJECT_H_
//////////////////////////
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <atomic>
/////////////////////////////
namespace info {
////////////////////////
template <typename T>
class DispatchQueue {
	std::mutex qlock;
	std::queue<T> ops_queue;
	std::condition_variable empty;
public:
	DispatchQueue() {}
	virtual ~DispatchQueue() {}
public:
	void clear(void) {
		std::lock_guard<std::mutex> guard(qlock);
		while (!ops_queue.empty()) {
			ops_queue.pop();
		}
	}// clear
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
/////////////////////////
class Active {
public:
	using Operation = std::function<void()>;
private:
	std::atomic<bool> done;
	DispatchQueue<Operation> dispatchQueue;
	std::thread runnable;
private:
	Active() : done(false) {
	} // Active
	Active(const Active&) = delete;
	Active& operator=(const Active&) = delete;
	void run() {
		while (!this->done.load()) {
			(this->dispatchQueue.take())();
		} // while
	} // run
public:
	~Active() {
		// Schedule a No-Op runnable to flush the dispatch queue
		this->done.store(true);
		dispatchQueue.put([this]() {this->done.store(true);});
		runnable.join();
	} // run
	void stop(void) {
		this->done.store(true);
		dispatchQueue.put([this]() {this->done.store(true); });
		runnable.join();
	}
	void send(Operation msg_) {
		this->dispatchQueue.put(msg_);
	} // send
	void clear(void) {
		this->dispatchQueue.clear();
	}
	  // Factory: safe construction of object before thread start
	static std::unique_ptr<Active> createActive(void) {
		std::unique_ptr<Active> aPtr(new Active());
		aPtr->runnable = std::thread(&Active::run, aPtr.get());
		return aPtr;
	}	 // create
};
// class ActiveObject
//////////////////////////////////
class Backgrounder {
public:
	using Operation = std::function<void()>;
private:
	std::unique_ptr<Active> _active;
protected:
	void put(Operation op) {
		this->_active->send(op);
	}	 // put
	void clear() {
		this->_active->clear();
	}// clear
public:
	Backgrounder(){
		this->_active = Active::createActive();
	}
	virtual ~Backgrounder() {
		this->_active.reset();
	}
	void send(Operation op) {
		this->_active->send(op);
	}	 // send
};
// class Backgrounder
//////////////////////////////
}// namespace info
///////////////////////
#endif /* ACTIVEOBJECT_H_ */

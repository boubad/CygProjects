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
	bool is_empty(void) {
		std::lock_guard<std::mutex> guard(qlock);
		return (this->ops_queue.empty());
	}
	void clear(void) {
		std::lock_guard<std::mutex> guard(qlock);
		while (!ops_queue.empty()) {
			ops_queue.pop();
		}
	}// clear
	void put(T op) {
		std::lock_guard<std::mutex> guard(qlock);
		ops_queue.push(op);
		empty.notify_all();
	} // put
	T take() {
		std::unique_lock<std::mutex> lock(qlock);
		empty.wait(lock, [this] {return !this->ops_queue.empty(); });
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
	std::mutex qlock;
	std::queue<Operation> ops_queue;
	std::condition_variable empty;
	std::thread runnable;
private:
	Active() : done(false) {
		this->runnable = std::thread([this]() {
			this->run();
		});
	} // Active
	Active(const Active&) = delete;
	Active& operator=(const Active&) = delete;
	void run() {
		while (!this->done.load()) {
			try {
				std::unique_lock<std::mutex> lock(qlock);
				empty.wait(lock, [this] {return !this->ops_queue.empty(); });
				Operation op = ops_queue.front();
				ops_queue.pop();
				op();
			}catch(...){}
		} // while
	} // run
public:
	~Active() {
		// Schedule a No-Op runnable to flush the dispatch queue
		this->done.store(true);
		this->send([this]() {this->done.store(true);});
		runnable.join();
	} // run
	void send(Operation msg_) {
		std::lock_guard<std::mutex> guard(qlock);
		ops_queue.push(msg_);
		empty.notify_all();
	} // send
	  // Factory: safe construction of object before thread start
	static std::unique_ptr<Active> createActive(void) {
		std::unique_ptr<Active> aPtr(new Active());
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
public:
	Backgrounder():_active(Active::createActive()){
	}
	virtual ~Backgrounder() {
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

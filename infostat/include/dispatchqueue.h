/*
 * dispatchqueue.h
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */

#ifndef DISPATCHQUEUE_H_
#define DISPATCHQUEUE_H_
//////////////////////////
#include "info_includes.h"
/////////////////////////////
namespace info {
////////////////////////////
typedef std::function<void()> Operation;
/////////////////////////
class DispatchQueue {
	std::mutex qlock;
	std::queue<Operation> ops_queue;
	std::condition_variable empty;
public:
	void put(Operation op) {
		std::lock_guard<std::mutex> guard(qlock);
		ops_queue.push(op);
		empty.notify_one();
	}
	Operation take() {
		std::unique_lock<std::mutex> lock(qlock);
		empty.wait(lock, [&] {return !ops_queue.empty();});
		Operation op = ops_queue.front();
		ops_queue.pop();
		return op;
	}
};
//////////////////////////////
class ActiveObject
{
private:
    std::atomic<bool> done;
    std::thread runnable;
    DispatchQueue dispatchQueue;
protected:
    void put(Operation p){
    	dispatchQueue.put(p);
    }
public:
    ActiveObject() : done(false) {
        runnable = std::thread([=]{ run(); });
    }
    virtual ~ActiveObject() {
        // Schedule a No-Op runnable to flush the dispatch queue
        dispatchQueue.put([&]() { done = true; });
        runnable.join();
    }
    void run() {
        while (!done) {
            dispatchQueue.take()();
        }
    }// run
};
/////////////////////////
}// namespace info
/////////////////////////////
#endif /* DISPATCHQUEUE_H_ */

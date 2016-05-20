#pragma once
/*
 * interruptable_object.h
 *
 *  Created on: 20 mai 2016
 *      Author: boubad
 */
#ifndef INTERRUPTABLE_OBJECT_H_
#define INTERRUPTABLE_OBJECT_H_
//////////////////////////////////
#include <atomic>
///////////////////////////
namespace info {
///////////////////////////////
class InterruptObject {
private:
	std::atomic_bool *m_pcancel;
public:
	InterruptObject(std::atomic_bool *pCancel = nullptr) :
			m_pcancel(pCancel) {
	}
	InterruptObject(std::atomic_bool & oCancel) :
				m_pcancel(&oCancel) {
		}
	InterruptObject(const InterruptObject &other) :
			m_pcancel(other.m_pcancel) {

	}
	InterruptObject & operator=(const InterruptObject &other) {
		if (this != &other) {
			this->m_pcancel = other.m_pcancel;
		}
		return (*this);
	}
	virtual ~InterruptObject() {
	}
public:
	void set_cancelleable_flag(std::atomic_bool &b) {
		this->m_pcancel = &b;
	}
	void set_cancelleable_flag(std::atomic_bool *pb) {
		this->m_pcancel = pb;
	}
	std::atomic_bool *get_cancelleable_flag(void) const {
		return (this->m_pcancel);
	}
	bool check_interrupt(void) const {
		InterruptObject *x = const_cast<InterruptObject *>(this);
		std::atomic_bool *p = x->m_pcancel;
		if (p != nullptr) {
			if (p->load()) {
				return (true);
			}
		}
		return (false);
	} //check_interrup
};
// class InterruptObject
//////////////////////////////////
}// namespace info

//////////////////////////////
#endif /* INTERRUPTABLE_OBJECT_H_ */

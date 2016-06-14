#pragma once
/*
 * crititem.h
 *
 *  Created on: 22 mai 2016
 *      Author: boubad
 */

#ifndef CRITITEM_H_
#define CRITITEM_H_
///////////////////////////
#include "info_global.h"
/////////////////////////////
namespace info {
//////////////////////////////////////
enum class ClusterMergeMode {
	invalid, topTop, topBottom, bottomTop, bottomBottom
};
////////////////////////////////////////
template<typename U, typename W>
class CritItem {
public:
	using CritItemType = CritItem<U,W>;
	using crititems_vector = std::vector<CritItemType>;
private:
	U m_first;
	U m_second;
	W m_crit;
	ClusterMergeMode m_mode;
public:
	CritItem() :
			m_first(0), m_second(0), m_crit(0), m_mode(
					ClusterMergeMode::invalid) {
	}
	CritItem(const U i1, const U i2) :
			m_first(i1), m_second(i2), m_crit(0), m_mode(
					ClusterMergeMode::invalid) {
		if (this->m_first > this->m_second) {
			const U t = this->m_first;
			this->m_first = this->m_second;
			this->m_second = t;
		}
	}
	CritItem(const U i1, const U i2, const W c) :
			m_first(i1), m_second(i2), m_crit(c), m_mode(
					ClusterMergeMode::invalid) {
		if (this->m_first > this->m_second) {
			const U t = this->m_first;
			this->m_first = this->m_second;
			this->m_second = t;
		}
	}
	CritItem(const U i1, const U i2, const W c, ClusterMergeMode m) :
			m_first(i1), m_second(i2), m_crit(c), m_mode(m) {
		if (this->m_first > this->m_second) {
			const U t = this->m_first;
			this->m_first = this->m_second;
			this->m_second = t;
		}
	}
	CritItem(const CritItemType &other) :
			m_first(other.m_first), m_second(other.m_second), m_crit(
					other.m_crit), m_mode(other.m_mode) {
	}
	CritItemType & operator=(const CritItemType &other) {
		if (this != &other) {
			this->m_first = other.m_first;
			this->m_second = other.m_second;
			this->m_crit = other.m_crit;
			this->m_mode = other.m_mode;
		}
		return (*this);
	}
	virtual ~CritItem() {
	}
public:
	bool is_valid(void) const {
		return (this->m_first != this->m_second);
	}
	U first(void) const {
		return (this->m_first);
	}
	U second(void) const {
		return (this->m_second);
	}
	W criteria(void) const {
		return (this->m_crit);
	}
	void criteria(const W c) {
		this->m_crit = c;
	}
	ClusterMergeMode merge_mode(void) const {
		return (this->m_mode);
	}
	void merge_mode(ClusterMergeMode m) {
		this->m_mode = m;
	}
	static size_t generate(const size_t n, crititems_vector &oRes) {
		oRes.clear();
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < i; ++j) {
				CritItemType c((U) j, (U) i);
				oRes.push_back(c);
			} // j
		} // i
		return (oRes.size());
	} // generators
	CritItemType & add(const CritItemType &other) {
		if (this->is_valid() && other.is_valid()) {
			if (other.m_crit < this->m_crit) {
				*this = other;
			}
		} else if ((!this->is_valid()) && other.is_valid()) {
			*this = other;
		}
		return (*this);
	} // add
	CritItemType & operator+=(const CritItemType &other) {
		return this->add(other);
	}
	CritItemType operator+(const CritItemType &other) const {
		CritItemType t(*this);
		t.add(other);
		return (t);
	}
	bool operator==(const CritItemType &other) const {
		return ((this->m_first == other.m_first)
				&& (this->m_second == other.m_second));
	}
	bool operator<(const CritItemType &other) const {
		if (this->m_first < other.m_first) {
			return (true);
		} else if (this->m_first > other.m_first) {
			return (false);
		}
		return (this->m_second < other.m_second);
	}
};
// class CritItem<U,W>
/////////////////////////////////////
}// namespace info
//////////////////////////////
#endif /* CRITITEM_H_ */

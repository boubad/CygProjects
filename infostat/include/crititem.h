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
template<typename U = unsigned long, typename W = double>
class CritItem: private boost::noncopyable {
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
			m_first(other.m_crit), m_second(other.m_second), m_crit(
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
///////////////////////////////////
template<typename U = unsigned long, typename W = double>
class DistanceMap: private boost::noncopyable {
public:
	using ints_set = std::set<U>;
	using entry_type = std::map<U,W>;
	using map_type = std::map<U, entry_type>;
	using DistanceType = DistanceMap<U,W>;
	using CritItemType = CritItem<U,W>;
	using DistanceMapType = DistanceMap<U,W>;
private:
	map_type m_map;
	ints_set m_set;
	//
	std::mutex _mutex;
public:
	DistanceMap() {
	}
	virtual ~DistanceMap() {
	}
public:
	bool has_entry(const U i1, const U i2) {
		U index1 = i1, index2 = i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		std::unique_lock<std::mutex> oLock(this->_mutex);
		map_type &oMap = this->m_map;
		auto it = oMap.find(index1);
		if (it == oMap.end()) {
			return (false);
		}
		entry_type &e = (*it).second;
		return (e.find() != e.end());
	}
	bool get(const U i1, const U i2, W &res) {
		U index1 = i1, index2 = i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		std::unique_lock<std::mutex> oLock(this->_mutex);
		map_type &oMap = this->m_map;
		auto it = oMap.find(index1);
		if (it == oMap.end()) {
			return (false);
		}
		entry_type &e = (*it).second;
		auto jt = e.find(index2);
		if (jt != e.end()) {
			res = (*jt).second;
			return (true);
		}
		return (false);
	}
	void clear(void) {
		std::unique_lock<std::mutex> oLock(this->_mutex);
		this->m_map.clear();
		this->m_set.clear();
	} // clear
	void indexes(ints_set &oSet) {
		std::unique_lock<std::mutex> oLock(this->_mutex);
		oSet = this->m_set;
	}
	void add(const U i1, const U i2, const W val) {
		if (i1 == i2) {
			return;
		}
		U index1 = i1, index2 = i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		std::unique_lock<std::mutex> oLock(this->_mutex);
		map_type &oMap = this->m_map;
		auto it = oMap.find(index1);
		if (it == oMap.end()) {
			entry_type e;
			e[index2] = val;
			oMap[index1] = e;
		} else {
			entry_type &e = (*it).second;
			e[index2] = val;
		}
		ints_set &oSet = this->m_set;
		oSet.insert(index1);
		oSet.insert(index2);
	} // add
	void add(const CritItemType &oCrit) {
		this->add(oCrit.first(), oCrit.second(), oCrit.criteria());
	} // add
	void increase_count(const U i1, const U i2) {
		if (i1 == i2) {
			return;
		}
		U index1 = i1, index2 = i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		std::unique_lock<std::mutex> oLock(this->_mutex);
		map_type &oMap = this->m_map;
		auto it = oMap.find(index1);
		if (it == oMap.end()) {
			entry_type e;
			e[index2] = (W) 1;
			oMap[index1] = e;
		} else {
			entry_type &e = (*it).second;
			auto jt = e.find(index2);
			if (jt == e.end()) {
				e[index2] = (W) 1;
			} else {
				W old = (*jt).second;
				++old;
				e[index2] = old;
			}
		}
		ints_set &oSet = this->m_set;
		oSet.insert(index1);
		oSet.insert(index2);
	} // add
};
// class DistanceMap<U,W>
/////////////////////////////////////
}// namespace info
//////////////////////////////
#endif /* CRITITEM_H_ */

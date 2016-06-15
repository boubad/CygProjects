#pragma once
#ifndef __INDIVSUMMATOR_H__
#define __INDIVSUMMATOR_H__
///////////////////////////////
#include "info_global.h"
////////////////////////////////////
namespace info {
//////////////////////////////////
template<typename U>
class IndivSummator {
public:
	using variable_entry = std::pair<size_t, double>;
	using entries_map = std::map<U, variable_entry>;
	using DataMap = std::map<U, InfoValue>;
private:
	entries_map m_map;
public:
	IndivSummator() {
	}
	IndivSummator(const IndivSummator<U> &other) :
			m_map(other.m_map) {
	}
	IndivSummator<U> & operator=(const IndivSummator<U> &other) {
		if (this != &other) {
			this->m_map = other.m_map;
		}
		return (*this);
	}
	virtual ~IndivSummator() {
	}
public:
	void reset(void) {
		this->m_map.clear();
	}
	template<typename XU>
	void add(const std::map<XU, InfoValue> &oMap) {
		entries_map &m = this->m_map;
		for (auto &p : oMap) {
			const InfoValue &v = p.second;
			if ((!v.empty()) && v.is_numerical()) {
				double x = 0;
				v.get_value(x);
				size_t nc = 1;
				U key = (U) p.first;
				auto it = m.find(key);
				if (it != m.end()) {
					variable_entry &e = (*it).second;
					e.first = e.first + 1;
					e.second = e.second + x;
				} else {
					m[key] = std::make_pair(nc, x);
				}
			}	// v
		}	// p
	}	// add
	void get_result(DataMap &oMap) const {
		oMap.clear();
		const entries_map &m = this->m_map;
		for (auto &p : m) {
			const variable_entry &ve = p.second;
			const size_t nc = ve.first;
			if (nc > 0) {
				double ss = ve.second;
				if (nc > 1) {
					ss /= nc;
				}
				U key = p.first;
				oMap[key] = InfoValue(ss);
			}	// nc
		}	// p
	}	// get_result
};
// class IndivSummator<U>
////////////////////////
}// namespace info
////////////////////////////////
#endif // !__INDIVSUMMATOR_H__

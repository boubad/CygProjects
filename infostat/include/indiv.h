#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
////////////////////////////////
#include <map>
#include <memory>
#include <algorithm>
/////////////////////////////////
#include "infovalue.h"
#include "interruptable_object.h"
////////////////////////////////////
namespace info {
//////////////////////////////////
template<typename U1, typename U2, typename W>
bool info_global_compute_distance(const std::map<U1, InfoValue> &oMap1,
		const std::map<U2, InfoValue> &oMap2, W &res,
		std::atomic_bool *pCancel = nullptr) {
	size_t nc = 0;
	double dRes = 0;
	for (auto kt = oMap1.begin(); kt != oMap1.end(); ++kt) {
		if ((pCancel != nullptr) && pCancel->load()) {
			return (false);
		}
		const std::pair<U1, InfoValue> &p = *kt;
		const InfoValue &vv1 = p.second;
		if ((!vv1.empty()) && vv1.is_numerical()) {
			const U2 key = (U2) p.first;
			auto it = oMap2.find(key);
			if (it != oMap2.end()) {
				const InfoValue &vv2 = (*it).second;
				if ((!vv2.empty()) && vv2.is_numerical()) {
					double v1, v2;
					vv1.get_value(v1);
					vv2.get_value(v2);
					const double tt = v1 - v2;
					dRes += tt * tt;
					++nc;
				} // vv2
			} // it
		} // vv1
	} // kt
	if (nc > 1) {
		dRes /= nc;
	}
	res = (W) dRes;
	return (nc > 0);
} //info_global_compute_distance
//////////////////////////////////
template<typename U>
class IndivSummator: public InterruptObject {
public:
	using variable_entry = std::pair<size_t,double>;
	using entries_map = std::map<U,variable_entry>;
	using DataMap = std::map<U, InfoValue>;
private:
	entries_map m_map;
public:
	IndivSummator(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel) {
	}
	IndivSummator(const IndivSummator<U> &other) :
			InterruptObject(other), m_map(other.m_map) {
	}
	IndivSummator<U> & operator=(const IndivSummator<U> &other) {
		if (this != &other) {
			InterruptObject::operator=(other);
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
		for (auto kt = oMap.begin(); kt != oMap.end(); ++kt) {
			if (this->check_interrupt()) {
				return;
			}
			const std::pair<XU, InfoValue> &p = *kt;
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
		}	// kt
	}	// add
	void get_result(DataMap &oMap) const {
		oMap.clear();
		const entries_map &m = this->m_map;
		for (auto kt = m.begin(); kt != m.end(); ++kt) {
			if (this->check_interrupt()) {
				return;
			}
			const std::pair<U, variable_entry> &p = *kt;
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
		}	// kt
	}	// get_result
};
// class IndivSummaror<U>
/////////////////////////////
template<typename U = unsigned long>
class Indiv : public InterruptObject {
public:
	using IndexType = U;
	using IndivType = Indiv<U>;
	using DataMap = std::map<U, InfoValue>;
private:
	U m_index;
	DataMap m_center;
public:
	Indiv(const U aIndex = 0,std::atomic_bool *pCancel = nullptr) : InterruptObject(pCancel),
			m_index(0) {
	}
	template<typename XU>
	Indiv(const XU aIndex, const std::map<XU, InfoValue> &oMap,
			std::atomic_bool *pCancel = nullptr) : InterruptObject(pCancel),
			m_index((U) aIndex) {
		DataMap &m = this->m_center;
		std::for_each(oMap.begin(), oMap.end(),
				[&m](const std::pair<XU, InfoValue> &p) {
					InfoValue v = p.second;
					if (!v.empty()) {
						U key = (U)p.first;
						m[key] = v;
					}
				});
	}
	Indiv(IndivType &other) : InterruptObject(other),
			m_index(other.m_index), m_center(other.m_center) {
	}
	IndivType & operator=(const IndivType &other) {
		if (this != &other) {
			InterruptObject::operator=(other);
			this->m_index = other.m_index;
			this->m_center = other.m_center;
		}
		return (*this);
	}
	virtual ~Indiv() {
	}
public:
	U id(void) const {
		return (this->m_index);
	}
	void id(const U a) {
		this->m_index = a;
	}
	const DataMap & center(void) const {
		return (this->m_center);
	}
	bool empty(void) const {
		return (this->m_center.empty());
	}
	bool has_numeric_fields(void) const {
		const DataMap &oMap = this->m_center;
		for (auto it = oMap.begin(); it != oMap.end(); ++it) {
			const InfoValue &v = (*it).second;
			if (v.is_numerical()) {
				return (true);
			}
		}
		return (false);
	}	// has_numeric_fields
	template<typename XU>
	void center(const std::map<XU, InfoValue> &oMap) {
		DataMap &m = this->m_center;
		m.clear();
		std::for_each(oMap.begin(), oMap.end(),
				[&m](const std::pair<XU, InfoValue> &p) {
					InfoValue v = p.second;
					if (!v.empty()) {
						U key = (U)p.first;
						m[key] = v;
					}
				});
	}
	template<typename W>
	bool distance(const IndivType &other, W &res) const {
		return (info_global_compute_distance(this->m_center, other.m_center,
				res, this->get_cancelleable_flag()));
	}	// distance
};
// class Indiv<U,T>
//////////////////////////////////////
template<typename U = unsigned long>
class IIndivSource {
public:
	using IndexType = U;
	using IndivType = Indiv<U>;
	using DataMap = std::map<U, InfoValue>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
public:
	virtual size_t count(void) = 0;
	virtual IndivTypePtr get(const size_t pos) = 0;
	virtual void reset(void) = 0;
	virtual IndivTypePtr next(void) = 0;
public:
	virtual ~IIndivSource() {
	}
};
// class IIndivSource<U,T>
////////////////////////////////
}// namespace info
///////////////////////////////
#endif // !__INDIV_H__

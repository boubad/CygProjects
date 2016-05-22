#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
////////////////////////////////
#include "info_global.h"
#include "interruptable_object.h"
#include "stringconvert.h"
////////////////////////////////////
namespace info {
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
template<typename U = unsigned long, typename STRINGTYPE = std::string>
class Indiv: public InterruptObject {
public:
	using IndexType = U;
	using IndivType = Indiv<U,STRINGTYPE>;
	using DataMap = std::map<U, InfoValue>;
	using iterator = typename DataMap::const_iterator;
private:
	U m_index;
	STRINGTYPE m_sigle;
	DataMap m_center;
public:
	Indiv(const U aIndex = 0, const STRINGTYPE &sSigle = STRINGTYPE(),
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(0),m_sigle(sSigle) {
	}
	template<typename XU>
	Indiv(const XU aIndex, const std::map<XU, InfoValue> &oMap,
			const STRINGTYPE &sSigle = STRINGTYPE(),
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index((U) aIndex),m_sigle(sSigle) {
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
	Indiv(IndivType &other) :
			InterruptObject(other), m_index(other.m_index), m_sigle(other.m_sigle),m_center(
					other.m_center) {
	}
	IndivType & operator=(const IndivType &other) {
		if (this != &other) {
			InterruptObject::operator=(other);
			this->m_index = other.m_index;
			this->m_sigle = other.m_sigle;
			this->m_center = other.m_center;
		}
		return (*this);
	}
	virtual ~Indiv() {
	}
public:
	iterator begin(void) const {
		return (this->m_center.begin());
	}
	iterator end(void) const {
		return (this->m_center.end());
	}
	U id(void) const {
		return (this->m_index);
	}
	void id(const U a) {
		this->m_index = a;
	}
	const STRINGTYPE &sigle(void) const {
		return (this->m_sigle);
	}
	void sigle(const STRINGTYPE &s){
		this->m_sigle = s;
	}
	const DataMap & center(void) const {
		return (this->m_center);
	}
	bool empty(void) const {
		return (this->m_center.empty());
	}
	bool is_empty(void) const {
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
	template<typename XU, typename W>
	bool distance(const std::map<XU, InfoValue> &oPoint, W &res) const {
		return (info_global_compute_distance(this->m_center, oPoint, res,
				this->get_cancelleable_flag()));
	}	// distance
public:
	std::ostream & write_to(std::ostream &os) const {
		std::string sx = info_2s(this->m_sigle);
		os << "{" << this->m_index << ", " << sx << " ,[";
		std::string s;
		info_global_write_map(this->m_center, s);
		os << s << "]}";
		return os;
	}	// write_to
	std::wostream & write_to(std::wostream &os) const {
		std::wstring sx = info_2ws(this->m_sigle);
		os << L"{" << this->m_index << L", " << sx << L" ,[";
		std::wstring s;
		info_global_write_map(this->m_center, s);
		os << s << L"]}";
		return os;
	}	// write_to
};
// class Indiv<U,T>
//////////////////////////////////////
template<typename U = unsigned long,typename STRINGTYPE = std::string>
class IIndivSource {
public:
	using IndexType = U;
	using IndivType = Indiv<U,STRINGTYPE>;
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
/////////////////////////////////////
template <typename U,typename STRINGTYPE>
inline std::ostream & operator<<(std::ostream &os, const info::Indiv<U,STRINGTYPE> &d){
	return d.write_to(os);
}
template <typename U,typename STRINGTYPE>
inline std::wostream & operator<<(std::wostream &os, const info::Indiv<U,STRINGTYPE> &d){
	return d.write_to(os);
}
///////////////////////////////
#endif // !__INDIV_H__

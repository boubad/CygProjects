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
	template<typename U = unsigned long, typename STRINGTYPE = std::string>
	class Indiv : public InterruptObject {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using DataMap = std::map<U, InfoValue>;
		using iterator = typename DataMap::const_iterator;
	private:
		U m_index;
		STRINGTYPE m_sigle;
		DataMap m_center;
	public:
		Indiv(const U aIndex = 0, const STRINGTYPE &sSigle = STRINGTYPE(),
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(0), m_sigle(sSigle) {
		}
		template<typename XU>
		Indiv(const XU aIndex, const std::map<XU, InfoValue> &oMap,
			const STRINGTYPE &sSigle = STRINGTYPE(),
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index((U)aIndex), m_sigle(sSigle) {
			DataMap &m = this->m_center;
			for (auto &p : oMap) {
				InfoValue v = p.second;
				if (!v.empty()) {
					U key = (U)p.first;
					m[key] = v;
				}
			}// p
		}
		Indiv(IndivType &other) :
			InterruptObject(other), m_index(other.m_index), m_sigle(other.m_sigle), m_center(
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
		void sigle(const STRINGTYPE &s) {
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
			for (auto &p : oMap) {
				const InfoValue &v = p.second;
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
			for (auto &p : oMap) {
				InfoValue v = p.second;
				if (!v.empty()) {
					U key = (U)p.first;
					m[key] = v;
				}
			}// p
		}
		template<typename XU, typename XSTRING, typename W>
		bool distance(const Indiv<XU,XSTRING> &other, W &res) const {
			return (info_global_compute_distance(this->m_center, other.m_center,
				res, this->get_cancelleable_flag()));
		}	// distance
		template<typename XU, typename W>
		bool distance(const std::map<XU, InfoValue> &oPoint, W &res) const {
			return (info_global_compute_distance(this->m_center, oPoint, res,
				this->get_cancelleable_flag()));
		}	// distance
		template<typename XU, typename XSTRING, typename W>
		bool distance(const std::shared_ptr<Indiv<XU, XSTRING> > &oPtr, W &res) const {
			const Indiv<XU, XSTRING> *p = oPtr.get();
			if (p != nullptr) {
				return this->distance(*p, res);
			}
			return (false);
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
	template<typename U = unsigned long, typename STRINGTYPE = std::string>
	class IIndivSource {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using DataMap = std::map<U, InfoValue>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using SourceType = IIndivSource<U, STRINGTYPE>;
	public:
		virtual size_t count(void) = 0;
		virtual IndivTypePtr get(const size_t pos) = 0;
		virtual void reset(void) = 0;
		virtual IndivTypePtr next(void) = 0;
		virtual IndivTypePtr find(const IndexType aIndex) = 0;
		virtual ~IIndivSource() {
		}
	};
	// class IIndivSource<U,T>
	////////////////////////////////
}// namespace info
/////////////////////////////////////
template <typename U, typename STRINGTYPE>
inline std::ostream & operator<<(std::ostream &os, const info::Indiv<U, STRINGTYPE> &d) {
	return d.write_to(os);
}
template <typename U, typename STRINGTYPE>
inline std::wostream & operator<<(std::wostream &os, const info::Indiv<U, STRINGTYPE> &d) {
	return d.write_to(os);
}
///////////////////////////////
#endif // !__INDIV_H__

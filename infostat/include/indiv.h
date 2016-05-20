#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
////////////////////////////////
#include <map>
#include <memory>
#include <algorithm>
/////////////////////////////////
#include "infovalue.h"
////////////////////////////////////
namespace info {
	//////////////////////////////////
	template <typename U1, typename U2, typename W>
	bool info_global_compute_distance(const std::map<U1, InfoValue> &oMap1,
		const std::map<U2, InfoValue> &oMap2, W &res) {
		size_t nc = 0;
		double dRes = 0;
		std::for_each(oMap1.begin(), oMap1.end(), [&](const std::pair<U1, InfoValue> &p) {
			const InfoValue &vv1 = p.second;
			if ((!vv1.empty()) && vv1.is_numerical()) {
				const U2 key = (U2)p.first;
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
					}// vv2
				}// it
			}// vv1
		});
		if (nc > 1) {
			dRes /= nc;
		}
		res = (W)dRes;
		return (nc > 0);
	}//info_global_compute_distance
	/////////////////////////////
	template <typename U = unsigned long>
	class Indiv {
	public:
		using IndexType = U;
		using IndivType = Indiv<U>;
		using DataMap = std::map<U, InfoValue>;
	private:
		U m_index;
		DataMap m_center;
	public:
		Indiv(const U aIndex = 0) :m_index(0) {}
		template <typename XU>
		Indiv(const XU aIndex, const std::map<XU, InfoValue> &oMap) : m_index((U)aIndex) {
			DataMap &m = this->m_center;
			std::for_each(oMap.begin(), oMap.end(), [&m](const std::pair<XU, InfoValue> &p) {
				InfoValue v = p.second;
				if (!v.empty()) {
					U key = (U)p.first;
					m[key] = v;
				}
			});
		}
		Indiv(IndivType &other) :m_index(other.m_index), m_center(other.m_center) {
		}
		IndivType & operator=(const IndivType &other) {
			if (this != &other) {
				this->m_index = other.m_index;
				this->m_center = other.m_center;
			}
			return (*this);
		}
		virtual ~Indiv() {}
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
			const DataMap &oMap = this->m_center();
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				const InfoValue &v = (*it).second;
				if (v.is_numerical()) {
					return (true);
				}
			}
			return (false);
		}// has_numeric_fields
		template <typename XU>
		void center(const std::map<XU, InfoValue> &oMap) {
			DataMap &m = this->m_center;
			m.clear();
			std::for_each(oMap.begin(), oMap.end(), [&m](const std::pair<XU, InfoValue> &p) {
				InfoValue v = p.second;
				if (!v.empty()) {
					U key = (U)p.first;
					m[key] = v;
				}
			});
		}
		template <typename W>
		bool distance(const IndivType &other, W &res) const {
			return (info_global_compute_distance(this->m_center, other.m_center, res));
		}// distance
	};// class Indiv<U,T>
	//////////////////////////////////////
	template <typename U = unsigned long>
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
		virtual ~IIndivSource() {}
	};// class IIndivSource<U,T>
	////////////////////////////////
}// namespace info
///////////////////////////////
#endif // !__INDIV_H__

#pragma once
#ifndef __INDIVCLUSTER_H__
#define __INDIVCLUSTER_H__
//////////////////////////////
#include "indiv.h"
////////////////////////////////////
#include <vector>
/////////////////////////////////
namespace info {
	///////////////////////////////
	template <typename U = unsigned long>
	class IndivCluster {
	public:
		using IndexType = U;
		using IndivType = Indiv<U>;
		using DataMap = std::map<U, InfoValue>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using SourceType = IIndivSource<U>;
		using indivptrs_vector = std::vector<IndivTypePtr>;
		using IndivClusterType = IndivCluster<U>;
		using iterator = indivptrs_vector::const_iterator;
	private:
		IndexType m_index;
		indivptrs_vector m_indivs;
		DataMap m_center;
	public:
		IndivCluster() :m_index(0) {}
		IndivCluster(const IndexType aIndex, const DataMap &oMap) :m_index(aIndex),
			m_center(oMap) {}
		IndivCluster(const IndexType aIndex, const IndivTypePtr &oInd) :m_index(aIndex) {
			const IndivType *p = oInd.get();
			if (p != nullptr) {
				this->m_center = p->center();
			}
		}
		IndivCluster(const IndivClusterType &other) :m_index(other.m_index),
			m_indivs(other.m_indivs), m_center(other.m_center) {}
		IndivClusterType & operator=(const IndivClusterType &other) {
			if (this != &other) {
				this->m_index = other.m_index;
				this->m_indivs = other.m_indivs;
				this->m_center = other.m_center;
			}
			return (*this);
		}
		virtual ~IndivCluster() {}
	public:
		IndexType id(void) const {
			return (this->m_index);
		}
		void id(const IndexType n) {
			this->m_index = n;
		}
		const indivptrs_vector members(void) const {
			return (this->m_indivs);
		}
		const DataMap & center(void) const {
			return (this->m_center);
		}
		iterator begin(void) const {
			return (this->m_indivs.begin());
		}
		iterator end(void) const {
			return (this->m_indivs.end());
		}
	public:
		void clear_members(void) {
			this->m_indivs.clear();
		}
		bool add(const IndivTypePtr &oInd) {
			const IndivType *p = oInd.get();
			if (p == nullptr) {
				return (false);
			}
			const U aIndex = p->id();
			bool bFound = false;
			indivptrs_vector &v = this->m_indivs;
			for (auto it = v.begin(); it != v.end(); ++it) {
				IndivTypePtr xInd = *it;
				IndivType *px = xInd.get();
				if (px != nullptr) {
					if (px->id() == aIndex) {
						bFound = true;
						break;
					}
				}
			}// it
			if (bFound) {
				return (false);
			}
			IndivTypePtr o(oInd);
			v.push_back(o);
			return (true);
		}// add
		IndivClusterType & operator+=(const IndivTypePtr &oInd) {
			this->add_indiv(oInd);
			return (true);
		}
		bool inertia(double &dRes) const {
			const indivptrs_vector &v = this->m_indivs;
			const DataMap &oCenter = this->m_center;
			dRes = 0;
			size_t nc = 0;
			std::for_each(v.begin(), v.end(), [&](const IndivTypePtr &oInd) {
				const IndivType *p = oInd.get();
				if (p != nullptr) {
					const DataMap &oMap = p->center();
					double d = 0;
					if (info_global_compute_distance(oCenter, oMap, d)) {
						dRes += d;
						++nc;
					}
				}
			});
			if (nc > 1) {
				dRes /= nc;
			}
			return (nc > 0);
		}// inertia
		void update_center(void) {
			std::map<U, size_t> counts;
			std::map<U, double> sommes;
			const indivptrs_vector &v = this->m_indivs;
			std::for_each(v.begin(), v.end(), [&](const IndivTypePtr &oInd) {
				const IndivType *p = oInd.get();
				if (p != nullptr) {
					const DataMap &oMap = p->center();
					std::for_each(oMap.begin(), oMap.end(), [&](const std::pair<U, InfoValue> &p) {
						const InfoValue &vx = p.second;
						if ((!vx.empty()) && vx.is_numerical()) {
							double x = 0;
							vx.get_value(x);
							const U key = p.first;
							size_t nc = 1;
							auto it = counts.find(key);
							if (it != counts.end()) {
								nc = (*it).second;
								double ss = sommes[key];
								x += ss;
								++nc;
							}// it
							counts[key] = nc;
							sommes[key] = x;
						}// vx
					});
				}// p
			});
			DataMap &rMap = this->m_center;
			rMap.clear();
			std::for_each(counts.begin(), counts.end(), [&](const std::pair<U, size_t> &p) {
				const U key = p.first;
				size_t nc = p.second;
				double ss = sommes[key];
				if (nc > 1) {
					ss /= nc;
				}
				InfoValue vx(ss);
				rMap[key] = vx;
			});
		}// update_center
		template <typename W>
		bool distance(const IndivTypePtr &oInd, W &res) const {
			const IndivType *p = oInd.get();
			if (p == nullptr) {
				res = 0;
				return (false);
			}
			return info_global_compute_distance(this->m_center, p->center(), res);
		}// distance
		template <typename W>
		bool distance(const IndivClusterType &other, W &res) const {
			return info_global_compute_distance(this->m_center, other.m_center, res);
		}// distance
	};// clas IndivCluster<U>
	///////////////////////////////
}// namespace info
/////////////////////////////////
#endif // !__INDIVCLUSTER_H__

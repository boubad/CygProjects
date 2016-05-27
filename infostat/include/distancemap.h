#pragma once
#ifndef __DISTANCEMAP_H__
#define __DISTANCEMAP_H__
///////////////////////////////////
#include "crititem.h"
#include "indiv.h"
/////////////////////////////
namespace info {
	//////////////////////////////////////
	template<typename U = unsigned long, typename W = long>
	class DistanceMap {
	public:
		using ints_set = std::set<U>;
		using ints_vector = std::vector<U>;
		using entry_type = std::map<U, W>;
		using map_type = std::map<U, entry_type>;
		using DistanceType = DistanceMap<U, W>;
		using DistanceMapType = DistanceMap<U, W>;
	private:
		map_type m_map;
		ints_set m_set;
	public:
		DistanceMap() {
		}
		template <typename XU, typename STRINGTYPE>
		DistanceMap(IIndivSource<XU, STRINGTYPE> *pProvider) {
			using sizets_vector = std::vector<size_t>;
			using DataMap = std::map<XU, InfoValue>;
			using IndivType = Indiv<XU, STRINGTYPE>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			assert(pProvider != nullptr);
			const size_t n = pProvider->count();
			sizets_vector args(n);
			for (size_t i = 0; i < n; ++i) {
				args[i] = i;
			}//i
			std::mutex _mutex;
			info_parallel_for_each(args.begin(), args.end(), [&](const size_t &i) {
				IndivTypePtr oInd1 = pProvider->get(i);
				const IndivType *pInd1 = oInd1.get();
				if ((pInd1 != nullptr) && pInd1->has_numeric_fields()) {
					const U aIndex1 = pInd1->id();
					const DataMap &m1 = pInd1->center();
					for (size_t j = 0; j < i; ++j) {
						IndivTypePtr oInd2 = pProvider->get(j);
						const IndivType *pInd2 = oInd2.get();
						if ((pInd2 != nullptr) && pInd2->has_numeric_fields()) {
							const U aIndex2 = pInd2->id();
							W d = 0;
							if (info_global_compute_distance(m1, pInd2->center(), d)) {
								std::lock_guard<std::mutex> oLock(_mutex);
								this->add(aIndex1, aIndex2, d);
							}
						} // pInd2
					} // j
				} // pInd1
			});
		}// DistanceMap
		DistanceMap(const DistanceMapType &other) :m_map(other.m_map), m_set(other.m_set) {}
		DistanceMapType & operator=(const DistanceMapType &other) {
			if (this != &other) {
				this->m_map = other.m_map;
				this->m_set = other.m_map;
			}
			return (*this);
		}
		virtual ~DistanceMap() {
		}
	public:
		template <typename XU>
		bool has_entry(const XU i1, const XU i2) {
			U index1 = (U)i1, index2 = (U)i2;
			if (index1 > index2) {
				const U t = index1;
				index1 = index2;
				index2 = t;
			}
			map_type &oMap = this->m_map;
			auto it = oMap.find(index1);
			if (it == oMap.end()) {
				return (false);
			}
			entry_type &e = (*it).second;
			return (e.find() != e.end());
		}
		template <typename XU, typename XW>
		bool get(const XU i1, const XU i2, XW &res) {
			U index1 = (U)i1, index2 = (U)i2;
			if (index1 > index2) {
				const U t = index1;
				index1 = index2;
				index2 = t;
			}
			map_type &oMap = this->m_map;
			auto it = oMap.find(index1);
			if (it == oMap.end()) {
				return (false);
			}
			entry_type &e = (*it).second;
			auto jt = e.find(index2);
			if (jt != e.end()) {
				res = (XW)(*jt).second;
				return (true);
			}
			return (false);
		}
		void clear(void) {
			this->m_map.clear();
			this->m_set.clear();
		} // clear
		const ints_set & indexes(void) const {
			return (this->m_set);
		}
		void indexes(ints_set &oSet) {
			oSet = this->m_set;
		}
		void indexes(ints_vector &oIds) const {
			oIds.clear();
			for (U aIndex : this->m_set) {
				oIds.push_back(aIndex);
			}
		}
		template <typename XU, typename XW>
		void add(const XU i1, const XU i2, const XW xval) {
			if (i1 == i2) {
				return;
			}
			U index1 = (U)i1, index2 = (U)i2;
			if (index1 > index2) {
				const U t = index1;
				index1 = index2;
				index2 = t;
			}
			W val = (W)xval;
			map_type &oMap = this->m_map;
			auto it = oMap.find(index1);
			if (it == oMap.end()) {
				entry_type e;
				e[index2] = val;
				oMap[index1] = e;
			}
			else {
				entry_type &e = (*it).second;
				e[index2] = val;
			}
			ints_set &oSet = this->m_set;
			oSet.insert(index1);
			oSet.insert(index2);
		} // add
		template <typename XU, typename XW>
		void add(const CritItem<XU, XW> &oCrit) {
			this->add((U)oCrit.first(), (U)oCrit.second(), (W)oCrit.criteria());
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
			map_type &oMap = this->m_map;
			auto it = oMap.find(index1);
			if (it == oMap.end()) {
				entry_type e;
				e[index2] = (W)1;
				oMap[index1] = e;
			}
			else {
				entry_type &e = (*it).second;
				auto jt = e.find(index2);
				if (jt == e.end()) {
					e[index2] = (W)1;
				}
				else {
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
////////////////////////////////////
#endif // !__DISTANCEMAP_H__

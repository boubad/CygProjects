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
	using mutex_type = std::mutex;
public:
	using ints_set = std::set<U>;
	using ints_vector = std::vector<U>;
	using entry_type = std::map<U, W>;
	using map_type = std::map<U, entry_type>;
	using DistanceType = DistanceMap<U, W>;
	using DistanceMapType = DistanceMap<U, W>;
	using CritItemType = CritItem<U,W>;
	using crititems_vector = std::vector<CritItemType>;
	using sizets_vector = std::vector<size_t>;
private:
	map_type m_map;
	ints_set m_set;
	ints_vector m_ids;
private:
	template<typename XU, typename XW>
	void add_lock(const XU i1, const XU i2, const XW xval, mutex_type *pMutex) {
		if (i1 == i2) {
			return;
		}
		U index1 = (U) i1, index2 = (U) i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		W val = (W) xval;
		std::unique_lock<mutex_type> oLock(*pMutex);
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
	template<typename XU, typename STRINGTYPE>
	void compute_serial(IIndivSource<XU, STRINGTYPE> *pProvider) {
		using DataMap = std::map<XU, InfoValue>;
		using IndivType = Indiv<XU, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		//
		const size_t n = pProvider->count();
		for (size_t i = 0; i < n; ++i) {
			const XU ii = (XU) i;
			IndivTypePtr oInd1 = pProvider->get(ii);
			const IndivType *pInd1 = oInd1.get();
			if ((pInd1 != nullptr) && pInd1->has_numeric_fields()) {
				const U aIndex1 = (U) pInd1->id();
				const DataMap &m1 = pInd1->center();
				for (size_t j = 0; j < i; ++j) {
					const XU jj = (XU) j;
					IndivTypePtr oInd2 = pProvider->get(jj);
					const IndivType *pInd2 = oInd2.get();
					if ((pInd2 != nullptr) && pInd2->has_numeric_fields()) {
						const U aIndex2 = (U) pInd2->id();
						W d = 0;
						if (info_global_compute_distance(m1, pInd2->center(),
								d)) {
							this->add(aIndex1, aIndex2, d);
						}
					} // pInd2
				} // j
			} // pInd1
		} // i
	} // compute_onz_step_serial
	template<typename XU, typename STRINGTYPE>
	static void compute_one(IIndivSource<XU, STRINGTYPE> *pProvider,
			DistanceMapType *pDist, const int iStart, const int iEnd,
			mutex_type *pMutex) {
		using DataMap = std::map<XU, InfoValue>;
		using IndivType = Indiv<XU, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		//
		for (int i = iStart; i < iEnd; ++i) {
			const XU ii = (XU) i;
			IndivTypePtr oInd1 = pProvider->get(ii);
			const IndivType *pInd1 = oInd1.get();
			if ((pInd1 != nullptr) && pInd1->has_numeric_fields()) {
				const U aIndex1 = (U) pInd1->id();
				const DataMap &m1 = pInd1->center();
				for (int j = 0; j < i; ++j) {
					const XU jj = (XU) j;
					IndivTypePtr oInd2 = pProvider->get(jj);
					const IndivType *pInd2 = oInd2.get();
					if ((pInd2 != nullptr) && pInd2->has_numeric_fields()) {
						const U aIndex2 = (U) pInd2->id();
						W d = 0;
						if (info_global_compute_distance(m1, pInd2->center(),
								d)) {
							pDist->add_lock(aIndex1, aIndex2, d, pMutex);
						}
					} // pInd2
				} // j
			} // pInd1
		} // i
	} // compute_one
public:
	DistanceMap() {
	}
	template<typename XU, typename STRINGTYPE>
	DistanceMap(IIndivSource<XU, STRINGTYPE> *pProvider) {
		this->compute(pProvider);
	}
	DistanceMap(const DistanceMapType &other) :
			m_map(other.m_map), m_set(other.m_set), m_ids(other.m_ids) {
	}
	DistanceMapType & operator=(const DistanceMapType &other) {
		if (this != &other) {
			this->m_map = other.m_map;
			this->m_set = other.m_set;
			this->m_ids = other.m_ids;
		}
		return (*this);
	}
	virtual ~DistanceMap() {
	}
public:
	template<typename XU, typename STRINGTYPE>
	void compute(IIndivSource<XU, STRINGTYPE> *pProvider) {
		assert(pProvider != nullptr);
		this->clear();
		int np = (int) std::thread::hardware_concurrency();
		if (np < 2) {
			this->compute_serial(pProvider);
		} else {
			const int n = (int) pProvider->count();
			if (n < np) {
				this->compute_serial(pProvider);
			} else {
				using future_type = std::future<bool>;
				using futures_vec = std::vector<future_type>;
				DistanceMapType *pDist = this;
				mutex_type _mutex;
				mutex_type *pMutex = &_mutex;
				int nChunk = (int) (n / np);
				int nLast = n;
				int iMainEnd = nChunk;
				if (iMainEnd > nLast) {
					iMainEnd = nLast;
				}
				int nStart = iMainEnd;
				futures_vec oTasks;
				while (nStart < nLast) {
					int nEnd = nStart + nChunk;
					if (nEnd > nLast) {
						nEnd = nLast;
					}
					auto fTask =
							[nStart,nEnd,&pProvider,&pDist,&pMutex]()->bool {
								compute_one(pProvider,pDist, nStart,nEnd,pMutex);
								return (true);
							};
					oTasks.push_back(std::async(std::launch::async, fTask));
					nStart = nEnd;
				} // while nStart
				compute_one(pProvider, pDist, 0, iMainEnd, pMutex);
				bool bRet = true;
				for (future_type &t : oTasks) {
					bRet = bRet && t.get();
				} //t
			}
		}
		ints_vector &vz = this->m_ids;
		vz.clear();
		for (auto &aIndex : this->m_set) {
			vz.push_back(aIndex);
		}
	} // compute
public:
	size_t size(void) const {
		return (this->m_ids.size());
	}
	template<typename XU>
	bool has_entry(const XU i1, const XU i2) const {
		U index1 = (U) i1, index2 = (U) i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		const map_type &oMap = this->m_map;
		auto it = oMap.find(index1);
		if (it == oMap.end()) {
			return (false);
		}
		const entry_type &e = (*it).second;
		return (e.find() != e.end());
	}
	template<typename XU, typename XW>
	bool get(const XU i1, const XU i2, XW &res) const {
		U index1 = (U) i1, index2 = (U) i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		const map_type &oMap = this->m_map;
		auto it = oMap.find(index1);
		if (it == oMap.end()) {
			return (false);
		}
		const entry_type &e = (*it).second;
		auto jt = e.find(index2);
		if (jt != e.end()) {
			res = (XW) (*jt).second;
			return (true);
		}
		return (false);
	}
	bool get_id_from_index(const size_t &pos, U &aIndex) const {
		const ints_vector &oIndex = this->m_ids;
		const size_t n = oIndex.size();
		if (pos < n) {
			aIndex = oIndex[pos];
			return (true);
		}
		aIndex = 0;
		return (false);
	} // get_id_from_index
	template<typename XW>
	bool indexed_get(const size_t i1, const size_t i2, XW &res) const {
		const ints_vector &oIndex = this->m_ids;
		const size_t n = oIndex.size();
		res = 0;
		if ((i1 >= n) || (i2 >= n) || (i1 == i2)) {
			return (false);
		}
		return this->get(oIndex[i1], oIndex[i2], res);
	} // indexed_get
	U operator()(const size_t i1, const size_t i2) const {
		U res = 0;
		this->indexed_get(i1, i2, res);
		return (res);
	}
	void clear(void) {
		this->m_map.clear();
		this->m_set.clear();
		this->m_ids.clear();
	} // clear
	const ints_set & indexes(void) const {
		return (this->m_set);
	}
	void indexes(ints_set &oSet) {
		oSet = this->m_set;
	}
	void indexes(ints_vector &oIds) const {
		oIds = this->m_ids;
	}
	template<typename XU, typename XW>
	void add(const XU i1, const XU i2, const XW xval) {
		if (i1 == i2) {
			return;
		}
		U index1 = (U) i1, index2 = (U) i2;
		if (index1 > index2) {
			const U t = index1;
			index1 = index2;
			index2 = t;
		}
		W val = (W) xval;
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
	template<typename XU, typename XW>
	void add(const CritItem<XU, XW> &oCrit) {
		this->add((U) oCrit.first(), (U) oCrit.second(), (W) oCrit.criteria());
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
}
;
// class DistanceMap<U,W>
/////////////////////////////////////
}// namespace info
//////////////////////////////
////////////////////////////////////
#endif // !__DISTANCEMAP_H__

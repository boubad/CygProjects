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
template<typename U = unsigned long>
class IndivCluster: public InterruptObject {
public:
	using IndexType = U;
	using IndivType = Indiv<U>;
	using DataMap = std::map<U, InfoValue>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = IIndivSource<U>;
	using indivptrs_vector = std::vector<IndivTypePtr>;
	using IndivClusterType = IndivCluster<U>;
	using ints_sizet_map = std::map<U, size_t>;
	using iterator = typename indivptrs_vector::const_iterator;
	using ints_vector = std::vector<U>;
private:
	IndexType m_index;
	indivptrs_vector m_indivs;
	DataMap m_center;
public:
	IndivCluster(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(0) {
	}
	IndivCluster(const IndexType aIndex, const DataMap &oMap,
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(aIndex), m_center(oMap) {
	}
	IndivCluster(const IndexType aIndex, const IndivTypePtr &oInd,
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(aIndex) {
		const IndivType *p = oInd.get();
		if (p != nullptr) {
			this->m_center = p->center();
		}
	}
	IndivCluster(const IndivClusterType &other) :
			InterruptObject(other), m_index(other.m_index), m_indivs(
					other.m_indivs), m_center(other.m_center) {
	}
	IndivClusterType & operator=(const IndivClusterType &other) {
		if (this != &other) {
			InterruptObject::operator=(other);
			this->m_index = other.m_index;
			this->m_indivs = other.m_indivs;
			this->m_center = other.m_center;
		}
		return (*this);
	}
	virtual ~IndivCluster() {
	}
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
	bool is_empty(void) const {
		return (this->m_indivs.empty());
	}
	bool add(const IndivTypePtr &oInd) {
		if (this->check_interrupt()) {
			return (false);
		}
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
		} // it
		if (bFound) {
			return (false);
		}
		IndivTypePtr o(oInd);
		v.push_back(o);
		return (true);
	} // add
	IndivClusterType & operator+=(const IndivTypePtr &oInd) {
		this->add(oInd);
		return (true);
	}
	void get_indivs_map(ints_sizet_map &oMap, const size_t val) const {
		const indivptrs_vector &v = this->m_indivs;
		oMap.clear();
		for (auto kt = v.begin(); kt != v.end(); ++kt) {
			if (this->check_interrupt()) {
				return;
			}
			const IndivTypePtr &oInd = *kt;
			const IndivType *p = oInd.get();
			if (p != nullptr) {
				U key = p->id();
				oMap[key] = val;
			}
		} // kt
	} //get_indivs_map
	void get_ids(ints_vector &oVec) const {
		const indivptrs_vector &v = this->m_indivs;
		oVec.clear();
		for (auto kt = v.begin(); kt != v.end(); ++kt) {
			if (this->check_interrupt()) {
				return;
			}
			const IndivTypePtr &oInd = *kt;
			const IndivType *p = oInd.get();
			if (p != nullptr) {
				oVec.push_back(p->id());
			}
		} // kt
	} //get_indivs_map
	bool inertia(double &dRes) const {
		const indivptrs_vector &v = this->m_indivs;
		const DataMap &oCenter = this->m_center;
		dRes = 0;
		size_t nc = 0;
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		for (auto kt = v.begin(); kt != v.end(); ++kt) {
			if (this->check_interrupt()) {
				return (false);
			}
			const IndivTypePtr &oInd = *kt;
			const IndivType *p = oInd.get();
			if (p != nullptr) {
				const DataMap &oMap = p->center();
				double d = 0;
				if (info_global_compute_distance(oCenter, oMap, d, pCancel)) {
					dRes += d;
					++nc;
				}
			}
		} // kt
		if (nc > 1) {
			dRes /= nc;
		}
		return (nc > 0);
	} // inertia
	void update_center(void) {
		if (this->check_interrupt()) {
			return;
		}
		IndivSummator<U> summator(this->get_cancelleable_flag());
		const indivptrs_vector &v = this->m_indivs;
		std::for_each(v.begin(), v.end(), [&](const IndivTypePtr &oInd) {
			if (this->check_interrupt()) {
				return;
			}
			const IndivType *p = oInd.get();
			if (p != nullptr) {
				const DataMap &oMap = p->center();
				summator.add(oMap);
			} // p
			});
		summator.get_result(this->m_center);
	} // update_center
	template<typename W>
	bool distance(const IndivTypePtr &oInd, W &res) const {
		const IndivType *p = oInd.get();
		if (p == nullptr) {
			res = 0;
			return (false);
		}
		return info_global_compute_distance(this->m_center, p->center(), res,
				this->get_cancelleable_flag());
	} // distance
	template<typename W>
	bool distance(const IndivClusterType &other, W &res) const {
		return info_global_compute_distance(this->m_center, other.m_center, res,
				this->get_cancelleable_flag());
	} // distance
}
;
// clas IndivCluster<U>
///////////////////////////////
}// namespace info
/////////////////////////////////
#endif // !__INDIVCLUSTER_H__
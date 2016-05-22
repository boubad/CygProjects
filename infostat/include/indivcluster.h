#pragma once
#ifndef __INDIVCLUSTER_H__
#define __INDIVCLUSTER_H__
//////////////////////////////
#include "indiv.h"
#include "crititem.h"
////////////////////////////////////
namespace info {
////////////////////////////////////
template<typename U = unsigned long, typename STRINGTYPE = std::string>
class IndivCluster: public InterruptObject {
public:
	using IndexType = U;
	using IndivType = Indiv<U,STRINGTYPE>;
	using DataMap = std::map<U, InfoValue>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = IIndivSource<U>;
	using indivptrs_vector = std::deque<IndivTypePtr>;
	using IndivClusterType = IndivCluster<U,STRINGTYPE>;
	using ints_sizet_map = std::map<U, size_t>;
	using iterator = typename indivptrs_vector::const_iterator;
	using ints_vector = std::vector<U>;
private:
	IndexType m_index;
	STRINGTYPE m_sigle;
	indivptrs_vector m_indivs;
	DataMap m_center;
public:
	IndivCluster(const IndexType aIndex = 0, const STRINGTYPE & sSigle =
			STRINGTYPE(), std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(aIndex), m_sigle(sSigle) {
	}
	IndivCluster(const IndexType aIndex, const DataMap &oMap,
			const STRINGTYPE & sSigle = STRINGTYPE(),
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(aIndex), m_sigle(sSigle), m_center(
					oMap) {
	}
	IndivCluster(const IndexType aIndex, const IndivTypePtr &oInd,
			const STRINGTYPE & sSigle = STRINGTYPE(),
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_index(aIndex), m_sigle(sSigle) {
		const IndivType *p = oInd.get();
		if (p != nullptr) {
			this->m_center = p->center();
		}
	}
	IndivCluster(const IndivClusterType &other) :
			InterruptObject(other), m_index(other.m_index), m_sigle(
					other.m_sigle), m_indivs(other.m_indivs), m_center(
					other.m_center) {
	}
	IndivClusterType & operator=(const IndivClusterType &other) {
		if (this != &other) {
			InterruptObject::operator=(other);
			this->m_index = other.m_index;
			this->m_sigle = other.m_sigle;
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
	const STRINGTYPE &sigle(void) const {
		return (this->m_sigle);
	}
	void sigle(const STRINGTYPE &s) {
		this->m_sigle = s;
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
	bool empty(void) const {
		return (this->m_indivs.empty());
	}
	IndivTypePtr front(void) const {
		assert(!this->m_indivs.empty());
		return (this->m_indivs.front());
	}
	IndivTypePtr back(void) const {
		assert(!this->m_indivs.empty());
		return (this->m_indivs.back());
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
	void get_indivs_map(ints_sizet_map &oMap, const size_t val = 0) const {
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
				size_t xval = (val == 0) ? (size_t) this->m_index : val;
				oMap[key] = xval;
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
	template<typename XU, typename W>
	bool distance(const std::map<XU, InfoValue> &oPoint, W &res) const {
		return info_global_compute_distance(this->m_center, oPoint, res,
				this->get_cancelleable_flag());
	} // distance
	template<typename W>
	bool cluster_min_distance(const IndivClusterType &other,
			ClusterMergeMode &mode, W &res) const {
		res = 0;
		mode = ClusterMergeMode::invalid;
		if (this->is_empty() || other.is_empty()) {
			return (false);
		}
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		const indivptrs_vector &vv1 = this->m_indivs;
		const IndivType *pInd1Top = vv1.front().get();
		assert(pInd1Top != nullptr);
		const size_t n1 = vv1.size();
		const indivptrs_vector &vv2 = other.m_indivs;
		const IndivType *pInd2Top = vv2.front().get();
		assert(pInd2Top != nullptr);
		const size_t n2 = vv2.size();
		if ((n1 == 1) && (n2 == 1)) {
			mode = ClusterMergeMode::topTop;
			return (info_global_compute_distance(pInd1Top->center(),
					pInd2Top->center(), res, pCancel));
		} else if ((n1 == 1) && (n2 > 1)) {
			const IndivType *pInd2Bottom = vv2.back().get();
			assert(pInd2Bottom != nullptr);
			W d1 = 0;
			if (!info_global_compute_distance(pInd1Top->center(),
					pInd2Top->center(), d1, pCancel)) {
				return (false);
			}
			res = d1;
			mode = ClusterMergeMode::topTop;
			W d2 = 0;
			if (!info_global_compute_distance(pInd1Top->center(),
					pInd2Bottom->center(), d2, pCancel)) {
				return (false);
			}
			if (d2 < res) {
				mode = ClusterMergeMode::topBottom;
				return (true);
			}
		} else if ((n1 > 1) && (n2 == 1)) {
			const IndivType *pInd1Bottom = vv1.back().get();
			assert(pInd1Bottom != nullptr);
			W d1 = 0;
			if (!info_global_compute_distance(pInd1Top->center(),
					pInd2Top->center(), d1, pCancel)) {
				return (false);
			}
			res = d1;
			mode = ClusterMergeMode::topTop;
			W d2 = 0;
			if (!info_global_compute_distance(pInd1Bottom->center(),
					pInd2Top->center(), d2, pCancel)) {
				return (false);
			}
			if (d2 < res) {
				mode = ClusterMergeMode::bottomTop;
				return (true);
			}
		} else {
			const IndivType *pInd1Bottom = vv1.back().get();
			assert(pInd1Bottom != nullptr);
			const IndivType *pInd2Bottom = vv2.back().get();
			assert(pInd2Bottom != nullptr);
			W d1 = 0;
			if (!info_global_compute_distance(pInd1Top->center(),
					pInd2Top->center(), d1, pCancel)) {
				return (false);
			}
			res = d1;
			mode = ClusterMergeMode::topTop;
			W d2 = 0;
			if (!info_global_compute_distance(pInd1Top > center(),
					pInd2Bottom->center(), d2, pCancel)) {
				return (false);
			}
			if (d2 < res) {
				mode = ClusterMergeMode::topBottom;
				return (true);
			}
			W d3 = 0;
			if (!info_global_compute_distance(pInd1Bottom->center(),
					pInd2Top->center(), d3, pCancel)) {
				return (false);
			}
			if (d3 < res) {
				mode = ClusterMergeMode::bottomTop;
				return (true);
			}
			W d4 = 0;
			if (!info_global_compute_distance(pInd1Bottom->center(),
					pInd2Bottom->center(), d4, pCancel)) {
				return (false);
			}
			if (d4 < res) {
				mode = ClusterMergeMode::bottomBottom;
				return (true);
			}
		}
		return (false);
	} //cluster_min_distance
	template<typename W>
	bool cluster_min_distance(const IndivClusterType &other,
			DistanceMap<U, W> &distanceMap, ClusterMergeMode &mode,
			W &res) const {
		res = 0;
		mode = ClusterMergeMode::invalid;
		if (this->is_empty() || other.is_empty()) {
			return (false);
		}
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		const indivptrs_vector &vv1 = this->m_indivs;
		const IndivType *pInd1Top = vv1.front().get();
		assert(pInd1Top != nullptr);
		const U aIndex1Top = pInd1Top->id();
		const size_t n1 = vv1.size();
		const indivptrs_vector &vv2 = other.m_indivs;
		const IndivType *pInd2Top = vv2.front().get();
		assert(pInd2Top != nullptr);
		const U aIndex2Top = pInd1Top->id();
		const size_t n2 = vv2.size();
		if ((n1 == 1) && (n2 == 1)) {
			mode = ClusterMergeMode::topTop;
			return (distanceMap.get(aIndex1Top, aIndex2Top, res));
		} else if ((n1 == 1) && (n2 > 1)) {
			const IndivType *pInd2Bottom = vv2.back().get();
			assert(pInd2Bottom != nullptr);
			const U aIndex2Bottom = pInd2Bottom->id();
			W d1 = 0;
			if (!distanceMap.get(aIndex1Top, aIndex2Top, d1)) {
				return (false);
			}
			res = d1;
			mode = ClusterMergeMode::topTop;
			W d2 = 0;
			if (!distanceMap.get(aIndex1Top, aIndex2Bottom, d2)) {
				return (false);
			}
			if (d2 < res) {
				mode = ClusterMergeMode::topBottom;
				return (true);
			}
		} else if ((n1 > 1) && (n2 == 1)) {
			const IndivType *pInd1Bottom = vv1.back().get();
			assert(pInd1Bottom != nullptr);
			const U aIndex1Bottom = pInd1Bottom->id();
			W d1 = 0;
			if (!distanceMap.get(aIndex1Top, aIndex2Top, d1)) {
				return (false);
			}
			res = d1;
			mode = ClusterMergeMode::topTop;
			W d2 = 0;
			if (!distanceMap.get(aIndex1Bottom, aIndex2Top, d2)) {
				return (false);
			}
			if (d2 < res) {
				mode = ClusterMergeMode::bottomTop;
				return (true);
			}
		} else {
			const IndivType *pInd1Bottom = vv1.back().get();
			assert(pInd1Bottom != nullptr);
			const U aIndex1Bottom = pInd1Bottom->id();
			const IndivType *pInd2Bottom = vv2.back().get();
			assert(pInd2Bottom != nullptr);
			const U aIndex2Bottom = pInd2Bottom->id();
			W d1 = 0;
			if (!distanceMap.get(aIndex1Top, aIndex2Top, d1)) {
				return (false);
			}
			res = d1;
			mode = ClusterMergeMode::topTop;
			W d2 = 0;
			if (!distanceMap.get(aIndex1Top, aIndex2Bottom, d2)) {
				return (false);
			}
			if (d2 < res) {
				mode = ClusterMergeMode::topBottom;
				return (true);
			}
			W d3 = 0;
			if (!distanceMap.get(aIndex1Bottom, aIndex2Top, d3)) {
				return (false);
			}
			if (d3 < res) {
				mode = ClusterMergeMode::bottomTop;
				return (true);
			}
			W d4 = 0;
			if (!distanceMap.get(aIndex1Bottom, aIndex2Bottom, d4)) {
				return (false);
			}
			if (d4 < res) {
				mode = ClusterMergeMode::bottomBottom;
				return (true);
			}
		}
		return (false);
	} //cluster_min_distance
	void merge(const IndivClusterType &other, ClusterMergeMode mode) {
		indivptrs_vector &dest = this->m_indivs;
		const indivptrs_vector &src = other.m_indivs;
		const size_t n1 = dest.size();
		const size_t n2 = src.size();
		if (n2 < 1) {
			return;
		}
		switch (mode) {
		case ClusterMergeMode::topBottom: {
			for (size_t i = (size_t) (n2 - 1); i >= 0; i--) {
				dest.push_front(src[i]);
				if (i == 0) {
					break;
				}
			}
		}
			break;
		case ClusterMergeMode::topTop: {
			for (size_t i = 0; i < n2; ++i) {
				dest.push_front(src[i]);
			}
		}
			break;
		case ClusterMergeMode::bottomTop: {
			for (size_t i = 0; i < n2; ++i) {
				dest.push_back(src[i]);
			}
		}
			break;
		case ClusterMergeMode::bottomBottom: {
			for (size_t i = (size_t) (n2 - 1); i >= 0; i--) {
				dest.push_back(src[i]);
				if (i == 0) {
					break;
				}
			}
		}
			break;
		default:
			return;
		} //mode
		this->update_center();
	} //add
public:
	std::ostream & write_to(std::ostream &os) const {
		double d = 0;
		this->inertia(d);
		os << "{" << this->m_index << ", " << d << "," << std::endl << "[";
		const indivptrs_vector &v = this->m_indivs;
		for (auto it = v.begin(); it != v.end(); ++it) {
			if (it != v.begin()) {
				os << ", ";
			}
			const IndivTypePtr &oInd = *it;
			const IndivType *p = oInd.get();
			assert(p != nullptr);
			os << p->sigle();
		} // it
		os << "]," << std::endl;
		std::string s;
		info_global_write_map(this->m_center, s);
		os << s;
		os << std::endl << "}";
		return os;
	} // write_to
	std::wostream & write_to(std::wostream &os) const {
		double d = 0;
		this->inertia(d);
		os << L"{" << this->m_index << L", " << d << L"," << std::endl << L"[";
		const indivptrs_vector &v = this->m_indivs;
		for (auto it = v.begin(); it != v.end(); ++it) {
			if (it != v.begin()) {
				os << L", ";
			}
			const IndivTypePtr &oInd = *it;
			const IndivType *p = oInd.get();
			assert(p != nullptr);
			os << p->sigle();
		} // it
		os << L"]," << std::endl;
		std::wstring s;
		info_global_write_map(this->m_center, s);
		os << s;
		os << std::endl << L"}";
		return os;
	} // write_to
};
// clas IndivCluster<U>
///////////////////////////////
}// namespace info
/////////////////////////////////////
template<typename U, typename STRINGTYPE>
inline std::ostream & operator<<(std::ostream &os,
		const info::IndivCluster<U, STRINGTYPE> &d) {
	return d.write_to(os);
}
template<typename U, typename STRINGTYPE>
inline std::wostream & operator<<(std::wostream &os,
		const info::IndivCluster<U, STRINGTYPE> &d) {
	return d.write_to(os);
}
/////////////////////////////////
#endif // !__INDIVCLUSTER_H__

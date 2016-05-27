#pragma once
/*
 * arrangeindiv.h
 *
 *  Created on: 23 mai 2016
 *      Author: boubad
 */

#ifndef ARRANGEINDIV_H_
#define ARRANGEINDIV_H_
/////////////////////////////////
#include "clusterscollection.h"
/////////////////////////////////
namespace info {
////////////////////////////////////////
template<typename U = unsigned long, typename STRINGTYPE = std::string,
		typename DISTANCETYPE = unsigned long>
class ArrangeIndivs: public ClustersCollection<U, STRINGTYPE> {
public:
	using IndexType = U;
	using IndivType = Indiv<U,STRINGTYPE>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using indivptrs_vector = std::vector<IndivTypePtr>;
	using DataMap = std::map<U, InfoValue>;
	using ints_sizet_map = std::map<U, size_t>;
	using IndivClusterType = IndivCluster<U,STRINGTYPE>;
	using SourceType = IIndivSource<U,STRINGTYPE>;
	using clusters_vector = std::vector<IndivClusterType>;
	using ints_vector = std::vector<U>;
	using sizet_intsvector_map = std::map<size_t, ints_vector>;
	using datamaps_vector = std::vector<DataMap>;
	using CritItemType = CritItem<U,DISTANCETYPE>;
	using DistanceMapType = DistanceMap<U,DISTANCETYPE>;
	using ClustersCollectionType = ClustersCollection<U,STRINGTYPE>;
	using ArrangeIndivsType = ArrangeIndivs<U,STRINGTYPE,DISTANCETYPE>;
private:
	DistanceMapType m_distanceMap;
private:
	bool compute_distances(void) {
		DistanceMapType &oMap = this->m_distanceMap;
		oMap.clear();
		const size_t n = this->get_nbIndivs();
		SourceType *pProvider = this->source();
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		for (size_t i = 0; i < n; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			IndivTypePtr oInd1 = pProvider->get(i);
			const IndivType *pInd1 = oInd1.get();
			if ((pInd1 != nullptr) && pInd1->has_numeric_fields()) {
				const U aIndex1 = pInd1->id();
				const DataMap &m1 = pInd1->center();
				for (size_t j = 0; j < i; ++j) {
					if (this->check_interrupt()) {
						return (false);
					}
					IndivTypePtr oInd2 = pProvider->get(j);
					const IndivType *pInd2 = oInd2.get();
					if ((pInd2 != nullptr) && pInd2->has_numeric_fields()) {
						const U aIndex2 = pInd2->id();
						DISTANCETYPE d = 0;
						if (info_global_compute_distance(m1, pInd2->center(), d,
								pCancel)) {
							oMap.add(aIndex1, aIndex2, d);
						}
					} // pInd2
				} // j
			} // pInd1
		} // i
		return ((this->check_interrupt()) ? false : true);
	} // compute_distances
public:
	ArrangeIndivs(std::atomic_bool *pCancel = nullptr) :
			ClustersCollectionType(pCancel) {
	}
	ArrangeIndivs(const ArrangeIndivsType &other):ClustersCollectionType(other),
		m_distanceMap(other.m_distanceMap){}
	ArrangeIndivsType & operator=(const ArrangeIndivsType &other) {
		if (this != &other) {
			ClustersCollectionType::operator=(other);
			this->m_distanceMap = other.m_distanceMap;
		}
		return (*this);
	}
	virtual ~ArrangeIndivs() {
	}
public:
	virtual bool process(SourceType *pSource, const size_t nbClusters = 5,
		const size_t nbMaxIters = 100,
		std::atomic_bool *pCancel = nullptr) {
		size_t nc = 1;
		return ClustersCollectionType::process(pSource, nc, 100, pCancel);
	}// process
	const DistanceMapType & distances_map(void) const {
		return (this->m_distanceMap);
	}
	void indivs(indivptrs_vector &oInds) const {
		const clusters_vector &vv = this->clusters();
		oInds.clear();
		if (!vv.empty()) {
			const IndivClusterType &c = vv[0];
			oInds = c.members();
		}
	}
	void indexes(ints_vector &oInds) const {
		const clusters_vector &vv = this->clusters();
		oInds.clear();
		if (!vv.empty()) {
			const IndivClusterType &c = vv[0];
			auto v = c.members();
			for (auto it = v.begin(); it != v.end(); ++it) {
				IndivTypePtr o = *it;
				const IndivType *p = o.get();
				assert(p != nullptr);
				oInds.push_back(p->id());
			}// it
		}
	}// indexes
protected:
	virtual void clear(void) {
		ClustersCollectionType::clear();
		this->m_distanceMap.clear();
	} // clear
	virtual bool initialize_process(SourceType *pSource,
			const size_t nbClusters, const size_t nbMaxIters = 100,
			std::atomic_bool *pxCancel = nullptr) {
		if (!ClustersCollectionType::initialize_process(pSource, nbClusters,
				nbMaxIters, pxCancel)) {
			return (false);
		}
		this->clear();
		if (!this->compute_distances()) {
			return (false);
		}
		const size_t nbIndivs = this->get_nbIndivs();
		clusters_vector &clusters = this->clusters();
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		for (size_t i = 0; i < nbIndivs; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			IndivTypePtr oInd = pSource->get(i);
			IndivType *pInd = oInd.get();
			if ((pInd != nullptr) && pInd->has_numeric_fields()) {
				IndivClusterType c((U) i, STRINGTYPE(), pCancel);
				c.add(oInd);
				clusters.push_back(c);
			}
		} // i
		return (this->check_interrupt()) ? false : true;
	} // initalize_process
	virtual bool one_iteration(void) {
		clusters_vector &clusters = this->clusters();
		const size_t nc = clusters.size();
		if (nc <= this->get_nbClusters()) {
			return (false);
		}
		DistanceMapType &oMap = this->m_distanceMap;
		CritItemType oResCrit;
		for (size_t i = 0; i < nc; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			IndivClusterType &c1 = clusters[i];
			for (size_t j = 0; j < i; ++j) {
				if (this->check_interrupt()) {
					return (false);
				}
				IndivClusterType &c2 = clusters[j];
				DISTANCETYPE d = 0;
				ClusterMergeMode mode = ClusterMergeMode::invalid;
				if (c1.cluster_min_distance(c2, oMap, mode, d)) {
					CritItemType cur((U) i, (U) j, d, mode);
					oResCrit += cur;
				}
			} // j
		} // i
		size_t pos1 = (size_t) oResCrit.first();
		size_t pos2 = (size_t) oResCrit.second();
		if (pos1 == pos2) {
			return (false);
		}
		if (this->check_interrupt()) {
			return (false);
		}
		ClusterMergeMode mode = oResCrit.merge_mode();
		IndivClusterType &c1 = clusters[pos1];
		IndivClusterType &c2 = clusters[pos2];
		c1.merge(c2, mode);
		clusters.erase(clusters.begin() + pos2);
		return ((this->check_interrupt()) ? false : true);
	} // one_iteration
};
// class ArrangeIndivs<U>
/////////////////////////////////////////
}// namespace info
//////////////////////////

#endif /* ARRANGEINDIV_H_ */

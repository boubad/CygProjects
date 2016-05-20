#pragma once
/*
 * clusterize.h
 *
 *  Created on: 20 mai 2016
 *      Author: boubad
 */

#ifndef CLUSTERIZE_H_
#define CLUSTERIZE_H_
/////////////////////////////////
#include "treeitem.h"
/////////////////////////////////
#include <cstdlib>
#include <ctime>
//////////////////////////////
namespace info {
////////////////////////////////////////
template<typename U = unsigned long>
class ClusterizeKMeans: public InterruptObject, private boost::noncopyable {
public:
	using IndexType = U;
	using IndivType = Indiv<U>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using DataMap = std::map<U, InfoValue>;
	using TreeItemType = TreeItem<U>;
	using PTreeItemType = TreeItemType *;
	using ints_sizet_map = std::map<U, size_t>;
	using IndivClusterType = IndivCluster<U>;
	using treeitems_vector = std::vector<PTreeItemType>;
	using SourceType = IIndivSource<U>;
	using clusters_vector = std::vector<IndivClusterType>;
	using ints_vector = std::vector<U>;
	using sizet_intsvector_map = std::map<size_t, ints_vector>;
	using datamaps_vector = std::vector<DataMap>;
	using IndivsTreeType = IndivsTree<U>;
private:
	clusters_vector m_clusters;
	DataMap m_center;
	//
	void update_center(void) {
		if (this->check_interrupt()) {
			return;
		}
		IndivSummator<U> summator(this->get_cancelleable_flag());
		const clusters_vector &v = this->m_clusters;
		for (auto kt = v.begin(); kt != v.end(); ++kt) {
			if (this->check_interrupt()) {
				return;
			}
			const IndivClusterType &oInd = *kt;
			const DataMap &oMap = oInd.center();
			summator.add(oMap);
		} // kt
		summator.get_result(this->m_center);
	} // update_center
public:
	size_t clusters_count(void) const {
		return (this->m_clusters.size());
	}
	void get_indivs_map(ints_sizet_map &oMap) const {
		oMap.clear();
		const clusters_vector &v = this->m_clusters;
		for (auto kt = v.begin(); kt != v.end(); ++kt) {
			if (this->check_interrupt()) {
				return;
			}
			const IndivClusterType &oInd = *kt;
			size_t val = (size_t) oInd.id();
			oInd.get_indivs_map(oMap, val);
		} // kt
	} //get_indivs_map
	void get_clusters_ids(sizet_intsvector_map &oMap) const {
			if (this->check_interrupt()) {
				return;
			}
			oMap.clear();
			const clusters_vector &v = this->m_clusters;
			const size_t n = v.size();
			for (size_t i = 0; i < n; ++i) {
				if (this->check_interrupt()) {
					return;
				}
				const IndivClusterType & p = v[i];
				ints_vector oo;
				p->get_ids(oo);
				size_t key = (size_t)p.id();
				oMap[key] = oo;
			}	  // i
		}	  // get_clusters_ids
	bool get_criterias(double &fIntra, double &fInter, double &ff) const {
		if (this->check_interrupt()) {
			return (false);
		}
		const clusters_vector &v = this->m_clusters;
		fIntra = 0;
		fInter = 0;
		ff = 0;
		const size_t n = v.size();
		if (n < 1) {
			return (false);
		}
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		const DataMap &oCenter = this->m_center;
		for (size_t i = 0; i < n; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			const IndivClusterType &c = v[i];
			double d = 0;
			if (info_global_compute_distance(oCenter, c.center(), d, pCancel)) {
				fInter += d;
			}
			d = 0;
			if (c.inertia(d)) {
				fIntra += d;
			}
		} // i
		fInter /= n;
		fIntra /= n;
		if (fInter > 0) {
			ff = fIntra / fInter;
			return (true);
		}
		return (false);
	}	  // get_criterias
	const clusters_vector & clusters(void) const {
		return (this->m_clusters);
	}
	const DataMap & center(void) const {
		return (this->m_center);
	}
	size_t compute_random(const size_t nbClusters, SourceType *pProvider,
				LinkMode mode = LinkMode::linkMean, size_t nbIters = 100,
				std::atomic_bool *pxCancel = nullptr) {
			assert(nbClusters > 0);
			assert(pProvider != nullptr);
			assert(nbIters > 0);
			std::srand ( unsigned ( std::time(0) ) );
			if (pxCancel != nullptr) {
				this->set_cancelleable_flag(pxCancel);
			}
			size_t nbIndivs = pProvider->count();
			if (nbIndivs < nbClusters){
				return (0);
			}
			std::vector<size_t> temp(nbIndivs);
			for (size_t i = 0; i < nbIndivs; ++i){
				temp[i] = i;
			}// i
			std::random_shuffle(temp.begin(),temp.end());
			datamaps_vector oSeeds;
			size_t icur = 0;
			while ((oSeeds.size() < nbClusters) && (icur < nbIndivs)) {
				size_t pos = temp[icur++];
				IndivTypePtr oInd = pProvider->get(pos);
				const IndivType *pInd = oInd.get();
				if ((pInd != nullptr) && pInd->has_numeric_fields()){
					DataMap oMap = pInd->center();
					oSeeds.push_back(oMap);
				}// pInd
			}// while
			if (oSeeds.size() < 2){
				return (0);
			}
			std::atomic_bool *pCancel = this->get_cancelleable_flag();
			return (this->compute(oSeeds, pProvider, nbIters, pCancel));
		} //compute_random
	size_t compute_hierar(const size_t nbClusters, SourceType *pProvider,
			LinkMode mode = LinkMode::linkMean, size_t nbIters = 100,
			std::atomic_bool *pxCancel = nullptr) {
		assert(nbClusters > 0);
		assert(pProvider != nullptr);
		assert(nbIters > 0);
		if (pxCancel != nullptr) {
			this->set_cancelleable_flag(pxCancel);
		}
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		IndivsTreeType oTree(pCancel);
		oTree.process(pProvider, nbClusters, mode, pCancel);
		if (this->check_interrupt()) {
			return (0);
		}
		datamaps_vector oSeeds;
		oTree.get_centers(oSeeds);
		return (this->compute(oSeeds, pProvider, nbIters, pCancel));
	} //compute_hierar
	size_t compute(const datamaps_vector &oSeeds, SourceType *pProvider,
			size_t nbIters, std::atomic_bool *pxCancel = nullptr) {
		const size_t nbClusters = oSeeds.size();
		assert(nbClusters > 0);
		assert(pProvider != nullptr);
		assert(nbIters > 0);
		clusters_vector &clusters = this->m_clusters;
		clusters.clear();
		this->m_center.clear();
		size_t iter = 0;
		size_t nbIndivs = pProvider->count();
		if (nbClusters > nbIndivs) {
			return (iter);
		}
		if (pxCancel != nullptr) {
			this->set_cancelleable_flag(pxCancel);
		}
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		for (size_t i = 0; i < nbClusters; ++i) {
			const DataMap &oCenter = oSeeds[i];
			U aIndex = (U) (i + 1);
			IndivClusterType c(aIndex, oCenter, pCancel);
			clusters.push_back(c);
		} // i
		ints_sizet_map indivMap;
		const size_t nc = clusters.size();
		while (iter < nbIters) {
			if (this->check_interrupt()) {
				return (0);
			}
			//
			std::for_each(clusters.begin(), clusters.end(),
					[&](IndivClusterType &c) {
						c.clear_members();
					});
			for (size_t i = 0; i < nbIndivs; ++i) {
				if (this->check_interrupt()) {
					return (0);
				}
				bool bFirst = true;
				double dMin = 0;
				size_t iCluster = 0;
				IndivTypePtr oInd = pProvider->get(i);
				IndivType *pInd = oInd.get();
				if (pInd != nullptr) {
					for (size_t j = 0; j < nc; ++j) {
						IndivClusterType &c = clusters[j];
						double d = 0;
						if (c.distance(oInd, d)) {
							if (bFirst) {
								dMin = d;
								iCluster = j;
								bFirst = false;
							} else if (d < dMin) {
								dMin = d;
								iCluster = j;
							}
						} // distance
					} // j
					if (!bFirst) {
						IndivClusterType &c = clusters[iCluster];
						c.add(oInd);
					}
				} // ind
			} // i
			std::for_each(clusters.begin(), clusters.end(),
					[&](IndivClusterType &c) {
						c.update_center();
					});
			if (this->check_interrupt()) {
				return (0);
			}
			ints_sizet_map oMap;
			this->get_indivs_map(oMap);
			if (indivMap.empty()) {
				indivMap = oMap;
				continue;
			}
			//
			bool done = true;
			for (auto it = indivMap.begin(); it != indivMap.end(); ++it) {
				const U key = (*it).first;
				const size_t val = (*it).second;
				if (oMap.find(key) == oMap.end()) {
					return (0);
				}
				if (oMap[key] != val) {
					done = false;
					break;
				}
			} // it
			++iter;
			indivMap = oMap;
			if (done) {
				break;
			}
		} // iter
		clusters_vector temp;
		std::for_each(clusters.begin(), clusters.end(),
				[&](IndivClusterType c) {
					if (!c.is_empty()) {
						temp.push_back(c);
					}
				});
		if (temp.size() < clusters.size()) {
			clusters = temp;
		}
		this->update_center();
		return (iter);
	} // compute

public:
	ClusterizeKMeans(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel) {

	}
	virtual ~ClusterizeKMeans() {
	}
};
// class ClusterizeKMeans<U>
/////////////////////////////////////////
}// namespace info
//////////////////////////
#endif /* CLUSTERIZE_H_ */

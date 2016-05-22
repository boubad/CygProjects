#pragma once
/*
 * treecollection.h
 *
 *  Created on: 21 mai 2016
 *      Author: boubad
 */

#ifndef TREECOLLECTION_H_
#define TREECOLLECTION_H_
/////////////////////////////
#include "clusterscollection.h"
#include "treeitem.h"
/////////////////////////////////
namespace info {
///////////////////////////
template<typename U = unsigned long,typename STRINGTYPE = std::string>
class IndivsTree: public ClustersCollection<U,STRINGTYPE> {
public:
	using IndexType = U;
	using IndivType = Indiv<U,STRINGTYPE>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using DataMap = std::map<U, InfoValue>;
	using TreeItemType = TreeItem<U,STRINGTYPE>;
	using PTreeItemType = TreeItemType *;
	using ints_sizet_map = std::map<U, size_t>;
	using IndivClusterType = IndivCluster<U,STRINGTYPE>;
	using treeitems_vector = std::vector<PTreeItemType>;
	using SourceType = IIndivSource<U>;
	using clusters_vector = std::vector<IndivClusterType>;
	using ints_vector = std::vector<U>;
	using sizet_intsvector_map = std::map<size_t, ints_vector>;
	using datamaps_vector = std::vector<DataMap>;
	using ClustersCollectionType = ClustersCollection<U,STRINGTYPE>;
private:
	LinkMode m_mode;
	treeitems_vector m_items;
public:
	IndivsTree(std::atomic_bool *pCancel = nullptr) :
			ClustersCollectionType(pCancel), m_mode(LinkMode::linkMean) {
	}
	virtual ~IndivsTree() {
		treeitems_vector & v = this->m_items;
		for (auto it = v.begin(); it != v.end(); ++it) {
			PTreeItemType p = *it;
			delete p;
		}
		v.clear();
	}
	LinkMode link_mode(void) const {
		return (this->m_mode);
	}
	void link_mode(LinkMode m) {
		this->m_mode = m;
	}
	const treeitems_vector & items(void) const {
		return (this->m_items);
	}
protected:
	virtual void clear(void) {
		treeitems_vector &v = this->m_items;
		for (auto it = v.begin(); it != v.end(); ++it) {
			PTreeItemType p = *it;
			delete p;
		}
		v.clear();
		ClustersCollectionType::clear();
	}
	virtual bool initialize_process(SourceType *pSource,
			const size_t nbClusters, const size_t nbMaxIters = 100,
			std::atomic_bool *pxCancel = nullptr) {
		if (!ClustersCollectionType::initialize_process(pSource, nbClusters,
				nbMaxIters, pxCancel)) {
			return (false);
		}
		const size_t nbIndivs = this->get_nbIndivs();
		SourceType *pProvider = this->source();
		treeitems_vector & items = this->m_items;
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		IndivSummator<U> summator(pCancel);
		for (size_t i = 0; i < nbIndivs; ++i) {
			IndivTypePtr oInd = pProvider->get(i);
			const IndivType *pInd = oInd.get();
			if ((pInd != nullptr) && pInd->has_numeric_fields()) {
				PTreeItemType p = new TreeItemType(oInd, pCancel);
				assert(p != nullptr);
				items.push_back(p);
				summator.add(pInd->center());
			}	  // ok
		}	  // i
		DataMap &oCenter = this->center();
		summator.get_result(oCenter);
		return (this->check_interrupt()) ? false : true;
	} // initalize_process
	virtual bool one_iteration(void) {
		treeitems_vector & items = this->m_items;
		const size_t n = items.size();
		if (n <= this->get_nbClusters()) {
			return (false);
		}
		PTreeItemType pRes1 = nullptr;
		PTreeItemType pRes2 = nullptr;
		double distMin = 0;
		const LinkMode mode = this->m_mode;
		for (size_t i = 0; i < n; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			PTreeItemType p1 = items[i];
			for (size_t j = 0; j < i; ++j) {
				PTreeItemType p2 = items[j];
				double d = 0;
				if (p1->distance(*p2, d, mode)) {
					if (pRes1 == nullptr) {
						distMin = d;
						pRes1 = p2;
						pRes2 = p1;
					} else if (d < distMin) {
						distMin = d;
						pRes1 = p2;
						pRes2 = p1;
					}
				}	  // dist
			}	  // j
		}	  // i
		if (this->check_interrupt()) {
			return (false);
		}
		if ((pRes1 == nullptr) || (pRes2 == nullptr)) {
			return (false);
		}
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		auto it1 = std::find(items.begin(), items.end(), pRes1);
		assert(it1 != items.end());
		items.erase(it1);
		auto it2 = std::find(items.begin(), items.end(), pRes2);
		assert(it2 != items.end());
		items.erase(it2);
		PTreeItemType pNew = new TreeItemType(pRes1, pRes2, pCancel);
		assert(pNew != nullptr);
		items.push_back(pNew);
		assert(items.size() == (size_t )(n - 1));
		return (true);
	}	  // aggreg_one
	virtual bool post_terminate_process(void) {
		clusters_vector &oVec = this->clusters();
		oVec.clear();
		const treeitems_vector &v = this->m_items;
		const size_t n = v.size();
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		for (size_t i = 0; i < n; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			IndivClusterType c((U) (i + 1), STRINGTYPE(),pCancel);
			const PTreeItemType p = v[i];
			p->get_cluster(c);
			c.update_center();
			oVec.push_back(c);
		}	  // i
		return (ClustersCollectionType::post_terminate_process());
	} // post_terminate_process
};
// class IndivsTree
//////////////////////////////
}// namespaxe info
/////////////////////
#endif /* TREECOLLECTION_H_ */

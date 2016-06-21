#pragma once
#ifndef __INFO_TREEINDIVS_H__
#define __INFO_TREEINDIVS_H__
/////////////////////////////
#include "clusterscollection.h"
#include "info_treeitem.h"
/////////////////////////////////
namespace info {
	///////////////////////////
	template<typename U, typename STRINGTYPE, typename DISTANCETYPE>
	class IndivsTree : public ClustersCollection<U, STRINGTYPE> {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using DataMap = std::map<U, InfoValue>;
		using TreeItemType = TreeItem<U, STRINGTYPE, DISTANCETYPE>;
		using PTreeItemType = TreeItemType *;
		using ints_sizet_map = std::map<U, size_t>;
		using IndivClusterType = IndivCluster<U, STRINGTYPE>;
		using treeitems_vector = std::vector<PTreeItemType>;
		using SourceType = IIndivSource<U, STRINGTYPE>;
		using clusters_vector = std::vector<IndivClusterType>;
		using ints_vector = std::vector<U>;
		using sizet_intsvector_map = std::map<size_t, ints_vector>;
		using datamaps_vector = std::vector<DataMap>;
		using ClustersCollectionType = ClustersCollection<U, STRINGTYPE>;
		using cancelflag = std::atomic<bool>;
		using pcancelflag = cancelflag *;
		using PBackgrounder = Backgrounder *;
		using ClusterizeResultType = ClusterizeResult<U>;
		using ClusterizeResultPtr = std::shared_ptr< ClusterizeResultType>;
		using RESULT = std::shared_ptr< ClusterizeResultType>;
		using function_type = std::function<void(RESULT)>;
	private:
		LinkMode m_mode;
		treeitems_vector m_items;
	public:
		IndivsTree(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			function_type f = [](ClusterizeResultPtr arg) {}, LinkMode mode = LinkMode::linkMean) :
			ClustersCollectionType(pFlag, pq, f), m_mode(mode) {
		}
		virtual ~IndivsTree() {
			treeitems_vector &v = this->m_items;
			for (auto && p : v) {
				delete p;
			}// p
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
		void get_centers(datamaps_vector &oVec) const {
			oVec.clear();
			for (auto &p : this->m_items) {
				DataMap oMap = p->center();
				oVec.push_back(oMap);
			}//p
		}
		virtual ClusterizeResultPtr getResult(StageType s = StageType::current) {
			if (s != StageType::finished) {
				ClusterizeResultPtr oRes = std::make_shared<ClusterizeResultType>();
				ClusterizeResultType *p = oRes.get();
				assert(p != nullptr);
				p->stage(s);
				DISTANCETYPE fx = (DISTANCETYPE)this->m_items.size();
				p->criteria(fx);
				return (oRes);
			}
			else {
				this->post_terminate_process();
				ClusterizeResultPtr oRes = std::make_shared<ClusterizeResultType>(this->m_res);
				ClusterizeResultType *p = oRes.get();
				assert(p != nullptr);
				p->stage(s);
				return (oRes);
			}
		}
	protected:
		virtual void clear(void) {
			treeitems_vector &v = this->m_items;
			for (auto && p : v) {
				delete p;
			}// p
			v.clear();
			ClustersCollectionType::clear();
		}
		virtual bool initialize_process(SourceType *pSource,
			const size_t nbClusters, const size_t nbMaxIters = 100) {
			if (!ClustersCollectionType::initialize_process(pSource, nbClusters, nbMaxIters)) {
				return (false);
			}
			const size_t nbIndivs = this->get_nbIndivs();
			SourceType *pProvider = this->source();
			treeitems_vector & items = this->m_items;
			IndivSummator<U> summator;
			for (size_t i = 0; i < nbIndivs; ++i) {
				IndivTypePtr oInd = pProvider->get(i);
				const IndivType *pInd = oInd.get();
				if ((pInd != nullptr) && pInd->has_numeric_fields()) {
					PTreeItemType p = new TreeItemType(oInd);
					assert(p != nullptr);
					items.push_back(p);
					summator.add(pInd->center());
				}	  // ok
			}	  // i
			DataMap &oCenter = this->center();
			summator.get_result(oCenter);
			return (this->is_cancelled()) ? false : true;
		} // initalize_process
		virtual bool one_iteration(void) {
			treeitems_vector & items = this->m_items;
			const size_t n = items.size();
			if (n <= this->get_nbClusters()) {
				return (false);
			}
			PTreeItemType pRes1 = nullptr;
			PTreeItemType pRes2 = nullptr;
			DISTANCETYPE distMin = 0;
			const LinkMode mode = this->m_mode;
			for (size_t i = 0; i < n; ++i) {
				if (this->is_cancelled()) {
					return (false);
				}
				PTreeItemType p1 = items[i];
				for (size_t j = 0; j < i; ++j) {
					if (this->is_cancelled()) {
						return (false);
					}
					PTreeItemType p2 = items[j];
					DISTANCETYPE d = 0;
					if (p1->distance(*p2, d, mode)) {
						if (pRes1 == nullptr) {
							distMin = d;
							pRes1 = p2;
							pRes2 = p1;
						}
						else if (d < distMin) {
							distMin = d;
							pRes1 = p2;
							pRes2 = p1;
						}
					}	  // dist
				}	  // j
			}	  // i
			if (this->is_cancelled()) {
				return (false);
			}
			if ((pRes1 == nullptr) || (pRes2 == nullptr)) {
				return (false);
			}
			auto it1 = std::find(items.begin(), items.end(), pRes1);
			assert(it1 != items.end());
			items.erase(it1);
			auto it2 = std::find(items.begin(), items.end(), pRes2);
			assert(it2 != items.end());
			items.erase(it2);
			PTreeItemType pNew = new TreeItemType(pRes1, pRes2);
			assert(pNew != nullptr);
			items.push_back(pNew);
			assert(items.size() == (size_t)(n - 1));
			return (true);
		}	  // aggreg_one
		virtual bool post_terminate_process(void) {
			clusters_vector &oVec = this->clusters();
			oVec.clear();
			const treeitems_vector &v = this->m_items;
			const size_t n = v.size();
			for (size_t i = 0; i < n; ++i) {
				IndivClusterType c((U)(i + 1), STRINGTYPE());
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

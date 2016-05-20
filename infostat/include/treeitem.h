#pragma once
#ifndef __INDIVTREE_H__
#define __INDIVTREE_H__
/////////////////////////////////////
#include "indivcluster.h"
//////////////////////////////
#include <cassert>
#include <vector>
///////////////////////////////////
#include <boost/noncopyable.hpp>
//////////////////////////////////////////
namespace info {
	////////////////////////////////////
	enum class LinkMode { linkInvalid, linkMean, linkMin, linkMax };
	///////////////////////////////////
	template <typename U>
	class TreeItem : private boost::noncopyable {
	public:
		using IndexType = U;
		using IndivType = Indiv<U>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using DataMap = std::map<U, InfoValue>;
		using TreeItemType = TreeItem<U>;
		using ints_sizet_map = std::map<U, size_t>;
		using ints_vector = std::vector<U>;
		using IndivClusterType = IndivCluster<U>;
	private:
		TreeItemType *m_pleft;
		TreeItemType *m_pright;
		IndivTypePtr m_indiv;
		DataMap m_center;
	public:
		TreeItem(const IndivTypePtr &oInd) :
			m_pleft(nullptr), m_pright(nullptr), m_indiv(oInd) {
			IndivType *p = this->m_indiv.get();
			assert(p != nullptr);
			this->m_center = p->center();
		}
		TreeItem(TreeItemType *pLeft, TreeItemType *pRight) :
			m_pleft(pLeft), m_pright(pRight) {
			assert(pLeft != nullptr);
			assert(pRight != nullptr);
			DataMap &m1 = pLeft->m_center;
			DataMap &m2 = pRight->m_center;
			DataMap &res = this->m_center;
			std::for_each(m1.begin(), m1.end(), [&](const std::pair<U, InfoValue> &p) {
				const InfoValue &v1 = p.second;
				if ((!v1.empty()) && v1.is_numerical()) {
					const U key = p.first;
					auto it = m2.find(key);
					if (it != m2.end()) {
						const InfoValue &v2 = (*it).second;
						if ((!v2.empty()) && v2.is_numerical()) {
							double x1, x2;
							v1.get_value(x1);
							v2.get_value(x2);
							double vs = (x1 + x2) / 2.0;
							res[key] = InfoValue(vs);
						}// v2
					}// found
				}// v1
			});
		}// TreeItem
		virtual ~TreeItem() {
			delete this->m_pleft;
			delete this->m_pleft;
		}
	public:
		bool is_leaf(void) const {
			return ((this->m_pleft == nullptr) && (this->m_pright == nullptr));
		}
		const DataMap & center(void) const {
			return (this->m_center);
		}
		IndivTypePtr indiv(void) const {
			return (this - < m_indiv);
		}
		const TreeItemType *left(void) const {
			return (this->m_pleft);
		}
		const TreeItemType *right(void) const {
			return (this->m_pright);
		}
		void get_map(ints_sizet_map &oMap, const size_t val) const {
			if (this->is_leaf()) {
				const IndivType *p = this->m_indiv.get();
				if (p != nullptr) {
					U key = p->id();
					oMap[key] = val;
				}
				return;
			}
			if (this->m_pleft != nullptr) {
				this->m_pleft->get_map(oMap, val);
			}
			if (this->m_pright != nullptr) {
				this->m_pright->get_map(oMap, val);
			}
		}// get_map
		void get_ids(ints_vector &oVec) const {
			if (this->is_leaf()) {
				const IndivType *p = this->m_indiv.get();
				if (p != nullptr) {
					U key = p->id();
					oVec.push_back(key);
				}
				return;
			}
			if (this->m_pleft != nullptr) {
				this->m_pleft->get_ids(oVec);
			}
			if (this->m_pright != nullptr) {
				this->m_pright->get_ids(oVec);
			}
		}// get_map
		void get_cluster(IndivClusterType &oCluster) const {
			if (this->is_leaf()) {
				oCluster.add(this->m_indiv);
				return;
			}
			if (this->m_pleft != nullptr) {
				this->m_pleft->get_cluster(oCluster);
			}
			if (this->m_pright != nullptr) {
				this->m_pright->get_cluster(oCluster);
			}
		}// get_cluster
		bool distance(const TreeItemType &other,
			double &dRes, const LinkMode mode = LinkMode::linkMean) const {
			const DataMap &c1 = this->m_center;
			const DataMap &c2 = other.m_center;
			dRes = 0;
			if (mode == LinkMode::linkMean) {
				return info_global_compute_distance(c1, c2, dRes);
			}
			if (this->is_leaf() && other.is_leaf()) {
				return info_global_compute_distance(c1, c2, dRes);
			}
			if (this->is_leaf() && (!other.is_leaf())) {
				return other.distance(*this, dRes, mode);
			}
			const TreeItemType *pLeft1 = this->m_pleft;
			assert(pLeft1 != nullptr);
			const TreeItemType *pRight1 = this->m_pright;
			assert(pRight1 != nullptr);
			DataMap l1 = pLeft1->m_center;
			DataMap r1 = pRight1->m_pcenter;
			if (other.is_leaf()) {
				switch (mode) {
				case LinkMode::linkMax:
				{
					double d1 = 0, d2 = 0;
					if (!info_global_compute_distance(l1, c2, d1)) {
						return (false);
					}
					if (!info_global_compute_distance(r1, c2, d2)) {
						return (false);
					}
					dRes = (d1 > d2) ? d1 : d2;
					return true;
				}
				case LinkMode::linkMin:
				{
					double d1 = 0, d2 = 0;
					if (!info_global_compute_distance(l1, c2, d1)) {
						return (false);
					}
					if (!info_global_compute_distance(r1, c2, d2)) {
						return (false);
					}
					dRes = (d1 < d2) ? d1 : d2;
					return true;
				}
				default:
					return (false);
				}// mode
			}//
			const TreeItemType *pLeft2 = other.m_pleft;
			assert(pLeft2 != nullptr);
			const TreeItemType *pRight2 = other.m_pright;
			assert(pRight2 != nullptr);
			DataMap l2 = pLeft2->m_pcenter;
			DataMap r2 = pRight2->m_pcenter;
			std::vector<DataMap> src({ l1,r1 }), dest({ l2,r2 });
			bool bFirst = true;
			for (auto it = src.begin(); it != src.end(); ++it) {
				DataMap  &px1 = *it;
				for (auto jt = dest.begin(); jt != dest.end(); ++jt) {
					double d = 0;
					DataMap &px2 = *jt;
					if (!info_global_compute_distance(px1, px2, d)) {
						return (false);
					}
					if (bFirst) {
						bFirst = false;
						dRes = d;
					}
					else {
						if ((mode == LinkMode::linkMax) && (d > dRes)) {
							dRes = d;
						}
						else if ((mode == LinkMode::linkMin) && (d < dRes)) {
							dRes = d;
						}
					}
				}// jt
			}// it
			return (true);
		}// distance
	};// class TreeItem<U>
	  /////////////////////////////////////
	template <typename U = unsigned long>
	class IndivsTree : private boost::noncopyable {
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
	private:
		LinkMode m_mode;
		size_t m_nbclusters;
		treeitems_vector m_items;
	private:
		void clear(void) {
			treeitems_vector &v = this->m_items;
			for (auto it = v.begin(); it != v.end(); ++it) {
				PTreeItemType p = *it;
				delete p;
			}
			v.clear();
		}
		bool aggreg_one_step(void) {
			treeitems_vector & items = this->m_items;
			const size_t n = items.size();
			if (n <= this->m_nbclusters) {
				return (false);
			}
			PTreeItemType *pRes1 = nullptr;
			PTreeItemType *pRes2 = nullptr;
			double distMin = 0;
			const LinkMode mode = this->m_mode;
			for (size_t i = 0; i < n; ++i) {
				PTreeItemType p1 = items[i];
				for (size_t j = 0; j < i; ++j) {
					PTreeItemType p2 = items[j];
					double d = 0;
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
					}// dist
				}// j
			}// i
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
		}// aggreg_one
	public:
		IndivsTree() :m_mode(LinkMode::linkInvalid), m_nbclusters(0) {
		}
		virtual ~IndivsTree() {
			this->clear();
		}
	public:
		const treeitems_vector & items(void) const {
			return (this->m_items);
		}
		void get_clusters(clusters_vector &oVec) const {
			oVec.clear();
			const treeitems_vector &v = this->m_items;
			const size_t n = v.size();
			if (n > 0) {
				oVec.resize(n);
				for (size_t i = 0; i < n; ++i) {
					IndivClusterType &c = oVec[i];
					c.id((U)(i + 1));
					const PTreeItemType p = v[i];
					p->get_cluster(c);
					c.update_center();
				}// i
			}// n
		}// get_clusters
		void get_indivs_map(ints_sizet_map &oMap) const {
			oMap.clear();
			const treeitems_vector &v = this->m_items;
			const size_t n = v.size();
			for (size_t i = 0; i < n; ++i) {
				const PTreeItemType p = v[i];
				p->get_map(oMap, (size_t)(i + 1));
			}// i
		}// get_map
		void get_clusters_ids(sizet_intsvector_map &oMap) const {
			oMap.clear();
			const treeitems_vector &v = this->m_items;
			const size_t n = v.size();
			for (size_t i = 0; i < n; ++i) {
				const PTreeItemType p = v[i];
				ints_vector oo;
				p->get_ids(oo);
				size_t key = (size_t)(i + 1);
				oMap[key] = oo;
			}// i
		}// get_clusters_ids
		void process(SourceType *pProvider,
			const size_t nbClusters = 5,
			LinkMode mode = LinkMode::linkMean) {
			assert(pProvider != nullptr);
			assert(nbClusters > 0);
			assert(mode != LinkMode::linkInvalid);
			this->clear();
			this->m_mode = mode;
			this->m_nbclusters = nbClusters;
			size_t nbIndivs = pProvider->count();
			treeitems_vector & items = this->m_items;
			for (size_t i = 0; i < nbIndivs; ++i) {
				IndivTypePtr oInd = pProvider->get(i);
				const IndivType *pInd = oInd.get();
				if ((pInd != nullptr) && pInd->has_numeric_fields()) {
					PTreeItemType p = new TreeItemType(oInd);
					assert(p != nullptr);
					items.push_back(p);
				}// ok
			}// i
			do {
				if (!this->aggreg_one_step()) {
					break;
				}
			} while (true);
		}// process
	};// class IndivsTree
	  /////////////////////////////////////////
}// namespace info
 ////////////////////////////////////////
#endif // !__INDIVTREE_H__
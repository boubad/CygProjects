#pragma once
#ifndef __INDIVTREEITEM_H__
#define __INDIVTREEITEM_H__
/////////////////////////////////////
#include "indivcluster.h"
//////////////////////////////
namespace info {
	////////////////////////////////////
	enum class LinkMode {
		linkInvalid, linkMean, linkMin, linkMax
	};
	///////////////////////////////////
	template<typename U, typename STRINGTYPE, typename DISTANCETYPE>
	class TreeItem {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using DataMap = std::map<U, InfoValue>;
		using TreeItemType = TreeItem<U, STRINGTYPE, DISTANCETYPE>;
		using ints_sizet_map = std::map<U, size_t>;
		using ints_vector = std::vector<U>;
		using IndivClusterType = IndivCluster<U, STRINGTYPE>;
	private:
		TreeItemType *m_pleft;
		TreeItemType *m_pright;
		IndivTypePtr m_indiv;
		DataMap m_center;
	public:
		TreeItem(const IndivTypePtr &oInd) : m_pleft(nullptr), m_pright(nullptr), m_indiv(oInd) {
			IndivType *p = this->m_indiv.get();
			assert(p != nullptr);
			this->m_center = p->center();
		}
		TreeItem(TreeItemType *pLeft, TreeItemType *pRight) : m_pleft(pLeft), m_pright(pRight) {
			assert(pLeft != nullptr);
			assert(pRight != nullptr);
			DataMap &m1 = pLeft->m_center;
			DataMap &m2 = pRight->m_center;
			DataMap &res = this->m_center;
			for (auto &p : m1) {
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
						} // v2
					} // found
				} // v1
			}// p
		} // TreeItem
		virtual ~TreeItem() {
			delete this->m_pleft;
			delete this->m_pright;
		}
	public:
		bool is_leaf(void) const {
			return ((this->m_pleft == nullptr) && (this->m_pright == nullptr));
		}
		const DataMap & center(void) const {
			return (this->m_center);
		}
		DataMap & center(void) {
			return (this->m_center);
		}
		IndivTypePtr indiv(void) const {
			return (this->m_indiv);
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
		} // get_map
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
		} // get_map
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
		} // get_cluster
		bool distance(const TreeItemType &other, DISTANCETYPE &dRes, const LinkMode mode =
			LinkMode::linkMean) const {
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
			DataMap r1 = pRight1->m_center;
			if (other.is_leaf()) {
				switch (mode) {
				case LinkMode::linkMax: {
					DISTANCETYPE d1 = 0, d2 = 0;
					if (!info_global_compute_distance(l1, c2, d1)) {
						return (false);
					}
					if (!info_global_compute_distance(r1, c2, d2)) {
						return (false);
					}
					dRes = (d1 > d2) ? d1 : d2;
					return true;
				}
				case LinkMode::linkMin: {
					DISTANCETYPE d1 = 0, d2 = 0;
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
				} // mode
			} //
			const TreeItemType *pLeft2 = other.m_pleft;
			assert(pLeft2 != nullptr);
			const TreeItemType *pRight2 = other.m_pright;
			assert(pRight2 != nullptr);
			DataMap l2 = pLeft2->m_center;
			DataMap r2 = pRight2->m_center;
			std::vector<DataMap> src({ l1, r1 }), dest({ l2, r2 });
			bool bFirst = true;
			for (auto it = src.begin(); it != src.end(); ++it) {
				DataMap &px1 = *it;
				for (auto jt = dest.begin(); jt != dest.end(); ++jt) {
					DISTANCETYPE d = 0;
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
				} // jt
			} // it
			return (true);
		} // distance
	};
	// class TreeItem<U>
	/////////////////////////////////////////
}// namespace info
 ////////////////////////////////////////
#endif // !__INDIVTREE_H__

//
#include "../include/indivtree.h"
#include "../include/indiv.h"
///////////////////////////////
namespace info {
	//////////////////////////////
	IndivsTree::IndivsTree():m_maxindex(0),m_mode(LinkMode::linkInvalid),m_nbclusters(0) {

	}
	IndivsTree::~IndivsTree() {
		this->clear();
	}
	void IndivsTree::clear(void) {
		treeitems_vector & items = this->m_items;
		for (auto it = items.begin(); it != items.end(); ++it) {
			PTreeItem p = *it;
			delete p;
		}
		items.clear();
	}// clear
	void IndivsTree::get_map(ints_size_t_map &oMap) const {
		const treeitems_vector & items = this->m_items;
		size_t val = 0;
		for (auto it = items.begin(); it != items.end(); ++it) {
			PTreeItem p = *it;
			assert(p != nullptr);
			p->get_leaf_map(oMap, val++);
		}
	}// get_map
	void IndivsTree::get_centers(valuemaps_vector &oCenters) const {
		oCenters.clear();
		const treeitems_vector & items = this->m_items;
		for (auto it = items.begin(); it != items.end(); ++it) {
			PTreeItem p = *it;
			assert(p != nullptr);
			const DbValueMap *pMap = p->m_pcenter;
			assert(pMap != nullptr);
			DbValueMap o(*pMap);
			oCenters.push_back(o);
		}
	}// get_centers
	bool IndivsTree::aggreg_one_step(void) {
		treeitems_vector & items = this->m_items;
		const size_t n = items.size();
		if (n <= this->m_nbclusters) {
			return (false);
		}
		size_t i1 = 0, i2 = 0;
		double distMin = 0;
		const LinkMode mode = this->m_mode;
		for (size_t i = 0; i < n; ++i) {
			PTreeItem p1 = items[i];
			for (size_t j = 0; j < i; ++j) {
				PTreeItem p2 = items[j];
				double d = 0;
				if (p1->distance(*p2, d, mode)) {
					if (i1 == i2) {
						distMin = d;
						i1 = j;
						i2 = i;
					}
					else if (d < distMin) {
						distMin = d;
						i1 = j;
						i2 = i;
					}
				}// dist
			}// j
		}// i
		if (i1 == i2) {
			return (false);
		}
		PTreeItem p1 = items[i1];
		assert(p1 != nullptr);
		const size_t n1 = p1->m_index;
		PTreeItem p2 = items[i2];
		assert(p2 != nullptr);
		const size_t n2 = p2->m_index;
		auto it1 = std::find_if(items.begin(),items.end(),[n1](PTreeItem p)->bool{
			return (p->index() == n1);
		});
		assert(it1 != items.end());
		items.erase(it1);
		auto it2 = std::find_if(items.begin(), items.end(), [n2](PTreeItem p)->bool {
			return (p->index() == n2);
		});
		assert(it2 != items.end());
		items.erase(it2);
		PTreeItem pNew = new TreeItem(this->m_maxindex++, p1, p2);
		assert(pNew != nullptr);
		items.push_back(pNew);
		assert(items.size() == (size_t)(n -1));
		return (true);
	}//aggreg_one_step
	void IndivsTree::process(IIndivProvider *pProvider, const size_t nbClusters /*= 5*/, LinkMode mode /*= LinkMode::linkMean*/) {
		assert(pProvider != nullptr);
		assert(pProvider->is_valid());
		assert(nbClusters > 0);
		assert(mode != LinkMode::linkInvalid);
		this->clear();
		this->m_mode = mode;
		this->m_nbclusters = nbClusters;
		size_t nbIndivs = 0;
		if (!pProvider->indivs_count(nbIndivs)) {
			return;
		}
		size_t nMax = 0;
		treeitems_vector & items = this->m_items;
		for (size_t i = 0; i < nbIndivs; ++i) {
			Indiv oInd;
			if (pProvider->find_indiv_at(i, oInd)) {
				size_t nx = (size_t)oInd.id();
				if (nx > nMax) {
					nMax = nx;
				}
				PTreeItem p = new TreeItem(oInd);
				assert(p != nullptr);
				items.push_back(p);
			}// ok
		}// i
		this->m_maxindex = ++nMax;
		do {
			if (!this->aggreg_one_step()) {
				break;
			}
		} while (true);
	}//process
	/////////////////////////////
	TreeItem::TreeItem(const Indiv &oInd) :m_index(oInd.id()),m_pleft(nullptr), m_pright(nullptr), m_pcenter(nullptr) {
		this->m_pcenter = new DbValueMap(oInd.data());
		assert(this->m_pcenter != nullptr);
	}
	TreeItem::TreeItem(size_t aIndex, TreeItem *pLeft, TreeItem *pRight) :
		m_index(aIndex),m_pleft(pLeft), m_pright(pRight), m_pcenter(nullptr) {
		this->update_center();
	}
	TreeItem::~TreeItem() {
		delete m_pleft;
		delete m_pcenter;
		delete m_pright;
	}
	bool TreeItem::is_leaf(void) const {
		return ((this->m_pleft == nullptr) && (this->m_pright == nullptr));
	}
	void TreeItem::update_center(void) {
		TreeItem *pLeft = this->m_pleft;
		TreeItem *pRight = this->m_pright;
		if ((pLeft == nullptr) && (pRight == nullptr)) {
			return;
		}
		else if ((pLeft == nullptr) && (pRight != nullptr)) {
			DbValueMap *pMap = pRight->m_pcenter;
			if (pMap != nullptr) {
				this->m_pcenter = new DbValueMap(*pMap);
			}
			return;
		}
		else if ((pLeft != nullptr) && (pRight == nullptr)) {
			DbValueMap *pMap = pLeft->m_pcenter;
			if (pMap != nullptr) {
				this->m_pcenter = new DbValueMap(*pMap);
			}
			return;
		}
		else {
			DbValueMap *pMap1 = pLeft->m_pcenter;
			DbValueMap *pMap2 = pRight->m_pcenter;
			if ((pMap1 == nullptr) && (pMap2 == nullptr)) {
				return;
			}
			else if ((pMap1 == nullptr) && (pMap2 != nullptr)) {
				this->m_pcenter = new DbValueMap(*pMap2);
			}
			else if ((pMap1 != nullptr) && (pMap2 == nullptr)) {
				this->m_pcenter = new DbValueMap(*pMap1);
			}
			else {
				this->m_pcenter = new DbValueMap();
				assert(this->m_pcenter != nullptr);
				DbValueMap &oRes = *(this->m_pcenter);
				std::for_each(pMap1->begin(), pMap1->end(), [&](const std::pair<IntType, DbValue> &p) {
					const DbValue &v1 = p.second;
					if (!v1.empty()) {
						const IntType key = p.first;
						auto it = pMap2->find(key);
						if (it != pMap2->end()) {
							const DbValue &v2 = (*it).second;
							if (!v2.empty()) {
								oRes[key] = DbValue((v1.double_value() + v2.double_value()) / 2.0);
							}// v2
						}
					}// v1
				});
			}
		}
	}// update_center
	void TreeItem::get_leaf_indexes(ints_vector &ovec) const {
		if (this->is_leaf()) {
			ovec.push_back((IntType)this->m_index);
			return;
		}
		if (this->m_pleft != nullptr) {
			this->m_pleft->get_leaf_indexes(ovec);
		}
		if (this->m_pright != nullptr) {
			this->m_pright->get_leaf_indexes(ovec);
		}
	}//get_leaf_indexes
	void TreeItem::get_leaf_map(ints_size_t_map &oMap, const size_t val) const {
		if (this->is_leaf()) {
			IntType aIndex = (IntType)this->m_index;
			oMap[aIndex] = val;
			return;
		}
		if (this->m_pleft != nullptr) {
			this->m_pleft->get_leaf_map(oMap, val);
		}
		if (this->m_pright != nullptr) {
			this->m_pright->get_leaf_map(oMap, val);
		}
	}// get_leaf_map
	bool TreeItem::distance(const TreeItem &other, double &dRes, const LinkMode mode /*= LinkMode::linkMean*/) const {
		typedef const DbValueMap *PDbValueMap;
		PDbValueMap p1 = this->m_pcenter;
		assert(p1 != nullptr);
		PDbValueMap p2 = other.m_pcenter;
		assert(p2 != nullptr);
		dRes = 0;
		if (mode == LinkMode::linkMean) {
			return info_global_compute_distances(*p1, *p2, dRes);
		}
		if (this->is_leaf() && other.is_leaf()) {
			return info_global_compute_distances(*p1, *p2, dRes);
		}
		if (this->is_leaf() && (!other.is_leaf())) {
			return other.distance(*this, dRes, mode);
		}
		const TreeItem *pLeft1 = this->m_pleft;
		assert(pLeft1 != nullptr);
		const TreeItem *pRight1 = this->m_pright;
		assert(pRight1 != nullptr);
		PDbValueMap pp1 = pLeft1->m_pcenter;
		assert(pp1 != nullptr);
		PDbValueMap pp2 = pRight1->m_pcenter;
		assert(pp2 != nullptr);
		if (other.is_leaf()) {
			switch (mode) {
			case LinkMode::linkMax:
			{
				double d1 = 0, d2 = 0;
				if (!info_global_compute_distances(*pp1, *p2, d1)) {
					return (false);
				}
				if (!info_global_compute_distances(*pp2, *p2, d2)) {
					return (false);
				}
				dRes = (d1 > d2) ? d1 : d2;
				return true;
			}
			case LinkMode::linkMin:
			{
				double d1 = 0, d2 = 0;
				if (!info_global_compute_distances(*pp1, *p2, d1)) {
					return (false);
				}
				if (!info_global_compute_distances(*pp2, *p2, d2)) {
					return (false);
				}
				dRes = (d1 < d2) ? d1 : d2;
				return true;
			}
			default:
				return (false);
			}// mode
		}//
		const TreeItem *pLeft2 = other.m_pleft;
		assert(pLeft2 != nullptr);
		const TreeItem *pRight2 = other.m_pright;
		assert(pRight2 != nullptr);
		PDbValueMap mm1 = pLeft2->m_pcenter;
		assert(mm1 != nullptr);
		PDbValueMap mm2 = pRight2->m_pcenter;
		assert(mm2 != nullptr);
		std::vector<PDbValueMap> src(2), dest(2);
		src[0] = pp1;
		src[1] = pp2;
		dest[0] = mm1;
		dest[1] = mm2;
		bool bFirst = true;
		for (auto it = src.begin(); it != src.end(); ++it) {
			PDbValueMap px1 = *it;
			for (auto jt = dest.begin(); jt != dest.end(); ++jt) {
				double d = 0;
				PDbValueMap px2 = *jt;
				if (!info_global_compute_distances(*px1, *px2, d)) {
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
	}//distance
	////////////////////////////
}// namespace info

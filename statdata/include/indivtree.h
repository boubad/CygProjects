#pragma once
#ifndef __INDIVTREE_H__
#define __INDIVTREE_H__
/////////////////////////////////////
#include "dbvalue.h"
//////////////////////////////////////////
namespace info {
	////////////////////////////////////
	class Indiv;
	class IndivsTree;
	class IIndivProvider;
	///////////////////////////////////////
	enum class LinkMode {linkInvalid, linkMean, linkMin, linkMax};
	///////////////////////////////////
	class TreeItem : private boost::noncopyable {
		friend class IndivsTree;
	private:
		size_t m_index;
		TreeItem *m_pleft;
		TreeItem *m_pright;
		DbValueMap *m_pcenter;
		//
		void update_center(void);
	protected:
		TreeItem(const Indiv &oInd);
		TreeItem(size_t aIndex,TreeItem *pLeft, TreeItem *pRight);
		void get_leaf_map(ints_size_t_map &oMap, const size_t val) const;
		size_t index(void) const {
			return (this->m_index);
		}
		bool is_leaf(void) const;
		void get_leaf_indexes(ints_vector &ovec) const;
		bool distance(const TreeItem &other, double &dRes, const LinkMode mode = LinkMode::linkMean) const;
	public:
		~TreeItem();
	};// class TreeItem
	/////////////////////////////////////
	typedef TreeItem *PTreeItem;
	typedef std::vector<PTreeItem> treeitems_vector;
	////////////////////////////////////////
	class IndivsTree : private boost::noncopyable  {
	public:
		typedef std::vector<DbValueMap> valuemaps_vector;
	private:
		size_t m_maxindex;
		LinkMode m_mode;
		size_t m_nbclusters;
		treeitems_vector m_items;
	private:
		void clear(void);
		bool aggreg_one_step(void);
	public:
		IndivsTree();
		~IndivsTree();
	public:
		void process(IIndivProvider *pProvider, const size_t nbClusters = 5, LinkMode mode = LinkMode::linkMean);
		void get_map(ints_size_t_map &oMap) const;
		void get_centers(valuemaps_vector &oCenters) const;
	};// class IndivsTree
	/////////////////////////////////////////
}// namespace info
////////////////////////////////////////
#endif // !__INDIVTREE_H__
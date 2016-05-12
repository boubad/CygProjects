/*
 * indivcluster.h
 *
 *  Created on: 9 mai 2016
 *      Author: boubad
 */

#ifndef INDIVCLUSTER_H_
#define INDIVCLUSTER_H_
///////////////////////////////
#include "indiv.h"
#include "crititem.h"
//////////////////////////////
namespace info {
////////////////////////////////
enum class ClusterAppendMode
	: short {modeInvalid,
	modeTopTop,
	modeTopBottom,
	modeBottomTop,
	modeBottomBottom
};
enum class ClusterDistanceMode
	: short {modeInvalid, modeTop, modeBottom
};
////////////////////////////
class IndivCluster {
private:
	bool m_mustdelete;
	size_t m_index;
	IIndivProvider *m_provider;
	IndivDistanceMap *m_pdist;
	ints_deque m_individs;
	DbValueMap m_center;
public:
	IndivCluster();
	IndivCluster(IndivDistanceMap *pDist, const size_t aIndex);
	IndivCluster(IIndivProvider *pProvider, const size_t aIndex);
	IndivCluster(const IndivCluster &other);
	IndivCluster & operator=(const IndivCluster &other);
	~IndivCluster();
public:
	IIndivProvider *provider(void) const;
	void provider(IIndivProvider *pProvider);
	void index(const size_t n);
	size_t index(void) const;
	const ints_deque & members(void) const;
	const DbValueMap &center(void) const;
public:
	void get_map(ints_size_t_map &oMap) const;
	bool is_valid(void) const;
	bool is_empty(void) const;
	double distance(const Indiv &oInd) const;
	void add(const Indiv &oInd);
	bool min_distance(const IndivCluster &other, double &dRes,
			ClusterAppendMode &mode) const;
	void add(const IndivCluster &other, const ClusterAppendMode mode);
	void clear_members(void);
	void update_center(void);
	void swap(IndivCluster &other);
protected:
	double distance(const Indiv &oInd, ClusterDistanceMode &mode);
	double distance(const IntType aIndex, ClusterDistanceMode &mode);
	bool get_distance(const IntType aIndex1, const IntType &aIndex2,
			double &dRes)const;
};
// class IndivCluster
////////////////////////////////
typedef std::vector<IndivCluster> indivclusters_vector;
/////////////////////////////
extern size_t info_global_clusterize_kmeans(IIndivProvider *pProvider,
		const size_t nbClusters,indivclusters_vector &Res,
		const size_t nbMaxIterations = 100);
extern void info_global_add_clusterize_result(const indivclusters_vector &oClusters, InfoCritItems &oInfos);
/////////////////////////////
}// namespace info
/////////////////////////////////
inline void swap(info::IndivCluster &v1, info::IndivCluster &v2) {
	v1.swap(v2);
}
//////////////////////////////
#endif /* INDIVCLUSTER_H_ */

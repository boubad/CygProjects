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
//////////////////////////////
namespace info {
////////////////////////////////
enum class ClusterAppendMode : short { modeInvalid,
	modeTopTop, modeTopBottom,modeBottomTop, modeBottomBottom};
enum class ClusterDistanceMode : short {modeInvalid,modeTop,modeBottom};
////////////////////////////
class IndivCluster {
private:
	size_t	   m_index;
	IIndivProvider *m_provider;
	ints_deque m_individs;
	DbValueMap m_center;
public:
	IndivCluster(IIndivProvider *pProvider = nullptr, const size_t aIndex = 0);
	IndivCluster(const IndivCluster &other);
	IndivCluster & operator=(const IndivCluster &other);
	~IndivCluster();
public:
	IIndivProvider *provider(void) const;
	void provider(IIndivProvider *pProvider);
	void index(const size_t n);
	size_t  index(void) const;
	const ints_deque & members(void) const;
	const DbValueMap &center(void) const;
public:
	bool is_valid(void) const;
	bool is_empty(void) const;
	double distance(const Indiv &oInd) const;
	void add(const Indiv &oInd);
	double min_distance(const IndivCluster &other, ClusterAppendMode &mode);
	void add(const IndivCluster &other, const ClusterAppendMode mode);
	void clear_members(void);
	void update_center(void);
protected:
	double distance(const Indiv &oInd, ClusterDistanceMode &mode);
	double distance(const IntType aIndex, ClusterDistanceMode &mode);
}; // class IndivCluster
/////////////////////////////
}// namespace info
//////////////////////////////
#endif /* INDIVCLUSTER_H_ */

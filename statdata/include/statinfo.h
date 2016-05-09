/*
 * statinfo.h
 *
 *  Created on: 8 mai 2016
 *      Author: boubad
 */

#ifndef STATINFO_H_
#define STATINFO_H_
//////////////////////////////////////
#include "info_constants.h"
#include "dbvalue.h"
//////////////////////////////////////
#include <boost/container/flat_set.hpp>
#include <boost/noncopyable.hpp>
/////////////////////////////////////
#if defined(__CYGWIN__)
#include <mutex>
#else
#include <boost/thread/mutex.hpp>
#endif //__CYGWIN__
////////////////////////////////////
namespace info {
class IIndivProvider;
////////////////////////////////////
class StatInfo {
private:
	IntType index;
	size_t count;
	double min;
	double max;
	double mean;
	double median;
	double variance;
	double deviation;
	double somme1;
	double somme2;
public:
	StatInfo(IntType aIndex = 0);
	StatInfo(const StatInfo &other);
	StatInfo & operator=(const StatInfo &other);
	~StatInfo();
	bool operator==(const StatInfo &other) const;
	bool operator<(const StatInfo &other) const;
	IntType operator()(void) const;
	void swap(StatInfo &other);
	void add(const double v);
	void compute(void);
	//
	IntType key(void) const;
	void reset(void);
	void get_count(size_t &nCount);
	void get_mean_var_std(double &vm, double &vv, double &vs);
	void get_min_max(double &vmin, double &vmax);
};
// class StatInfo
////////////////////////////////////////
typedef boost::container::flat_set<StatInfo> statinfos_set;
typedef boost::container::flat_map<IntType, StatInfo>  statinfos_map;
//////////////////////////////////////////////////
class StatInfoMap : private boost::noncopyable{
private:
	statinfos_map m_map;
#if defined(__CYGWIN__)
	std::mutex _mutex;
#else
	boost::mutex _mutex;
#endif
public:
	StatInfoMap();
	~StatInfoMap();
public:
	const statinfos_map &map(void) const;
	void add(const DbValueMap &oMap);
};// class StatInfoMap
////////////////////////////////////
extern size_t info_global_compute_stats(IIndivProvider *pProvider, statinfos_map &oRes);
/////////////////////////////////////////
}// namespace info
///////////////////////////////////////
inline void swap(info::StatInfo &v1, info::StatInfo &v2) {
	v1.swap(v2);
} // swap
//////////////////////////
#endif /* STATINFO_H_ */

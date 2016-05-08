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
//////////////////////////////////////
#include <boost/container/flat_set.hpp>
////////////////////////////////////
namespace info {
class IIndivProvider;
////////////////////////////////////
class StatInfo {
public:
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
	StatInfo() :
			index(0), count(0), min(0), max(0), mean(0), median(0), variance(0), deviation(0),somme1(0),somme2(0) {
	}
	StatInfo(const StatInfo &other) :
			index(other.index), count(other.count), min(other.min), max(
					other.max), mean(other.min), median(other.median), variance(
					other.variance), deviation(other.deviation),somme1(other.somme1),somme2(other.somme2) {
	}
	StatInfo & operator=(const StatInfo &other) {
		if (this != &other) {
			this->index = other.index;
			this->count = other.count;
			this->min = other.min;
			this->max = other.max;
			this->mean = other.mean;
			this->median = other.median;
			this->variance = other.variance;
			this->deviation = other.deviation;
			this->somme1 = other.somme1;
			this->somme2 = other.somme2;
		}
		return (*this);
	}
	~StatInfo() {
	}
	bool operator==(const StatInfo &other) const {
		return (this->index == other.index);
	}
	bool operator<(const StatInfo &other) const {
		return (this->index == other.index);
	}
	IntType operator()(void) const {
		return (this->index);
	}
	void swap(StatInfo &other) {
		StatInfo t(*this);
		*this = other;
		other = t;
	} // swap
};
// class StatInfo
////////////////////////////////////////
typedef boost::container::flat_set<StatInfo> statinfos_set;
typedef boost::container::flat_map<IntType, StatInfo>  statinfos_map;
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

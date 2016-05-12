/*
 * crititem.h
 *
 *  Created on: 9 mai 2016
 *      Author: boubad
 */

#ifndef CRITITEM_H_
#define CRITITEM_H_
//////////////////////////////
#include "info_constants.h"
//////////////////////////////////
namespace info {
//////////////////////////////
class IIndivProvider;
//////////////////////////////////
class CritItem {
private:
	size_t m_first;
	size_t m_second;
	double m_crit;
public:
	CritItem();
	explicit CritItem(const size_t i1, const size_t i2, const double v = 0);
	CritItem(const CritItem & other);
	CritItem & operator=(const CritItem &other);
	~CritItem();
public:
	double value(void) const;
	void value(const double d);
	void get(std::pair<size_t,size_t> &oPair) const;
	void get(size_t &i1, size_t &i2, double &v) const;
	bool is_valid(void) const;
	bool operator==(const CritItem &other) const;
	bool operator<(const CritItem &other) const;
	CritItem & operator+=(const CritItem &other);
	const CritItem operator+(const CritItem &other) const;
	void swap(CritItem &other);
};
// class CritItem
typedef std::vector<CritItem> crititems_vector;
extern size_t info_global_generate_crititems(const size_t n,crititems_vector &oRes);
///////////////////////////////////
class IndivDistanceMap : private boost::noncopyable {
public:
	typedef std::map<IntType, double> intdoubles_map;
	typedef std::map<IntType, intdoubles_map> intdoubles_map_map;
private:
	ints_set m_set;
	intdoubles_map_map m_data;
public:
	IndivDistanceMap();
	~IndivDistanceMap();
public:
	void clear(void);
	const ints_set & indexes(void) const;
	void add(const IntType i1, const IntType i2, const double v);
	void add(const CritItem &item);
	bool get(const IntType i1, const IntType i2, double &v) const;
	bool get(CritItem &item) const;
}; // class IndivDistanceMap
///////////////////////////////////////////
typedef IndivDistanceMap	InfoCritItems;
///////////////////////////////////////
extern void info_global_compute_distances(IIndivProvider *pProvider,IndivDistanceMap &oDistances);
//////////////////////////////////////
}// namespace info
/////////////////////////////////
inline void swap(info::CritItem &v1, info::CritItem &v2) {
	v1.swap(v2);
}
/////////////////////////////
#endif /* CRITITEM_H_ */

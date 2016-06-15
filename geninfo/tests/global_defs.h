/*
 * global_defs.h
 *
 *  Created on: 27 mai 2016
 *      Author: boubad
 */

#ifndef GLOBAL_DEFS_H_
#define GLOBAL_DEFS_H_
//////////////////////////////
#include <vector>
#include <distancemap.h>
///////////////////////////////
using DATATYPE = int;
using IDTYPE = unsigned long;
using INTTYPE = unsigned long;
using WEIGHTYPE = double;
using DISTANCETYPE = unsigned long;
using FLOATTYPE = double;
//
using STRINGSTREAM = std::stringstream;
using STRINGTYPE = std::string;
//////////////////////////////////
template<typename T>
void write_vector(const std::vector<T> &data, std::string &s) {
	std::stringstream os;
	os << "[";
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it != data.begin()) {
			os << ", ";
		}
		os << *it;
	} // it
	os << "]";
	s = os.str();
} // writeçvector
template<typename T>
void write_vector(const std::vector<T> &data, std::wstring &s) {
	std::wstringstream os;
	os << L"[";
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it != data.begin()) {
			os << L", ";
		}
		os << *it;
	} // it
	os << L"]";
	s = os.str();
} // writeçvector
template <typename U, typename W>
void write_distancemap(const info::DistanceMap<U, W> &oDist, std::string &s) {
	std::stringstream os;
	std::vector<U> ids;
	oDist.indexes(ids);
	std::string ss;
	write_vector(ids, ss);
	os << ss << std::endl;
	for (auto it = ids.begin(); it != ids.end(); ++it) {
		U aIndex1 = *it;
		for (auto jt = ids.begin(); jt != it; ++jt) {
			if (jt != ids.begin()) {
				os << ", ";
			}
			W val = 0;
			U aIndex2 = *jt;
			oDist.get(aIndex1, aIndex2, val);
			os << "(" << aIndex1 << "," << aIndex2 << "," << val << ")";
		}// jt
		os << std::endl;
	}// it
	s = os.str();
}//write_distancemap
template <typename U, typename W>
void write_distancemap(const info::DistanceMap<U, W> &oDist, std::wstring &s) {
	std::wstringstream os;
	std::vector<U> ids;
	oDist.indexes(ids);
	std::wstring ss;
	write_vector(ids, ss);
	os << ss << std::endl;
	for (auto it = ids.begin(); it != ids.end(); ++it) {
		U aIndex1 = *it;
		for (auto jt = ids.begin(); jt != it; ++jt) {
			if (jt != ids.begin()) {
				os << L", ";
			}
			W val = 0;
			U aIndex2 = *jt;
			oDist.get(aIndex1, aIndex2, val);
			os << L"(" << aIndex1 << L"," << aIndex2 << L"," << val << L")";
		}// jt
		os << std::endl;
	}// it
	s = os.str();
}//write_distancemap
/////////////////////////////////
#endif /* GLOBAL_DEFS_H_ */

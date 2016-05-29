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
///////////////////////////////
using DATATYPE = int;
using IDTYPE = unsigned long;
using INTTYPE = unsigned long;
using STRINGTYPE = std::string;
using WEIGHTYPE = double;
using DISTANCETYPE = long;
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
/////////////////////////////////
#endif /* GLOBAL_DEFS_H_ */

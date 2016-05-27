#pragma once
/*
 * info_global.h
 *
 *  Created on: 21 mai 2016
 *      Author: boubad
 */
#ifndef INFO_GLOBAL_H_
#define INFO_GLOBAL_H_
 ///////////////////////////////
#include <cassert>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <atomic>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
/////////////////////////////////
#include "infovalue.h"
//////////////////////////////
#include <boost/noncopyable.hpp>
////////////////////////////////////
namespace info {
	///////////////////////////////////
	template<typename Iterator, typename Func>
	void info_parallel_for_each(Iterator first, Iterator last, Func &&f, ptrdiff_t range = 16)
	{
		const ptrdiff_t range_length = last - first;
		if (range_length <= range) {
			for (auto it = first; it != last; ++it) {
				f(*it);
			}// ir
			return;
		}
		const Iterator mid = first + (range_length / 2);
		std::future<void> bgtask = std::async(&info_parallel_for_each<Iterator, Func>, first, mid, f,range);
		try
		{
			info_parallel_for_each(mid, last, f,range);
		}
		catch (...)
		{
			bgtask.wait();
			throw;
		}
		bgtask.get();
	}// info_parallel_for_each
	//////////////////////////////////
	template<typename U>
	void info_global_write_map(const std::map<U, info::InfoValue> &oMap,
		std::string &ss) {
		std::stringstream os;
		for (auto it = oMap.begin(); it != oMap.end(); ++it) {
			if (it != oMap.begin()) {
				os << ", ";
			}
			std::string s;
			U key = (*it).first;
			const info::InfoValue &v = (*it).second;
			v.get_value(s);
			os << "(" << key << "," << s << ")";
		} // it
		ss = os.str();
	} //info_global_write_map
	template<typename U>
	void info_global_write_map(const std::map<U, info::InfoValue> &oMap,
		std::wstring &ss) {
		std::wstringstream os;
		for (auto it = oMap.begin(); it != oMap.end(); ++it) {
			if (it != oMap.begin()) {
				os << L", ";
			}
			std::wstring s;
			U key = (*it).first;
			const info::InfoValue &v = (*it).second;
			v.get_value(s);
			os << L"(" << key << L"," << s << L")";
		} // it
		ss = os.str();
	} //info_global_write_map
	//////////////////////////////////
	template<typename U1, typename U2, typename W>
	bool info_global_compute_distance(const std::map<U1, InfoValue> &oMap1,
		const std::map<U2, InfoValue> &oMap2, W &res,
		std::atomic_bool *pCancel = nullptr) {
		size_t nc = 0;
		W dRes = 0;
		for (auto & p : oMap1) {
			if ((pCancel != nullptr) && pCancel->load()) {
				return (false);
			}
			const InfoValue &vv1 = p.second;
			if ((!vv1.empty()) && vv1.is_numerical()) {
				const U2 key = (U2)p.first;
				auto it = oMap2.find(key);
				if (it != oMap2.end()) {
					const InfoValue &vv2 = (*it).second;
					if ((!vv2.empty()) && vv2.is_numerical()) {
						W v1, v2;
						vv1.get_value(v1);
						vv2.get_value(v2);
						const W tt = (W)(v1 - v2);
						dRes = (W)(dRes + (tt * tt));
						++nc;
					} // vv2
				} // it
			} // vv1
		}// p
		if (nc > 1) {
			dRes = (W)(dRes / nc);
		}
		res = dRes;
		return (nc > 0);
	} //info_global_compute_distance
	//////////////////////////////////////////
} // namespace info
////////////////////////////////
#endif /* INFO_GLOBAL_H_ */

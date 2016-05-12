#pragma once
#ifndef __INFO_CONSTANTS_H__
#define  __INFO_CONSTANTS_H__
//////////////////////////////
#define DATATRANSFER_CHUNK_SIZE (100)
////////////////////////////////////////////
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <list>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <mutex>
////////////////////////////////////////////
#include "boost_inc.h"
/////////////////////////////////////
#if defined(USE_DLIB)
#include "dlib_inc.h"
#endif // USE_DLIB
////////////////////////////////////
namespace info {
	typedef unsigned long IntType;
	//////////////////////////////////////////////////////
	enum class VariableMode { modeInvalid, modeNumeric, modeNominal, modeAll };
	////////////////////////////////////////////////////
	typedef std::list<IntType> ints_list;
	typedef std::vector<IntType> ints_vector;
	typedef std::deque<IntType> ints_deque;
	typedef std::set<IntType> ints_set;
	typedef std::vector<double> doubles_vector;
	typedef std::map<IntType, std::string> inttype_string_map;
	typedef std::vector<std::string> strings_vector;
	typedef std::map<IntType, size_t> ints_size_t_map;
	///////////////////////////////////////////////////////////////
#if defined(USE_DLIB)
	typedef dlib::mutex info_mutex;
	typedef dlib::auto_mutex  info_lock;
	typedef dlib::read_write_mutex info_read_write_mutex;
	typedef dlib::auto_unlock_readonly info_read_lock;
	typedef dlib::auto_unlock info_write_lock;
#else
	typedef std::mutex info_mutex;
	typedef std::lock_guard<std::mutex>  info_lock;
	typedef std::mutex info_read_write_mutex;
	typedef std::lock_guard<std::mutex> info_read_lock;
	typedef std::lock_guard<std::mutex> info_write_lock;
#endif // USE_DLIB
}// namspace info
//////////////////////////////
#endif // !__INFO_CONSTANTS_H__

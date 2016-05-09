#pragma once
#ifndef __INFO_CONSTANTS_H__
#define  __INFO_CONSTANTS_H__
//////////////////////////////
#define INFO_NUM_THREADS  (2)
#define DATATRANSFER_CHUNK_SIZE (100)
////////////////////////////////////////////
#include <boost/container/list.hpp>
#include <boost/container/deque.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
////////////////////////////////////
namespace info {
	typedef unsigned long IntType;
	//////////////////////////////////////////////////////
	enum class VariableMode { modeInvalid, modeNumeric, modeNominal, modeAll };
	////////////////////////////////////////////////////
	typedef boost::container::list<IntType> ints_list;
	typedef boost::container::vector<IntType> ints_vector;
	typedef boost::container::deque<IntType> ints_deque;
	typedef boost::container::flat_set<IntType> ints_set;
	typedef boost::container::vector<double> doubles_vector;
	typedef boost::container::flat_map<IntType, std::string> inttype_string_map;
	typedef boost::container::vector<std::string> strings_vector;
	///////////////////////////////////////////////////////////////
}// namspace info
//////////////////////////////
#endif // !__INFO_CONSTANTS_H__

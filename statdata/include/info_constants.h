#pragma once
#ifndef __INFO_CONSTANTS_H__
#define  __INFO_CONSTANTS_H__
//////////////////////////////
#define INFO_NUM_THREADS  (2)
#define DATATRANSFER_CHUNK_SIZE (100)
////////////////////////////////////////////
#include <boost/container/vector.hpp>
#include <boost/container/flat_map.hpp>
////////////////////////////////////
namespace info {
	typedef unsigned long IntType;
	//////////////////////////////////////////////////////
	enum class VariableMode { modeInvalid, modeNumeric, modeNominal, modeAll };
	////////////////////////////////////////////////////
	typedef boost::container::vector<IntType> ints_vector;
	typedef boost::container::flat_map<IntType, std::string> inttype_string_map;
	typedef boost::container::vector<std::string> strings_vector;
	///////////////////////////////////////////////////////////////
}// namspace info
//////////////////////////////
#endif // !__INFO_CONSTANTS_H__

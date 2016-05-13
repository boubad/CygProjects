#pragma once
#ifndef __INFOTESTDATA_H__
#define __INFOTESTDATA_H__
///////////////////////////////////
#include <memory>
#include <vector>
//////////////////////////////////////
namespace info {
	////////////////////////////////
	class InfoTestData
	{
	public:
		static void get_test_indiv_data(std::string &name, size_t &nRows, size_t &nCols);
		static void get_mortal_name(std::string &name);
		static void get_conso_name(std::string &name);
		static void get_default_type_genre_status(std::string &type, std::string &genre, std::string &status);
		static void get_database_filename(std::string &filename);
		static void get_conso_data(std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_mortal_data(std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_data(const std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_data_names(std::vector<std::string> &onames);
	};// classInfoTestData
	  ////////////////////////////////////////
}// namespace info
 ///////////////////////////////////
#endif // !__INFOTESTDATA_H__

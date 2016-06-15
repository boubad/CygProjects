#pragma once
#ifndef __INFOTESTDATA_H__
#define __INFOTESTDATA_H__
///////////////////////////////////
#include <string>
#include <vector>
//////////////////////////////////////
namespace info {
	////////////////////////////////
	class InfoTestData
	{
	public:
		static void get_test_indiv_data(std::string &name, size_t &nRows, size_t &nCols);
		static void get_mortal_name(std::string &name);
		static void get_mortal_name(std::string &name,size_t &nRows, size_t &nCols);
		static void get_conso_name(std::string &name);
		static void get_default_type_genre_status(std::string &type, std::string &genre, std::string &status);
		static void get_database_filename(std::string &filename);
		static void get_conso_data(std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_mortal_data(std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_test_data(std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_data(const std::string &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames);
		static void get_data_names(std::vector<std::string> &onames);
		static void get_test_name(std::string &name);
	//
		static void get_test_indiv_data(std::wstring &name, size_t &nRows, size_t &nCols);
		static void get_mortal_name(std::wstring &name);
		static void get_mortal_name(std::wstring &name, size_t &nRows, size_t &nCols);
		static void get_conso_name(std::wstring &name);
		static void get_default_type_genre_status(std::wstring &type, std::wstring &genre, std::wstring &status);
		static void get_database_filename(std::wstring &filename);
		static void get_conso_data(std::wstring &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::wstring> &indNames, std::vector<std::wstring> &varNames);
		static void get_mortal_data(std::wstring &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::wstring> &indNames, std::vector<std::wstring> &varNames);
		static void get_test_data(std::wstring &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::wstring> &indNames, std::vector<std::wstring> &varNames);
		static void get_data(const std::wstring &name, size_t &nRows, size_t &nCols,
			std::vector<int> &data, std::vector<std::wstring> &indNames, std::vector<std::wstring> &varNames);
		static void get_data_names(std::vector<std::wstring> &onames);
		static void get_test_name(std::wstring &name);
	};// classInfoTestData
	  ////////////////////////////////////////
}// namespace info
 ///////////////////////////////////
#endif // !__INFOTESTDATA_H__

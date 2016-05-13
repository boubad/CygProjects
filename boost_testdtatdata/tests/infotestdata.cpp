#include "infotestdata.h"
////////////////////////////////
#include <cassert>
#include <map>
#include <iostream>
#include <sstream>
////////////////////////////////////
namespace info {
	///////////////////////////////////
	static const size_t st_conso_cols = 7;
	static const size_t st_conso_rows = 12;
	static const int st_conso_data[] = {
		332,428,354,1437,526,247,437,
		293,559,388,1527,567,239,258,
		372,767,562,1948,927,235,433,
		406,563,341,1507,544,324,407,
		386,608,396,1501,558,319,363,
		438,843,689,2345,1148,243,341,
		534,660,367,1620,638,414,407,
		460,699,484,1856,762,400,416,
		385,789,621,2366,1149,304,282,
		655,776,423,1848,759,495,486,
		584,995,548,2056,893,518,319,
		515,1097,887,2630,1187,561,284
	};
	static const size_t st_mortal_cols = 6;
	static const size_t st_mortal_rows = 19;
	static const int st_mortal_data[] = { 241,16,330,43,363,325,
		156, 9, 225,10,535,328,
		85,19,349,7,281,345,
		210,12,230,21,298,269,
		156,10,260,13,367,144,
		251, 26,180,29,387,55,
		194,11,151,13,384,122,
		225, 9, 195,26,276,128,
		54,11,219,19,224,319,
		40,136,215,18,320,43,
		241,6,168,11,230,107,
		101,5,179,23,380,9,
		82,15,155,18,342,59,
		40,4,136,17,237,225,
		104,6,138,22,346,41,
		38,7,182,32,314,37,
		89,7,169,10,218,47,
		79,10,130,14,203,36,
		121,102,220,26,273,158
	};
	////////////////////////////////////
	static const char *st_type = "float";
	static const char *st_genre = "I";
	static const char *st_status = "OK";
	static const char *st_database_filename = "info_test.sqlite";
	static const char *st_conso_name = "conso_data";
	static const char *st_conso_vars[] = { "PAIN","LEGUMES","FRUITS","VIANDES","VOLAILLES","LAIT","VIN" };
	static const char *st_conso_inds[] = { "MA2","EM2","CA2","MA3","EM3",
		"CA3","MA4","EM4","CA4","MA5","EM5","CA5" };
	///////////////////////////////////
	static const char *st_mortal_name = "mortal_data";
	static const char *st_mortal_vars[] = { "SUICI","HOMIC","AROUT","AINDU","AAUTR","CIRFO" };
	static const char *st_mortal_inds[] = { "AUTRICHE","FRANCE","PORTUGAL","ALLFEDER","BELGIQUE",
		"FINLANDE","SUEDE","SUISSE","ITALIE","IRLNORD",
		"DANNEMARK","ISLANDE","ECOSSE","ESPAGNE","NORVEGE",
		"IRLSUD","PABAS","ANGGALLE","USA" };
	///////////////////////////////////
	static const size_t TEST_NROWS = 2048;
	static const size_t TEST_NCOLS = 20;
	static const char *st_test_name = "info_test";
	///////////////////////////////////////
	void InfoTestData::get_test_indiv_data(std::string &name, size_t &nRows, size_t &nCols) {
		name = st_test_name;
		nRows = TEST_NROWS;
		nCols = TEST_NCOLS;
	}
	 ///////////////////////////////////////
	void InfoTestData::get_data(const std::string &name, size_t &nRows, size_t &nCols,
		std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames) {
		nRows = 0;
		nCols = 0;
		data.clear();
		indNames.clear();
		varNames.clear();
		if (name == st_mortal_name) {
			std::string s;
			get_mortal_data(s, nRows, nCols, data, indNames, varNames);
		}
		else if (name == st_conso_name) {
			std::string s;
			get_mortal_data(s, nRows, nCols, data, indNames, varNames);
		}
	}// get_data
	void InfoTestData::get_data_names(std::vector<std::string> &onames) {
		onames.clear();
		onames.push_back(st_conso_name);
		onames.push_back(st_mortal_name);
	}// get_data_names
	void InfoTestData::get_mortal_name(std::string &name) {
		name = st_mortal_name;
	}
	void InfoTestData::get_conso_name(std::string &name) {
		name = st_conso_name;
	}
	void InfoTestData::get_default_type_genre_status(std::string &type, std::string &genre, std::string &status) {
		type = st_type;
		genre = st_genre;
		status = st_status;
	}//get_default_type_genre_status
	void InfoTestData::get_database_filename(std::string &filename) {
		filename = st_database_filename;
	}// get_database_filename
	 ///////////////////////////////////////
	void InfoTestData::get_mortal_data(std::string &name, size_t &nRows, size_t &nCols,
		std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames) {
		name = st_mortal_name;
		nRows = st_mortal_rows;
		assert(nRows > 1);
		nCols = st_mortal_cols;
		assert(nCols > 1);
		const size_t n = (size_t)(nCols * nRows);
		data.resize(n);
		assert(data.size() == n);
		indNames.resize(nRows);
		assert(indNames.size() == nRows);
		varNames.resize(nCols);
		assert(varNames.size() == nCols);
		const int *pData = &(st_mortal_data[0]);
		for (size_t i = 0; i < nCols; ++i) {
			varNames[i] = st_mortal_vars[i];
		}// i
		for (size_t i = 0; i < nRows; ++i) {
			indNames[i] = st_mortal_inds[i];
			for (size_t j = 0; j < nCols; ++j) {
				const size_t pos = (size_t)(i * nCols + j);
				data[pos] = pData[pos];
			}// j
		}// i
	}//get_conso_data
	void InfoTestData::get_conso_data(std::string &name, size_t &nRows, size_t &nCols,
		std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames) {
		name = st_conso_name;
		nRows = st_conso_rows;
		assert(nRows > 1);
		nCols = st_conso_cols;
		assert(nCols > 1);
		const size_t n = (size_t)(nCols * nRows);
		data.resize(n);
		assert(data.size() == n);
		indNames.resize(nRows);
		assert(indNames.size() == nRows);
		varNames.resize(nCols);
		assert(varNames.size() == nCols);
		const int *pData = &(st_conso_data[0]);
		for (size_t i = 0; i < nCols; ++i) {
			varNames[i] = st_conso_vars[i];
		}// i
		for (size_t i = 0; i < nRows; ++i) {
			indNames[i] = st_conso_inds[i];
			for (size_t j = 0; j < nCols; ++j) {
				const size_t pos = (size_t)(i * nCols + j);
				data[pos] = pData[pos];
			}// j
		}// i
	}//get_conso_data
	 ////////////////////////////////////
}// namespace info

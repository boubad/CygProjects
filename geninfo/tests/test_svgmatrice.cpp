/*
 * test_svgmatrice.cpp
 *
 *  Created on: 21 juin 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <svg_arranger.h>
/////////////////////////////
#include "global_defs.h"
#include "infotestdata.h"
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using ArrangerType = SVGMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
using strings_vector = std::vector<STRINGTYPE>;
//////////////////////////
BOOST_AUTO_TEST_SUITE(SVGMatriceTestSuite)
BOOST_AUTO_TEST_CASE(SVGFile) {
	size_t nRows, nCols;
	STRINGTYPE name;
	strings_vector rowNames, colNames;
	std::vector<DATATYPE> data;
	InfoTestData::get_test_data(name, nRows, nCols, data, rowNames, colNames);
	STRINGTYPE filename("test_test_data.svg");
	//
	ArrangerType oArrange(name);
	std::future<bool> bFuture = oArrange.export_async(filename, nRows, nCols,
			data, rowNames, colNames);
	bool bRet = bFuture.get();
	BOOST_CHECK(bRet);
} //testSVGFile
BOOST_AUTO_TEST_CASE(SVGStream) {
	size_t nRows, nCols;
	STRINGTYPE name;
	strings_vector rowNames, colNames;
	std::vector<DATATYPE> data;
	InfoTestData::get_mortal_data(name, nRows, nCols, data, rowNames, colNames);
	//
	std::stringstream os;
	ArrangerType oArrange(name);
	std::future<bool> bFuture = oArrange.export_async(os, nRows, nCols, data,
			rowNames, colNames);
	bool bRet = bFuture.get();
	BOOST_CHECK(bRet);
	std::string ss = os.str();
	BOOST_TEST_MESSAGE(ss);
} //testSVGFile
BOOST_AUTO_TEST_SUITE_END();


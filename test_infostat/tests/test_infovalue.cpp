/*
 * test_infovalue.cpp
 *
 *  Created on: 27 mai 2016
 *      Author: boubad
 */
#include "stdafx.h"
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include <infovalue.h>
/////////////////////////////
using namespace info;
///////////////////////////
BOOST_AUTO_TEST_SUITE(InfoValueTestSuite)
BOOST_AUTO_TEST_CASE(testShortValue) {
	short val = 11;
	InfoValue v(val);
	//
	BOOST_CHECK(!v.is_empty());
	BOOST_CHECK(v.is_valid());
	InfoValueType t = v.get_infovalue_type();
	BOOST_CHECK(t == InfoValueType::shortType);
	BOOST_CHECK(v.is_integral());
	BOOST_CHECK(!v.is_floating());
	BOOST_CHECK(!v.is_text());
	BOOST_CHECK(v.is_numerical());
	//
	BOOST_CHECK(v.short_value() == (short )val);
	BOOST_CHECK(v.unsigned_short_value() == (unsigned short )val);
	BOOST_CHECK(v.int_value() == (int )val);
	BOOST_CHECK(v.unsigned_int_value() == (unsigned int )val);
	BOOST_CHECK(v.long_value() == (long )val);
	BOOST_CHECK(v.unsigned_long_value() == (unsigned long )val);
	BOOST_CHECK(v.long_long_value() == (long long )val);
	BOOST_CHECK(v.float_value() == (float )val);
	BOOST_CHECK(v.double_value() == (double )val);
	BOOST_CHECK(v.long_double_value() == (long double )val);
	//
	std::string sval;
	bool bRet = v.string_value(sval);
	BOOST_CHECK(bRet);
	BOOST_CHECK(sval == "11");
	std::wstring svalw;
	bRet = v.string_value(svalw);
	BOOST_CHECK(bRet);
	BOOST_CHECK(svalw == L"11");
} //testShortValue
BOOST_AUTO_TEST_SUITE_END();

/*
 * dummy.cpp
 *
 *  Created on: 13 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>

using namespace std;

struct CMyFooTestFixture {
	CMyFooTestFixture() :
			m_configFile("test.tmp") {
		// TODO: Common set-up each test case here.
		fclose(fopen(m_configFile.c_str(), "w+"));
	}

	~CMyFooTestFixture() {
		// TODO: Common tear-down for each test case here.
		remove(m_configFile.c_str());
	}

	// TODO: Declare some common values accesses in tests here.
	string m_configFile;
};

BOOST_FIXTURE_TEST_SUITE(MyFooTest, CMyFooTestFixture)
;

BOOST_AUTO_TEST_CASE(LoadTestConfigFile) {
	//  CMyFoo foo;
	int n = 5;
	BOOST_REQUIRE(n == 5);   // Stop here if it fails.
}

BOOST_AUTO_TEST_CASE(Name) {
	int n = 5;
	BOOST_CHECK(n == 4);
}

BOOST_AUTO_TEST_SUITE_END();


/*
 * test_infostat.cpp
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */
#include "stdafx.h"
#include <boost/test/unit_test.hpp>
/////////////////////////
#include <infostat.h>
/////////////////////////////
#include <mytestfixture.h>
////////////////////////////////////
#include <global_defs.h>
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using MyFixture = MyTestFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
/////////////////////////////////////
using IndivType = typename MyFixture::IndivType;
using DataMap = typename MyFixture::DataMap;
using IndivTypePtr = typename MyFixture::IndivTypePtr;
using SourceType = typename MyFixture::SourceType;
////////////////////////////////
using StatInfoType = StatInfo<IDTYPE, STRINGTYPE>;
using StatSummatorType = StatSummator<IDTYPE,STRINGTYPE>;
using ints_vector = std::vector<IDTYPE>;
/////////////////////////////////////
BOOST_FIXTURE_TEST_SUITE(StatInfoTestSuite,MyFixture)
BOOST_AUTO_TEST_CASE(testMortalStatInfo) {
	SourceType *pProvider = this->mortal_source();
	BOOST_CHECK(pProvider != nullptr);
	//
	StatSummatorType oStat;
	pProvider->reset();
	size_t nTotal = 0;
	do {
		IndivTypePtr oInd = pProvider->next();
		if (oInd.get() == nullptr) {
			break;
		}
		oStat.add(oInd);
		++nTotal;
	} while (true);
	ints_vector keys;
	oStat.get_keys(keys);
	size_t nc = keys.size();
	BOOST_CHECK(nc > 0);
	const double epsilon = 0.000001;
	for (auto &key : keys) {
		bool bFound = oStat.has_key(key);
		BOOST_CHECK(bFound);
		StatInfoType oInfo;
		bool bRet = oStat.get(key, oInfo);
		BOOST_CHECK(bRet);
		size_t nz = oInfo.get_count();
		BOOST_CHECK(nz > 0);
		double vmin = oInfo.get_min();
		double vmax = oInfo.get_max();
		BOOST_CHECK(vmin < vmax);
		double vmean = oInfo.get_mean();
		BOOST_CHECK(vmean > vmin);
		BOOST_CHECK(vmean < vmax);
		double vcov = oInfo.get_variance();
		BOOST_CHECK(vcov > 0);
		double vstd = oInfo.get_deviation();
		BOOST_CHECK(vstd > 0);
		for (auto &key1 : keys) {
			double xxcov = 0;
			bRet = oStat.get_covariance(key, key1, xxcov);
			BOOST_CHECK(bRet);
			BOOST_CHECK(xxcov != 0);
			double xxcor = -5.0;
			bRet = oStat.get_correlation(key, key1, xxcor);
			BOOST_CHECK(bRet);
			double x = std::abs(xxcor);
			BOOST_CHECK(x <= (1.0 + epsilon));
		} // keye1
	} // key
} //testMortalMatElem

BOOST_AUTO_TEST_SUITE_END();


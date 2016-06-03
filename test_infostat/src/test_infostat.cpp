// boost_teststatdata.cpp : Defines the entry point for the console application.
//
#define BOOST_TEST_MODULE "C++ Unit Tests for statdata"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
/////////////////////////////
#include <store_fixture.h>
////////////////////////////////////
#include <global_defs.h>
////////////////////
using namespace info;
using namespace std;
///////////////////////////////////
using StoreFixture = TestStoreFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
using IStoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
/////////////////////////////////////
StoreFixture *pGlobalFixture = nullptr;
////////////////////////////////////////
class InfoStatGlobalFixture{
private:
	unique_ptr<StoreFixture> m_store;
public:
	InfoStatGlobalFixture(){
		pGlobalFixture = new StoreFixture();
		assert(pGlobalFixture != nullptr);
		m_store.reset(pGlobalFixture);
		IStoreType *p1 = pGlobalFixture->get_memory_store();
		assert(p1 != nullptr);
		IStoreType *p2 = pGlobalFixture->get_sql_store();
		assert(p2 != nullptr);
	}
	~InfoStatGlobalFixture(){
		m_store.reset();
		pGlobalFixture = nullptr;
	}
};// class InfoStatGlobalFixture
///////////////////////////////////
//BOOST_GLOBAL_FIXTURE(InfoStatGlobalFixture);

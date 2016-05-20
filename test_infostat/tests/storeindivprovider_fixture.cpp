#include "storeindivprovider_fixture.h"
/////////////////////////////
using namespace info;
//////////////////////////////////
StoreIndivSorceFixture::StoreIndivSorceFixture() {
	std::string sigle = this->m_oset.sigle();
	IStoreType *pStore = this->get_store();
	assert(pStore != nullptr);
	StoreIndivSourceType *p = new StoreIndivSourceType(pStore, sigle);
	assert(p != nullptr);
	this->m_source.reset(p);
}
StoreIndivSorceFixture::~StoreIndivSorceFixture() {
	this->m_source.reset();
}
StoreIndivSorceFixture::IndivSourceType *StoreIndivSorceFixture::get_source(void) {
	IndivSourceType *p = this->m_source.get();
	assert(p != nullptr);
	return (p);
}

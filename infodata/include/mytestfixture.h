/*
 * mytestfixture.h
 *
 *  Created on: 28 mai 2016
 *      Author: boubad
 */

#ifndef MYTESTFIXTURE_H_
#define MYTESTFIXTURE_H_
////////////////////////////////
#include "store_fixture.h"
#include "indivsource_fixture.h"
/////////////////////////////////////
template<typename IDTYPE = unsigned long, typename INTTYPE = unsigned long,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
class MyTestFixture {
public:
	using StoreFixture = info::TestStoreFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using SourceFixture = info::TestSourceFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using StoreType = info::IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using IndivType = info::Indiv<IDTYPE, STRINGTYPE>;
	using DataMap = std::map<IDTYPE, info::InfoValue>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = info::IIndivSource<IDTYPE, STRINGTYPE>;
private:
	std::unique_ptr<StoreFixture> m_store;
	std::unique_ptr<SourceFixture> m_fixture;
public:
	MyTestFixture() {
		StoreFixture *p = new StoreFixture();
		assert(p != nullptr);
		m_store.reset(p);
		StoreType *ps = p->get_memory_store();
		assert(ps != nullptr);
		m_fixture.reset(new SourceFixture(ps));
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
	} // MyTestFixture
	virtual ~MyTestFixture() {
	}
public:
	StoreType *get_store(bool bMemory = true) {
		StoreFixture *px = m_store.get();
		assert(px != nullptr);
		return (px->get_store(bMemory));
	}
	SourceType *mortal_source(void) {
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
		SourceType *pProvider = px->mortal_source();
		assert(pProvider != nullptr);
		pProvider->reset();
		return (pProvider);
	}
	SourceType *conso_source(void) {
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
		SourceType *pProvider = px->conso_source();
		assert(pProvider != nullptr);
		pProvider->reset();
		return (pProvider);
	}
	SourceType *test_source(void) {
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
		SourceType *pProvider = px->test_source();
		assert(pProvider != nullptr);
		pProvider->reset();
		return (pProvider);
	}
};
// class MyTestFuxture<IDTYPE,INTTYPE,STRINGNAME,WEIGHTYPE>
///////////////////////////////
#endif /* MYTESTFIXTURE_H_ */

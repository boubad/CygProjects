/*
 * mytestvariablefixture.h
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */

#ifndef MYTESTVARIABLEFIXTURE_H_
#define MYTESTVARIABLEFIXTURE_H_
////////////////////////////////
#include "store_fixture.h"
#include "variablesource_fixture.h"
/////////////////////////////////////
template<typename IDTYPE = unsigned long, typename INTTYPE = unsigned long,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
class MyTestVariableFixture {
public:
	using StoreFixture = info::TestStoreFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using SourceFixture = info::TestVariableSourceFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using StoreType = info::IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using IndivType = info::Indiv<IDTYPE, STRINGTYPE>;
	using DataMap = std::map<IDTYPE, info::InfoValue>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = info::IIndivSource<IDTYPE, STRINGTYPE>;
private:
	std::unique_ptr<StoreFixture> m_store;
	std::unique_ptr<SourceFixture> m_fixture;
public:
	MyTestVariableFixture() {
		StoreFixture *p = new StoreFixture();
		assert(p != nullptr);
		m_store.reset(p);
		StoreType *ps = p->get_memory_store();
		assert(ps != nullptr);
		m_fixture.reset(new SourceFixture(ps));
		//SourceFixture *px = m_fixture.get();
		//assert(px != nullptr);
	} // MyTestFixture
	virtual ~MyTestVariableFixture() {
	}
public:
	SourceType *mortal_source(void) {
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
		SourceType *pProvider = px->mortal_source();
		assert(pProvider != nullptr);
		return (pProvider);
	}
	SourceType *conso_source(void) {
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
		SourceType *pProvider = px->conso_source();
		assert(pProvider != nullptr);
		return (pProvider);
	}
	SourceType *test_source(void) {
		SourceFixture *px = m_fixture.get();
		assert(px != nullptr);
		SourceType *pProvider = px->test_source();
		assert(pProvider != nullptr);
		return (pProvider);
	}
};
// class MyTestFuxture<IDTYPE,INTTYPE,STRINGNAME,WEIGHTYPE>
///////////////////////////////




#endif /* MYTESTVARIABLEFIXTURE_H_ */

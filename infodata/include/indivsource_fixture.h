#pragma once
#ifndef __INDIVSTORE_FIXTURE_H__
#define  __INDIVSTORE_FIXTURE_H__
////////////////////////////////////
#include "store_fixture.h"
///////////////////////////////////////
#include <storeindivsource.h>
////////////////////////////////////////
namespace info {
	///////////////////////////////////////
	template<typename IDTYPE, typename INTTYPE,typename STRINGTYPE, typename WEIGHTYPE>
		class TestSourceFixture {
		public:
			using IndivType = Indiv<IDTYPE, STRINGTYPE>;
			using DataMap = std::map<IDTYPE, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using StoreType = IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using StoreIndivSourceType = StoreIndivSource<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
			using StoreFixture = TestStoreFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using SourceFixture = TestSourceFixture<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		private:
			std::unique_ptr<StoreFixture> m_store;
			std::unique_ptr<StoreIndivSourceType> m_test_source;
			std::unique_ptr<StoreIndivSourceType> m_conso_source;
			std::unique_ptr<StoreIndivSourceType> m_mortal_source;
		public:
			TestSourceFixture(StoreType *pStore = nullptr, bool bMemory = true) {
				StoreType *ps = pStore;
				if (ps == nullptr) {
					StoreFixture *pf = m_store.get();
					if (pf == nullptr) {
						m_store.reset(new StoreFixture());
						pf = m_store.get();
						assert(pf != nullptr);
					}
					ps = pf->get_store(bMemory);
					assert(ps != nullptr);
				}
				{
					STRINGTYPE sigle;
					InfoTestData::get_conso_name(sigle);
					assert(!sigle.empty());
					m_conso_source.reset(new StoreIndivSourceType(ps, sigle));
				}
				{
					STRINGTYPE sigle;
					InfoTestData::get_mortal_name(sigle);
					assert(!sigle.empty());
					m_mortal_source.reset(new StoreIndivSourceType(ps, sigle));
				}
				{
					STRINGTYPE sigle;
					InfoTestData::get_test_name(sigle);
					assert(!sigle.empty());
					m_test_source.reset(new StoreIndivSourceType(ps, sigle));
				}
			}//TestSourceFixture
			virtual ~TestSourceFixture() {}
		public:
			SourceType *mortal_source(void) {
				SourceType *p = m_mortal_source.get();
				assert(p != nullptr);
				return (p);
			}
			SourceType *conso_source(void) {
				SourceType *p = m_conso_source.get();
				assert(p != nullptr);
				return (p);
			}
			SourceType *test_source(void) {
				SourceType *p = m_test_source.get();
				assert(p != nullptr);
				return (p);
			}
	};
	////////////////////////////////////////////////
}// namespace info
//////////////////////////////////////
#endif // !__INDIVSTORE_FIXTU>RE_H__

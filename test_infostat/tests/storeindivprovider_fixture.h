#pragma once
#ifndef __STOREINDIVPROVIDER_FIXTURE_H__
#define __STOREINDIVPROVIDER_FIXTURE_H__
///////////////////////////////////////////
#include "memorystorefixture.h"
#include <storeindivsource.h>
//////////////////////////////////////
template<typename IDTYPE = unsigned long, typename INTTYPE = int,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = float>
class StoreIndivSorceFixture: public MemoryStoreFixture<IDTYPE, INTTYPE,
		STRINGTYPE, WEIGHTYPE> {
protected:
	using StoreIndivSourceType = info::StoreIndivSource<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using IndivType = info::Indiv<IDTYPE,STRINGTYPE>;
	using IndexType = typename IndivType::IndexType;
	using DataMap = typename IndivType::DataMap;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using MemoryStoreType = info::MemoryStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using IStoreType = info::IStatStore<IDTYPE, INTTYPE, STRINGTYPE, WEIGHTYPE>;
	using DatasetType = typename IStoreType::DatasetType;
	using IndivSourceType = info::IIndivSource<IDTYPE,STRINGTYPE>;
	//
	std::unique_ptr<StoreIndivSourceType> m_source;
	IndivSourceType *get_source(void) {
		IndivSourceType *p = this->m_source.get();
		assert(p != nullptr);
		return (p);
	}
public:
	StoreIndivSorceFixture() {
		std::string sigle = this->m_oset.sigle();
		IStoreType *pStore = this->get_store();
		assert(pStore != nullptr);
		StoreIndivSourceType *p = new StoreIndivSourceType(pStore, sigle);
		assert(p != nullptr);
		this->m_source.reset(p);
	}
	virtual ~StoreIndivSorceFixture() {
		this->m_source.reset();
	}
};
////////////////////////////////////////////
#endif // !__STOREINDIVPROVIDER_FIXTURE_H__

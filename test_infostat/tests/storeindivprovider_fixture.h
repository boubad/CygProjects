#pragma once
#ifndef __STOREINDIVPROVIDER_FIXTURE_H__
#define __STOREINDIVPROVIDER_FIXTURE_H__
///////////////////////////////////////////
#include "memorystorefixture.h"
#include <storeindivsource.h>
//////////////////////////////////////
class StoreIndivSorceFixture : public MemoryStoreFixture {
protected:
	using StoreIndivSourceType = info::TranformedStoreIndivSource<>;
	using IndivType = info::Indiv<>;
	using IndexType = IndivType::IndexType;
	using DataMap = IndivType::DataMap;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using MemoryStoreType = info::MemoryStatStore<>;
	using IStoreType = info::IStatStore<>;
	using DatasetType = IStoreType::DatasetType;
	using IndivSourceType = info::IIndivSource<>;
	//
	std::unique_ptr<StoreIndivSourceType> m_source;
	IndivSourceType *get_source(void);
public:
	StoreIndivSorceFixture();
	virtual ~StoreIndivSorceFixture();
};
////////////////////////////////////////////
#endif // !__STOREINDIVPROVIDER_FIXTURE_H__

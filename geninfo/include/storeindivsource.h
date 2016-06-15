#pragma once
#ifndef __STOREINDIVSOURCE_H__
#define __STOREINDIVSOURCE_H__
/////////////////////////////////
#include "storebaseprovider.h"
///////////////////////////////////
namespace info {
	/////////////////////////////
	template<typename U, typename INTTYPE,typename STRINGTYPE, typename WEIGHTYPE>
		class StoreIndivSource : public StoreBaseProvider<U, INTTYPE, STRINGTYPE, WEIGHTYPE> {
		public:
			using IndexType = U;
			using ints_vector = std::vector<U>;
			using IndivType = Indiv<U, STRINGTYPE>;
			using DataMap = std::map<U, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using StoreType = IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DBIndivType = StatIndiv<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = StatDataset<U, INTTYPE, STRINGTYPE>;
			using ValueType = StatValue<U, INTTYPE, STRINGTYPE>;
			using values_vector = std::vector<ValueType>;
			using indivptrs_vector = std::vector<IndivTypePtr>;
			using SourceType = IIndivSource<U, STRINGTYPE>;
			using ints_doubles_map = std::map<U, double>;
			using StatSummatorType = StatSummator<U, STRINGTYPE>;
			using StatInfoType = StatInfo<U, STRINGTYPE>;
			using SourceBaseType = StoreBaseProvider<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using StoreIndivSourceType = StoreIndivSource<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		public:
			StoreIndivSource(StoreType *pStore, const STRINGTYPE &datasetName) : SourceBaseType(pStore, datasetName) {
			}
			virtual ~StoreIndivSource() {
			}
		protected:
			virtual size_t impl_get_count(StoreType *pStore, const DatasetType &oSet) {
				size_t nc = 0;
				pStore->find_dataset_indivs_count(oSet, nc);
				return (nc);
			}//impl_get_count
			virtual void impl_get_ids(StoreType *pStore, const DatasetType &oSet, ints_vector &oIds) {
				oIds.clear();
				size_t nc = 0;
				if (pStore->find_dataset_indivs_count(oSet, nc)) {
					pStore->find_dataset_indivs_ids(oSet, oIds, 0, nc);
				}// ok
			}// impl_get_ids
			virtual IndivTypePtr impl_get_indiv(StoreType *pStore, const DatasetType &oSet, const IndexType aIndex) {
				IndivTypePtr oRet;
				DBIndivType cur(aIndex);
				cur.dataset_id(oSet.id());
				size_t nc = 0;
				if (!pStore->find_indiv_values_count(cur, nc)) {
					return (oRet);
				}
				if (nc < 1) {
					return (oRet);
				}
				values_vector oVals;
				if (!pStore->find_indiv_values(cur, oVals, 0, nc)) {
					return (oRet);
				}
				DataMap oMap;
				for (auto &v : oVals) {
					if (!v.empty()) {
						const U key = (U)v.variable_id();
						InfoValue val = v.value();
						oMap[key] = val;
					} // v
				}
				oRet = std::make_shared<IndivType>(aIndex, oMap, cur.sigle());
				return (oRet);
			}//impl_get_indiv
	};
	// class StoreIndivSource<U,T,INTTYPE,STRINGTYPE,WEIGHTYPE>
	/////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // __STOREINDIVSOURCE_H__

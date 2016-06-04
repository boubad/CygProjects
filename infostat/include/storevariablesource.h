/*
 * storevariablesource.h
 *
 *  Created on: 29 mai 2016
 *      Author: boubad
 */

#ifndef STOREVARIABLESOURCE_H_
#define STOREVARIABLESOURCE_H_
/////////////////////////////////
#include "storebaseprovider.h"
///////////////////////////////////
namespace info {
	/////////////////////////////
	template<typename U = unsigned long, typename INTTYPE = unsigned long,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
		class StoreVariableSource : public StoreBaseProvider<U, INTTYPE, STRINGTYPE, WEIGHTYPE> {
		public:
			using IndexType = U;
			using ints_vector = std::vector<U>;
			using IndivType = Indiv<U, STRINGTYPE>;
			using DataMap = std::map<U, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using StoreType = IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DBIndivType = StatVariable<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = StatDataset<U, INTTYPE, STRINGTYPE>;
			using ValueType = StatValue<U, INTTYPE, STRINGTYPE>;
			using values_vector = std::vector<ValueType>;
			using indivptrs_vector = std::vector<IndivTypePtr>;
			using SourceType = IIndivSource<U, STRINGTYPE>;
			using ints_doubles_map = std::map<U, double>;
			using StatSummatorType = StatSummator<U, STRINGTYPE>;
			using StatInfoType = StatInfo<U, STRINGTYPE>;
			using SourceBaseType = StoreBaseProvider<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		public:
			StoreVariableSource(StoreType *pStore, const STRINGTYPE &datasetName) : SourceBaseType(pStore, datasetName) {
			}
			virtual ~StoreVariableSource() {
			}
		protected:
			virtual void update_weights(void) {
				// do nothing
			}// update_weights
			virtual size_t impl_get_count(StoreType *pStore, const DatasetType &oSet) {
				size_t nc = 0;
				pStore->find_dataset_variables_count(oSet, nc);
				return (nc);
			}//impl_get_count
			virtual void impl_get_ids(StoreType *pStore, const DatasetType &oSet, ints_vector &oIds) {
				oIds.clear();
				size_t nc = 0;
				if (pStore->find_dataset_variables_count(oSet, nc)) {
					pStore->find_dataset_variables_ids(oSet, oIds, 0, nc);
				}// ok
			}// impl_get_ids
			virtual IndivTypePtr impl_get_indiv(StoreType *pStore, const DatasetType &oSet, const IndexType aIndex) {
				IndivTypePtr oRet;
				DBIndivType cur(aIndex);
				cur.dataset_id(oSet.id());
				size_t nc = 0;
				if (!pStore->find_variable_values_count(cur, nc)) {
					return (oRet);
				}
				if (nc < 1) {
					return (oRet);
				}
				values_vector oVals;
				if (!pStore->find_variable_values(cur, oVals, 0, nc)) {
					return (oRet);
				}
				DataMap oMap;
				for (auto &v : oVals) {
					if (!v.empty()) {
						const U key = (U)v.indiv_id();
						InfoValue val = v.value();
						oMap[key] = val;
					} // v
				}
				oRet = std::make_shared<IndivType>(aIndex, oMap, cur.sigle());
				return (oRet);
			}//impl_get_indiv
	};
	// class StoreIndivSource<U,T,INTTYPE,STRINGTYPE,WEIGHTYPE>
}// namespace info
	/////////////////////////////////////////////////////////////////////
#endif /* STOREVARIABLESOURCE_H_ */

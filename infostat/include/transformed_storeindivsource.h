#pragma once
#ifndef __TRANSFORMED_STOREINDIVSOURCE_H__
#define __TRANSFORMED_STOREINDIVSOURCE_H__
///////////////////////////////////////////////
#include "storeindivsource.h"
///////////////////////////////////
namespace info {
	/////////////////////////////
	template<typename U = unsigned long, typename INTTYPE = int,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = float>
		class TranformedStoreIndivSource : public StoreIndivSource<U, INTTYPE,
		STRINGTYPE, WEIGHTYPE> {
		public:
			using ints_vector = std::vector<U>;
			using IndivType = Indiv<U, STRINGTYPE>;
			using DataMap = std::map<U, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using StoreType = IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DBIndivType = StatIndiv<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = StatDataset<U, INTTYPE, STRINGTYPE>;
			using ValueType = StatValue<U, INTTYPE, STRINGTYPE>;
			using values_vector = std::vector<ValueType>;
			using StoreIndivSourceType = StoreIndivSource<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using indivptrs_vector = std::vector<IndivTypePtr>;
			using StatSummatorType = StatSummator<U, STRINGTYPE>;
		private:
			TransformationType m_transf;
			StatSummatorType m_summator;
			//
		public:
			TranformedStoreIndivSource(StoreType *pStore, const STRINGTYPE &datasetName,
				const TransformationType mode = TransformationType::normalize) :
				StoreIndivSourceType(pStore, datasetName), m_transf(mode) {
				StatSummatorType &oSum = this->m_summator;
				oSum.clear();
				const size_t nc = this->count();
				for (size_t i = 0; i < nc; ++i) {
					IndivTypePtr oInd = StoreIndivSourceType::get(i);
					oSum.add(oInd);
				} // i
			}
			virtual ~TranformedStoreIndivSource() {
			}
		public:
			TransformationType transformation(void) {
				return (this->m_transf);
			}
			void transformation(TransformationType t) {
				this->m_transf = t;
			}
		public:
			virtual IndivTypePtr get(const size_t pos) {
				IndivTypePtr oInd = StoreIndivSourceType::get(pos);
				return (this->m_summator.transform(oInd, this->m_transf));
			}
			virtual void reset(void) {
				StoreIndivSourceType::reset();
				StatSummatorType &oSum = this->m_summator;
				oSum.clear();
				const size_t nc = this->count();
				for (size_t i = 0; i < nc; ++i) {
					IndivTypePtr oInd = StoreIndivSourceType::get(i);
					oSum.add(oInd);
				} // i
			} // reset
	};
	// class StoreIndivSource<U,INTTYPE,STRINGTYPE,WEIGHTYPE>
	/////////////////////////////////
}// namespace info
/////////////////////////////////////////////////
#endif // !__TRANSFORMED_STOREINDIVSOURCE_H__

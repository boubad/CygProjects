#pragma once
#ifndef __STOREINDIVSOURCE_H__
#define __STOREINDIVSOURCE_H__
/////////////////////////////////
#include "istatstore.h"
#include "indiv.h"
///////////////////////////////////
#include <cassert>
#include <mutex>
/////////////////////////////////
#include <boost/noncopyable.hpp>
//////////////////////////////////
namespace info {
	/////////////////////////////
	template <typename U = unsigned long,
		typename INTTYPE = int,
		typename STRINGTYPE = std::string,
		typename WEIGHTYPE = float>
		class StoreIndivSource : public IIndivSource<U>,
		private boost::noncopyable {
		public:
			using ints_vector = std::vector<U>;
			using IndivType = Indiv<U>;
			using DataMap = std::map<U, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using StoreType = IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DBIndivType = StatIndiv<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = StatDataset<U, INTTYPE, STRINGTYPE>;
			using ValueType = StatValue<U, INTTYPE, STRINGTYPE>;
			using values_vector = std::vector<ValueType>;
			using StoreIndivSourceType = StoreIndivSource<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		private:
			StoreType *m_pstore;
			size_t m_current;
			ints_vector m_ids;
			DatasetType m_oset;
			//
			std::mutex _mutex;
		public:
			StoreIndivSource(StoreType *pStore, const STRINGTYPE &datasetName) :m_pstore(pStore), m_current(0) {
				assert(this->m_pstore != nullptr);
				assert(this->m_pstore->is_valid());
				DatasetType &oSet = this->m_oset;
				oSet.sigle(datasetName);
				pStore->find_dataset(oSet);
				assert(oSet.id() != 0);
				size_t nc = 0;
				pStore->find_dataset_indivs_count(oSet, nc);
				pStore->find_dataset_indivs_ids(oSet, this->m_ids, 0, nc);
			}
			virtual ~StoreIndivSource() {}
		public:
			virtual size_t count(void) {
				std::unique_lock<std::mutex> oLock(this->_mutex);
				return (this->m_ids.size());
			}
			virtual IndivTypePtr get(const size_t pos) {
				std::unique_lock<std::mutex> oLock(this->_mutex);
				assert(pos < this->m_ids.size());
				StoreType *pStore = this->m_pstore;
				assert(pStore != nullptr);
				DatasetType &oSet = this->m_oset;
				U aIndex = (this->m_ids)[pos];
				DBIndivType oInd(aIndex);
				oInd.dataset_id(oSet.id());
				IndivTypePtr oRet;
				size_t nc = 0;
				if (!pStore->find_indiv_values_count(oInd, nc)) {
					return (oRet);
				}
				values_vector oVals;
				if (!pStore->find_indiv_values(oInd, oVals, 0, nc)) {
					return (oRet);
				}
				DataMap oMap;
				std::for_each(oVals.begin(), oVals.end(), [&](const ValueType &v) {
					if (!v.empty()) {
						const U key = (U)v.variable_id();
						InfoValue val = v.value();
						oMap[key] = val;
					}// v
				});
				oRet = std::make_shared<IndivType>(aIndex, oMap);
				return (oRet);
			}
			virtual void reset(void) {
				std::unique_lock<std::mutex> oLock(this->_mutex);
				StoreType *pStore = this->m_pstore;
				this->m_current = 0;
				size_t nc = 0;
				DatasetType &oSet = this->m_oset;
				pStore->find_dataset_indivs_count(oSet, nc);
				pStore->find_dataset_indivs_ids(oSet, this->m_ids, 0, nc);
			}// reset
			virtual IndivTypePtr next(void) {
				std::unique_lock<std::mutex> oLock(this->_mutex);
				DatasetType &oSet = this->m_oset;
				ints_vector & ids = this->m_ids;
				IndivTypePtr oRet;
				size_t pos = this->m_current;
				if (pos >= ids.size()) {
					return (oRet);
				}
				this->m_current = pos + 1;
				StoreType *pStore = this->m_pstore;
				assert(pStore != nullptr);
				U aIndex = ids[pos];
				DBIndivType oInd(aIndex);
				oInd.dataset_id(oSet.id());
				size_t nc = 0;
				if (!pStore->find_indiv_values_count(oInd, nc)) {
					return (oRet);
				}
				values_vector oVals;
				if (!pStore->find_indiv_values(oInd, oVals, 0, nc)) {
					return (oRet);
				}
				DataMap oMap;
				std::for_each(oVals.begin(), oVals.end(), [&](const ValueType &v) {
					if (!v.empty()) {
						const U key = (U)v.variable_id();
						InfoValue val = v.value();
						oMap[key] = val;
					}// v
				});
				oRet = std::make_shared<IndivType>(aIndex, oMap);
				return (oRet);
			}// next
	};// class StoreIndivSource<U,T,INTTYPE,STRINGTYPE,WEIGHTYPE>
	/////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // __STOREINDIVSOURCE_H__

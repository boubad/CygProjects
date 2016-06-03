#pragma once
#ifndef __STOREBASEPROVIDER_H__
#define __STOREBASEPROVIDER_H__
//////////////////////////////////
#include "istatstore.h"
#include "infostat.h"
#include "indiv.h"
///////////////////////////////////
namespace info {
	/////////////////////////////
	template<typename U = unsigned long, typename INTTYPE = unsigned long,
		typename STRINGTYPE = std::string, typename WEIGHTYPE = double>
		class StoreBaseProvider : public IIndivSource<U, STRINGTYPE>, private boost::noncopyable {
		using mutex_type = std::mutex;
		using lock_type = std::lock_guard<mutex_type>;
		public:
			using IndexType = U;
			using ints_vector = std::vector<U>;
			using IndivType = Indiv<U, STRINGTYPE>;
			using DataMap = std::map<U, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using StoreType = IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			using DatasetType = StatDataset<U, INTTYPE, STRINGTYPE>;
			using ValueType = StatValue<U, INTTYPE, STRINGTYPE>;
			using values_vector = std::vector<ValueType>;
			using indivptrs_vector = std::vector<IndivTypePtr>;
			using SourceType = IIndivSource<U, STRINGTYPE>;
			using ints_doubles_map = std::map<U, double>;
			using StatSummatorType = StatSummator<U, STRINGTYPE>;
			using StatInfoType = StatInfo<U, STRINGTYPE>;
			using SourceBaseType = StoreBaseProvider<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
		private:
			StoreType *m_pstore;
			STRINGTYPE m_sigle;
			std::atomic<size_t> m_ncount;
			std::atomic<size_t> m_current;
			ints_vector m_ids;
			DatasetType m_oset;
			indivptrs_vector m_indivs;
			StatSummatorType m_summator;
			ints_doubles_map m_weights;
			//
			mutex_type _mutex;
		protected:
			virtual void update_weights(void) {
				lock_type oLock(this->_mutex);
				this->m_summator.compute();
				ints_doubles_map &oWeights = this->m_weights;
				oWeights.clear();
				StatSummatorType &oSum = this->m_summator;
				ints_vector keys;
				oSum.get_keys(keys);
				for (auto &key : keys) {
					StatInfoType info;
					oSum.get(key, info);
					double s2 = info.get_variance();
					if ((s2 > 0) && (info.get_count() > 1)) {
						double px = info.get_weight();
						oWeights[key] = px;
					}
				}// keys
			}// update_weights
			virtual size_t impl_get_count(StoreType *pStore, const DatasetType &oSet) {
				return 0;
			}
			virtual void impl_get_ids(StoreType *pStore, const DatasetType &oSet, ints_vector &oIds) {

			}
			virtual IndivTypePtr impl_get_indiv(StoreType *pStore, const DatasetType &oSet, const IndexType aIndex) {
				return (IndivTypePtr());
			}
		public:
			StoreBaseProvider(StoreType *pStore, const STRINGTYPE &datasetName) :
				m_pstore(pStore), m_sigle(datasetName), m_ncount(0), m_current(0) {
				assert(this->m_pstore != nullptr);
				assert(this->m_pstore->is_valid());
			}
			virtual ~StoreBaseProvider() {
			}
		public:
			StatSummatorType & get_summator(void) {
				return (this->m_summator);
			}
			virtual void set_weights(const ints_doubles_map &oWeights) {
				lock_type oLock(this->_mutex);
				this->m_weights = oWeights;
			}// weights
			virtual void weights(ints_doubles_map &oWeights) {
				oWeights.clear();
				lock_type oLock(this->_mutex);;
				oWeights = this->m_weights;
			}// weights
			virtual size_t count(void) {
				return (this->m_ncount.load());
			}
			virtual IndivTypePtr find(const IndexType aIndex) {
				bool bFound = false;
				size_t ipos = 0;
				{
					ints_vector & vv = this->m_ids;
					const size_t n = vv.size();
					for (size_t i = 0; i < n; ++i) {
						if (vv[i] == aIndex) {
							bFound = true;
							ipos = i;
							break;
						}
					}
				}
				if (bFound) {
					return this->get(ipos);
				}
				return (IndivTypePtr());
			}
			virtual IndivTypePtr get(const size_t pos) {
				IndivTypePtr oRet;
				if (pos >= this->m_ncount.load()) {
					return oRet;
				}
				indivptrs_vector & vv = this->m_indivs;
				oRet = vv[pos];
				if (oRet.get() != nullptr) {
					return (oRet);
				}
				U aIndex = (this->m_ids)[pos];
				oRet = impl_get_indiv(this->m_pstore, this->m_oset, aIndex);
				if (oRet.get() != nullptr) {
					lock_type oLock(this->_mutex);
					vv[pos] = oRet;
					this->m_summator.add(oRet);
				}
				return (oRet);
			}
			virtual void reset(void) {
				{
					lock_type oLock(this->_mutex);
					StoreType *pStore = this->m_pstore;
					this->m_current.store(0);
					DatasetType &oSet = this->m_oset;
					oSet.sigle(this->m_sigle);
					pStore->find_dataset(oSet);
					this->m_indivs.clear();
					this->m_summator.clear();
					size_t nc = impl_get_count(pStore, oSet);
					impl_get_ids(pStore, oSet, this->m_ids);
					m_indivs.resize(nc);
					this->m_ncount.store(nc);
					this->m_weights.clear();
				}// sync
				size_t nc = this->m_ncount.load();
				for (size_t i = 0; i < nc; ++i) {
					(void)SourceBaseType::get(i);
				}
				update_weights();
			} // reset
			virtual IndivTypePtr next(void) {
				size_t pos = this->m_current.load();
				if (pos >= this->m_ncount.load()) {
					return (IndivTypePtr());
				}
				{
					lock_type oLock(this->_mutex);
					this->m_current.store((size_t)(pos + 1));
				}
				return this->get(pos);
			} // next
	};
	// class StoreIndivSource<U,T,INTTYPE,STRINGTYPE,WEIGHTYPE>
	/////////////////////////////////
}// namespace info
 ////////////////////////////////////
///////////////////////////////////
#endif // !__STOREBASEPROVIDER_H__

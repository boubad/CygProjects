#pragma once
#ifndef __INDIVMAP_H__
#define __INDIVMAP_H__
/////////////////////////////
#include "istatstore.h"
#include "distancemap.h"
#include "transformed_storeindivsource.h"
////////////////////////////////////
namespace info {
	//////////////////////////////////////
	template<typename U, typename STRINGTYPE, typename W>
	class IndivMap {
	public:
		using IndexType = U;
		using IndivType = Indiv<U, STRINGTYPE>;
		using DataMap = std::map<U, InfoValue>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using ints_doubles_map = std::map<U, double>;
		using SourceType = IIndivSource<U, STRINGTYPE>;
		using indivs_ints_map = std::map<U, IndivTypePtr>;
		using ints_set = std::set<U>;
		using DistanceMapType = DistanceMap<U, double>;
		using ints_vector = std::vector<U>;
		using IndivMapType = IndivMap<U, STRINGTYPE, W>;
		using iterator = typename indivs_ints_map::iterator;
		using const_iterator = typename indivs_ints_map::const_iterator;
	private:
		double m_fmin;
		double m_fmax;
		W m_zmax;
		indivs_ints_map m_map;
		DistanceMapType m_dist;
		ints_doubles_map m_weights;
	public:
		IndivMap() :m_fmin(0), m_fmax(0), m_zmax(1000) {}
		IndivMap(SourceType *pSource) :m_fmin(0), m_fmax(0), m_zmax(1000) {
			this->compute(pSource);
		}
		template <typename INTTYPE, typename WEIGHTYPE>
		IndivMap(IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE> *pStore, const STRINGTYPE &datasetSigle) :
			m_fmin(0), m_fmax(0), m_zmax(1000) {
			this->compute(pStore, datasetSigle);
		}
		IndivMap(const IndivMapType &other) :m_fmin(other.m_fmin), m_fmax(other.m_fmax), m_zmax(other.m_zmax),
			m_map(other.m_map), m_dist(other.m_dist), m_weights(other.m_weights) {}
		IndivMapType & operator=(const IndivMapType &other) {
			if (this != &other) {
				this->m_fmin = other.m_fmin;
				this->m_fmax = other.m_fmax;
				this->m_zmax = other.m_zmax;
				this->m_map = other.m_map;
				this->m_dist = other.m_dist;
				this->m_weights = other.m_weights;
			}
			return (*this);
		}
		virtual ~IndivMap() {}
	public:
		void distance_map(DistanceMap<U, W> &oDist) const {
			oDist.clear();
			const DistanceMapType &src = this->m_dist;
			ints_vector ids;
			src.indexes(ids);
			for (auto it = ids.begin(); it != ids.end(); ++it) {
				U aIndex1 = *it;
				for (auto jt = ids.begin(); jt != it; ++jt) {
					U aIndex2 = *jt;
					W res = 0;
					if (this->distance(aIndex1, aIndex2, res)) {
						oDist.add(aIndex1, aIndex2, res);
					}
				}// jt
			}// it
		}// distance_map
		DistanceMapType *distance_map(void) const {
			return (&(this->m_dist));
		}
		iterator begin(void) {
			return (this->m_map.begin());
		}
		const_iterator begin(void) const {
			return (this->m_map.begin());
		}
		iterator end(void) {
			return (this->m_map.end());
		}
		const_iterator end(void)  const {
			return (this->m_map.end());
		}
		const ints_set & ids(void) const {
			return (this->m_dist.indexes());
		}
		void ids(ints_set &oSet) {
			this->m_dist.indexes(oSet);
		}
		void ids(ints_vector &oIds) const {
			this->m_dist.indexes(oIds);
		}
		size_t size(void) const {
			return (this->m_dist.size());
		}
		bool get_id_from_index(const size_t &pos, U &aIndex) const {
			return (this->m_dist.get_id_from_index(pos, aIndex));
		} // get_id_from_index
		void clear(void) {
			this->m_fmin = 0;
			this->m_fmax = 0;
			this->m_map.clear();
			this->m_dist.clear();
		}// clear
		W scale_max(void) const {
			return (this->m_zmax);
		}
		void scale_max(W v) {
			if (v > 0) {
				this->m_zmax = v;
			}
		}
		bool indexed_distance(size_t pos1, size_t pos2, W &res) const {
			double delta = this->m_fmax - this->m_fmin;
			if (delta <= 0) {
				return (false);
			}
			double dr = 0;
			if (!this->m_dist.indexed_get(pos1, pos2, dr)) {
				return (false);
			}
			res = (W)(((dr - this->m_fmin) / delta) * this->m_zmax);
			return (true);
		}//indexed_distance
		W operator()(const size_t i1, const size_t i2) const {
			W res = 0;
			this->indexed_distance(i1, i2, res);
			return (res);
		}
		bool distance(U aIndex1, U aIndex2, W &res) const {
			double delta = this->m_fmax - this->m_fmin;
			if (delta <= 0) {
				return (false);
			}
			double dr = 0;
			if (!this->m_dist.get(aIndex1, aIndex2, dr)) {
				return (false);
			}
			res = (W)(((dr - this->m_fmin) / delta) * this->m_zmax);
			return (true);
		}// distance
		const ints_doubles_map & weights(void) const {
			return (this->m_weights);
		}
		void weights(const ints_doubles_map &ww) {
			this->m_weights = ww;
		}
		IndivTypePtr get(U aIndex) const {
			IndivTypePtr oRet;
			const indivs_ints_map &oMap = this->m_map;
			auto it = oMap.find(aIndex);
			if (it != oMap.end()) {
				oRet = (*it).second;
			}
			return (oRet);
		}// get
		bool add(IndivTypePtr oInd) {
			IndivType *pInd = oInd.get();
			if (pInd == nullptr) {
				return (false);
			}
			U aIndex = pInd->id();
			if (aIndex == 0) {
				return (false);
			}
			m_map[aIndex] = oInd;
			indivs_ints_map &oMap = this->m_map;
			bool bFirst = (oMap.size() < 3);
			ints_doubles_map &weights = this->m_weights;
			const DataMap &m0 = pInd->center();
			DistanceMapType &oDist = this->m_dist;
			for (auto &p : oMap) {
				if (p.first != aIndex) {
					IndivTypePtr xind = p.second;
					IndivType *px = xind.get();
					assert(px != nullptr);
					U xIndex = px->id();
					double d = 0;
					if (info_global_compute_distance(m0, px->center(), weights, d)) {
						if (bFirst) {
							this->m_fmin = d;
							this->m_fmax = d;
						}
						else if (d < this->m_fmin) {
							this->m_fmin = d;
						}
						else if (d > this->m_fmax) {
							this->m_fmax = d;
						}
						oDist.add(aIndex, xIndex, d);
					}// dist
				}// ok
			}// p
			return (true);
		}// add
		bool compute(SourceType *pProvider) {
			assert(pProvider != nullptr);
			pProvider->reset();
			pProvider->weights(this->m_weights);
			const size_t n = pProvider->count();
			for (size_t i = 0; i < n; ++i) {
				IndivTypePtr oInd1 = pProvider->get(i);
				this->add(oInd1);
			} // i
			return (true);
		} // compute
		template <typename INTTYPE, typename WEIGHTYPE>
		bool compute(IStatStore<U, INTTYPE, STRINGTYPE, WEIGHTYPE> *pStore, const STRINGTYPE &datasetSigle)
		{
			//
			using StoreIndivSourceType = TranformedStoreIndivSource<U, INTTYPE, STRINGTYPE, WEIGHTYPE>;
			assert(pStore != nullptr);
			assert(!datasetSigle.empty());
			//
			StoreIndivSourceType oProvider(pStore, datasetSigle);
			return (this->compute(&oProvider));
		}// compute
	}; // class IndivMap
	//////////////////////////////////////
}// namespace info
/////////////////////////////////
#endif // !__INDIVMAP_H__

#pragma once
/*
 * matcomputeparams.h
 *
 *  Created on: 23 mai 2016
 *      Author: boubad
 */

#ifndef MATCOMPUTEPARAMS_H_
#define MATCOMPUTEPARAMS_H_
 ////////////////////////////////
#include "distancemap.h"
#include "indiv.h"
////////////////////////////////////
namespace info {
	/////////////////////////////////
	template<typename U = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
		class MatComputeParams {
		public:
			using IndexType = U;
			using ints_set = std::set<IndexType>;
			using SourceType = IIndivSource<IndexType, STRINGTYPE>;
			using IndivType = Indiv<IndexType, STRINGTYPE>;
			using DataMap = std::map<IndexType, InfoValue>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using ints_vector = std::vector<IndexType>;
			using sizets_vector = std::vector<size_t>;
			using DistanceMapType = DistanceMap<IndexType, DISTANCETYPE>;
			using MatComputeParamsType = MatComputeParams<IndexType, DISTANCETYPE, STRINGTYPE>;
		private:
			DistanceMapType *m_pdistances;
			ints_vector *m_pids;
		public:
			MatComputeParams():m_pdistances(nullptr),m_pids(nullptr){}
			MatComputeParams(DistanceMapType *pDist, ints_vector *pids) :
				m_pdistances(pDist), m_pids(pids) {
			}
			MatComputeParams(const MatComputeParamsType &other):
				m_pdistances(other.m_pdistances),m_pids(other.m_pids){}
			MatComputeParamsType & operator=(const MatComputeParamsType &other) {
				if (this != &other) {
					this->m_pdistances = other.m_pdistances;
					this->m_pids = other.m_pids;
				}
				return (*this);
			}
			virtual ~MatComputeParams() {
			}
		public:
			size_t size(void) {
				assert(this->m_pids != nullptr);
				return (this->m_pids->size());
			}
			void set(DistanceMapType *pDist, ints_vector *pids) {
				this->m_pdistances = pDist;
				this->m_pids = pids;
			}
			DISTANCETYPE distance(const size_t i1, const size_t i2) {
				assert(this->m_pids != nullptr);
				assert(this->m_pdistances != nullptr);
				ints_vector &oIds = *(this->m_pids);
				assert(i1 < oIds.size());
				assert(i2 < oIds.size());
				const U aIndex1 = oIds[i1];
				const U aIndex2 = oIds[i2];
				DISTANCETYPE dRet = 0;
				this->m_pdistances->get(aIndex1, aIndex2, dRet);
				return (dRet);
			}
			DISTANCETYPE criteria(sizets_vector &indexes) {
				const size_t n = indexes.size();
				DISTANCETYPE dRet = 0;
				if (n < 2) {
					return (0);
				}
				const size_t nx = (size_t)(n - 1);
				for (size_t i = 0; i < nx; ++i) {
					const size_t i1 = indexes[i];
					const size_t i2 = indexes[i + 1];
					dRet = (DISTANCETYPE)(dRet + this->distance(i1, i2));
				} // i
				return (dRet);
			} // criteria
			DISTANCETYPE operator()(const size_t i1, const size_t i2) {
				return (this->distance(i1, i2));
			}
			DISTANCETYPE operator()(sizets_vector &indexes) {
				return (this->criteria(indexes));
			}
	};
	// class MatComputeParams<U,W>
	////////////////////////////////
}// namespace info
/////////////////////////////////

#endif /* MATCOMPUTEPARAMS_H_ */

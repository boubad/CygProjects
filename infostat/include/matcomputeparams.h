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
#include "crititem.h"
#include "indiv.h"
////////////////////////////////////
namespace info {
/////////////////////////////////
template<typename U = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
class MatComputeParams: private boost::noncopyable {
public:
	using IndexType = U;
	using ints_set = std::set<IndexType>;
	using SourceType = IIndivSource<IndexType,STRINGTYPE>;
	using IndivType = Indiv<IndexType,STRINGTYPE>;
	using DataMap = std::map<IndexType, InfoValue>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using ints_vector = std::vector<IndexType>;
	using sizets_vector = std::vector<size_t>;
	using DistanceMapType = DistanceMap<IndexType,DISTANCETYPE>;
	using MatComputeParamsType = MatComputeParams<IndexType,DISTANCETYPE,STRINGTYPE>;
private:
	DistanceMapType m_distances;
	ints_vector m_ids;
private:
	void compute_distances(SourceType *pProvider) {
		DistanceMapType &oMap = this->m_distances;
		oMap.clear();
		const size_t n = pProvider->count();
		for (size_t i = 0; i < n; ++i) {
			IndivTypePtr oInd1 = pProvider->get(i);
			const IndivType *pInd1 = oInd1.get();
			if ((pInd1 != nullptr) && pInd1->has_numeric_fields()) {
				const U aIndex1 = pInd1->id();
				const DataMap &m1 = pInd1->center();
				for (size_t j = 0; j < i; ++j) {
					IndivTypePtr oInd2 = pProvider->get(j);
					const IndivType *pInd2 = oInd1.get();
					if ((pInd2 != nullptr) && pInd2->has_numeric_fields()) {
						const U aIndex2 = pInd2->id();
						double d = 0;
						if (info_global_compute_distance(m1, pInd2->center(), d)) {
							oMap.add(aIndex1, aIndex2, d);
						}
					} // pInd2
				} // j
			} // pInd1
		} // i
	} // compute_distances
public:
	MatComputeParams(SourceType *pProvider) {
		assert(pProvider != nullptr);
		this->compute_distances(pProvider);
		ints_vector &ids = this->m_ids;
		ints_set oSet;
		this->m_distances.indexes(oSet);
		std::for_each(oSet.begin(),oSet.end(),[&](IndexType aId){
			ids.push_back(aId);
		});
	}
	virtual ~MatComputeParams() {
	}
public:
	size_t size(void)  {
		return (this->m_ids.size());
	}
	DISTANCETYPE distance(const size_t i1, const size_t i2)  {
		const ints_vector &oIds = this->m_ids;
		assert(i1 < oIds.size());
		assert(i2 < oIds.size());
		const U aIndex1 = oIds[i1];
		const U aIndex2 = oIds[i2];
		DISTANCETYPE dRet = 0;
		this->m_distances.get(aIndex1, aIndex2, dRet);
		return (dRet);
	}
	DISTANCETYPE criteria(sizets_vector &indexes) {
		const size_t n = indexes.size();
		DISTANCETYPE dRet = 0;
		if (n < 2) {
			return (0);
		}
		const size_t nx = (size_t) (n - 1);
		for (size_t i = 0; i < nx; ++i) {
			const size_t i1 = indexes[i];
			const size_t i2 = indexes[i + 1];
			dRet = (DISTANCETYPE) (dRet + this->distance(i1, i2));
		} // i
		return (dRet);
	} // criteria
	DISTANCETYPE operator()(const size_t i1, const size_t i2)  {
		return (this->distance(i1, i2));
	}
	DISTANCETYPE operator()(sizets_vector &indexes)  {
		return (this->criteria(indexes));
	}
};
// class MatComputeParams<U,W>
////////////////////////////

////////////////////////////////
}// namespace info
/////////////////////////////////

#endif /* MATCOMPUTEPARAMS_H_ */

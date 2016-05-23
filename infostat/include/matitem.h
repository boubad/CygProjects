#pragma once
/*
 * matitem.h
 *
 *  Created on: 23 mai 2016
 *      Author: boubad
 */

#ifndef MATITEM_H_
#define MATITEM_H_
////////////////////////////////
#include "crititem.h"
#include "interruptable_object.h"
////////////////////////////////////
namespace info {
/////////////////////////////////
template<typename U, typename W>
class MatComputeParams {
public:
	using ints_vector = std::vector<U>;
	using sizets_vector = std::vector<size_t>;
	using DistanceMapType = DistanceMap<U,W>;
	using MatComputeParamsType = MatComputeParams<U,W>;
private:
	const DistanceMapType *m_pdistances;
	const ints_vector *m_pids;
public:
	MatComputeParams(const ints_vector *pids, const DistanceMapType *pdist) :
			m_pdistances(pdist), m_pids(pids) {
		assert(this->m_pdistances != nullptr);
		assert(this->m_pids != nullptr);
	}
	MatComputeParams(const MatComputeParamsType &other) :
			m_pdistances(other.m_pdistances), m_pids(other.m_pids) {
	}
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
	size_t size(void) const {
		assert(this->m_pids != nullptr);
		return (this->m_pids->size());
	}
	W distance(const size_t i1, const size_t i2) const {
		assert(this->m_pdistances != nullptr);
		assert(this->m_pids != nullptr);
		const ints_vector &oIds = *(this->m_pids);
		assert(i1 < oIds.size());
		assert(i2 < oIds.size());
		const U aIndex1 = oIds[i1];
		const U aIndex2 = oIds[i2];
		W dRet = 0;
		this->m_pdistances->get(aIndex1, aIndex2, dRet);
		return (dRet);
	}
	W criteria(const sizets_vector &indexes) const {
		const size_t n = indexes.size();
		W dRet = 0;
		if (n < 2) {
			return (0);
		}
		const size_t nx = (size_t) (n - 1);
		for (size_t i = 0; i < nx; ++i) {
			const size_t i1 = indexes[i];
			const size_t i2 = indexes[i + 1];
			dRet = (W) (dRet + this->distance(i1, i2));
		} // i
		return (dRet);
	} // criteria
	W operator()(const size_t i1, const size_t i2) const {
		return (this->distance(i1, i2));
	}
	W operator()(const sizets_vector &indexes) const {
		return (this->criteria(indexes));
	}
};
// class MatComputeParams<U,W>
////////////////////////////
template<typename U = unsigned long, typename W = double>
class MatItem: public InterruptObject {
public:
	using sizets_vector = std::vector<size_t>;
	using MatComputeParamsType = MatComputeParams<U,W>;
	using MatItemType = MatItem<U,W>;
private:
	const MatComputeParamsType *m_params;
	W m_criteria;
	sizets_vector m_indexes;
public:
	MatItem(const MatComputeParamsType *pParams, std::atomic_bool *pCancel =
			nullptr) :
			InterruptObject(pCancel), m_params(pParams), m_criteria(0) {
		assert(this->m_params != nullptr);
		const size_t n = pParams->size();
		sizets_vector & indexes = this->m_indexes;
		indexes.resize(n);
		for (size_t i = 0; i < n; ++i) {
			indexes[i] = i;
		}
		this->m_criteria = pParams->criteria(indexes);
	} // MatItem
	MatItem(const MatComputeParamsType *pParams, const sizets_vector &oIndexes,
			std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_params(pParams), m_criteria(0), m_indexes(
					oIndexes) {
		assert(this->m_params != nullptr);
		sizets_vector & indexes = this->m_indexes;
		this->m_criteria = pParams->criteria(indexes);
	} // MatItem
	MatItem(const MatItemType &other) :
			InterruptObject(other), m_params(other.m_params), m_criteria(
					other.m_criteria), m_indexes(other.m_indexes) {
	}
	MatItemType & operator=(const MatItemType &other) {
		if (this != &other) {
			InterruptObject::operator=(other);
			this->m_params = other.m_params;
			this->m_criteria = other.m_criteria;
			this->m_indexes = other.m_indexes;
		}
		return (*this);
	}
	virtual ~MatItem() {
	}
public:
	const MatComputeParamsType *params(void) const {
		return (this->m_params);
	}
	W criteria(void) const {
		return (this->m_criteria);
	}
	const sizets_vector &indexes(void) const {
		return (this->m_indexes);
	}
public:
	bool try_permute(const size_t i1, const size_t i2, W &crit) const {
		assert(this->m_params != nullptr);
		sizets_vector temp(this->m_indexes);
		assert(i1 < temp.size());
		assert(i2 < temp.size());
		const size_t tt = temp[i1];
		temp[i1] = temp[i2];
		temp[i2] = tt;
		W c = this->m_params->criteria(temp);
		if (c < crit) {
			crit = c;
			return (true);
		}
		return (false);
	} // try_permute
	bool find_best_try(size_t &i1, size_t &i2, W &crit) const {
		crit = this->m_criteria;
		const sizets_vector &indexes = this->m_indexes;
		const size_t n = indexes.size();
		bool bPermuted = false;
		for (size_t i = 0; i < n; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			for (size_t j = 0; j < i; ++j) {
				if (this->check_interrupt()) {
					return (false);
				}
				if (this->try_permute(i, j, crit)) {
					i1 = i;
					i2 = j;
					bPermuted = true;
				}
			} // j
		} // i
		return (bPermuted);
	} //find_best_try
	bool permute() {
		do {
			if (this->check_interrupt()) {
				return (false);
			}
			size_t i1 = 0, i2 = 0;
			W crit = 0;
			if (!this->find_best_try(i1,i2,crit)){
				break;
			}
			if (i1 == i2){
				break;
			}
			sizets_vector &indexes = this->m_indexes;
			const size_t tt = indexes[i1];
			indexes[i1] = indexes[i2];
			indexes[i2] = tt;
			this->m_criteria = crit;
		} while (true);
		return ((this->check_interrupt()) ? false : true);
	} // permute
};
// class MatItem
////////////////////////////////
}// namespace info
/////////////////////////////////
#endif /* MATITEM_H_ */

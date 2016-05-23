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
#include "matcomputeparams.h"
////////////////////////////////////
namespace info {
/////////////////////////////////
template<typename U = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
class MatItem: public InterruptObject {
public:
	using IndexType = U;
	using sizets_pair = std::pair<size_t,size_t>;
	using pairs_queue = std::queue<sizets_pair>;
	using sizets_vector = std::vector<size_t>;
	using MatComputeParamsType = MatComputeParams<IndexType,DISTANCETYPE,STRINGTYPE>;
	using MatItemType = MatItem<IndexType,DISTANCETYPE,STRINGTYPE>;
private:
	MatComputeParamsType *m_params;
	DISTANCETYPE m_criteria;
	sizets_vector m_indexes;
public:
	MatItem(MatComputeParamsType *pParams, std::atomic_bool *pCancel =
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
	MatItem(MatComputeParamsType *pParams, const sizets_vector &oIndexes,
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
	DISTANCETYPE criteria(void) const {
		return (this->m_criteria);
	}
	const sizets_vector &indexes(void) const {
		return (this->m_indexes);
	}
public:
	bool try_permute(const size_t i1, const size_t i2, DISTANCETYPE &crit) const {
		assert(this->m_params != nullptr);
		sizets_vector temp(this->m_indexes);
		assert(i1 < temp.size());
		assert(i2 < temp.size());
		const size_t tt = temp[i1];
		temp[i1] = temp[i2];
		temp[i2] = tt;
		DISTANCETYPE c = this->m_params->criteria(temp);
		if (c < crit) {
			crit = c;
			return (true);
		}
		return (false);
	} // try_permute
	bool find_best_try(pairs_queue &q, DISTANCETYPE &crit) const {
		while (!q.empty()) {
			q.pop();
		}
		MatComputeParamsType *pParams = this->m_params;
		crit = this->m_criteria;
		const sizets_vector &indexes = this->m_indexes;
		const size_t n = indexes.size();
		for (size_t i = 0; i < n; ++i) {
			if (this->check_interrupt()) {
				return (false);
			}
			for (size_t j = 0; j < i; ++j) {
				if (this->check_interrupt()) {
					return (false);
				}
				sizets_vector temp(indexes);
				const size_t tt = temp[i];
				temp[i] = temp[j];
				temp[j] = tt;
				DISTANCETYPE c = pParams->criteria(temp);
				if (c < crit) {
					while (!q.empty()) {
						q.pop();
					}
					q.push(std::make_pair(i, j));
					crit = c;
				} else if (c == crit) {
					if (!q.empty()) {
						q.push(std::make_pair(i, j));
					}
				}
			} // j
		} // i
		return (!q.empty());
	} //find_best_try
	bool permute() {
		MatComputeParamsType *pParams = this->m_params;
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		do {
			if (this->check_interrupt()) {
				return (false);
			}
			pairs_queue q;
			DISTANCETYPE crit = 0;
			if (!this->find_best_try(q, crit)) {
				break;
			}
			if (this->check_interrupt()) {
				return (false);
			}
			sizets_vector best_indexes;
			DISTANCETYPE bestCrit = crit;
			while (!q.empty()) {
				if (this->check_interrupt()) {
					return (false);
				}
				sizets_pair p = q.front();
				q.pop();
				size_t i1 = p.first;
				size_t i2 = p.second;
				if (i1 == i2) {
					continue;
				}
				sizets_vector &indexes = this->m_indexes;
				sizets_vector temp(indexes);
				const size_t tt = temp[i1];
				temp[i1] = temp[i2];
				temp[i2] = tt;
				MatItem oMat(pParams, temp, pCancel);
				oMat.permute();
				if (this->check_interrupt()) {
					return (false);
				}
				DISTANCETYPE cc = oMat.criteria();
				if (best_indexes.empty()) {
					bestCrit = cc;
					best_indexes = oMat.indexes();
				} else if (cc < bestCrit) {
					bestCrit = cc;
					best_indexes = oMat.indexes();
				}
			} // while
			this->m_indexes = best_indexes;
			this->m_criteria = bestCrit;
		} while (true);
		return ((this->check_interrupt()) ? false : true);
	} // permute
};
// class MatItem
////////////////////////////////
}// namespace info
/////////////////////////////////
#endif /* MATITEM_H_ */

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
#include "arrangeindiv.h"
////////////////////////////////////
namespace info {
	/////////////////////////////////
	template<typename U = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
		class MatItem : public InterruptObject {
		public:
			using IndexType = U;
			using IndivType = Indiv<U, STRINGTYPE>;
			using IndivTypePtr = std::shared_ptr<IndivType>;
			using ints_vector = std::vector<IndexType>;
			using SourceType = IIndivSource<U, STRINGTYPE>;
			using sizets_pair = std::pair<size_t, size_t>;
			using pairs_queue = std::queue<sizets_pair>;
			using sizets_vector = std::vector<size_t>;
			using DistanceMapType = DistanceMap<IndexType, DISTANCETYPE>;
			using MatComputeParamsType = MatComputeParams<IndexType, DISTANCETYPE, STRINGTYPE>;
			using MatItemType = MatItem<IndexType, DISTANCETYPE, STRINGTYPE>;
			using ArrangeIndivsType = ArrangeIndivs<U, STRINGTYPE, DISTANCETYPE>;
		private:
			ArrangeIndivsType m_arrange;
			DISTANCETYPE m_criteria;
			MatComputeParamsType m_params;
			ints_vector m_ids;
			sizets_vector m_indexes;
		public:
			MatItem(SourceType *pProvider, std::atomic_bool *pCancel =
				nullptr) :InterruptObject(pCancel),
				m_arrange(pCancel), m_criteria(0) {
				assert(pProvider != nullptr);
				if (this->m_arrange.process(pProvider, (size_t)1, 100, pCancel)) {
					this->m_arrange.indexes(this->m_ids);
				}
				DistanceMapType *pDist = const_cast<DistanceMapType *>(&(this->m_arrange.distances_map()));
				this->m_params.set(pDist, &(this->m_ids));
				const size_t n = this->m_params.size();
				sizets_vector & indexes = this->m_indexes;
				indexes.resize(n);
				for (size_t i = 0; i < n; ++i) {
					indexes[i] = i;
				}
				this->m_criteria = this->m_params.criteria(indexes);
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
			void ids(ints_vector &v) const {
				v.clear();
				const sizets_vector &src = this->m_indexes;
				const ints_vector & dest = this->m_ids;
				for (auto it = src.begin(); it != src.end(); ++it) {
					size_t pos = *it;
					assert(pos < dest.size());
					v.push_back(dest[pos]);
				}
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
			bool find_best_try(pairs_queue &q, DISTANCETYPE &crit) {
				while (!q.empty()) {
					q.pop();
				}
				MatComputeParamsType *pParams = &(this->m_params);
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
						}
						else if (c == crit) {
							if (!q.empty()) {
								q.push(std::make_pair(i, j));
							}
						}
					} // j
				} // i
				return (!q.empty());
			} //find_best_try
			bool permute() {
				MatComputeParamsType *pParams = &(this->m_params);
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
					const size_t nx = q.size();
					if (nx < 1) {
						break;
					}
					size_t i1 = 0, i2 = 0;
					if (nx > 1) {
						size_t ix = (size_t)(::rand() % nx);
						size_t icur = 0;
						do {
							sizets_pair p = q.front();
							q.pop();
							i1 = p.first;
							i2 = p.second;
							if (icur == nx) {
								break;
							}
							if (q.empty()) {
								break;
							}
							++icur;
						} while (true);
					}
					else {
						sizets_pair p = q.front();
						q.pop();
						i1 = p.first;
						i2 = p.second;
					}
					if (i1 == i2) {
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

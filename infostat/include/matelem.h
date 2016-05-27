#pragma once
#ifndef __MATELEM_H__
#define __MATELEM_H__
////////////////////////////////
#include "indiv.h"
#include "distancemap.h"
#include "interruptable_object.h"
#include "crititem.h"
//////////////////////////////////
namespace info {
	/////////////////////////////////////
	template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long, typename STRINGTYPE = std::string>
	class MatElem : public InterruptObject {
	public:
		using CritItemType = CritItem<IDTYPE, DISTANCETYPE>;
		using crititems_vector = std::vector<CritItemType>;
		using sizets_pair = std::pair<size_t, size_t>;
		using pairs_queue = std::queue<sizets_pair>;
		using ints_vector = std::vector<IDTYPE>;
		using sizets_vector = std::vector<size_t>;
		using DistanceMapType = DistanceMap<IDTYPE, DISTANCETYPE>;
		using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using IndivType = Indiv<IDTYPE, STRINGTYPE>;
		using IndivTypePtr = std::shared_ptr<IndivType>;
		using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
	private:
		DISTANCETYPE m_crit;
		DistanceMapType *m_pdist;
		ints_vector *m_pids;
		sizets_vector m_indexes;
		crititems_vector m_args;
	public:
		MatElem() :m_crit(0), m_pdist(nullptr), m_pids(nullptr) {}
		MatElem(DistanceMapType *pMap, ints_vector *pids, sizets_vector *pindexes = nullptr, std::atomic_bool *pCancel = nullptr) :InterruptObject(pCancel),
			m_crit(0), m_pdist(pMap), m_pids(pids) {
			assert(this->m_pdist != nullptr);
			assert(this->m_pids != nullptr);
			const size_t n = pids->size();
			assert(n > 0);
			sizets_vector &indexes = this->m_indexes;
			indexes.resize(n);
			CritItemType::generate(n, this->m_args);
			if (pindexes != nullptr) {
				assert(pindexes->size() >= n);
				for (size_t i = 0; i < n; ++i) {
					indexes[i] = (*pindexes)[i];
				}// i
			}
			else {
				for (size_t i = 0; i < n; ++i) {
					indexes[i] = i;
				}
			}
			this->m_crit = this->criteria(indexes);
		}
		MatElem(const MatElemType &other) :InterrupObject(other), m_crit(other.m_crit), m_pdist(other.m_pdist), m_pids(other.m_pids),
			m_indexes(other.m_indexes), m_args(other.m_args) {}
		MatElemType & operator=(const MatElemType &other) {
			if (this != &other) {
				InterruptObject::operator=(other);
				this->m_crit = other.m_crit;
				this->m_pdist = other.m_pdist;
				this->m_pids = other.m_pids;
				this->m_indexes = other.m_indexes;
				this->m_args = other.m_args;
			}
			return (*this);
		}
		virtual ~MatElem() {}
	public:
		DISTANCETYPE criteria(void) const {
			return (this->m_crit);
		}
		const sizets_vector & indexes(void) const {
			return (this->m_indexes);
		}
		void ids(ints_vector &v) const {
			assert(this->m_pids);
			v.clear();
			const sizets_vector &src = this->m_indexes;
			const ints_vector & dest = *(this->m_pids);
			for (auto it = src.begin(); it != src.end(); ++it) {
				size_t pos = *it;
				assert(pos < dest.size());
				v.push_back(dest[pos]);
			}
		}
		bool process(void) {
			std::atomic_bool *pCancel = this->get_cancelleable_flag();
			DistanceMapType *pDist = this->m_pdist;
			ints_vector *pIds = this->m_pids;
			using result_type = std::pair<DISTANCETYPE, sizets_vector>;
			using future_type = std::future<result_type>;
			using future_vec = std::vector<future_type>;
			do {
				if (this->check_interrupt()) {
					break;
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
				sizets_pair p = q.front();
				q.pop();
				i1 = p.first;
				i2 = p.second;
				if (i1 == i2) {
					break;
				}
				sizets_vector oldIndexes = this->m_indexes;
				this->permute_items(i1, i2);
				this->m_crit = crit;
				future_vec oTasks;
				while (!q.empty()) {
					sizets_pair pp = q.front();
					size_t j1 = pp.first;
					size_t j2 = pp.second;
					if (j1 != j2) {
						oTasks.push_back(std::async([&]()->result_type {
							MatElemType xMat(pDist, pIds, &oldIndexes, pCancel);
							xMat.permute_items(j1, j2);
							if (!xMat.process()) {
								xMat.criteria(crit);
							}
							sizets_vector v = xMat.indexes();
							DISTANCETYPE cc = xMat.criteria();
							return (std::make_pair(cc, v));
						}));
					}// j1/j2
				}// while
				if (!oTasks.empty()) {
					for (future_type & t : oTasks) {
						result_type p = t.get();
						DISTANCETYPE cur = p.first;
						if (cur < this->m_crit) {
							this->m_indexes = p.second;
							this->m_crit = cur;
						}
					}// t
				}// not empty
			} while (true);
			return ((this->check_interrupt()) ? false : true);
		}// process
	protected:
		void permute_items(const size_t i1, const size_t i2) {
			assert(i1 != i2);
			sizets_vector &vv = this->m_indexes;
			assert(i1 < vv.size());
			assert(i2 < vv.size());
			const size_t tt = vv[i1];
			vv[i1] = vv[i2];
			vv[i2] = tt;
		}// permute_items
		size_t size(void) {
			assert(this->m_pids != nullptr);
			return (this->m_pids->size());
		}
		DISTANCETYPE distance(const size_t i1, const size_t i2) const {
			assert(this->m_pids != nullptr);
			assert(this->m_pdist != nullptr);
			ints_vector &oIds = *(this->m_pids);
			assert(i1 < oIds.size());
			assert(i2 < oIds.size());
			const IDTYPE aIndex1 = oIds[i1];
			const IDTYPE aIndex2 = oIds[i2];
			DISTANCETYPE dRet = 0;
			this->m_pdist->get(aIndex1, aIndex2, dRet);
			return (dRet);
		}
		void criteria(const DISTANCETYPE c) {
			this->m_crit = c;
		}
		DISTANCETYPE criteria(sizets_vector &indexes) const {
			const size_t n = indexes.size();
			DISTANCETYPE dRet = 0;
			if (n < 2) {
				return (0);
			}
			const size_t nx = (size_t)(n - 1);
			for (size_t i = 0; i < nx; ++i) {
				if (this->check_interrupt()) {
					return (dRet);
				}
				const size_t i1 = indexes[i];
				const size_t i2 = indexes[i + 1];
				dRet = (DISTANCETYPE)(dRet + this->distance(i1, i2));
			} // i
			return (dRet);
		} // criteria
		DISTANCETYPE operator()(const size_t i1, const size_t i2) const {
			return (this->distance(i1, i2));
		}
		DISTANCETYPE operator()(sizets_vector &indexes) const {
			return (this->criteria(indexes));
		}
		bool try_permute(const size_t i1, const size_t i2, DISTANCETYPE &crit) const {
			assert(i1 != i2);
			sizets_vector temp(this->m_indexes);
			assert(i1 < temp.size());
			assert(i2 < temp.size());
			const size_t tt = temp[i1];
			temp[i1] = temp[i2];
			temp[i2] = tt;
			DISTANCETYPE c = this->criteria(temp);
			if (c < crit) {
				crit = c;
				return (true);
			}
			return (false);
		} // try_permute
		bool find_best_try(pairs_queue &qq, DISTANCETYPE &crit) const {
			const sizets_vector &indexes = this->m_indexes;
			std::mutex _mutex;
			crititems_vector &args = const_cast<crititems_vector &>(this->m_args);
			std::atomic<DISTANCETYPE> resCrit(this->m_crit);
			crititems_vector q;
			//for (auto &cc : args) {
			info_parallel_for_each(args.begin(), args.end(), [&](CritItemType &cc) {
				const size_t i = (size_t)cc.first();
				const size_t j = (size_t)cc.second();
				sizets_vector temp(indexes);
				const size_t tt = temp[i];
				temp[i] = temp[j];
				temp[j] = tt;
				DISTANCETYPE c = this->criteria(temp);
				cc.criteria(c);
				if (c <= resCrit.load()) {
					std::lock_guard<std::mutex> oLock(_mutex);
					auto it = std::find(q.begin(), q.end(), cc);
					if (it == q.end()) {
						if ((c == resCrit.load()) && (!q.empty())) {
							CritItemType cx(cc);
							q.push_back(cx);
						}
						else if (c < resCrit.load()) {
							resCrit.store(c);
							q.clear();
							CritItemType cx(cc);
							q.push_back(cx);
						}
					}
				}// check
			});// cc
			for (auto &cc : q) {
				sizets_pair p(std::make_pair(cc.first(), cc.second()));
				qq.push(p);
			}
			/*
			info_parallel_for_each(args.begin(), args.end(), [&](CritItemType &cc) {
				const size_t i = (size_t)cc.first();
				const size_t j = (size_t)cc.second();
				sizets_vector temp(indexes);
				const size_t tt = temp[i];
				temp[i] = temp[j];
				temp[j] = tt;
				DISTANCETYPE c = this->criteria(temp);
				sizets_pair oPair(std::make_pair(j, i));
				if (c == resCrit.load()) {
					std::lock_guard<std::mutex> oLock(_mutex);
					if (!q.empty()) {
						q.push(oPair);
					}
				}
				else if (c < resCrit.load()) {
					resCrit.store(c);
					std::lock_guard<std::mutex> oLock(_mutex);
					while (!q.empty()) {
						q.pop();
					}
					q.push(oPair);
				}
			});*/
			crit = resCrit.load();
			return (!q.empty());
		} //find_best_try

	}; // class MatItem<IDTYPE,DISTANCETYPE,STRINGTYPE>
	/////////////////////////////////////////
}// namespace infi
///////////////////////////////////
#endif // !__MATELEM_H__


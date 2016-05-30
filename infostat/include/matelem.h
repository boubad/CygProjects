#pragma once
#ifndef __MATELEM_H__
#define __MATELEM_H__
////////////////////////////////
#include "indiv.h"
#include "distancemap.h"
#include "interruptable_object.h"
#include "crititem.h"
/////////////////////////////////
#include <boost/signals2/signal.hpp>
//////////////////////////////////
namespace info {
///////////////////////////////////////////
template<typename DISTANCETYPE = long>
class MatElemResult {
public:
	using sizets_vector = std::vector<size_t>;
	using MatElemResultType = MatElemResult<DISTANCETYPE>;
	using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
	//
	DISTANCETYPE first;
	sizets_vector second;
public:
	MatElemResult() :
			first(0) {
	}
	MatElemResult(const DISTANCETYPE c, const sizets_vector &v) :
			first(c), second(v) {
	}
	MatElemResult(const MatElemResultType &other) :
			first(other.first), second(other.second) {
	}
	MatElemResultType & operator=(const MatElemResultType &other) {
		if (this != &other) {
			this->first = other.first;
			this->second = other.second;
		}
		return (*this);
	}
	virtual ~MatElemResult() {
	}
};
/////////////////////////////////////
template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
class MatElem: public InterruptObject {
public:
	using CritItemType = CritItem<IDTYPE, DISTANCETYPE>;
	using crititems_vector = std::vector<CritItemType>;
	using sizets_pair = std::pair<size_t, size_t>;
	using pairs_list = std::list<sizets_pair>;
	using ints_vector = std::vector<IDTYPE>;
	using sizets_vector = std::vector<size_t>;
	using DistanceMapType = DistanceMap<IDTYPE, DISTANCETYPE>;
	using IndivType = Indiv<IDTYPE, STRINGTYPE>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using RescritType = std::atomic<DISTANCETYPE>;
	using MatElemResultType = MatElemResult<DISTANCETYPE>;
	using MatElemResultPtr = typename MatElemResultType::MatElemResultPtr;
	using SignalType = typename boost::signals2::signal<void (MatElemResultPtr)>;
	using SlotType = typename SignalType::slot_type;
	using ConnectionType = boost::signals2::connection;
private:
	DISTANCETYPE m_crit;
	DistanceMapType *m_pdist;
	sizets_vector m_indexes;
	crititems_vector m_args;
	SignalType m_signal;
	ints_vector m_resids;
public:
	MatElem(std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_crit(0), m_pdist(nullptr) {
	}
	MatElem(DistanceMapType *pMap, ints_vector *pids, sizets_vector *pindexes =
			nullptr, std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_crit(0), m_pdist(pMap) {
		assert(this->m_pdist != nullptr);
		this->m_resids = *pids;
		const size_t n = pids->size();
		assert(n > 0);
		sizets_vector &indexes = this->m_indexes;
		indexes.resize(n);
		CritItemType::generate(n, this->m_args);
		if (pindexes != nullptr) {
			assert(pindexes->size() >= n);
			sizets_vector &oids = *pindexes;
			for (size_t i = 0; i < n; ++i) {
				indexes[i] = oids[i];
			} // i
		} else {
			for (size_t i = 0; i < n; ++i) {
				indexes[i] = i;
			}
		}
		this->m_crit = this->criteria(indexes);
	}
	virtual ~MatElem() {
	}
public:
	ConnectionType connect(const SlotType &subscriber) {
		return m_signal.connect(subscriber);
	}
	DISTANCETYPE criteria(void) const {
		return (this->m_crit);
	}
	const sizets_vector & indexes(void) const {
		return (this->m_indexes);
	}
	void ids(ints_vector &v) const {
		v.clear();
		const sizets_vector &src = this->m_indexes;
		const ints_vector & dest = this->m_resids;
		for (auto it = src.begin(); it != src.end(); ++it) {
			size_t pos = *it;
			assert(pos < dest.size());
			v.push_back(dest[pos]);
		}
	}
	bool process(void) {
		if (this->m_pdist == nullptr) {
			return (false);
		}
		RescritType crit(this->criteria());
		do {
			if (this->check_interrupt()) {
				break;
			}
			if (!this->one_iteration(&crit)) {
				break;
			}
		} while (true);
		return ((this->check_interrupt()) ? false : true);
	} // process
	bool process(DistanceMapType *pMap, ints_vector *pids) {
		assert(pMap != nullptr);
		assert(pids != nullptr);
		this->m_pdist = pMap;
		this->m_resids = *pids;
		const size_t n = pids->size();
		assert(n > 0);
		sizets_vector &indexes = this->m_indexes;
		indexes.resize(n);
		CritItemType::generate(n, this->m_args);
		for (size_t i = 0; i < n; ++i) {
			indexes[i] = i;
		}
		this->m_crit = this->criteria(indexes);
		bool bRet = this->process();
		return (bRet);
	} // process
	bool process(SourceType *pProvider) {
		assert(pProvider != nullptr);
		DistanceMapType oDist(pProvider);
		ints_vector ids;
		oDist.indexes(ids);
		bool bRet = this->process(&oDist, &ids);
		this->m_pdist = nullptr;
		return bRet;
	} // pProvider
	template<typename FUNC>
	bool process_interm(FUNC &&f) {
		if (this->m_pdist == nullptr) {
			return (false);
		}
		RescritType crit(this->criteria());
		do {
			if (this->check_interrupt()) {
				break;
			}
			bool bRet = this->one_iteration(&crit);
			if (this->check_interrupt()) {
				break;
			}
			MatElemResultPtr oPtr(
					new MatElemResultType(crit.load(), this->m_indexes));
			f(oPtr);
			if (!bRet) {
				break;
			}
		} while (true);
		return ((this->check_interrupt()) ? false : true);
	} // process
	bool process_signal(void) {
		if (this->m_pdist == nullptr) {
			return (false);
		}
		RescritType crit(this->criteria());
		do {
			if (this->check_interrupt()) {
				break;
			}
			bool bRet = this->one_iteration(&crit);
			if (this->check_interrupt()) {
				break;
			}
			MatElemResultPtr oPtr(
					new MatElemResultType(crit.load(), this->m_indexes));
			this->m_signal(oPtr);
			if (!bRet) {
				break;
			}
		} while (true);
		return ((this->check_interrupt()) ? false : true);
	} // process
protected:
	bool one_iteration(RescritType *pCrit) {
		pairs_list q;
		if (!this->find_best_try(q, pCrit)) {
			return (false);
		}
		if (this->check_interrupt()) {
			return (false);
		}
		const size_t nx = q.size();
		if (nx < 1) {
			return (false);
		}
		size_t i1 = 0, i2 = 0;
		sizets_pair p = q.front();
		q.pop_front();
		i1 = p.first;
		i2 = p.second;
		if (i1 == i2) {
			return (false);
		}
		this->permute_items(i1, i2);
		this->m_crit = pCrit->load();
		//
		if (!q.empty()) {
			std::atomic_bool *pCancel = this->get_cancelleable_flag();
			DistanceMapType *pDist = this->m_pdist;
			ints_vector *pIds = &(this->m_resids);
			sizets_vector oldIndexes(this->m_indexes);
			sizets_vector *poldIndexes = &oldIndexes;
			size_t nt = q.size();
			if (nt < 2) {
				sizets_pair pp = q.front();
				size_t j1 = pp.first;
				size_t j2 = pp.second;
				q.pop_front();
				if (j1 != j2) {
					MatElemType xMat(pDist, pIds, poldIndexes, pCancel);
					xMat.permute_items(j1, j2);
					xMat.one_iteration(pCrit);
					if (xMat.m_crit < this->m_crit) {
						this->m_crit = xMat.m_crit;
						this->m_indexes = xMat.m_indexes;
					}
				}
			} else {
				using result_type = std::pair<DISTANCETYPE, sizets_vector>;
				using future_type = std::future<result_type>;
				using future_vec = std::vector<future_type>;
				//
				sizets_pair pp = q.front();
				size_t iFirst = pp.first;
				size_t iSecond = pp.second;
				q.pop_front();
				future_vec oTasks;
				while (!q.empty()) {
					sizets_pair pp = q.front();
					size_t j1 = pp.first;
					size_t j2 = pp.second;
					q.pop_front();
					if (j1 != j2) {
						auto tt =
								[j1,j2,pDist,pIds,poldIndexes,pCancel,pCrit]()->result_type {
									MatElemType xMat(pDist, pIds, poldIndexes, pCancel);
									xMat.permute_items(j1, j2);
									xMat.one_iteration(pCrit);
									return (std::make_pair(xMat.m_crit, xMat.m_indexes));
								};
						oTasks.push_back(std::async(std::launch::async,tt));
					} // j1/j2
				} // while
				if (iFirst != iSecond) {
					MatElemType xMat(pDist, pIds, poldIndexes, pCancel);
					xMat.permute_items(iFirst, iSecond);
					xMat.one_iteration(pCrit);
					if (xMat.m_crit < this->m_crit) {
						this->m_crit = xMat.m_crit;
						this->m_indexes = xMat.m_indexes;
					}
				}
				if (!oTasks.empty()) {
					for (future_type & t : oTasks) {
						result_type p = t.get();
						if (!this->check_interrupt()) {
							const DISTANCETYPE cur = p.first;
							if (cur < this->m_crit) {
								this->m_indexes = p.second;
								this->m_crit = cur;
							}
						}
					} // t
				} // not empty
			} // else
		} // more paths
		return ((this->check_interrupt()) ? false : true);
	} //one_iteration
	void permute_items(const size_t i1, const size_t i2) {
		assert(i1 != i2);
		sizets_vector &vv = this->m_indexes;
		const size_t tt = vv[i1];
		vv[i1] = vv[i2];
		vv[i2] = tt;
	} // permute_items
	size_t size(void) {
		return (this->m_indexes.size());
	}
	DISTANCETYPE distance(const size_t i1, const size_t i2) const {
		const ints_vector &oIds = this->m_resids;
		IDTYPE aIndex1 = oIds[i1];
		IDTYPE aIndex2 = oIds[i2];
		DISTANCETYPE dRet = 0;
		this->m_pdist->get(aIndex1, aIndex2, dRet);
		return (dRet);
	}
	DISTANCETYPE criteria(sizets_vector &indexes) const {
		const size_t n = indexes.size();
		DISTANCETYPE dRet = 0;
		if (n < 2) {
			return (0);
		}
		const size_t nx = (size_t) (n - 1);
		for (size_t i = 0; i < nx; ++i) {
			if (this->check_interrupt()) {
				return (dRet);
			}
			const size_t i1 = indexes[i];
			const size_t i2 = indexes[i + 1];
			dRet = (DISTANCETYPE) (dRet + this->distance(i1, i2));
		} // i
		return (dRet);
	} // criteria
	static void best_try_step(int kk, const MatElemType *pMat, pairs_list *pqq,
			std::atomic<DISTANCETYPE> *pCrit, std::mutex *pMutex) {
		const CritItemType &cc = pMat->m_args[kk];
		const size_t i = (size_t) cc.first();
		const size_t j = (size_t) cc.second();
		sizets_vector temp(pMat->m_indexes);
		const size_t tt = temp[i];
		temp[i] = temp[j];
		temp[j] = tt;
		DISTANCETYPE c = pMat->criteria(temp);
		if (c <= pCrit->load()) {
			std::lock_guard<std::mutex> oLock(*pMutex);
			sizets_pair oPair(std::make_pair(i, j));
			DISTANCETYPE old = pCrit->load();
			auto it = std::find_if(pqq->begin(), pqq->end(),
					[i,j](const sizets_pair &p)->bool {
						if ((p.first == i) && (p.second == j)) {
							return (true);
						} else if ((p.first == j) && (p.second == i)) {
							return (true);
						} else {
							return (false);
						}
					});
			if (it == pqq->end()) {
				if ((c == old) && (!pqq->empty())) {
					pqq->push_back(oPair);
				} else if (c < old) {
					pCrit->store(c);
					pqq->clear();
					pqq->push_back(oPair);
				}
			} // may add
		} // candidate
	} // best_try_ste
	bool find_best_try_serial(pairs_list &qq,
			std::atomic<DISTANCETYPE> *pCrit) const {
		const sizets_vector &indexes = this->m_indexes;
		crititems_vector &args = const_cast<crititems_vector &>(this->m_args);
		const size_t nn = args.size();
		DISTANCETYPE oldCrit = pCrit->load();
		for (size_t kk = 0; kk < nn; ++kk) {
			const CritItemType &cc = args[kk];
			const size_t i = (size_t) cc.first();
			const size_t j = (size_t) cc.second();
			sizets_vector temp(indexes);
			const size_t tt = temp[i];
			temp[i] = temp[j];
			temp[j] = tt;
			DISTANCETYPE c = this->criteria(temp);
			if (c <= oldCrit) {
				sizets_pair oPair(std::make_pair(i, j));
				auto it = std::find_if(qq.begin(), qq.end(),
						[i,j](const sizets_pair &p)->bool {
							if ((p.first == i) && (p.second == j)) {
								return (true);
							} else if ((p.first == j) && (p.second == i)) {
								return (true);
							} else {
								return (false);
							}
						});
				if (it == qq.end()) {
					if ((c == oldCrit) && (!qq.empty())) {
						qq.push_back(oPair);
					} else if (c < oldCrit) {
						oldCrit = c;
						qq.clear();
						qq.push_back(oPair);
					}
				} // may add
			} // candidate
		} // kk
		pCrit->store(oldCrit);
		return (!qq.empty());
	} //find_best_try_serial
	bool find_best_try(pairs_list &qq, std::atomic<DISTANCETYPE> *pCrit) const {
		using future_type = std::future<bool>;
		using futures_vec = std::vector<future_type>;
		//
		qq.clear();
		crititems_vector &args = const_cast<crititems_vector &>(this->m_args);
		const size_t nn = args.size();
		size_t np = std::thread::hardware_concurrency();
		if ((np < 2) || (nn <= np)) {
			return this->find_best_try_serial(qq, pCrit);
		}
		int nChunk = (int) (nn / np);
		std::mutex _mutex;
		pairs_list *pqq = &qq;
		std::mutex *pMutex = &_mutex;
		const MatElemType *pMat = this;
		int nLast = (int) nn;
		int iMainEnd = nChunk;
		if (iMainEnd > nLast) {
			iMainEnd = nLast;
		}
		int nStart = iMainEnd;
		futures_vec oTasks;
		while (nStart < nLast) {
			int nEnd = nStart + nChunk;
			if (nEnd > nLast) {
				nEnd = nLast;
			}
			auto fTask = [nStart,nEnd,pMat,&pqq,&pCrit,&pMutex]()->bool {
				for (int k = nStart; k < nEnd; ++k) {
					best_try_step(k, pMat,pqq,pCrit,pMutex);
				} // k
					return (true);
				};
			oTasks.push_back(std::async(std::launch::async,fTask));
			nStart = nEnd;
		} // while nStart
		for (int i = 0; i < iMainEnd; ++i) {
			best_try_step(i, pMat, pqq, pCrit, pMutex);
		} // i
		bool bRet = true;
		for (future_type &t : oTasks) {
			bRet = bRet && t.get();
		} //t
		return (!qq.empty());
	} //find_best_try
};

/////////////////////////////////////////
}// namespace infi
///////////////////////////////////
#endif // !__MATELEM_H__


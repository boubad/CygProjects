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
	ints_vector *m_pids;
	sizets_vector m_indexes;
	crititems_vector m_args;
	SignalType m_signal;
public:
	MatElem() :
			m_crit(0), m_pdist(nullptr), m_pids(nullptr) {
	}
	MatElem(DistanceMapType *pMap, ints_vector *pids, sizets_vector *pindexes =
			nullptr, std::atomic_bool *pCancel = nullptr) :
			InterruptObject(pCancel), m_crit(0), m_pdist(pMap), m_pids(pids) {
		assert(this->m_pdist != nullptr);
		assert(this->m_pids != nullptr);
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
	ConnectionType connect( const SlotType &subscriber) {
		return m_signal.connect(subscriber);
	}
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
	bool one_iteration(RescritType *pCrit) {
		assert(pCrit != nullptr);
		using result_type = std::pair<DISTANCETYPE, sizets_vector>;
		using future_type = std::future<result_type>;
		using future_vec = std::vector<future_type>;
		//
		if (this->check_interrupt()) {
			return (false);
		}
		//
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		DistanceMapType *pDist = this->m_pdist;
		ints_vector *pIds = this->m_pids;
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
		if (this->check_interrupt()) {
			return (false);
		}
		//
		if (!q.empty()) {
			sizets_vector oldIndexes(this->m_indexes);
			future_vec oTasks;
			while (!q.empty()) {
				if (this->check_interrupt()) {
					break;
				}
				sizets_pair pp = q.front();
				size_t j1 = pp.first;
				size_t j2 = pp.second;
				q.pop_front();
				if (j1 != j2) {
					oTasks.push_back(std::async([&]()->result_type {
						MatElemType xMat(pDist, pIds, &oldIndexes, pCancel);
						xMat.permute_items(j1, j2);
						xMat.one_iteration(pCrit);
						return (std::make_pair(xMat.m_crit, xMat.m_indexes));
					}));
				} // j1/j2
			} // while
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
		} // more paths
		return ((this->check_interrupt()) ? false : true);
	} //one_iteration
	bool process() {
		RescritType crit(this->criteria());
		return internal_process(&crit);
	} // process
	template<typename FUNC>
	bool process_interm(FUNC &&f) {
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
	bool internal_process(std::atomic<DISTANCETYPE> *pCrit) {
		using result_type = std::pair<DISTANCETYPE, sizets_vector>;
		using future_type = std::future<result_type>;
		using future_vec = std::vector<future_type>;
		//
		std::atomic_bool *pCancel = this->get_cancelleable_flag();
		DistanceMapType *pDist = this->m_pdist;
		ints_vector *pIds = this->m_pids;
		do {
			if (this->check_interrupt()) {
				break;
			}
			pairs_list q;
			if (!this->find_best_try(q, pCrit)) {
				break;
			}
			if (this->check_interrupt()) {
				break;
			}
			const size_t nx = q.size();
			if (nx < 1) {
				break;
			}
			size_t i1 = 0, i2 = 0;
			sizets_pair p = q.front();
			q.pop_front();
			i1 = p.first;
			i2 = p.second;
			const DISTANCETYPE crit = pCrit->load();
			if (i1 == i2) {
				break;
			}
			this->permute_items(i1, i2);
			this->m_crit = crit;
			if (this->check_interrupt()) {
				break;
			}
			//
			if (!q.empty()) {
				sizets_vector oldIndexes(this->m_indexes);
				future_vec oTasks;
				while (!q.empty()) {
					if (this->check_interrupt()) {
						break;
					}
					sizets_pair pp = q.front();
					size_t j1 = pp.first;
					size_t j2 = pp.second;
					q.pop_front();
					if (j1 != j2) {
						oTasks.push_back(
								std::async(
										[&]()->result_type {
											MatElemType xMat(pDist, pIds, &oldIndexes, pCancel);
											xMat.permute_items(j1, j2);
											xMat.internal_process(pCrit);
											return (std::make_pair(xMat.m_crit, xMat.m_indexes));
										}));
					} // j1/j2
				} // while
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
			} // more paths
		} while (true);
		return ((this->check_interrupt()) ? false : true);
	} // process
	void permute_items(const size_t i1, const size_t i2) {
		assert(i1 != i2);
		if (this->check_interrupt()) {
			return;
		}
		sizets_vector &vv = this->m_indexes;
		assert(i1 < vv.size());
		assert(i2 < vv.size());
		const size_t tt = vv[i1];
		vv[i1] = vv[i2];
		vv[i2] = tt;
	} // permute_items
	size_t size(void) {
		assert(this->m_pids != nullptr);
		return (this->m_pids->size());
	}
	DISTANCETYPE distance(const size_t i1, const size_t i2, ints_vector *pinds =
			nullptr) const {
		IDTYPE aIndex1, aIndex2;
		if (pinds == nullptr) {
			ints_vector &oIds = *(this->m_pids);
			aIndex1 = oIds[i1];
			aIndex2 = oIds[i2];
		} else {
			ints_vector &oIds = *(pinds);
			aIndex1 = oIds[i1];
			aIndex2 = oIds[i2];
		}
		DISTANCETYPE dRet = 0;
		this->m_pdist->get(aIndex1, aIndex2, dRet);
		return (dRet);
	}
	bool fingerprint(const size_t ii1, const size_t ii2,
			DISTANCETYPE &delta) const {
		if (ii1 == ii2) {
			delta = 0;
			return (false);
		}
		ints_vector &oIds = *(this->m_pids);
		const size_t n = oIds.size();
		const size_t nm1 = (size_t) (n - 1);
		DISTANCETYPE dRetOld = 0, dRetNew = 0, d = 0;
		size_t i1 = ii1, i2 = ii2;
		if (i1 > i2) {
			const size_t t = i1;
			i1 = i2;
			i2 = t;
		}
		if (i1 > 0) {
			d = this->distance(i1 - 1, i1);
			dRetOld += d;
			d = this->distance(i1 - 1, i2);
			dRetNew += d;
		}
		d = this->distance(i1, i1 + 1);
		dRetOld += d;
		d = this->distance(i2, i1 + 1);
		dRetNew += d;
		d = this->distance(i2 - 1, i2);
		dRetOld += d;
		d = this->distance(i2 - 1, i1);
		dRetNew += d;
		if (i2 < nm1) {
			d = this->distance(i2, i2 + 1);
			dRetOld += d;
			d = this->distance(i1, i2 + 1);
			dRetNew += d;
		}
		bool bRet = false;
		if (dRetOld >= dRetNew) {
			bRet = true;
			delta = (DISTANCETYPE) (dRetOld - dRetNew);
		} else {
			delta = (DISTANCETYPE) (dRetNew - dRetOld);
		}
		return (bRet);
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
#if defined(__CYGWIN__)
	bool find_best_try(pairs_list &qq, std::atomic<DISTANCETYPE> *pCrit) const {
		qq.clear();
		const sizets_vector &indexes = this->m_indexes;
		crititems_vector &args = const_cast<crititems_vector &>(this->m_args);
		const int nn = (int) args.size();
		std::mutex _mutex;
#pragma omp parallel for
		for (int kk = 0; kk < nn; ++kk) {
			const CritItemType &cc = args[kk];
			const size_t i = (size_t) cc.first();
			const size_t j = (size_t) cc.second();
			sizets_vector temp(indexes);
			const size_t tt = temp[i];
			temp[i] = temp[j];
			temp[j] = tt;
			DISTANCETYPE c = this->criteria(temp);
			if (c <= pCrit->load()) {
				sizets_pair oPair(std::make_pair(i, j));
//#pragma omp critical
				{
					DISTANCETYPE old = pCrit->load();
					std::lock_guard<std::mutex> oLock(_mutex);
					auto it =
							std::find_if(qq.begin(), qq.end(),
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
						if ((c == old) && (!qq.empty())) {
							qq.push_back(oPair);
						} else if (c < old) {
							pCrit->store(c);
							qq.clear();
							qq.push_back(oPair);
						}
					}
				} // sync
			} // check
		} // i
		return (!qq.empty());
	} //find_best_try
	bool find_best_try(pairs_list &qq) const {
		qq.clear();
		crititems_vector &args = const_cast<crititems_vector &>(this->m_args);
		const int nn = (int) args.size();
		RescritType delta(0);
		std::mutex _mutex;
		std::atomic_bool bFirst(true);
//#pragma omp parallel for
		for (int kk = 0; kk < nn; ++kk) {
			const CritItemType &cc = args[kk];
			const size_t i = (size_t) cc.first();
			const size_t j = (size_t) cc.second();
			DISTANCETYPE d = 0;
			if (this->fingerprint(i, j, d)) {
				sizets_pair oPair(std::make_pair(i, j));
				if (bFirst.load()) {
					std::lock_guard<std::mutex> oLock(_mutex);
					bFirst.store(false);
					delta.store(d);
					qq.push_back(oPair);
				} else if (d > delta.load()) {
					std::lock_guard<std::mutex> oLock(_mutex);
					qq.clear();
					qq.push_back(oPair);
					delta.store(d);
				} else if (d == delta.load()) {
					std::lock_guard<std::mutex> oLock(_mutex);
					auto it =
							std::find_if(qq.begin(), qq.end(),
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
						if (!qq.empty()) {
							qq.push_back(oPair);
						}
					}
				}
			} // progress
		} // kk
		return (!qq.empty());
	} //find_best_try

#else
	bool find_best_try(pairs_list &qq, std::atomic<DISTANCETYPE> *pCrit) const {
		qq.clear();
		const sizets_vector &indexes = this->m_indexes;
		crititems_vector &args = const_cast<crititems_vector &>(this->m_args);
		std::mutex _mutex;
		info_parallel_for_each(args.begin(), args.end(),
				[&](const CritItem &cc) {
					const size_t i = (size_t) cc.first();
					const size_t j = (size_t) cc.second();
					sizets_vector temp(indexes);
					const size_t tt = temp[i];
					temp[i] = temp[j];
					temp[j] = tt;
					DISTANCETYPE c = this->criteria(temp);
					if (c <= pCrit->load()) {
						sizets_pair oPair(std::make_pair(i, j));
						{
							std::lock_guard<std::mutex> oLock(_mutex);
							DISTANCETYPE old = pCrit->load();
							auto it =
							std::find_if(qq.begin(), qq.end(),
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
								if ((c == old) && (!qq.empty())) {
									qq.push_back(oPair);
								} else if (c < old) {
									pCrit->store(c);
									qq.clear();
									qq.push_back(oPair);
								}
							}
						} // sync
					} // check
				});
		return (!qq.empty());
	} //find_best_try
#endif // __CYGWIN__

};

/////////////////////////////////////////
}// namespace infi
///////////////////////////////////
#endif // !__MATELEM_H__


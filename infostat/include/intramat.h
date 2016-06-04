/*
 * intramat.h
 *
 *  Created on: 31 mai 2016
 *      Author: boubad
 */

#ifndef INTRAMAT_H_
#define INTRAMAT_H_

#include "indiv.h"
#include "distancemap.h"
/////////////////////////////////
#include <boost/signals2/signal.hpp>
//////////////////////////////////
namespace info {
///////////////////////////////////////////
template<typename DISTANCETYPE = long>
class IntraMatElemResult {
public:
	using sizets_vector = std::vector<size_t>;
	using IntraMatElemResultType = IntraMatElemResult<DISTANCETYPE>;
	using IntraMatElemResultPtr = std::shared_ptr<IntraMatElemResultType>;
	//
	DISTANCETYPE first;
	sizets_vector second;
public:
	IntraMatElemResult() :
			first(0) {
	}
	IntraMatElemResult(const DISTANCETYPE c, const sizets_vector &v) :
			first(c), second(v) {
	}
	IntraMatElemResult(const IntraMatElemResultType &other) :
			first(other.first), second(other.second) {
	}
	IntraMatElemResultType & operator=(const IntraMatElemResultType &other) {
		if (this != &other) {
			this->first = other.first;
			this->second = other.second;
		}
		return (*this);
	}
	virtual ~IntraMatElemResult() {
	}
};
// class IntraMatElemResult
/////////////////////////////////////
template<typename IDTYPE = unsigned long, typename DISTANCETYPE = long,
		typename STRINGTYPE = std::string>
class IntraMatElem {
public:
	using sizets_pair = std::pair<size_t, size_t>;
	using pairs_list = std::list<sizets_pair>;
	using ints_vector = std::vector<IDTYPE>;
	using sizets_vector = std::vector<size_t>;
	using DistanceMapType = DistanceMap<IDTYPE, DISTANCETYPE>;
	using IndivType = Indiv<IDTYPE, STRINGTYPE>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
	using IntraMatElemType = IntraMatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using RescritType = std::atomic<DISTANCETYPE>;
	using IntraMatElemResultType = IntraMatElemResult<DISTANCETYPE>;
	using IntraMatElemResultPtr = typename IntraMatElemResultType::IntraMatElemResultPtr;
	using SignalType = typename boost::signals2::signal<void (IntraMatElemResultPtr)>;
	using SlotType = typename SignalType::slot_type;
	using ConnectionType = boost::signals2::connection;
private:
	std::atomic_bool m_cancel;
	DISTANCETYPE m_crit;
	DistanceMapType *m_pdist;
	sizets_vector m_indexes;
	SignalType m_signal;
	std::unique_ptr<DistanceMapType> m_odist;

	//
public:
	IntraMatElem() :
			m_cancel(false), m_crit(0), m_pdist(nullptr) {
	}
	virtual ~IntraMatElem() {
	}
public:
	void cancel(void) {
		this->m_cancel = true;
	} // cancel
	bool is_cancelled(void) const {
		return (this->m_cancel.load());
	}
	ConnectionType connect(const SlotType &subscriber) {
		return m_signal.connect(subscriber);
	}

	DISTANCETYPE criteria(void) const {
		return (this->m_crit);
	}
	void indexes(sizets_vector &oIndex) {
		oIndex = this->m_indexes;
	}
	void arrange(SourceType *pProvider) {
		this->m_cancel.store(false);
		this->initialize(pProvider);
		DISTANCETYPE oCrit(this->m_crit);
		do {
			IntraMatElemResultPtr res = this->one_iteration(oCrit);
			this->m_signal(res);
			if (this->m_cancel.load()) {
				break;
			}
			IntraMatElemResultType *pRes = res.get();
			if (pRes == nullptr) {
				break;
			}
		} while (true);
	} // arrange
	static IntraMatElemResultPtr perform_arrange(SourceType *pProvider) {
		IntraMatElemType oMat;
		oMat.arrange(pProvider);
		IntraMatElemResultPtr oRet(
				new IntraMatElemResultType(oMat.m_crit, oMat.m_indexes));
		return (oRet);
	} //perform_arrange
protected:
	IntraMatElemResultPtr one_iteration(DISTANCETYPE &oCrit) {
		IntraMatElemResultPtr oRet;
		pairs_list q;
		bool bRet = this->find_best_try(q, oCrit);
		if (!bRet) {
			return (oRet);
		}
		if (this->m_cancel.load()) {
			return (oRet);
		}
		const size_t nx = q.size();
		if (nx < 1) {
			return (oRet);
		}
		size_t i1 = 0, i2 = 0;
		sizets_pair p = q.front();
		q.pop_front();
		i1 = p.first;
		i2 = p.second;
		if (i1 == i2) {
			return (oRet);
		}
		if (oCrit >= this->m_crit){
			return (oRet);
		}
		this->permute_items(i1, i2);
		this->m_crit = oCrit;
		//
		if (!q.empty()) {
			sizets_vector oldIndexes(this->m_indexes);
			while (!q.empty()) {
				sizets_pair pp = q.front();
				size_t j1 = pp.first;
				size_t j2 = pp.second;
				q.pop_front();
				if (j1 != j2) {
					IntraMatElemType xMat;
					xMat.m_pdist = this->m_pdist;
					xMat.m_indexes = oldIndexes;
					xMat.permute_items(j1, j2);
					xMat.m_crit = xMat.criteria(xMat.m_indexes);
					xMat.one_iteration(oCrit);
					if (xMat.m_crit < this->m_crit) {
						this->m_crit = xMat.m_crit;
						this->m_indexes = xMat.m_indexes;
					}
				}
			} // more paths
		} // not empty
		if (this->m_cancel.load()) {
			return (oRet);
		}
		oRet.reset(new IntraMatElemResultType(this->m_crit, this->m_indexes));
		return (oRet);
	} //one_iteration
	bool find_best_try(pairs_list &qq, DISTANCETYPE &oCrit) {
		const sizets_vector &indexes = this->m_indexes;
		const size_t n = this->m_pdist->size();
		DISTANCETYPE oldCrit = oCrit;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < i; ++j) {
				sizets_vector temp(indexes);
				const size_t tt = temp[i];
				temp[i] = temp[j];
				temp[j] = tt;
				DISTANCETYPE c = this->criteria(temp);
				if (c <= oldCrit) {
					auto it =
							std::find_if(qq.begin(), qq.end(),
									[i,j](const sizets_pair &p)->bool {
										if ((p.first == (size_t)i) && (p.second == (size_t)j)) {
											return (true);
										} else if ((p.first == (size_t)j) && (p.second == (size_t)i)) {
											return (true);
										} else {
											return (false);
										}
									});
					if (it == qq.end()) {
						if ((c == oldCrit) && (!qq.empty())) {
							sizets_pair oPair(std::make_pair(i, j));
							qq.push_back(oPair);
						} else if (c < oldCrit) {
							oldCrit = c;
							qq.clear();
							sizets_pair oPair(std::make_pair(i, j));
							qq.push_back(oPair);
						}
					} // may add
				} // candidate
			} // j
		} // i
		oCrit = oldCrit;
		if (this->m_cancel.load()) {
			return (false);
		}
		return (!qq.empty());
	} //find_best_try_serial
private:
	DISTANCETYPE distance(const size_t i1, const size_t i2) const {
		DISTANCETYPE d = 0;
		this->m_pdist->indexed_get(i1, i2, d);
		return (d);
	} // distance
	DISTANCETYPE criteria(const sizets_vector &indexes) {
		const size_t n = indexes.size();
		DISTANCETYPE dRet = 0;
		if (n < 2) {
			return (dRet);
		}
		const size_t nx = (size_t) (n - 1);
		for (size_t i = 0; i < nx; ++i) {
			if (this->m_cancel.load()) {
				return (dRet);
			}
			const size_t i1 = indexes[i];
			const size_t i2 = indexes[i + 1];
			dRet = (DISTANCETYPE) (dRet + this->distance(i1, i2));
		} // i
		return (dRet);
	} // criteria
	void permute_items(const size_t i1, const size_t i2) {
		sizets_vector &vv = this->m_indexes;
		const size_t tt = vv[i1];
		vv[i1] = vv[i2];
		vv[i2] = tt;
	} // permute_items
	void initialize(DistanceMapType *pDist) {
		this->m_pdist = pDist;
		const size_t n = pDist->size();
		sizets_vector & indexes = this->m_indexes;
		indexes.resize(n);
		for (size_t i = 0; i < n; ++i) {
			indexes[i] = i;
		} // i
		this->m_crit = this->criteria(indexes);
	} // initialize
	void initialize(SourceType *pProvider) {
		this->m_odist.reset(new DistanceMapType(pProvider));
		DistanceMapType *pDist = this->m_odist.get();
		initialize(pDist);
	} // initialize
};

/////////////////////////////////////////
}// namespace infi
///////////////////////////////////

#endif /* INTRAMAT_H_ */

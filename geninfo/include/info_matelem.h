/*
 * info_matelem.h
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */

#ifndef INFO_MATELEM_H_
#define INFO_MATELEM_H_
////////////////////////////////////////
#include "matresult.h"
#include "indiv.h"
#include "distancemap.h"
#include "indivmap.h"
#include "datavector_indivprovider.h"
/////////////////////////////////
namespace info {
///////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
class MatElem: public MatElemObject<IDTYPE, DISTANCETYPE, STRINGTYPE> {
public:
	using cancelflag = std::atomic<bool>;
	using pcancelflag = cancelflag *;
	using PBackgrounder = Backgrounder *;
	using sizets_pair = std::pair<size_t, size_t>;
	using pairs_list = std::list<sizets_pair>;
	using ints_vector = std::vector<IDTYPE>;
	using sizets_vector = std::vector<size_t>;
	using DistanceMapType = DistanceMap<IDTYPE, DISTANCETYPE>;
	using IndivType = Indiv<IDTYPE, STRINGTYPE>;
	using IndivTypePtr = std::shared_ptr<IndivType>;
	using SourceType = IIndivSource<IDTYPE, STRINGTYPE>;
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultType = MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using aMatElemResultPtr = std::shared_ptr<MatElemResultType>;
	using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
	using IndivMapType = IndivMap<IDTYPE, STRINGTYPE, DISTANCETYPE>;
	using BaseType = MatElemObject<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using ints_doubles_map = std::map<IDTYPE, double>;
	using ints_vector = std::vector<IDTYPE>;
	using strings_vector = std::vector<STRINGTYPE>;
	using DataSourceType = DataVectorIndivSource<IDTYPE, STRINGTYPE>;
private:
	DispositionType m_disp;
	DISTANCETYPE m_crit;
	DistanceMapType *m_pdist;
	STRINGTYPE m_sigle;
	sizets_vector m_indexes;
	std::unique_ptr<DistanceMapType> m_odist;
public:
	MatElem(DispositionType disp = DispositionType::invalid, pcancelflag pFlag =
			nullptr, PBackgrounder pq = nullptr, MatElemFunctionType f =
			[](MatElemResultPtr o) {}) :
			BaseType(pFlag, pq, f), m_disp(disp), m_crit(0), m_pdist(nullptr) {
	}
	virtual ~MatElem() {
	}
public:
	void sigle(const STRINGTYPE &s) {
		this->m_sigle = s;
	}
	MatElemResultPtr getResult(
			StageType stage = StageType::current) const {
		MatElemResultPtr oRet = std::make_shared<MatElemResultType>();
		MatElemResultType *p = oRet.get();
		assert(p != nullptr);
		p->stage(stage);
		p->disposition(this->m_disp);
		p->sigle(this->m_sigle);
		p->criteria(this->m_crit);
		p->indexes(this->m_indexes);
		return (oRet);
	}
	DISTANCETYPE criteria(void) const {
		return (this->m_crit);
	}
	void indexes(sizets_vector &oIndex) {
		oIndex = this->m_indexes;
	}
	void arrange(void) {
		if (this->m_pdist == nullptr) {
			return;
		}
		if (this->is_cancelled()) {
			return;
		}
		DISTANCETYPE oCrit(this->m_crit);
		this->notify(StageType::started);
		do {
			if (!this->one_iteration(oCrit)) {
				break;
			}
			if (this->is_cancelled()) {
				break;
			}
			this->notify(StageType::current);
		} while (true);
		this->notify(StageType::finished);
	} // arrange
	void arrange(SourceType *pProvider) {
		if (pProvider == nullptr) {
			return;
		}
		this->initialize(pProvider);
		this->arrange();
	} // arrange
	void arrange(IndivMapType *pMap) {
		if (pMap == nullptr) {
			return;
		}
		this->initialize(pMap);
		this->arrange();
	} // arrange
	void arrange(DistanceMapType *pDist) {
		if (pDist == nullptr) {
			return;
		}
		this->initialize(pDist);
		this->arrange();
	} // arrange
	template<typename T, typename F>
	void arrange(size_t nRows, size_t nCols, const std::vector<T> &oData,
			const strings_vector &names, const std::vector<F> &weights,
			bool bComputeWeights = false) {
		assert(nRows > 0);
		assert(nCols > 0);
		assert(oData.size() >= (size_t )(nRows * nCols));
		assert(names.size() >= nRows);
		ints_vector indids(nRows), varids(nCols);
		ints_doubles_map oWeights, ww;
		for (size_t i = 0; i < nRows; ++i) {
			IDTYPE key = (IDTYPE) (i + 1);
			indids[i] = key;
		} // i
		for (size_t i = 0; i < nCols; ++i) {
			IDTYPE key = (IDTYPE) (i + 1);
			varids[i] = key;
		} // i
		bool bOk = true;
		double somme = 0.0;
		if (weights.size() < nCols) {
			bOk = false;
			double vf = 1.0 / nCols;
			for (size_t i = 0; i < nCols; ++i) {
				IDTYPE key = (IDTYPE) (i + 1);
				ww[key] = vf;
				somme += vf;
			} // i
		} else {
			for (size_t i = 0; i < nCols; ++i) {
				double c = (double) weights[i];
				if (c < 0.0) {
					bOk = false;
					break;
				}
				IDTYPE key = (IDTYPE) (i + 1);
				ww[key] = c;
				somme += c;
			} // i
		}
		if (bComputeWeights) {
			bOk = true;
			for (size_t i = 0; i < nCols; ++i) {
				double s1 = 0.0;
				double s2 = 0.0;
				for (size_t j = 0; j < nRows; ++j) {
					double x = (double) oData[j * nCols + i];
					s1 += x;
					s2 += x * x;
				} // j
				s1 /= nRows;
				s2 = (s2 / nRows) - (s1 * s1);
				if (s2 <= 0.0) {
					bOk = false;
					break;
				}
				IDTYPE key = (IDTYPE) (i + 1);
				double ff = 1.0 / s2;
				ww[key] = ff;
				somme += ff;
			} // i
		} // bComputeWeights
		if (bOk && (somme <= 0.0)) {
			bOk = false;
		}
		if (!bOk) {
			double vf = 1.0 / nCols;
			somme = 0.0;
			for (size_t i = 0; i < nCols; ++i) {
				IDTYPE key = (IDTYPE) (i + 1);
				oWeights[key] = vf;
				somme += vf;
			} // i
		} else {
			for (auto it = ww.begin(); it != ww.end(); ++it) {
				const std::pair<IDTYPE, double> &p = *it;
				oWeights[p.first] = p.second / somme;
			} // it
		}
		DataSourceType oProvider(nRows, nCols, oData, indids, varids, names,
				oWeights);
		oProvider.recode(1000);
		this->arrange(&oProvider);
	} // arrange
protected:
	void notify(StageType stage = StageType::current) {
		if (!this->is_cancelled()) {
			IntraMatElemResultPtr res = this->getResult(stage);
			this->send(res);
		}
	} //notify
	bool one_iteration(DISTANCETYPE &oCrit) {
		if (this->is_cancelled()) {
			return (false);
		}
		//
		pairs_list q;
		bool bRet = this->find_best_try(q, oCrit);
		if (this->is_cancelled()) {
			return (false);
		}
		if (!bRet) {
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
		if (oCrit >= this->m_crit) {
			return (false);
		}
		this->permute_items(i1, i2);
		this->m_crit = oCrit;
		//
		if (!q.empty()) {
			sizets_vector oldIndexes(this->m_indexes);
			DISTANCETYPE oldCrit(oCrit);
			while (!q.empty()) {
				if (this->is_cancelled()) {
					return (false);
				}
				sizets_pair pp = q.front();
				size_t j1 = pp.first;
				size_t j2 = pp.second;
				q.pop_front();
				if (j1 != j2) {
					MatElemType xMat;
					xMat.m_pdist = this->m_pdist;
					xMat.m_indexes = oldIndexes;
					xMat.permute_items(j1, j2);
					xMat.m_crit = xMat.criteria(xMat.m_indexes);
					xMat.one_iteration(oldCrit);
					if (xMat.m_crit < this->m_crit) {
						this->m_crit = xMat.m_crit;
						this->m_indexes = xMat.m_indexes;
					}
				}
			} // more paths
		} // not empty
		return (true);
	} //one_iteration
	bool find_best_try(pairs_list &qq, DISTANCETYPE &oCrit) {
		const sizets_vector indexes(this->m_indexes);
		const size_t n = this->m_pdist->size();
		DISTANCETYPE oldCrit = oCrit;
		for (size_t i = 0; i < n; ++i) {
			if (this->is_cancelled()) {
				qq.clear();
				return (false);
			}
			for (size_t j = 0; j < i; ++j) {
				sizets_vector temp(indexes);
				if (temp.size() < n) {
					return (false);
				}
				if (this->is_cancelled()) {
					qq.clear();
					return (false);
				}
				const size_t tt = temp[i];
				temp[i] = temp[j];
				temp[j] = tt;
				DISTANCETYPE c = this->criteria(temp);
				if (this->is_cancelled()) {
					qq.clear();
					return (false);
				}
				if (c <= oldCrit) {
					auto it =
							std::find_if(qq.begin(), qq.end(),
									[i, j](const sizets_pair &p)->bool {
										if ((p.first == (size_t)i) && (p.second == (size_t)j)) {
											return (true);
										}
										else if ((p.first == (size_t)j) && (p.second == (size_t)i)) {
											return (true);
										}
										else {
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
		return (!qq.empty());
	} //find_best_try_serial
private:
	DISTANCETYPE distance(const size_t i1, const size_t i2) const {
		assert(this->m_pdist != nullptr);
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
		assert(pDist != nullptr);
		this->m_pdist = pDist;
		const size_t n = pDist->size();
		sizets_vector & indexes = this->m_indexes;
		indexes.resize(n);
		for (size_t i = 0; i < n; ++i) {
			indexes[i] = i;
		} // i
		this->m_crit = this->criteria(indexes);
	} // initialize
	void initialize(IndivMapType *pMap) {
		assert(pMap != nullptr);
		m_odist.reset(new DistanceMapType());
		DistanceMapType *pDist = m_odist.get();
		assert(pDist != nullptr);
		pMap->distance_map(*pDist);
		this->initialize(pDist);
	} // initialize
	void initialize(SourceType *pProvider) {
		assert(pProvider != nullptr);
		this->m_odist.reset(new DistanceMapType(pProvider));
		DistanceMapType *pDist = this->m_odist.get();
		assert(pDist != nullptr);
		initialize(pDist);
	} // initialize
};
///////////////////////////////////////
}// namespace infO
///////////////////////////////////

#endif /* INFO_MATELEM_H_ */

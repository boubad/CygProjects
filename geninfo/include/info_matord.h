/*
 * info_matord.h
 *
 *  Created on: 15 juin 2016
 *      Author: boubad
 */

#ifndef INFO_MATORD_H_
#define INFO_MATORD_H_
/////////////////////////////
#include "info_matelem.h"
/////////////////////////////////
namespace info {
///////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE>
class InfoMatrice: public MatElemObject<IDTYPE, DISTANCETYPE, STRINGTYPE> {
public:
	using BaseType = MatElemObject<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using cancelflag = std::atomic<bool>;
	using pcancelflag = cancelflag *;
	using PBackgrounder = Backgrounder *;
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	//
	using DistanceMapType = typename MatElemType::DistanceMapType;
	using MatElemResultType = typename MatElemType::MatElemResultType;
	using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
	using MatElemFunctionType = std::function<void(MatElemResultPtr)>;
	using SourceType = typename MatElemType::SourceType;
	using ints_vector = typename MatElemType::ints_vector;
	using sizets_vector = typename MatElemType::sizets_vector;
	using IndivType = typename MatElemType::IndivType;
	using IndivTypePtr = typename MatElemType::IndivTypePtr;
	using InfoMatriceType = InfoMatrice<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using IndivMapType = IndivMap<IDTYPE, STRINGTYPE, DISTANCETYPE>;
	//
	using ints_doubles_map = std::map<IDTYPE, double>;
	using strings_vector = std::vector<STRINGTYPE>;
	using DataSourceType = DataVectorIndivSource<IDTYPE, STRINGTYPE>;
	//
	using InfoMatriceResultType = std::tuple<MatElemResultPtr,MatElemResultPtr>;
private:
	STRINGTYPE m_sigle;
	std::unique_ptr<MatElemType> m_vars;
	std::unique_ptr<MatElemType> m_inds;
protected:
	bool prep_vars(SourceType *pProvider) {
		pcancelflag pCancel = this->get_cancelflag();
		PBackgrounder pBack = this->get_backgrounder();
		MatElemFunctionType ff = this->get_function();
		this->m_vars.reset(
				new MatElemType(DispositionType::variable, pCancel, pBack, ff));
		MatElemType *pMat = this->m_vars.get();
		assert(pMat != nullptr);
		pMat->sigle(this->m_sigle);
		pMat->arrange(pProvider);
		return (true);
	} // prep_vars
	bool prep_vars(IndivMapType *pProvider) {
		pcancelflag pCancel = this->get_cancelflag();
		PBackgrounder pBack = this->get_backgrounder();
		MatElemFunctionType ff = this->get_function();
		this->m_vars.reset(
				new MatElemType(DispositionType::variable, pCancel, pBack, ff));
		MatElemType *pMat = this->m_vars.get();
		assert(pMat != nullptr);
		pMat->sigle(this->m_sigle);
		pMat->arrange(pProvider);
		return (true);
	} // prep_vars
	bool prep_inds(SourceType *pProvider) {
		pcancelflag pCancel = this->get_cancelflag();
		PBackgrounder pBack = this->get_backgrounder();
		MatElemFunctionType ff = this->get_function();
		this->m_inds.reset(
				new MatElemType(DispositionType::indiv, pCancel, pBack, ff));
		MatElemType *pMat = this->m_inds.get();
		assert(pMat != nullptr);
		pMat->sigle(this->m_sigle);
		pMat->arrange(pProvider);
		return (true);
	} // prep_inds
	bool prep_inds(IndivMapType *pProvider) {
		pcancelflag pCancel = this->get_cancelflag();
		PBackgrounder pBack = this->get_backgrounder();
		MatElemFunctionType ff = this->get_function();
		this->m_inds.reset(
				new MatElemType(DispositionType::indiv, pCancel, pBack, ff));
		MatElemType *pMat = this->m_inds.get();
		assert(pMat != nullptr);
		pMat->sigle(this->m_sigle);
		pMat->arrange(pProvider);
		return (true);
	} // prep_inds
public:
	InfoMatrice(pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			MatElemFunctionType f = [](MatElemResultPtr o) {},
			const STRINGTYPE &sSigle = STRINGTYPE()) :
			BaseType(pFlag, pq, f), m_sigle(sSigle) {
	} // MatOrd
	InfoMatrice(SourceType *pIndsSource, SourceType *pVarsSource,
			pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			MatElemFunctionType f = [](MatElemResultPtr o) {},
			const STRINGTYPE &sSigle = STRINGTYPE()) :
			BaseType(pFlag, pq, f), m_sigle(sSigle) {
		this->arrange(pIndsSource, pVarsSource);
	} // MatOrd
	InfoMatrice(IndivMapType *pIndsSource, IndivMapType *pVarsSource,
			pcancelflag pFlag = nullptr, PBackgrounder pq = nullptr,
			MatElemFunctionType f = [](MatElemResultPtr o) {},
			const STRINGTYPE &sSigle = STRINGTYPE()) :
			BaseType(pFlag, pq, f), m_sigle(sSigle) {
		this->arrange(pIndsSource, pVarsSource);
	} // MatOrd
	template<typename T>
	InfoMatrice(size_t nRows, size_t nCols, const std::vector<T> &oData,
			const strings_vector &indsNames, const strings_vector &colsNames,
			bool bComputeWeights = false, pcancelflag pFlag = nullptr,
			PBackgrounder pq = nullptr, MatElemFunctionType f =
					[](MatElemResultPtr o) {}, const STRINGTYPE &sSigle =
					STRINGTYPE()) :
			BaseType(pFlag, pq, f), m_sigle(sSigle) {
		this->arrange(nRows, nCols, oData, indsNames, colsNames,
				bComputeWeights);
	} // MatOrd
	virtual ~InfoMatrice() {
	}
	void sigle(const STRINGTYPE &s) {
		this->m_sigle = s;
	}
	MatElemType *variables(void) const {
		return (this->m_vars.get());
	}
	MatElemType *indivs(void) const {
		return (this->m_inds.get());
	}
	MatElemResultPtr get_vars_result(void) const {
		MatElemResultPtr oRet;
		const MatElemType *p = this->m_vars.get();
		if (p != nullptr) {
			oRet = p->getResult();
		}
		return (oRet);
	}
	MatElemResultPtr get_inds_result(void) const {
		MatElemResultPtr oRet;
		const MatElemType *p = this->m_inds.get();
		if (p != nullptr) {
			oRet = p->getResult();
		}
		return (oRet);
	}
	InfoMatriceResultType getResult(void) const {
		MatElemResultPtr oInds = this->get_inds_result();
		MatElemResultPtr oVars = this->get_vars_result();
		return (std::make_tuple(oInds, oVars));
	} // getResultr
	void arrange(SourceType *pIndsSource, SourceType *pVarsSource) {
		assert(pIndsSource != nullptr);
		assert(pVarsSource != nullptr);
		std::future<bool> fInd = std::async([this, pIndsSource]()->bool {
			return this->prep_inds(pIndsSource);
		});
		bool bRet = this->prep_vars(pVarsSource);
		bRet = bRet && fInd.get();
	} // arrange
	void arrange(IndivMapType *pIndsSource, IndivMapType *pVarsSource) {
		assert(pIndsSource != nullptr);
		assert(pVarsSource != nullptr);
		std::future<bool> fInd = std::async([this, pIndsSource]()->bool {
			return this->prep_inds(pIndsSource);
		});
		bool bRet = this->prep_vars(pVarsSource);
		bRet = bRet && fInd.get();
	} // arrange
	void arrange(DistanceMapType *pIndsSource, DistanceMapType *pVarsSource) {
		assert(pIndsSource != nullptr);
		assert(pVarsSource != nullptr);
		std::future<bool> fInd = std::async([this, pIndsSource]()->bool {
			return this->prep_inds(pIndsSource);
		});
		bool bRet = this->prep_vars(pVarsSource);
		bRet = bRet && fInd.get();
	} // arrange
	template<typename T>
	void arrange(size_t nRows, size_t nCols, const std::vector<T> &oData,
			const strings_vector &indsNames, const strings_vector &colsNames,
			bool bComputeWeights = false) {
		assert(nRows > 0);
		assert(nCols > 0);
		assert(oData.size() >= (size_t )(nRows * nCols));
		assert(indsNames.size() >= nRows);
		assert(colsNames.size() >= nCols);
		std::vector<T> oVarsData(nCols * nRows);
		ints_vector indids(nRows), varids(nCols);
		ints_doubles_map weights;
		for (size_t i = 0; i < nRows; ++i) {
			for (size_t j = 0; j < nCols; ++j) {
				oVarsData[j * nRows + i] = oData[i * nCols + j];
			} // j
		} // i
		for (size_t i = 0; i < nRows; ++i) {
			IDTYPE key = (IDTYPE) (i + 1);
			indids[i] = key;
		} // i
		for (size_t i = 0; i < nCols; ++i) {
			IDTYPE key = (IDTYPE) (i + 1);
			varids[i] = key;
		} // i
		DataSourceType oIndProvider(nRows, nCols, oData, indids, varids,
				indsNames, weights, bComputeWeights);
		oIndProvider.recode(1000);
		DataSourceType oVarProvider(nCols, nRows, oVarsData, varids, indids,
				colsNames, weights, false);
		oVarProvider.recode(1000);
		this->arrange(&oIndProvider, &oVarProvider);
	} //arrange
};
////////////////////////////////////

///////////////////////////////////////
}// namespace infO
///////////////////////////////////

#endif /* INFO_MATORD_H_ */

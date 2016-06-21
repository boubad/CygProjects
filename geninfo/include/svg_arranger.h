#pragma once
#ifndef __SVG_ARRANGER_H__
#define __SVG_ARRANGER_H__
/////////////////////////////////
#include "matricedata.h"
#include "info_matord.h"
#include "svg/svg_matrice.h"
/////////////////////////////
namespace info {
/////////////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
class SVGMatriceArranger {
public:
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using SourceType = typename MatElemType::SourceType;
	using MatriceDataType = MatriceData<IDTYPE, STRINGTYPE>;
	using MatriceType = InfoMatrice<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using sizets_vector = typename MatriceType::sizets_vector;
	using MatElemResultType = typename MatriceType::MatElemResultType;
	using MatElemResultPtr = typename MatriceType::MatElemResultPtr;
	using strings_vector = std::vector<STRINGTYPE>;
	using doubles_vector = std::vector<double>;
	using BaseDrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using ContextType = SVGDrawContext<STRINGTYPE, FLOATTYPE>;
	using coord_type = typename BaseDrawItemType::coord_type;
	using dist_type = typename BaseDrawItemType::dist_type;
private:
	STRINGTYPE m_sigle;
	sizets_vector m_varindexes;
	sizets_vector m_indindexes;
	std::unique_ptr<MatriceDataType> m_matdata;
	std::unique_ptr<MatriceType> m_matrice;
public:
	SVGMatriceArranger(const STRINGTYPE &sSigle = STRINGTYPE()) :
			m_sigle(sSigle) {
		this->m_matdata.reset(new MatriceDataType());
		this->m_matrice.reset(
				new MatriceType(nullptr, nullptr, [](MatElemResultPtr o) {},
						sSigle, false));
	}
	~SVGMatriceArranger() {
	}
	template<typename T>
	std::future<bool> export_svg(const STRINGTYPE &filename, size_t nRows,
			size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames,
			MatCellType aType = MatCellType::histogCell) {
		return std::async(std::launch::async,
				[this, filename, nRows, nCols, data, rowNames, colNames, aType]()->bool {
					try {
						return (this->perform_arrange(filename, this->m_sigle, nRows, nCols, data,
										rowNames, colNames, aType));
					}
					catch (...) {

					}
					return (false);
				});
	} // export svg
	template<typename T>
	std::future<bool> export_svg(std::ostream &os, size_t nRows, size_t nCols,
			const std::vector<T> &data, const strings_vector &rowNames,
			const strings_vector &colNames, MatCellType aType =
					MatCellType::histogCell) {
		return std::async(std::launch::async,
				[this, nRows, nCols, data, rowNames, colNames, aType, &os]()->bool {
					try {
						return (this->perform_arrange(os, this->m_sigle, nRows, nCols, data,
										rowNames, colNames, aType));
					}
					catch (...) {

					}
					return (false);
				});
	} // export svg
	template<typename T>
	std::future<bool> export_svg(std::wostream &os, size_t nRows, size_t nCols,
			const std::vector<T> &data, const strings_vector &rowNames,
			const strings_vector &colNames, MatCellType aType =
					MatCellType::histogCell) {
		return std::async(std::launch::async,
				[this, nRows, nCols, data, rowNames, colNames, aType, &os]()->bool {
					try {
						return (this->perform_arrange(os, this->m_sigle, nRows, nCols, data,
										rowNames, colNames, aType));
					}
					catch (...) {

					}
					return (false);
				});
	} // export svg
private:
	void draw(ContextType &oContext, MatCellType aType) {

		MatriceDataType *pData = this->m_matdata.get();
		assert(pData != nullptr);
		const size_t nCols = pData->cols_count();
		const size_t nRows = pData->rows_count();
		DrawContextParams *pp = const_cast<DrawContextParams *>(oContext.draw_params());
		pp->update(nRows,nCols);
		oContext.set_viewBox();
		const sizets_vector &rowindexes = this->m_indindexes;
		assert(rowindexes.size() >= nRows);
		const sizets_vector &colindexes = this->m_varindexes;
		assert(colindexes.size() >= nCols);
		const strings_vector &rowNames = pData->rows_names();
		const strings_vector &colNames = pData->cols_names();
		const doubles_vector &data = pData->data();
		const doubles_vector &varSum = pData->variables_summary();
		const doubles_vector &indSum = pData->indivs_summary();
		DrawContextParams *pParams =
				const_cast<DrawContextParams *>(oContext.draw_params());
		pParams->update(nCols, nRows);
		coord_type x0 = 0, y0 = 0;
		dist_type w = 0, h = 0, dx = 0, dy = 0;
		oContext.get_origin(x0, y0);
		oContext.get_cell_draw_params(w, dx, h, dy);
		coord_type y = y0 + (dy / 2);
		const coord_type startx = x0 + (w / 2);
		const dist_type deltax = dx + w;
		const dist_type deltay = dy + h;
		coord_type x = startx + (2 * deltax);
		for (size_t i = 0; i < nRows; ++i) {
			size_t ii = rowindexes[i];
			STRINGTYPE s = rowNames[ii];
			BaseDrawItemType oItem(MatCellType::indCell, s);
			oContext.draw(&oItem, x, y);
			x += deltax;
		} // i
		y += deltay;
		x = startx + (2 * deltax);
		for (size_t i = 0; i < nRows; ++i) {
			size_t ii = rowindexes[i];
			double f = indSum[ii];
			BaseDrawItemType oItem(MatCellType::summaryIndCell, f);
			oContext.draw(&oItem, x, y);
			x += deltax;
		} // i
		y += deltay;
		for (size_t i = 0; i < nCols; ++i) {
			x = startx;
			size_t ii = colindexes[i];
			STRINGTYPE s = colNames[ii];
			BaseDrawItemType oItem(MatCellType::varCell, s);
			oContext.draw(&oItem, x, y);
			x += deltax;
			double f = varSum[ii];
			BaseDrawItemType oItem1(MatCellType::summaryVarCell, f);
			oContext.draw(&oItem1, x, y);
			x += deltax;
			for (size_t j = 0; j < nRows; ++j) {
				size_t jj = rowindexes[j];
				double f = data[jj * nCols + ii];
				BaseDrawItemType oItem2(aType, f);
				oContext.draw(&oItem2, x, y);
				x += deltax;
			} // j
			y += deltay;
		} // i
	} // draw
	void draw(const STRINGTYPE &filename, MatCellType aType =
			MatCellType::histogCell) {
		ContextType oContext;
		this->draw(oContext, aType);
		oContext.save(filename);
	} // draw
	void draw(std::ostream &os, MatCellType aType = MatCellType::histogCell) {
		ContextType oContext;
		this->draw(oContext, aType);
		oContext.save(os);
	} // draw
	void draw(std::wostream &os, MatCellType aType = MatCellType::histogCell) {
		ContextType oContext;
		this->draw(oContext, aType);
		oContext.save(os);
	} // draw
	template<typename T>
	bool perform_arrange(const STRINGTYPE &filename, const STRINGTYPE &name,
			size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames,
			MatCellType aType = MatCellType::histogCell) {
		bool bRet = false;
		MatriceDataType *pData = this->m_matdata.get();
		assert(pData != nullptr);
		std::future<bool> bf = pData->initializeAsync(name, nRows, nCols, data,
				rowNames, colNames);
		bf.wait();
		SourceType *pInd = pData->indiv_provider();
		SourceType *pVar = pData->variable_provider();
		if ((pInd != nullptr) && (pVar != nullptr)) {
			MatriceType *pMat = this->m_matrice.get();
			if (pMat != nullptr) {
				pMat->arrange(pInd, pVar);
				MatElemResultPtr oIndRes = pMat->get_inds_result();
				MatElemResultType *p1 = oIndRes.get();
				if (p1 != nullptr) {
					this->m_indindexes = p1->indexes();
					MatElemResultPtr oVarRes = pMat->get_vars_result();
					MatElemResultType *p2 = oVarRes.get();
					if (p2 != nullptr) {
						this->m_varindexes = p2->indexes();
						this->draw(filename, aType);
						bRet = true;
					} // p1
				} // p1
			} // pMat
		} //source
		return (bRet);
	} // perform_arrange
	template<typename T>
	bool perform_arrange(std::ostream &os, const STRINGTYPE &name, size_t nRows,
			size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames,
			MatCellType aType = MatCellType::histogCell) {
		bool bRet = false;
		MatriceDataType *pData = this->m_matdata.get();
		assert(pData != nullptr);
		std::future<bool> bf = pData->initializeAsync(name, nRows, nCols, data,
				rowNames, colNames);
		bf.wait();
		SourceType *pInd = pData->indiv_provider();
		SourceType *pVar = pData->variable_provider();
		if ((pInd != nullptr) && (pVar != nullptr)) {
			MatriceType *pMat = this->m_matrice.get();
			if (pMat != nullptr) {
				pMat->arrange(pInd, pVar);
				MatElemResultPtr oIndRes = pMat->get_inds_result();
				MatElemResultType *p1 = oIndRes.get();
				if (p1 != nullptr) {
					this->m_indindexes = p1->indexes();
					MatElemResultPtr oVarRes = pMat->get_vars_result();
					MatElemResultType *p2 = oVarRes.get();
					if (p2 != nullptr) {
						this->m_varindexes = p2->indexes();
						this->draw(os, aType);
						bRet = true;
					} // p1
				} // p1
			} // pMat
		} //source
		return (bRet);
	} // perform_arrange
	template<typename T>
	bool perform_arrange(std::wostream &os, const STRINGTYPE &name,
			size_t nRows, size_t nCols, const std::vector<T> &data,
			const strings_vector &rowNames, const strings_vector &colNames,
			MatCellType aType = MatCellType::histogCell) {
		bool bRet = false;
		MatriceDataType *pData = this->m_matdata.get();
		assert(pData != nullptr);
		std::future<bool> bf = pData->initializeAsync(name, nRows, nCols, data,
				rowNames, colNames);
		bool b = bf.get();
		SourceType *pInd = pData->indiv_provider();
		SourceType *pVar = pData->variable_provider();
		if ((pInd != nullptr) && (pVar != nullptr)) {
			MatriceType *pMat = this->m_matrice.get();
			if (pMat != nullptr) {
				pMat->arrange(pInd, pVar);
				MatElemResultPtr oIndRes = pMat->get_inds_result();
				MatElemResultType *p1 = oIndRes.get();
				if (p1 != nullptr) {
					this->m_indindexes = p1->indexes();
					MatElemResultPtr oVarRes = pMat->get_vars_result();
					MatElemResultType *p2 = oVarRes.get();
					if (p2 != nullptr) {
						this->m_varindexes = p2->indexes();
						this->draw(os, aType);
						bRet = true;
					} // p1
				} // p1
			} // pMat
		} //source
		return (bRet);
	} // perform_arrange
};
////////////////////////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // !__SVG8ARRAGER_H__


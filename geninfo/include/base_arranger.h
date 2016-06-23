/*
* base_arranger.h
*
*  Created on: 22 juin 2016
*      Author: boubad
*/

#ifndef BASE_ARRANGER_H_
#define BASE_ARRANGER_H_
/////////////////////////////////
#include "matricedata.h"
#include "info_matord.h"
#include "info_drawcontext.h"
/////////////////////////////
namespace info {
	/////////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
		class BASEMatriceArranger {
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
			using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
			using coord_type = typename BaseDrawItemType::coord_type;
			using dist_type = typename BaseDrawItemType::dist_type;
			using ArrangerType = BASEMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
		private:
			STRINGTYPE m_sigle;
			sizets_vector m_varindexes;
			sizets_vector m_indindexes;
			std::unique_ptr<MatriceDataType> m_matdata;
			std::unique_ptr<MatriceType> m_matrice;
		protected:
			virtual ContextType *create_context(void) = 0;
		public:
			BASEMatriceArranger(const STRINGTYPE &sSigle = STRINGTYPE()) :
				m_sigle(sSigle) {
			}
			virtual ~BASEMatriceArranger() {
			}
			template<typename T>
			std::future<bool> initializeAsync(const STRINGTYPE &sSigle, size_t nRows, size_t nCols,
				const std::vector<T> &data, const strings_vector &rowNames, const strings_vector &colNames) {
				bool bRet = false;
				this->m_sigle = sSigle;
				this->m_matdata.reset(new MatriceDataType());
				this->m_matrice.reset();
				MatriceDataType *pData = this->m_matdata.get();
				assert(pData != nullptr);
				sizets_vector &v1 = this->m_varindexes;
				v1.resize(nCols);
				for (size_t i = 0; i < nCols; ++i) {
					v1[i] = i;
				}
				sizets_vector &v2 = this->m_indindexes;
				v2.resize(nRows);
				for (size_t i = 0; i < nRows; ++i) {
					v2[i] = i;
				}
				return (pData->initializeAsync(sSigle, nRows, nCols, data, rowNames, colNames));
			}//initializeAsync
			std::future<bool> arrangeAsync(void) {
				return std::async(std::launch::async, [this]()->bool {
					bool bRet = false;
					MatriceDataType *pData = this->m_matdata.get();
					if (pData == nullptr) {
						return (bRet);
					}
					SourceType *pInd = pData->indiv_provider();
					SourceType *pVar = pData->variable_provider();
					if ((pInd != nullptr) && (pVar != nullptr)) {
						this->m_matrice.reset(
							new MatriceType(nullptr, nullptr, [](MatElemResultPtr o) {},
								this->m_sigle, false));
						MatriceType *pMat = this->m_matrice.get();
						assert(pMat != nullptr);
						pMat->arrange(pInd, pVar);
						MatElemResultPtr oIndRes = pMat->get_inds_result();
						MatElemResultType *p1 = oIndRes.get();
						if (p1 != nullptr) {
							this->m_indindexes = p1->indexes();
							MatElemResultPtr oVarRes = pMat->get_vars_result();
							MatElemResultType *p2 = oVarRes.get();
							if (p2 != nullptr) {
								this->m_varindexes = p2->indexes();
								bRet = true;
							} // p1
						} // p1
					} //source
					return (bRet);
				});
			}// arrangeAsync
			std::future<bool> exportAsync(const STRINGTYPE &filename, MatCellType aType = MatCellType::histogCell) {
				return std::async(std::launch::async, [this, filename, aType]()->bool {
					this->draw(filename, aType);
					return (true);
				});
			}// exportAsync
			template <typename T>
			std::future<bool> export_async(const STRINGTYPE &filename, size_t nRows,
				size_t nCols, const std::vector<T> &data,
				const strings_vector &rowNames, const strings_vector &colNames,
				MatCellType aType = MatCellType::histogCell) {
				return std::async(std::launch::async,
					[this, filename, nRows, nCols, data, rowNames, colNames, aType]()->bool {
					try {
						this->m_matdata.reset(new MatriceDataType());
						this->m_matrice.reset(
							new MatriceType(nullptr, nullptr, [](MatElemResultPtr o) {},
								this->m_sigle, false));
						return (this->perform_arrange(filename, this->m_sigle, nRows, nCols, data,
							rowNames, colNames, aType));
					}
					catch (...) {

					}
					return (false);
				});
			} // export svg
			template<typename T>
			std::future<bool> export_async(std::ostream &os, size_t nRows, size_t nCols,
				const std::vector<T> &data, const strings_vector &rowNames,
				const strings_vector &colNames, MatCellType aType =
				MatCellType::histogCell) {
				return std::async(std::launch::async,
					[this, nRows, nCols, data, rowNames, colNames, aType, &os]()->bool {
					try {
						this->m_matdata.reset(new MatriceDataType());
						this->m_matrice.reset(
							new MatriceType(nullptr, nullptr, [](MatElemResultPtr o) {},
								this->m_sigle, false));
						return (this->perform_arrange(os, this->m_sigle, nRows, nCols, data,
							rowNames, colNames, aType));
					}
					catch (...) {

					}
					return (false);
				});
			} // export svg
		private:
			static void draw(ContextType &oContext, MatriceDataType *pData,
				const sizets_vector rowindexes, const sizets_vector &colindexes, MatCellType aType) {
				assert(pData != nullptr);
				const size_t nCols = pData->cols_count();
				const size_t nRows = pData->rows_count();
				assert(rowindexes.size() >= nRows);
				assert(colindexes.size() >= nCols);
				const strings_vector &rowNames = pData->rows_names();
				const strings_vector &colNames = pData->cols_names();
				const doubles_vector &data = pData->data();
				const doubles_vector &varSum = pData->variables_summary();
				const doubles_vector &indSum = pData->indivs_summary();
				DrawContextParams *pParams = oContext.draw_params();
				pParams->update(nRows, nCols);
				coord_type x0 = 0, y0 = 0;
				dist_type w = 0, h = 0, dx = 0, dy = 0;
				oContext.get_origin(x0, y0);
				oContext.get_cell_draw_params(w, dx, h, dy);
				coord_type y = y0 + (dy / 2);
				const coord_type startx = x0 + (w / 2);
				const dist_type deltax = dx + w;
				const dist_type deltay = dy + h;
				coord_type x = startx + (2 * deltax);
				oContext.set_indivs_font();
				for (size_t i = 0; i < nRows; ++i) {
					size_t ii = rowindexes[i];
					STRINGTYPE s = rowNames[ii];
					BaseDrawItemType oItem(MatCellType::indCell, s);
					if (i != 0) {
						oContext.set_separator();
					}
					oContext.draw(&oItem, x, y);
					x += deltax;
				} // i
				oContext.set_endline();
				y += deltay;
				x = startx + (2 * deltax);
				oContext.set_indivs_summary_color();
				for (size_t i = 0; i < nRows; ++i) {
					size_t ii = rowindexes[i];
					double f = indSum[ii];
					BaseDrawItemType oItem(MatCellType::summaryIndCell, f);
					oContext.draw(&oItem, x, y);
					x += deltax;
					if (i != 0) {
						oContext.set_separator();
					}
				} // i
				oContext.set_endline();
				y += deltay;
				oContext.set_variables_font();
				for (size_t i = 0; i < nCols; ++i) {
					oContext.set_variables_font_color();
					x = startx;
					size_t ii = colindexes[i];
					STRINGTYPE s = colNames[ii];
					BaseDrawItemType oItem(MatCellType::varCell, s);
					oContext.draw(&oItem, x, y);
					oContext.set_separator();
					x += deltax;
					double f = varSum[ii];
					BaseDrawItemType oItem1(MatCellType::summaryVarCell, f);
					oContext.set_variables_summary_color();
					oContext.draw(&oItem1, x, y);
					x += deltax;
					if (aType == MatCellType::histogCell) {
						oContext.set_histog_color();
					}
					for (size_t j = 0; j < nRows; ++j) {
						size_t jj = rowindexes[j];
						double f = data[jj * nCols + ii];
						BaseDrawItemType oItem2(aType, f);
						oContext.set_separator();
						oContext.draw(&oItem2, x, y);
						x += deltax;
					} // j
					y += deltay;
					oContext.set_endline();
				} // i
			}// draw
			void draw(ContextType &oContext, MatCellType aType) {
				MatriceDataType *pData = this->m_matdata.get();
				assert(pData != nullptr);
				ArrangerType::draw(oContext, pData, this->m_indindexes, this->m_varindexes, aType);
			} // draw
			void draw(const STRINGTYPE &filename, MatCellType aType =
				MatCellType::histogCell) {
				std::unique_ptr<ContextType> ctx(this->create_context());
				ContextType *pctx = ctx.get();
				assert(pctx != nullptr);
				this->draw(*pctx, aType);
				pctx->save(filename);
			} // draw
			void draw(std::ostream &os, MatCellType aType = MatCellType::histogCell) {
				std::unique_ptr<ContextType> ctx(this->create_context());
				ContextType *pctx = ctx.get();
				assert(pctx != nullptr);
				this->draw(*pctx, aType);
				pctx->save(os,this->m_sigle);
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
	};
	////////////////////////////////////////////////////
}// namespace info
 ///////////////////////////////////
#endif /* EPS_ARRANGER_H_ */

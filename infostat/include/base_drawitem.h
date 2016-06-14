#pragma once
#ifndef __BASEDRAWITEM_H__
#define __BASEDRAWITEM_H__
////////////////////////////////
#include "matresult.h"
/////////////////////////////////
namespace info {
	using byte = unsigned char;
	using coord_type = long;
	using dist_type = unsigned long;
	////////////////////////////////////////////
	struct InfoColor {
		byte red;
		byte green;
		byte blue;
		InfoColor() :red(0), green(0), blue(0) {}
		InfoColor(byte r, byte g, byte b) :
			red(r), green(g), blue(b) {}
		InfoColor(byte c) :red(c), green(c), blue(c) {}
	};// struct InfoColor
	/////////////////////////////////////
	struct DrawContextParams {
		bool	bIndsNames;
		bool	bIndsSum;
		bool	bVarsNames;
		bool	bVarsSum;
		coord_type x0;
		coord_type y0;
		dist_type deltax;
		dist_type dx;
		dist_type deltay;
		dist_type dy;
		InfoColor upcolor;
		InfoColor downcolor;
		InfoColor sumindcolor;
		InfoColor sumvarcolor;
		InfoColor textcolor;
		InfoColor donecolor;
		//
		DrawContextParams() : bIndsNames(true), bIndsSum(true), bVarsNames(true), bVarsSum(true),
			x0(0), y0(0), deltax(0), dx(32), deltay(0), dy(32), upcolor(255), downcolor(0),
			sumindcolor(0, 255, 0), sumvarcolor(255, 0, 0), textcolor(0),donecolor(127) {}
	};// struct DrawContextParams
	/////////////////////////////////////////
	enum class MatCellType { noCell, varCell, indCell, summaryVarCell, summaryIndCell,histogCell, plainCell };
	////////////////////////////////////
	template <typename STRINGTYPE, typename FLOATTYPE>
	class BaseDrawItem;
	/////////////////////////////////////////
	template <typename STRINGTYPE, typename FLOATTYPE>
	class DrawContext : boost::noncopyable {
		using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	protected:
		const DrawContextParams *m_params;
	public:
		DrawContext(const DrawContextParams *pParams) : m_params(pParams){
			assert(this->m_params != nullptr);
		}
			virtual ~DrawContext() {}
	public:
		const DrawContextParams *draw_params(void) const {
			return (this->m_params);
		}
		virtual void draw(DrawItem *pItem, coord_type x0 = 0, coord_type y0 = 0) const {
			// do nothing here
		}// draw
	public:
		void get_origin(coord_type &x, coord_type &y) const {
			DrawContextParams *p = this->draw_params();
			assert(p != nullptr);
			x = p->x0;
			y = p->y0;
		}
		void get_cell_draw_params(dist_type &w, dist_type &dx, dist_type &h, dist_type &dy) const {
			DrawContextParams *p = this->draw_params();
			assert(p != nullptr);
			w = p->dx;
			dx = p->deltax;
			h = p->dy;
			dx = p->deltay;
		}
	private:
		FLOATTYPE m_fdummy;
		STRINGTYPE m_dummy;
	}; // class DrawContext<STRINGTYPE,FLOATTYPE>
	////////////////////////////////////////
	template <typename STRINGTYPE, typename FLOATTYPE>
	class BaseDrawItem {
	public:
		using BaseDrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	private:
		FLOATTYPE m_val;
		MatCellType m_type;
		STRINGTYPE m_text;
	public:
		BaseDrawItem() :m_val(0), m_type(MatCellType::noCell) {}
		BaseDrawItem(MatCellType t, FLOATTYPE val) :m_val(val), m_type(t) {
		}
		BaseDrawItem(MatCellType t, const STRINGTYPE &s) :m_val(0), m_type(t), m_text(s) {
		}
		BaseDrawItem(const BaseDrawItemType &other) :m_val(other.m_val), m_type(other.m_type), m_text(other.m_text) {}
		BaseDrawItemType & operator=(const BaseDrawItemType &other) {
			if (this != &other) {
				this->m_val = other.m_val;
				this->m_type = other.m_type;
				this->m_text = other.m_text;
			}
			return (*this);
		}
		virtual ~BaseDrawItem() {}
	public:
		MatCellType type(void) const {
			return (this->m_type);
		}
		void type(MatCellType t) {
			this->m_type = t;
		}
		const STRINGTYPE &text(void) const {
			return (this->m_text);
		}
		void text(const STRINGTYPE &s) {
			this->m_text = s;
		}
		FLOATTYPE value(void) const {
			return (this->m_val);
		}
		void value(FLOATTYPE n) {
			this->m_val = n;
		}
	};// class BaseDrawItem
	//////////////////////////////////////////
	template <typename STRINGTYPE, typename FLOATTYPE>
	class DrawItems : public BaseDrawItem<STRINGTYPE, FLOATTYPE>, private boost::noncopyable {
	public:
		using DrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
		using PDrawItemType = DrawItemType *;
		using items_vector = std::vector<PDrawItemType>;
		using DrawItemsType = DrawItems<STRINGTYPE, FLOATTYPE>;
		using sizets_vector = std::vector<size_t>;
		using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
		using strings_vector = std::vector<STRINGTYPE>;
	private:
		size_t m_nrows;
		size_t m_ncols;
		items_vector m_items;
		sizets_vector m_colindexes;
		sizets_vector m_rowindexes;
		//
	public:
		DrawItems():m_nrows(0),m_ncols(0){}
		template <typename T, typename F>
		DrawItems(MatCellType aType, size_t nRows, size_t nCols,
			const std::vector<T> &data,
			const strings_vector &rowNames,
			const strings_vector &colNames,
			const std::vector<F> &indsSum,
			const std::vector<F> &varsSum) {
			this->initialize(aType, nRows, nCols, data, rowNames, colNames, indsSum, varsSum);
		}
		virtual ~DrawItems() {
			items_vector &vv = this->m_items;
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				PDrawItemType p = *it;
				delete p;
			}
			vv.clear();
		}
	public:
		template <typename T, typename F>
		bool initialize(MatCellType aType, size_t nRows, size_t nCols,
			const std::vector<T> &data,
			const strings_vector &rowNames,
			const strings_vector &colNames,
			const std::vector<F> &indsSum,
			const std::vector<F> &varsSum
		) {
			if ((nRows < 1) || (nCols < 1)) {
				return (false);
			}
			if (data.size() < (size_t)(nCols * nRows)) {
				return (false);
			}
			//
			T vMin = *(std::min_element(data.begin(), data.end()));
			T vMax = *(std::max_element(data.begin(), data.end()));
			if (vMax <= vMin) {
				return (false);
			}
			double deltaVal = (double)(vMax - vMin);
			//
			bool bIndsNames = (rowNames.size() >= nRows);
			bool bIndsSum = (indsSum.size() >= nRows);
			F fIndMin = 0, fIndMax = 0;
			double deltaInd = 0;
			if (bIndsSum) {
				bIndsSum = false;
				fIndMin = *(std::min_element(indsSum.begin(), indsSum.end()));
				fIndMax = *(std::max_element(indsSum.begin(), indsSum.end()));
					if (fIndMax > fIndMin) {
						bIndsSum = true;
						deltaInd = (double)(fIndMax - fIndMin);
					}
			}
			bool bVarsNames(colNames.size() >= nCols);
			bool bVarsSum = (varsSum.size() >= nCols);
			F fVarMin = 0, fVarMax = 0;
			double deltaVar = 0;
			if (bVarsSum) {
				bVarsSum = false;
				fVarMin = *(std::min_element(varsSum.begin(), varsSum.end()));
				fVarMax = *(std::max_element(varsSum.begin(), varsSum.end()));
					if (fVarMax > fVarMin) {
						bVarsSum = true;
						deltaVar = (double)(fVarMax - fVarMin);
					}
			}
			//
			this->resize(nRows, nCols);
			const size_t nRowsMax = (size_t)(nRows + 2);
			const size_t nColsMax = (size_t)(nCols + 2);
			items_vector &vv = this->m_items;
			for (size_t i = 0; i < nRowsMax; ++i) {
				if (i == 0) {
					if (bVarsNames) {
						for (size_t j = 0; j < nCols; ++j) {
							PDrawItemType p = new DrawItemType(MatCellType::varCell, colNames[j]);
							assert(p != nullptr);
							size_t pos = (size_t)(i * nColsMax + j + 1);
							vv[pos] = p;
						}// j
					}// inds
				}
				else if (i == 1) {
					if (bVarsSum) {
						for (size_t j = 0; j < nCols; ++j) {
							double fx = ((double)varsSum[i] - fVarMin) / deltaVar;
							FLOATTYPE fr = (FLOATTYPE)fx;
							PDrawItemType p = new DrawItemType(MatCellType::summaryVarCell, fr);
							assert(p != nullptr);
							size_t pos = (size_t)(i * nColsMax + j + 1);
							vv[pos] = p;
						}// j
					}// varsSul
				}
				else {
					size_t iIndex = (size_t)(i - 2);
					if (bIndsNames) {
						STRINGTYPE sx = rowNames[iIndex];
						PDrawItemType p = new DrawItemType(MatCellType::indCell, sx);
						assert(p != nullptr);
						vv[i * nColsMax] = p;
					}
					for (size_t j = 0; j < nCols; ++j) {
						double fx = ((double)data[iIndex * nCols + j] - vMin) / deltaVal;
						FLOATTYPE fr = (FLOATTYPE)fx;
						PDrawItemType p = new DrawItemType(aType, fr);
						vv[i*nColsMax + 1 + j] = p;
					}// j
					if (bIndsSum) {
						double fx = ((double)indsSum[iIndex] - fIndMin) / deltaInd;
						FLOATTYPE fr = (FLOATTYPE)fx;
						PDrawItemType p = new DrawItemType(MatCellType::summaryIndCell, fr);
						assert(p != nullptr);
						vv[i * nColsMax + 1 + nCols] = p;
					}
				}
			}// i
			return (true);
		}// initialize
		void row_indexes(const sizets_vector &oInds) {
			if (oInds.size() >= this->m_nrows) {
				this->m_rowindexes = oInds;
			}
		}
		void col_indexes(const sizets_vector &oInds) {
			if (oInds.size() >= this->m_ncols) {
				this->m_colindexes = oInds;
			}
		}
		virtual void draw(const DrawContext<STRINGTYPE,FLOATTYPE> *pContext, coord_type xpos = 0, coord_type ypos = 0) {
			assert(pContext != nullptr);
			const DrawContextParams *pParams = pContext->draw_params();
			assert(pParams != nullptr);
			const size_t nRows = this->m_nrows;
			const size_t nCols = this->m_ncols;
			const size_t nTotalRows = nRows + 2;
			const size_t nTotalCols = nCols + 2;
			coord_type y = ypos + pParams->y0;
			coord_type x0 = xpos + pParams->x0;
			const dist_type ddx = pParams->deltax + pParams->dx;
			const dist_type ddy = pParams->deltay + pParams->dy;
			const sizets_vector &rowIndexes = this->m_rowindexes;
			const sizets_vector &colIndexes = this->m_colindexes;
			const items_vector &vv = this->m_items;
			//const size_t nTotal = vv.size();
			bool bVarsNames = pParams->bVarsNames;
			bool bVarsSum = pParams->bVarsSum;
			bool bIndsNames = pParams->bIndsNames;
			bool bIndsSum = pParams->bIndsSum;
			for (size_t i = 0; i < nTotalRows; ++i) {
				coord_type x = x0;
				if (i == 0) {
					if (bVarsNames) {
						for (size_t j = 0; j < nCols; ++j) {
							x += ddx;
							size_t jj = colIndexes[j];
							PDrawItemType p = vv[i * nTotalCols + jj + 1];
							if (p != nullptr) {
								pContext->draw(p, x, y);
							}
						}// j
					}// varNames
				}
				else if (i == 1) {
					if (bVarsSum) {
						for (size_t j = 0; j < nCols; ++j) {
							x += ddx;
							size_t jj = colIndexes[j];
							PDrawItemType p = vv[i * nTotalCols + jj + 1];
							if (p != nullptr) {
								pContext->draw(p, x, y);
							}
						}// j
					}// varssum
				}
				else {
					size_t iIndex = (size_t)(i - 2);
					size_t ii = rowIndexes[iIndex];
					size_t base_pos = (ii + 2) * nTotalCols;
					if (bIndsNames) {
						PDrawItemType p = vv[base_pos];
						if (p != nullptr) {
							pContext->draw(p, x, y);
						}
					}
					for (size_t j = 0; j < nCols; ++j) {
						x += ddx;
						size_t jj = colIndexes[j];
						PDrawItemType p = vv[base_pos + jj + 1];
						if (p != nullptr) {
							pContext->draw(p, x, y);
						}
					}// j
					if (bIndsSum) {
						PDrawItemType p = vv[base_pos + nCols + 1];
						if (p != nullptr) {
							x += ddx;
							pContext->draw(p, x, y);
						}
					}
				}
				y += ddy;
			}// i
		}// draw

		void draw(const sizets_vector &rowIndexes, const sizets_vector &colIndexes,
			const DrawContext<STRINGTYPE,FLOATTYPE> *pContext, coord_type xpos = 0, coord_type ypos = 0) {
			this->row_indexes(rowIndexes);
			this->col_indexes(colIndexes);
			this->draw(pContext, xpos, ypos);
		}// draw
		void draw(DispositionType disp, const sizets_vector &oInd,
			const DrawContext<STRINGTYPE, FLOATTYPE> *pContext, coord_type xpos = 0, coord_type ypos = 0) {
			if (disp == DispositionType::indiv) {
				this->row_indexes(oInd);
				this->draw(pContext, xpos, ypos);
			}
			else if (disp == DispositionType::variable) {
				this->col_indexes(oInd);
				this->draw(pContext, xpos, ypos);
			}
		}
		template <typename IDTYPE, typename DISTANCETYPE, typename XS>
		void draw(std::shared_ptr<IntraMatElemResult<IDTYPE, DISTANCETYPE, XS> > oRes,
			const DrawContext<STRINGTYPE, FLOATTYPE> *pContext, coord_type xpos = 0, coord_type ypos = 0) {
			IntraMatElemResult<IDTYPE, DISTANCETYPE, XS> *p = oRes.get();
			if (p != nullptr) {
				this->draw(p->disposition, p->second, pContext, xpos, ypos);
			}
		}
		template <typename IDTYPE, typename DISTANCETYPE, typename XS>
		void set_result(std::shared_ptr<IntraMatElemResult<IDTYPE, DISTANCETYPE, XS> > oRes) {
			IntraMatElemResult<IDTYPE, DISTANCETYPE, XS> *p = oRes.get();
			if (p != nullptr) {
				if (p->disposition == DispositionType::indiv) {
					this->row_indexes(p->second);
				}
				else if (p->disposition == DispositionType::variable) {
					this->col_indexes(p->second);
				}
			}
		}
	protected:
		void resize(size_t nRows, size_t nCols) {
			items_vector &vv = this->m_items;
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				PDrawItemType p = *it;
				delete p;
			}
			vv.clear();
			size_t nn = (size_t)((nRows + 2) * (nCols + 2));
			PDrawItemType pNull(nullptr);
			vv.resize(nn, pNull);
			this->m_ncols = nCols;
			this->m_nrows = nRows;
			this->m_rowindexes.resize(nRows);
			this->m_colindexes.resize(nCols);
			for (size_t i = 0; i < nRows; ++i) {
				(this->m_rowindexes)[i] = i;
			}
			for (size_t j = 0; j < nCols; ++j) {
				(this->m_colindexes)[j] = j;
			}
		}// resize
	};// class DrawItems<STRINGTYPE>
	///////////////////////////////////////
}// namespace info
///////////////////////////////////
#endif // !__BASEDRAWITEM_H__

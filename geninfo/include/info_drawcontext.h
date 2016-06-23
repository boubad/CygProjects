#pragma once
#ifndef __INFO_DRAWCONTEXT_H__
#define __INFO_DRAWCONTEXT_H__
////////////////////////////////
#include <fstream>
////////////////////////////////////
#include "matresult.h"
#include "matricedata.h"
#include "info_matord.h"
#include "activeobject.h"
/////////////////////////////////
namespace info {
////////////////////////////////////////////
struct InfoColor {
public:
	using byte = unsigned char;
	//
	byte red;
	byte green;
	byte blue;
	InfoColor() :
			red(0), green(0), blue(0) {
	}
	InfoColor(byte r, byte g, byte b) :
			red(r), green(g), blue(b) {
	}
	InfoColor(byte c) :
			red(c), green(c), blue(c) {
	}
};
// struct InfoColor
///////////////////////////////////////
enum class MatriceDrawType {
	drawIndivs, drawVariables
};
/////////////////////////////////////
struct DrawContextParams {
public:
	using coord_type = double;
	using dist_type = double;
	//
	size_t nRows;
	size_t nCols;
	dist_type dmin;
	double fangle;
	int kfactor;
	dist_type variableFontSize;
	dist_type indivFontSize;
	dist_type width;
	dist_type height;
	MatriceDrawType drawType;
	bool bIndsNames;
	bool bIndsSum;
	bool bVarsNames;
	bool bVarsSum;
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
	InfoColor strokecolor;
	//
	DrawContextParams() :
			nRows(0),nCols(0),dmin(16),fangle(285), kfactor(8), variableFontSize(15), indivFontSize(12), width(
					595), height(841), drawType(MatriceDrawType::drawIndivs), bIndsNames(
					true), bIndsSum(true), bVarsNames(true), bVarsSum(true), x0(
					0), y0(0), deltax(0), dx(32), deltay(0), dy(32), upcolor(
					255), downcolor(0), sumindcolor(0, 255, 0), sumvarcolor(255,
					0, 0), textcolor(0), donecolor(127), strokecolor(0) {
	}
	void update(size_t nRows, size_t nCols,bool bUpdate = true) {
		this->nRows = nRows;
		this->nCols = nCols;
		double fcol = (double) this->kfactor
				/ ((this->kfactor + 1) * (nRows + 3));
		this->dx = (dist_type) (fcol * this->width);
		this->deltax = (dist_type) (this->dx / this->kfactor);
		if ((this->dx < this->dmin) && bUpdate) {
			this->dx = this->dmin;
			this->deltax = (dist_type) (this->dx / this->kfactor);
			this->width = (dist_type) ((nRows + 3) * (this->dx + this->deltax));
		}
		double frow = (double) this->kfactor
				/ ((this->kfactor + 1) * (nCols + 3));
		this->dy = (dist_type) (frow * this->height);
		this->deltay = (dist_type) (this->dy / this->kfactor);
		if ((this->dy < this->dmin) && bUpdate) {
			this->dy = this->dmin;
			this->deltay = (dist_type) (this->dy / this->kfactor);
			this->height =
					(dist_type) ((nCols + 3) * (this->dy + this->deltay));
		}
	}	  // update
};
// struct DrawContextParams
/////////////////////////////////////////
enum class MatCellType {
	noCell,
	varCell,
	indCell,
	summaryVarCell,
	summaryIndCell,
	histogCell,
	plainCell
};
////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class BaseDrawItem;
/////////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class DrawContext {
public:
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
protected:
	DrawContextParams *m_params;
	FLOATTYPE m_fdummy;
	STRINGTYPE m_dummy;
	std::unique_ptr<DrawContextParams> m_oparam;
public:
	virtual void save(std::ostream &os, const std::string &stitle = std::string()) {
		// do nothing here
	}
	void save(std::wostream &wos, const std::wstring &sTitle = std::wstring()) {
		std::stringstream os;
		this->save(os);
		std::string s2 = os.str();
		std::wstring sz = info_2ws(s2);
		wos << sz;
	}// save
	virtual void save(const std::string &filename) {
		std::ofstream os(filename.c_str());
		this->save(os, filename);
	} // save
	virtual void save(const std::wstring &filename) {
		std::wofstream os(info_2s(filename.c_str()));
		this->save(os, filename);
	} // sav
	virtual void set_indivs_font(void) {
	}//set_indivs_font
	virtual void set_variables_font(void) {
	}//set_indivs_font
	virtual void set_variables_font_color(void) {
	}//set_variables_font_color
	virtual void set_indivs_summary_color(void) {
	}// set_indivs_summary_color
	virtual void set_variables_summary_color(void) {
	}// set_variables_summary_color
	virtual void set_histog_color(void) {
	}// set_histog_color
	virtual void set_plain_color(double f) {
	}// set_plain_color
	virtual void set_separator(void) {
	}
	virtual void set_endline(void) {
	}
public:
	DrawContext(DrawContextParams *pParams = nullptr) :
			m_params(pParams), m_fdummy(0) {
	}
	virtual ~DrawContext() {
	}
	const DrawContextParams *draw_params(void) const {
		const DrawContextParams *pRet = this->m_params;
		if (pRet == nullptr) {
			pRet = this->m_oparam.get();
			if (pRet == nullptr) {
				ContextType *p = const_cast<ContextType *>(this);
				p->m_oparam.reset(new DrawContextParams());
				pRet = const_cast<const DrawContextParams *>(p->m_oparam.get());
			}
		}
		return (pRet);
	}	  // draw_params
	DrawContextParams *draw_params(void) {
		DrawContextParams *pRet = this->m_params;
		if (pRet == nullptr) {
			pRet = this->m_oparam.get();
			if (pRet == nullptr) {
				this->m_oparam.reset(new DrawContextParams());
				pRet = this->m_oparam.get();
			}
		}
		return (pRet);
	}	  // draw_params
	virtual void draw(DrawItem *pItem, coord_type x0 = 0, coord_type y0 = 0) {
		// do nothing here
	}	  // draw
public:
	void get_origin(coord_type &x, coord_type &y) {
		const DrawContextParams *p = this->draw_params();
		x = p->x0;
		y = p->y0;
	}
	void get_cell_draw_params(dist_type &w, dist_type &dx, dist_type &h,
			dist_type &dy) {
		const DrawContextParams *p = this->draw_params();
		w = p->dx;
		dx = p->deltax;
		h = p->dy;
		dy = p->deltay;
	}
};
// class DrawContext<STRINGTYPE,FLOATTYPE>
////////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class BaseDrawItem {
public:
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
	using BaseDrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
private:
	FLOATTYPE m_val;
	MatCellType m_type;
	STRINGTYPE m_text;
public:
	BaseDrawItem() :
			m_val(0), m_type(MatCellType::noCell) {
	}
	BaseDrawItem(MatCellType t, FLOATTYPE val) :
			m_val(val), m_type(t) {
	}
	BaseDrawItem(MatCellType t, const STRINGTYPE &s) :
			m_val(0), m_type(t), m_text(s) {
	}
	BaseDrawItem(const BaseDrawItemType &other) :
			m_val(other.m_val), m_type(other.m_type), m_text(other.m_text) {
	}
	BaseDrawItemType & operator=(const BaseDrawItemType &other) {
		if (this != &other) {
			this->m_val = other.m_val;
			this->m_type = other.m_type;
			this->m_text = other.m_text;
		}
		return (*this);
	}
	virtual ~BaseDrawItem() {
	}
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
public:
	virtual bool draw(ContextType *pContext, coord_type x0 = 0, coord_type y0 =
			0) {
		return (true);
	}	   // draw
};
// class BaseDrawItem
///////////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
class DrawItemsView {
public:
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using DrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using PDrawItemType = DrawItemType *;
	using items_vector = std::vector<PDrawItemType>;
	using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
	using DrawItemsViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using MatElemResultType = MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
	using function_type = std::function<void(MatElemResultPtr)>;
private:
	DispositionType m_type;
	size_t m_nrows;
	size_t m_ncols;
	items_vector *m_pitems;
	function_type m_f;
protected:
	virtual void update_context(ContextType *pContext) {
		DrawContextParams *pParams = pContext->draw_params();
		assert(pParams != nullptr);
		pParams->update(this->m_nrows, this->m_ncols);
	}// update_context
public:
	DrawItemsView(DispositionType atype, size_t nRows, size_t nCols,
			items_vector *pitems, function_type ff = [](MatElemResultPtr o) {}) :
			m_type(atype), m_nrows(nRows), m_ncols(nCols), m_pitems(pitems), m_f(
					ff) {
		assert(this->m_nrows > 0);
		assert(this->m_ncols > 0);
		assert(this->m_pitems != nullptr);
	}
	virtual ~DrawItemsView() {
	}
public:
	size_t get_nb_rows(void) const {
		return (this->m_nrows);
	}
	size_t get_nb_cols(void) const {
		return (this->m_ncols);
	}
	DispositionType disposition(void) const {
		return (this->m_type);
	}
	void set_function(function_type f) {
		this->m_f = f;
	}
	void notify(MatElemResultPtr o) {
		(this->m_f)(o);
	}
	virtual void draw(ContextType *pContext, coord_type xpos = 0, coord_type ypos = 0) {
		assert(pContext != nullptr);
		DrawContextParams *pParams = pContext->draw_params();
		assert(pParams != nullptr);
		this->update_context(pContext);
		const size_t nTotalRows = this->m_nrows + 2;
		const size_t nTotalCols = this->m_ncols + 2;
		items_vector &vv = (*this->m_pitems);
		assert(vv.size() >= (size_t) (nTotalCols * nTotalRows));
		dist_type ddx = pParams->dx + pParams->deltax;
		dist_type ddy = pParams->dy + pParams->deltay;
		coord_type x0 = xpos + pParams->x0 + (ddx / 2);
		coord_type y = ypos + pParams->y0 + (ddy / 2);
		for (size_t i = 0; i < nTotalRows; ++i) {
			coord_type x = x0;
			for (size_t j = 0; j < nTotalCols; ++j) {
				PDrawItemType p = vv[i * nTotalCols + j];
				if ((p != nullptr) && (p->type() != MatCellType::noCell)) {
					pContext->draw(p, x, y);
				}
				x += ddx;
			}	// j
			y += ddy;
		}	// i
	}	// draw
};
// class DrawItemsView<STRINGTYPE, FLOATTYPE>
///////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,typename FLOATTYPE>
	class CRTDrawItemsView :public DrawItemsView<IDTYPE,DISTANCETYPE,STRINGTYPE,FLOATTYPE> {
	public:
		using BaseType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
		using MatElemResultType = MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
		using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
		using function_type = std::function<void(MatElemResultPtr)>;
		using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
		using DrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
		using PDrawItemType = DrawItemType *;
		using items_vector = std::vector<PDrawItemType>;
	protected:
		virtual void update_context(ContextType *pContext) {
			DrawContextParams *pParams = pContext->draw_params();
			assert(pParams != nullptr);
			pParams->update(this->get_nb_rows(), this->get_nb_cols(),false);
		}// update_context
	public:
		CRTDrawItemsView(DispositionType atype, size_t nRows, size_t nCols,
			items_vector *pitems, function_type ff = [](MatElemResultPtr o) {}) : BaseType(atype,nRows,nCols,pitems,ff){
		}
		virtual ~CRTDrawItemsView() {
		}
};
// class DrawItemsView<STRINGTYPE, FLOATTYPE>
//////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
class DrawItems: public BaseDrawItem<STRINGTYPE, FLOATTYPE> {
public:
	using DrawItemType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using PDrawItemType = DrawItemType *;
	using items_vector = std::vector<PDrawItemType>;
	using DrawItemsType = DrawItems<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using sizets_vector = std::vector<size_t>;
	using strings_vector = std::vector<STRINGTYPE>;
	using MatElemResultType = MatElemResult<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultPtr = std::shared_ptr<MatElemResultType>;
	using ViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using ViewTypePtr = std::shared_ptr<ViewType>;
	using views_vector = std::vector<ViewTypePtr>;
	using SourceType = DataVectorIndivSource<IDTYPE, STRINGTYPE>;
	using ints_vector = std::vector<IDTYPE>;
	using ints_doubles_map = std::map<IDTYPE, double>;
	using function_type = std::function<void(MatElemResultPtr)>;
private:
	size_t m_nrows;
	size_t m_ncols;
	items_vector m_items;
	sizets_vector m_colindexes;
	sizets_vector m_rowindexes;
	items_vector m_indivitems;
	items_vector m_variableitems;
	views_vector m_views;
	STRINGTYPE m_sigle;
	std::unique_ptr<SourceType> m_varsource;
	std::unique_ptr<SourceType> m_indsource;
public:
	DrawItems() :
			m_nrows(0), m_ncols(0) {
	}

	template<typename T>
	DrawItems(MatCellType aType, size_t nRows, size_t nCols,
			const std::vector<T> &data, const strings_vector &rowNames,
			const strings_vector &colNames) :
			m_nrows(0), m_ncols(0) {
		this->initialize(aType, nRows, nCols, data, rowNames, colNames);
	}
	template<typename T, typename F>
	DrawItems(MatCellType aType, size_t nRows, size_t nCols,
			const std::vector<T> &data, const strings_vector &rowNames,
			const strings_vector &colNames, const std::vector<F> &indsSum,
			const std::vector<F> &varsSum) :
			m_nrows(0), m_ncols(0) {
		this->initialize(aType, nRows, nCols, data, rowNames, colNames, indsSum,
				varsSum);
	}
	virtual ~DrawItems() {
		items_vector &vv = this->m_items;
		for (auto it = vv.begin(); it != vv.end(); ++it) {
			PDrawItemType p = *it;
			delete p;
		}
		vv.clear();
		this->m_indivitems.clear();
		this->m_variableitems.clear();
	}
public:
	const STRINGTYPE sigle(void) const {
		return (this->m_sigle);
	}
	SourceType *get_indiv_provider(void) {
		return (this->m_indsource.get());
	}
	SourceType *get_variable_provider(void) {
		return (this->m_varsource.get());
	}
	
	ViewType *add_view(DispositionType type) {
		assert(this->m_nrows > 0);
		assert(this->m_ncols);
		ViewType *pRet = nullptr;
		ViewTypePtr oRet;
		if (type == DispositionType::indiv) {
			oRet = this->create_view(type, this->m_nrows, this->m_ncols, &(this->m_indivitems));
		} else if (type == DispositionType::variable) {
			oRet = this->create_view(type, this->m_ncols, this->m_nrows, &(this->m_variableitems));
		}
		pRet = oRet.get();
		if (pRet != nullptr) {
			this->m_views.push_back(oRet);
		}
		return (pRet);
	}
	void remove_view(ViewType *pView) {
		views_vector & vv = this->m_views;
		auto it = std::find_if(vv.negin(), vv.end(),
				[pView](ViewTypePtr o)->bool {
					return (o.get() == pView);
				});
		if (it != vv.end()) {
			vv.erase(it);
		}
	}	  // remove_view
	template<typename T>
	bool initialize(MatCellType aType, size_t nRows, size_t nCols,
			const std::vector<T> &data, const strings_vector &rowNames,
			const strings_vector &colNames, const STRINGTYPE &sSigle =
					STRINGTYPE()) {
		assert(nRows > 0);
		assert(nCols > 0);
		size_t nn = (size_t) (nCols * nRows);
		assert(data.size() >= nn);
		assert(rowNames.size() >= nRows);
		assert(colNames.size() >= nCols);
		this->m_sigle = sSigle;
		std::vector<double> fdata(nn), vardata(nn), varsum(nCols), indsum(
				nRows);
		ints_vector varids(nCols), indids(nRows);
		ints_doubles_map weights;
		double somvars = 0;
		double s1 = 0;
		double s2 = 0;
		double fmin = 0;
		double fmax = 0;
		double delta = 0;
		for (size_t i = 0; i < nCols; ++i) {
			IDTYPE varkey = (IDTYPE) (i + 1);
			varids[i] = varkey;
			s1 = 0;
			s2 = 0;
			fmin = 0;
			fmax = 0;
			for (size_t j = 0; j < nRows; ++j) {
				double x = (double) data[j * nCols + i];
				fdata[j * nCols + i] = x;
				s1 += x;
				s2 += x * x;
				if (j == 0) {
					fmin = x;
					fmax = x;
				} else if (x < fmin) {
					fmin = x;
				} else if (x > fmax) {
					fmax = x;
				}
			}	  // j
			s1 = s1 / (double) nRows;
			s2 = (s2 / (double) nRows) - (s1 * s1);
			s2 = 1.0 / s2;
			somvars += s2;
			varsum[i] = s2;
			delta = fmax - fmin;
			for (size_t j = 0; j < nRows; ++j) {
				double x = (fdata[j * nCols + i] - fmin) / delta;
				fdata[j * nCols + i] = x;
				vardata[i * nRows + j] = x;
			}	  // j
		}	  // i
		for (size_t i = 0; i < nCols; ++i) {
			varsum[i] = varsum[i] / somvars;
		}	  //i
		for (size_t i = 0; i < nRows; ++i) {
			indids[i] = (IDTYPE) (i + 1);
			double som = 0;
			for (size_t j = 0; j < nCols; ++j) {
				som += fdata[i * nCols + j];
			}
			indsum[i] = som / nCols;
		}	  // i
			  //
		this->m_varsource.reset(
				new SourceType(nCols, nRows, vardata, varids, indids, colNames,
						weights, false));
		SourceType *ps = this->m_varsource.get();
		assert(ps != nullptr);
		ps->recode(1000);
		this->m_indsource.reset(
				new SourceType(nRows, nCols, fdata, indids, varids, rowNames,
						weights, false));
		SourceType *psx = this->m_indsource.get();
		assert(psx != nullptr);
		psx->recode(1000);
		//
		this->resize(nRows, nCols);
		const size_t nRowsMax = (size_t) (nRows + 2);
		const size_t nColsMax = (size_t) (nCols + 2);
		items_vector &vv = this->m_items;
		for (size_t i = 0; i < nRowsMax; ++i) {
			if (i == 0) {
				vv[0] = this->create_empty_item();
				for (size_t j = 0; j < nCols; ++j) {
					PDrawItemType p = this->create_varname_item(colNames[j]);
					assert(p != nullptr);
					size_t pos = (size_t) (j + 1);
					vv[pos] = p;
				}			// j
				vv[nColsMax - 1] = this->create_empty_item();
			} else if (i == 1) {
				vv[nColsMax] = this->create_empty_item();
				for (size_t j = 0; j < nCols; ++j) {
					double fx = varsum[j];
					FLOATTYPE fr = (FLOATTYPE) fx;
					PDrawItemType p = this->create_varsum_item(fr);
					assert(p != nullptr);
					size_t pos = (size_t) (nColsMax + j + 1);
					vv[pos] = p;
				}			// j
				vv[2 * nColsMax - 1] = this->create_empty_item();
			} else {
				size_t ii = (size_t) (i - 2);
				STRINGTYPE sx = rowNames[ii];
				PDrawItemType p = this->create_indname_item(sx);
				assert(p != nullptr);
				vv[i * nColsMax] = p;
				for (size_t j = 0; j < nCols; ++j) {
					double fx = fdata[ii * nCols + j];
					FLOATTYPE fr = (FLOATTYPE) fx;
					PDrawItemType p = nullptr;
					if (aType == MatCellType::histogCell) {
						p = this->create_histog_item(fr);
					} else if (aType == MatCellType::plainCell) {
						p = this->create_plain_item(fr);
					} else {
						p = this->create_empty_item();
					}
					assert(p != nullptr);
					vv[i * nColsMax + 1 + j] = p;
				}			// j
				double fx = indsum[ii];
				FLOATTYPE fr = (FLOATTYPE) fx;
				p = this->create_indsum_item(fr);
				assert(p != nullptr);
				vv[i * nColsMax + 1 + nCols] = p;
			}
		}			// i
		items_vector &vv_inds = this->m_indivitems;
		items_vector &vv_vars = this->m_variableitems;
		for (size_t i = 0; i < nRowsMax; ++i) {
			for (size_t j = 0; j < nColsMax; ++j) {
				const size_t pos = (size_t) (i * nColsMax + j);
				PDrawItemType p = vv[pos];
				vv_inds[pos] = p;
				vv_vars[j * nRowsMax + i] = vv[pos];
			}			// j
		}			// i
		return (true);
	}			// initialize
	template<typename T, typename F>
	bool initialize(MatCellType aType, size_t nRows, size_t nCols,
			const std::vector<T> &data, const strings_vector &rowNames,
			const strings_vector &colNames, const std::vector<F> &indsSum,
			const std::vector<F> &varsSum, const STRINGTYPE &sSigle =
					STRINGTYPE()) {
		if ((nRows < 1) || (nCols < 1)) {
			return (false);
		}
		if (data.size() < (size_t) (nCols * nRows)) {
			return (false);
		}
		if ((rowNames.size() < nRows) || (colNames.size() < nCols)
				|| (indsSum.size() < nRows) || (varsSum.size() < nCols)) {
			return (false);
		}
		//
		this->resize(nRows, nCols);
		const size_t nRowsMax = (size_t) (nRows + 2);
		const size_t nColsMax = (size_t) (nCols + 2);
		items_vector &vv = this->m_items;
		for (size_t i = 0; i < nRowsMax; ++i) {
			if (i == 0) {
				vv[0] = this->create_empty_item();
				for (size_t j = 0; j < nCols; ++j) {
					PDrawItemType p = this->create_varname_item(colNames[j]);
					assert(p != nullptr);
					size_t pos = (size_t) (j + 1);
					vv[pos] = p;
				}			// j
				vv[nColsMax - 1] = this->create_empty_item();
			} else if (i == 1) {
				vv[nColsMax] = this->create_empty_item();
				for (size_t j = 0; j < nCols; ++j) {
					double fx = (double) varsSum[i];
					FLOATTYPE fr = (FLOATTYPE) fx;
					PDrawItemType p = this->create_varsum_item(fr);
					assert(p != nullptr);
					size_t pos = (size_t) (nColsMax + j + 1);
					vv[pos] = p;
				}			// j
				vv[2 * nColsMax - 1] = this->create_empty_item();

			} else {
				size_t ii = (size_t) (i - 2);
				STRINGTYPE sx = rowNames[ii];
				PDrawItemType p = this->create_indname_item(sx);
				assert(p != nullptr);
				vv[i * nColsMax] = p;
				for (size_t j = 0; j < nCols; ++j) {
					double fx = (double)data[ii * nCols + j];
					FLOATTYPE fr = (FLOATTYPE) fx;
					PDrawItemType p = nullptr;
					if (aType == MatCellType::histogCell) {
						p = this->create_histog_item(fr);
					} else if (aType == MatCellType::plainCell) {
						p = this->create_plain_item(fr);
					} else {
						p = this->create_empty_item();
					}
					assert(p != nullptr);
					vv[i * nColsMax + 1 + j] = p;
				}			// j
				double fx = (double) indsSum[ii];
				FLOATTYPE fr = (FLOATTYPE) fx;
				PDrawItemType pp = this->create_indsum_item(fr);
				assert(pp != nullptr);
				vv[i * nColsMax + 1 + nCols] = pp;
			}
		}			// i
		items_vector &vv_inds = this->m_indivitems;
		items_vector &vv_vars = this->m_variableitems;
		for (size_t i = 0; i < nRowsMax; ++i) {
			for (size_t j = 0; j < nColsMax; ++j) {
				const size_t pos = (size_t) (i * nColsMax + j);
				PDrawItemType p = vv[pos];
				vv_inds[pos] = p;
				vv_vars[j * nRowsMax + i] = vv[pos];
			}			// j
		}			// i
		return (true);
	}			// initialize
	void row_indexes(const sizets_vector &oInds) {
		if (oInds.size() >= this->m_nrows) {
			this->m_rowindexes = oInds;
			this->form_displays_lists();
		}
	}
	void col_indexes(const sizets_vector &oInds) {
		if (oInds.size() >= this->m_ncols) {
			this->m_colindexes = oInds;
			this->form_displays_lists();
		}
	}
	void reset_indivs(void) {
		const size_t n = this->m_nrows;
		sizets_vector &vv = this->m_rowindexes;
		for (size_t i = 0; i < n; ++i) {
			vv[i] = i;
		}			// i
		this->form_displays_lists();
	}
	void reset_variables(void) {
		const size_t n = this->m_ncols;
		sizets_vector &vv = this->m_colindexes;
		for (size_t i = 0; i < n; ++i) {
			vv[i] = i;
		}			// i
		this->form_displays_lists();
	}
	void reset_indexes(void) {
		const size_t n1 = this->m_nrows;
		sizets_vector &vv1 = this->m_rowindexes;
		for (size_t i = 0; i < n1; ++i) {
			vv1[i] = i;
		}			// i
		const size_t n2 = this->m_ncols;
		sizets_vector &vv2 = this->m_colindexes;
		for (size_t i = 0; i < n2; ++i) {
			vv2[i] = i;
		}			// i
		this->form_displays_lists();
	}
	void set_result(MatElemResultPtr oRes) {
		MatElemResultType *p = oRes.get();
		if (p != nullptr) {
			if (p->disposition() == DispositionType::indiv) {
				this->row_indexes(p->indexes());
				for (auto &o : this->m_views) {
					ViewType *pp = o.get();
					pp->notify(oRes);
				}			// o
			} else if (p->disposition() == DispositionType::variable) {
				this->col_indexes(p->indexes());
				for (auto &o : this->m_views) {
					ViewType *pp = o.get();
					pp->notify(oRes);
				}			// o
			}
		}
	}
protected:
	virtual std::shared_ptr<ViewType> create_view(DispositionType atype, size_t nRows, size_t nCols,
		items_vector *pitems, function_type ff = [](MatElemResultPtr o) {})
	{
		return std::make_shared<ViewType>(atype, nRows, nCols, pitems, ff);
	}
	virtual PDrawItemType create_empty_item(void) {
		return (new DrawItemType());
	}			// create_tem
	virtual PDrawItemType create_varname_item(const STRINGTYPE &s) {
		return (new DrawItemType(MatCellType::varCell, s));
	}			// create_tem
	virtual PDrawItemType create_indname_item(const STRINGTYPE &s) {
		return (new DrawItemType(MatCellType::indCell, s));
	}			// create_tem
	virtual PDrawItemType create_varsum_item(FLOATTYPE val) {
		return (new DrawItemType(MatCellType::summaryVarCell, val));
	}			// create_tem
	virtual PDrawItemType create_indsum_item(FLOATTYPE val) {
		return (new DrawItemType(MatCellType::summaryIndCell, val));
	}			// create_tem
	virtual PDrawItemType create_histog_item(FLOATTYPE val) {
		return (new DrawItemType(MatCellType::histogCell, val));
	}			// create_tem
	virtual PDrawItemType create_plain_item(FLOATTYPE val) {
		return (new DrawItemType(MatCellType::plainCell, val));
	}			// create_tem
				//
	void form_displays_lists(void) {
		const size_t nRows = this->m_nrows;
		const size_t nCols = this->m_ncols;
		const size_t nTotalRows = nRows + 2;
		const size_t nTotalCols = nCols + 2;
		const sizets_vector &rowIndexes = this->m_rowindexes;
		const sizets_vector &colIndexes = this->m_colindexes;
		const items_vector &vv = this->m_items;
		items_vector &vv_inds = this->m_indivitems;
		items_vector &vv_vars = this->m_variableitems;
		for (size_t i = 0; i < nTotalRows; ++i) {
			if (i < 2) {
				vv_inds[i * nTotalCols] = vv[i * nTotalCols];
				for (size_t j = 0; j < nCols; ++j) {
					const size_t jj = colIndexes[j];
					vv_inds[i * nTotalCols + j + 1] =
							vv[i * nTotalCols + jj + 1];
				}		 // j
				vv_inds[i * nTotalCols + nTotalCols - 1] = vv[i * nTotalCols
						+ nTotalCols - 1];
			} else {
				const size_t dest_base = (size_t) (i * nTotalCols);
				const size_t iIndex = (size_t) (i - 2);
				const size_t ii = rowIndexes[iIndex];
				const size_t source_base = (ii + 2) * nTotalCols;
				vv_inds[dest_base] = vv[source_base];
				for (size_t j = 0; j < nCols; ++j) {
					const size_t jj = colIndexes[j];
					vv_inds[dest_base + j + 1] = vv[source_base + jj + 1];
				}		 // j
				vv_inds[dest_base + nCols + 1] = vv[source_base + nCols + 1];
			}
			for (size_t j = 0; j < nTotalCols; ++j) {
				vv_vars[j * nTotalRows + i] = vv_inds[i * nTotalCols + j];
			}		 // j
		}		 // i

	}		 // form_displays_lists
	void resize(size_t nRows, size_t nCols) {
		items_vector &vv = this->m_items;
		for (auto it = vv.begin(); it != vv.end(); ++it) {
			PDrawItemType p = *it;
			delete p;
		}
		vv.clear();
		size_t nn = (size_t) ((nRows + 2) * (nCols + 2));
		PDrawItemType pNull(nullptr);
		vv.resize(nn, pNull);
		this->m_indivitems.resize(nn);
		this->m_variableitems.resize(nn);
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
	}		 // resize
};
// class DrawItems<STRINGTYPE>
///////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE, typename INTTYPE, typename WEIGHTYPE>
class MatriceModelData {
	using ViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using strings_vector = std::vector<STRINGTYPE>;
	using MatriceDataType = MatriceData<IDTYPE,STRINGTYPE>;
	using MatriceType = InfoMatrice<IDTYPE,DISTANCETYPE,STRINGTYPE>;
	//
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using SourceType = typename MatElemType::SourceType;
	using MatElemResultType = typename MatElemType::MatElemResultType;
	using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
	using matelem_function = std::function<void(MatElemResultPtr)>;
	using matelem_future = std::future<MatElemResultPtr>;
	//
	using InfoMatriceResultPair = std::pair<MatElemResultPtr, MatElemResultPtr>;
	using InfoMatriceResultPairPtr = std::shared_ptr<InfoMatriceResultPair>;
	using matrice_promise = std::promise<InfoMatriceResultPairPtr>;
	using matrice_future = std::future<InfoMatriceResultPairPtr>;
	using matrice_promise_ptr = std::shared_ptr<matrice_promise>;
	using DrawItemsViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using ModelDataType = MatriceModelData<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE, INTTYPE, WEIGHTYPE>;
	using DrawItemsType = DrawItems<IDTYPE,DISTANCETYPE, STRINGTYPE,FLOATTYPE>;
	//
private:
	std::atomic<bool> m_inited;
	matelem_function m_f;
	std::unique_ptr<MatriceDataType> m_matdata;
	std::unique_ptr<MatriceType> m_matrice;
	std::unique_ptr<Backgrounder> m_backgrounder;
	std::unique_ptr<DrawItemsType> m_items;
	STRINGTYPE m_sigle;
protected:
	virtual DrawItemsType *create_drawitems(void) {
		return new DrawItemsType();
	}	  // create_drawitems
public:
	MatriceModelData() :
			m_inited(false) {
	}
	virtual ~MatriceModelData() {
	}
	ViewType *add_view(DispositionType aType) {
		ViewType *pRet = nullptr;
		if (!this->m_inited.load()) {
			return (pRet);
		}
		DrawItemsType *pItems = this->m_items.get();
		if (pItems == nullptr) {
			return (pRet);
		}
		pRet = pItems->add_view(aType);
		return (pRet);
	}	  // add_view
	template<typename DATATYPE>
	std::future<bool> initialize(const STRINGTYPE &sigle, size_t nRows,
			size_t nCols, const std::vector<DATATYPE> &data,
			const strings_vector &rowNames, const strings_vector &colNames,
			MatCellType aType = MatCellType::histogCell) {
		this->m_inited.store(false);
		return std::async(std::launch::async,
				[this, sigle, nRows, nCols, data, rowNames, colNames, aType]()->bool {
					this->m_sigle = sigle;
					if (this->m_backgrounder.get() == nullptr) {
						this->m_backgrounder.reset(new Backgrounder());
					}
					if (this->m_matdata.get() == nullptr) {
						this->m_matdata.reset(new MatriceDataType());
					}
					if (this->m_items.get() == nullptr) {
						this->m_items.reset(this->create_drawitems());
					}
					MatriceDataType *pData = this->m_matdata.get();
					assert(pData != nullptr);
					std::future<bool> bf = pData->initializeAsync(sigle,nRows,nCols,data,rowNames,colNames);
					bf.wait();
					DrawItemsType *pItems = this->m_items.get();
					assert(pItems != nullptr);
					bool bRet = pItems-> initialize(aType, nRows, nCols,
							pData->data(), rowNames,colNames, pData->indivs_summary(),
							pData->variables_summary(), sigle);
					if (bRet) {
						this->m_inited.store(true);
					}	  // bRet
					return (bRet);
				});
	}	  // initialize
	matrice_future compute(void) {
		return std::async(std::launch::async,
				[this]()->InfoMatriceResultPairPtr {
					InfoMatriceResultPairPtr oRet;
					if (this->m_inited.load()) {
						MatriceDataType *pData = this->m_matdata.get();
						assert(pData != nullptr);
						Backgrounder *pBack = this->m_backgrounder.get();
						assert(pBack != nullptr);
						oRet = MatriceType::perform_arrange(pData->indiv_provider(), pData->variable_provider(),
								nullptr, pBack,[this](MatElemResultPtr o) {
									DrawItemsType *pp = this->m_items.get();
									if (pp != nullptr) {
										pp->set_result(o);
									}
								},
								this->m_sigle,true);
					}
					return (oRet);
				});
	}	  // compute
};
// class MatriceModelData
}// namespace info
///////////////////////////////////
#endif // !__BASEDRAWITEM_H__

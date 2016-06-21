#pragma once
#ifndef __DLIB_MATRICE__H__
#define __DLIB_MATRICE__H__
/////////////////////////////////
#include <dlib/threads.h>
#include <dlib/gui_widgets.h>
///////////////////////////////////
#include "../info_drawcontext.h"
#include "../matrunner.h"
//////////////////////////////////////
namespace info {
///////////////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibBaseDrawItem: public BaseDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
protected:
	dlib::drawable_window &m_drawable;
public:
	DLibBaseDrawItem(dlib::drawable_window &w) :
			m_drawable(w) {
	}
	DLibBaseDrawItem(dlib::drawable_window &w, MatCellType t, FLOATTYPE val) :
			BaseType(t, val), m_drawable(w) {
	}
	DLibBaseDrawItem(dlib::drawable_window &w, MatCellType t,
			const STRINGTYPE &s) :
			BaseType(t, s), m_drawable(w) {
	}
	virtual ~DLibBaseDrawItem() {
	}
};
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibTextDrawItem: public DLibBaseDrawItem<STRINGTYPE, FLOATTYPE> {
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using BaseType = DLibBaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using ContextType = DrawContext<STRINGTYPE, FLOATTYPE>;
	using ItemType = DLibTextDrawItem<STRINGTYPE,FLOATTYPE>;
protected:
	dlib::label m_label;
public:
	DLibTextDrawItem(dlib::drawable_window &w, MatCellType t,
			const STRINGTYPE &s) :
			BaseType(w, t, s), m_label(w) {
		this->m_label.set_text(s);
	}
	virtual ~DLibTextDrawItem() {
	}
	virtual bool draw(ContextType *pContext, coord_type x0 = 0, coord_type y0 =
			0) {
		const DrawContextParams *pParams = pContext->draw_params();
		assert(pParams != nullptr);
		//dist_type w = pParams->dx;
		dist_type h = pParams->dy;
		if (h < 0) {
			return (false);
		}
		ItemType *p = const_cast<ItemType *>(this);
		p->m_label.set_pos(x0, y0 + h / 2);
		return (true);
	} // draw
};
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibIndivDrawItem: public DLibTextDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibTextDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibIndivDrawItem(dlib::drawable_window &w, const STRINGTYPE &s) :
			BaseType(w, MatCellType::indCell, s) {
	}
	virtual ~DLibIndivDrawItem() {
	}
};
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibVarDrawItem: public DLibTextDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibTextDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibVarDrawItem(dlib::drawable_window &w, const STRINGTYPE &s) :
			BaseType(w, MatCellType::varCell, s) {
	}
	virtual ~DLibVarDrawItem() {
	}
};
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibSummaryDrawItem: public DLibBaseDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibBaseDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibSummaryDrawItem(dlib::drawable_window &w, MatCellType t, FLOATTYPE val) :
			BaseType(w, t, val) {
	}
	virtual ~DLibSummaryDrawItem() {
	}
};
//
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibIndSummaryDrawItem: public DLibSummaryDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibSummaryDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibIndSummaryDrawItem(dlib::drawable_window &w, FLOATTYPE val) :
			BaseType(w, MatCellType::summaryIndCell, val) {
	}
	virtual ~DLibIndSummaryDrawItem() {
	}
};
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibVarSummaryDrawItem: public DLibSummaryDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibSummaryDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibVarSummaryDrawItem(dlib::drawable_window &w, FLOATTYPE val) :
			BaseType(w, MatCellType::summaryVarCell, val) {
	}
	virtual ~DLibVarSummaryDrawItem() {
	}
};
////////////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibHistogDrawItem: public DLibBaseDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibBaseDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibHistogDrawItem(dlib::drawable_window &w, FLOATTYPE val) :
			BaseType(w, MatCellType::histogCell, val) {
	}
	virtual ~DLibHistogDrawItem() {
	}
};
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibPlainDrawItem: public DLibBaseDrawItem<STRINGTYPE, FLOATTYPE> {
	using BaseType = DLibBaseDrawItem<STRINGTYPE, FLOATTYPE>;
public:
	DLibPlainDrawItem(dlib::drawable_window &w, FLOATTYPE val) :
			BaseType(w, MatCellType::plainCell, val) {
	}
	virtual ~DLibPlainDrawItem() {
	}
};
////////////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class DLibDrawContext: public DrawContext<STRINGTYPE, FLOATTYPE> {
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using PDrawItem = DrawItem *;
	using BaseType = DrawContext<STRINGTYPE, FLOATTYPE>;
private:
	const dlib::canvas &m_canvas;
public:
	DLibDrawContext(const dlib::canvas &m, DrawContextParams *params) :
			BaseType(params), m_canvas(m) {
	}
	virtual ~DLibDrawContext() {
	}
public:
	virtual void draw(PDrawItem pItem, coord_type x0 = 0, coord_type y0 = 0) {
		if (pItem == nullptr) {
			return;
		}
		const DrawContextParams *pParams = this->draw_params();
		assert(pParams != nullptr);
		dist_type w = pParams->dx;
		dist_type h = pParams->dy;
		if ((w <= 0) || (h <= 0)) {
			return;
		}
		dlib::rectangle rClip(x0, y0, x0 + w, y0 + h);
		dlib::rectangle area = m_canvas.intersect(rClip);
		if (area.is_empty() == true) {
			return;
		}
		MatCellType aType = pItem->type();
		switch (aType) {
		case MatCellType::indCell: {
			pItem->draw(this, x0, y0);
		}
			break;
		case MatCellType::varCell: {
			pItem->draw(this, x0, y0);
		}
			break;
		case MatCellType::summaryIndCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				dist_type hx = (h < w) ? h : w;
				double rf = hx * std::sqrt(v);
				int r = (int) rf;
				dlib::point center(x0 + (w / 2), y0 + (h / 2));
				dlib::rgb_pixel color(pParams->sumindcolor.red,
						pParams->sumindcolor.green, pParams->sumindcolor.blue);
				dlib::draw_solid_circle(m_canvas, center, r, color, rClip);
			}
		}
			break;
		case MatCellType::summaryVarCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				dist_type hx = (h < w) ? h : w;
				double rf = hx * std::sqrt(v);
				int r = (int) rf;
				dlib::point center(x0 + (w / 2), y0 + (h / 2));
				dlib::rgb_pixel color(pParams->sumvarcolor.red,
						pParams->sumvarcolor.green, pParams->sumvarcolor.blue);
				dlib::draw_solid_circle(m_canvas, center, r, color, rClip);
			}
		}
			break;
		case MatCellType::plainCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double f = 256.0 * v;
				int n = (int) f;
				if (n > 255) {
					n = 255;
				}
				dlib::rectangle rr(x0, y0, x0 + w, y0 + h);
				dlib::rgb_pixel color((unsigned char) n, (unsigned char) n,
						(unsigned char) n);
				dlib::fill_rect(m_canvas, rr, color);
			}
		}
			break;
		case MatCellType::histogCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double f = h * v;
				int hx = (int) f;
				dlib::rectangle rr(x0, y0 + h - hx, x0 + w, y0 + h);
				unsigned char c = pParams->downcolor.red;
				dlib::rgb_pixel color(c, c, c);
				dlib::fill_rect(m_canvas, rr, color);
			}
		}
			break;
		default:
			break;
		}
	} // draw
};
// class DLibDrawContext
/////////////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
class DLibDrawItems: public DrawItems<IDTYPE, DISTANCETYPE, STRINGTYPE,
		FLOATTYPE> {
public:
	using BaseType = DrawItems<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using BaseViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using ViewType = CRTDrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
private:
	dlib::drawable_window &m_drawable;
public:
	DLibDrawItems(dlib::drawable_window &w) :
			m_drawable(w) {
	}
	virtual ~DLibDrawItems() {
	}
protected:
	virtual std::shared_ptr<BaseViewType> create_view(DispositionType atype, size_t nRows, size_t nCols,
		items_vector *pitems, function_type ff = [](MatElemResultPtr o) {})
	{
		return std::make_shared<ViewType>(atype, nRows, nCols, pitems, ff);
	}
	virtual PDrawItemType create_empty_item(void) {
		return (new DLibBaseDrawItem<STRINGTYPE, FLOATTYPE>(this->m_drawable));
	}	// create_tem
	virtual PDrawItemType create_varname_item(const STRINGTYPE &s) {
		return (new DLibVarDrawItem<STRINGTYPE, FLOATTYPE>(this->m_drawable, s));
	}	// create_tem
	virtual PDrawItemType create_indname_item(const STRINGTYPE &s) {
		return (new DLibIndivDrawItem<STRINGTYPE, FLOATTYPE>(this->m_drawable,
				s));
	}	// create_tem
	virtual PDrawItemType create_varsum_item(FLOATTYPE val) {
		return (new DLibVarSummaryDrawItem<STRINGTYPE, FLOATTYPE>(
				this->m_drawable, val));
	}	// create_tem
	virtual PDrawItemType create_indsum_item(FLOATTYPE val) {
		return (new DLibIndSummaryDrawItem<STRINGTYPE, FLOATTYPE>(
				this->m_drawable, val));
	}	// create_tem
	virtual PDrawItemType create_histog_item(FLOATTYPE val) {
		return (new DLibHistogDrawItem<STRINGTYPE, FLOATTYPE>(this->m_drawable,
				val));
	}	// create_tem
	virtual PDrawItemType create_plain_item(FLOATTYPE val) {
		return (new DLibPlainDrawItem<STRINGTYPE, FLOATTYPE>(this->m_drawable,
				val));
	}	// create_tem
};
// DLibDrawItems
////////////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE, typename INTTYPE, typename WEIGHTYPE>
class DLibMatriceModelData: public MatriceModelData<IDTYPE, DISTANCETYPE,
		STRINGTYPE, FLOATTYPE, INTTYPE, WEIGHTYPE> {
	using BaseType = MatriceModelData<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE, INTTYPE, WEIGHTYPE>;
	using MatRunnerType = MatRunner<IDTYPE, STRINGTYPE, DISTANCETYPE, INTTYPE, WEIGHTYPE>;
	using DrawItemsType = DLibDrawItems<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
private:
	dlib::drawable_window &m_drawable;
public:
	DLibMatriceModelData(dlib::drawable_window& w) : m_drawable(w) {
	}
	virtual ~DLibMatriceModelData() {
	}
protected:
	virtual DrawItemsType *create_drawitems(void) {
		return new DrawItemsType(this->m_drawable);
	}	  // create_drawitems
};
// class MatriceModelData
//////////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
class MatriceDisplayWindow: public dlib::button {
	using ViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultType = typename MatElemType::MatElemResultType;
	using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
	using matelem_future = std::future<MatElemResultPtr>;
	using MatriceDisplayType = MatriceDisplayWindow<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using ContextType = DLibDrawContext<STRINGTYPE, FLOATTYPE>;
	//
private:
	dlib::label m_label;
	ViewType *m_pview;
	DrawContextParams m_params;
private:
	void notify(MatElemResultPtr oRes) {
		MatElemResultType *p = oRes.get();
		if ((this->m_pview != nullptr) && (p != nullptr)) {
			STRINGTYPE ss;
			p->to_string(ss, false);
			this->m_label.set_text(ss);
		}	// p
		this->show();
	}	// notify
public:
	MatriceDisplayWindow(dlib::drawable_window& w) :
			dlib::button(w), m_label(w), m_pview(nullptr) {
		this->show();
	}	//MatriceDisplayWindow
	~MatriceDisplayWindow() {
	}
	void set_view(ViewType *pView) {
		this->m_pview = pView;
		if (this->m_pview != nullptr) {
			this->m_pview->set_function([this](MatElemResultPtr o) {
				this->notify(o);
			});
		}
	}
protected:
	virtual void draw(const dlib::canvas& c) const {
		MatriceDisplayType *p = const_cast<MatriceDisplayType *>(this);
		if (this->m_pview != nullptr) {
			size_t nRows = this->m_pview->get_nb_rows();
			size_t nCols = this->m_pview->get_nb_cols();
			if ((nRows > 0) && (nCols > 0)) {
				unsigned long ww = this->width();
				unsigned long hh = this->height();
				long h0 = (long) (5 + this->m_label.height() + 3);
				{
					p->m_params.width = ww;
					p->m_params.height = hh - h0;
					//p->m_params.update(nRows,nCols,false);
				}
				dlib::rectangle r(0, h0, ww, hh);
				dlib::fill_rect(c, r, dlib::rgb_pixel(255, 255, 255));
				ContextType oContext(c, const_cast<DrawContextParams *>(&m_params));
				this->m_pview->draw(&oContext, 0, h0);
			}
		}	// pview
	}	// draw
};
// class MatriceDisplayWindow
////////////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // !__DLIB_MATRICE__H__


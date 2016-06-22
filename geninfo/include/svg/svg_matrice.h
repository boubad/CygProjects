#pragma once
#ifndef __SVG_MATRICE_H__
#define __SVG_MATRICE_H__
////////////////////////////////////////
#include "../info_drawcontext.h"
/////////////////////////////////////////
#include "../pugixml.hpp"
////////////////////////////////////////////
namespace info {
//////////////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class SVGDrawContext: public DrawContext<STRINGTYPE, FLOATTYPE> {
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using PDrawItem = DrawItem *;
	using BaseType = DrawContext<STRINGTYPE, FLOATTYPE>;
	using ContextType = SVGDrawContext<STRINGTYPE, FLOATTYPE>;
	//
	using xml_document = pugi::xml_document;
	using xml_node = pugi::xml_node;
	using xml_attribute = pugi::xml_attribute;
private:
	std::unique_ptr<xml_document> m_doc;
public:
	SVGDrawContext(DrawContextParams *params = nullptr) : BaseType(params) {
		this->initialize();
	}
	virtual ~SVGDrawContext() {
	}
private:
	void initialize(void) {
		this->m_doc.reset(new xml_document());
		xml_document *p0 = this->m_doc.get();
		assert(p0 != nullptr);
		xml_node oNode = p0->append_child("svg");
		xml_node *pDoc = &oNode;
		pDoc->set_name("svg");
		pDoc->append_attribute("xmlns:svg").set_value(
				"http://www.w3.org/2000/svg");
		pDoc->append_attribute("xmlns").set_value("http://www.w3.org/2000/svg");
		pDoc->append_attribute("version").set_value("1.1");
		//pDoc->append_attribute("viewBox").set_value("0 0 595 841");
		pDoc->append_attribute("width").set_value("100%");
		pDoc->append_attribute("height").set_value("100%");
	} // initialize
public:
	virtual void save(std::ostream &os, const std::string & /*stitle = std::string()*/) {
		xml_document *pDoc = this->m_doc.get();
		assert(pDoc != nullptr);
		pDoc->save(os);
	} // save
	virtual void save(std::wostream &os, const std::wstring &  /*= std::wstring()*/) {
		xml_document *pDoc = this->m_doc.get();
		assert(pDoc != nullptr);
		pDoc->save(os);
	} // save
	virtual void save(const std::string &filename) {
		xml_document *pDoc = this->m_doc.get();
		assert(pDoc != nullptr);
		pDoc->save_file(filename.c_str());
	} // save
	virtual void save(const std::wstring &filename) {
		xml_document *pDoc = this->m_doc.get();
		assert(pDoc != nullptr);
		pDoc->save_file(filename.c_str());
	} // save
	static void convert_color(const InfoColor &c, std::string &ss) {
		std::stringstream os;
		os << "rgb(" << (int) c.red << "," << (int) c.green << ","
				<< (int) c.blue << ")";
		ss = os.str();
	}
	static void convert_color(const InfoColor &c, std::wstring &ss) {
		std::wstringstream os;
		os << L"rgb(" << (int) c.red << L"," << (int) c.green << L","
				<< (int) c.blue << L")";
		ss = os.str();
	}
	static void convert_rotate(double angle, double x, double y,
			std::string &ss) {
		std::stringstream os;
		os << "rotate(" << angle << " " << x << "," << y << ")";
		ss = os.str();
	}
	static void convert_rotate(double angle, double x, double y,
			std::wstring &ss) {
		std::wstringstream os;
		os << L"rotate(" << angle << L" " << x << L"," << y << L")";
		ss = os.str();
	}
	static void convert_font_size(double fsize, std::string &ss) {
		std::stringstream os;
		os << "font-size:" << fsize << "px";
		ss = os.str();
	}
	static void convert_font_size(double fsize, std::wstring &ss) {
		std::wstringstream os;
		os << L"font-size:" << fsize << L"px";
		ss = os.str();
	}
	void set_viewBox(void) {
		std::stringstream os;
		const DrawContextParams *pParams = this->draw_params();
		os << "0 0 " << pParams->width << " " << pParams->height;
		std::string ss = os.str();
		xml_document *p0 = this->m_doc.get();
		assert(p0 != nullptr);
		(p0->document_element()).append_attribute("viewBox").set_value(ss.c_str());
	} // setViewBox
	virtual void draw(PDrawItem pItem, coord_type x0 = 0, coord_type y0 = 0)  {
		if (pItem == nullptr) {
			return;
		}
		MatCellType aType = pItem->type();
		if (aType == MatCellType::noCell) {
			return;
		}
		const DrawContextParams *pParams = this->draw_params();
		assert(pParams != nullptr);
		double w = (double) pParams->dx;
		double h = (double) pParams->dy;
		if ((w <= 0) || (h <= 0)) {
			return;
		}
		xml_document *p0 = this->m_doc.get();
		assert(p0 != nullptr);
		xml_node oNode = p0->document_element();
		xml_node *pDoc = &oNode;
		switch (aType) {
		case MatCellType::indCell: {
			std::string s = info_2s(pItem->text());
			if (!s.empty()) {
				double x = x0 + (w / 2) + pParams->indivFontSize;
				double y = y0 + h - pParams->indivFontSize;
				std::string srotate, sfont;
				convert_rotate(pParams->fangle, x, y, srotate);
				convert_font_size(pParams->indivFontSize, sfont);
				xml_node node = pDoc->append_child("text");
				node.append_attribute("x").set_value(x);
				node.append_attribute("y").set_value(y);
				node.append_attribute("style").set_value(sfont.c_str());
				node.append_attribute("transform").set_value(srotate.c_str());
				node.append_child(pugi::node_pcdata).set_value(s.c_str());
			} // s
		}
			break;
		case MatCellType::varCell: {
			std::string s = info_2s(pItem->text());
			if (!s.empty()) {
				double y = y0 + (h / 2) + (pParams->variableFontSize / 2);
				std::string sfont;
				convert_font_size(pParams->variableFontSize, sfont);
				xml_node node = pDoc->append_child("text");
				node.append_attribute("x").set_value((double) x0);
				node.append_attribute("y").set_value(y);
				node.append_attribute("style").set_value(sfont.c_str());
				node.append_child(pugi::node_pcdata).set_value(s.c_str());
			} // s
		}
			break;
		case MatCellType::histogCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double height = h * v;
				double y = y0 + h - height;
				xml_node node = pDoc->append_child("rect");
				node.append_attribute("x").set_value((double) x0);
				node.append_attribute("y").set_value(y);
				node.append_attribute("width").set_value(w);
				node.append_attribute("height").set_value(height);
				STRINGTYPE sFill;
				convert_color(pParams->downcolor, sFill);
				node.append_attribute("fill").set_value(sFill.c_str());
			}
		}
			break;
		case MatCellType::plainCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double f = 256.0 * v;
				unsigned char n = (unsigned char) f;
				if (n > 255) {
					n = 255;
				}
				STRINGTYPE sFill;
				convert_color(InfoColor(n), sFill);
				double height = h;
				double y = y0;
				xml_node node = pDoc->append_child("rect");
				node.append_attribute("x").set_value((double) x0);
				node.append_attribute("y").set_value(y);
				node.append_attribute("width").set_value(w);
				node.append_attribute("height").set_value(height);
				node.append_attribute("fill").set_value(sFill.c_str());
			} // v
		}
			break;
		case MatCellType::summaryVarCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double hx = (h < w) ? h : w;
				double r = hx * std::sqrt(v);
				double x = x0 + (w / 2);
				double y = y0 + (h / 2);
				std::string sFill;
				convert_color(pParams->sumvarcolor, sFill);
				xml_node node = pDoc->append_child("circle");
				node.append_attribute("cx").set_value(x);
				node.append_attribute("cy").set_value(y);
				node.append_attribute("r").set_value(r);
				node.append_attribute("fill").set_value(sFill.c_str());
			}
		}
			break;
		case MatCellType::summaryIndCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double hx = (h < w) ? h : w;
				double r = hx * std::sqrt(v);
				double x = x0 + (w / 2);
				double y = y0 + (h / 2);
				std::string sFill;
				convert_color(pParams->sumindcolor, sFill);
				xml_node node = pDoc->append_child("circle");
				node.append_attribute("cx").set_value(x);
				node.append_attribute("cy").set_value(y);
				node.append_attribute("r").set_value(r);
				node.append_attribute("fill").set_value(sFill.c_str());
			}
		}
			break;
		default:
			break;
		} // aType
	} // draw
};
// class SVGDrawContext
/////////////////////////////////////////
template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
class MatriceSVG {
	using ViewType = DrawItemsView<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using MatElemType = MatElem<IDTYPE, DISTANCETYPE, STRINGTYPE>;
	using MatElemResultType = typename MatElemType::MatElemResultType;
	using MatElemResultPtr = typename MatElemType::MatElemResultPtr;
	using matelem_future = std::future<MatElemResultPtr>;
	using MatriceSVGType = MatriceSVG <IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
	using ContextType = SVGDrawContext<STRINGTYPE, FLOATTYPE>;
	using dist_type = unsigned long;
	//
private:
	ViewType *m_pview;
	DrawContextParams m_params;
public:
	MatriceSVG() :
			m_pview(nullptr) {
	}	//MatriceDisplayWindow
	~MatriceSVG() {
	}
	void set_view(ViewType *pView) {
		this->m_pview = pView;
	}
public:
	void draw(const std::string &filename) {
		if (this->m_pview != nullptr) {
			size_t nRows = this->m_pview->get_nb_rows();
			size_t nCols = this->m_pview->get_nb_cols();
			if ((nRows > 0) && (nCols > 0)) {
				DrawContextParams *pParams = &(this->m_params);
				double dx = (double) pParams->width / (nCols + 3);
				double dy = (double) pParams->height / (nRows + 3);
				pParams->dx = (dist_type) dx;
				pParams->dy = (dist_type) dy;
				ContextType oContext(pParams);
				dist_type startx = (dist_type) (dx / 2);
				dist_type starty = (dist_type) (dy / 2);
				this->m_pview->draw(&oContext, startx, starty);
				oContext.save(filename);
			}
		}	// pview
	}	// draw
	void draw(const std::wstring &filename) {
		if (this->m_pview != nullptr) {
			size_t nRows = this->m_pview->get_nb_rows();
			size_t nCols = this->m_pview->get_nb_cols();
			if ((nRows > 0) && (nCols > 0)) {
				DrawContextParams *pParams = &(this->m_params);
				int k = 8;
				double dx = (double) (pParams->width * k)
						/ (nCols + k * nCols + 3 + 3 * k);
				double dy = (double) (pParams->height * k)
						/ (nRows + k * nRows + 3 + 3 * k);
				pParams->dx = (dist_type) dx;
				pParams->dy = (dist_type) dy;
				pParams->deltax = dx / k;
				pParams->deltay = dy / k;
				ContextType oContext(pParams);
				dist_type startx = (dist_type) (dx / 2);
				dist_type starty = (dist_type) (dy / 2);
				this->m_pview->draw(&oContext, startx, starty);
				oContext.save(filename);
			}
		}	// pview
	}	// draw
};
// class MatriceDisplayWindow
///////////////////////////////////////
}// namespace info
////////////////////////////////////////
#endif // !__SVG_MATRICE_H__


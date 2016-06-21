/*
 * eps_matrice.h
 *
 *  Created on: 21 juin 2016
 *      Author: boubad
 */

#ifndef EPS_EPS_MATRICE_H_
#define EPS_EPS_MATRICE_H_
////////////////////////////////////////
#include "../info_drawcontext.h"
#include <fstream>
/////////////////////////////////////////
namespace info {
//////////////////////////////
template<typename STRINGTYPE, typename FLOATTYPE>
class EPSDrawContext: public DrawContext<STRINGTYPE, FLOATTYPE> {
	using coord_type = DrawContextParams::coord_type;
	using dist_type = DrawContextParams::dist_type;
	using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
	using PDrawItem = DrawItem *;
	using BaseType = DrawContext<STRINGTYPE, FLOATTYPE>;
	using ContextType = EPSDrawContext<STRINGTYPE, FLOATTYPE>;
	//
private:
	coord_type m_maxx;
	coord_type m_maxy;
	coord_type m_minx;
	coord_type m_miny;
	bool m_first;
	std::unique_ptr<std::stringstream> m_os;
public:
	EPSDrawContext(DrawContextParams *params = nullptr) :
			BaseType(params), m_maxx(0), m_maxy(0), m_minx(0), m_miny(0), m_first(
					true) {
		this->initialize();
	}
	virtual ~EPSDrawContext() {
	}
private:
	void initialize(void) {
		this->m_os.reset(new std::stringstream());
	} // initialize
public:
	void save(std::ostream &os) {
		os << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
		os << "%%BoundingBox: " << ((int) this->m_minx) << " "
				<< ((int) this->m_miny);
		os << " " << ((int) (this->m_maxx + 1)) << " "
				<< ((int) (this->m_maxy + 1)) << std::endl;
		os << "%%Title: (Matrice)" << std::endl;
		os << "%%CreationDate: (10/17/89) (5:04 PM)" << std::endl;
		os << "%%DocumentFonts: Times-Roman" << std::endl;
		os << "%%DocumentNeededFonts: Times-Roman" << std::endl;
		os << "%%EndComments" << std::endl;
		std::string ss = this->m_os->str();
		os << ss;
		os << "%%EOF" << std::endl;
	} // save
	void save(const std::string &filename) {
		std::ofstream os(filename.c_str());
		os << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
		os << "%%BoundingBox: " << ((int) this->m_minx) << " "
				<< ((int) this->m_miny);
		os << " " << ((int) (this->m_maxx + 1)) << " "
				<< ((int) (this->m_maxy + 1)) << std::endl;
		os << "%%Title: " << filename << std::endl;
		os << "%%Creator: GenInfo stats suite" << std::endl;
		os << "%%CreationDate: (10/17/89) (5:04 PM)" << std::endl;
		os << "%%DocumentFonts: Times-Roman" << std::endl;
		os << "%%DocumentNeededFonts: Times-Roman" << std::endl;
		os << "%%EndComments" << std::endl;
		std::string ss = this->m_os->str();
		os << ss;
		os << "%%EOF" << std::endl;
	} // save

	static void convert_color(const InfoColor &c, std::string &ss) {
		std::stringstream os;
		os << (c.red / 255.0) << " " << (c.green / 255.0) << " "
				<< (c.blue / 255.0);
		os << " setrgbcolor";
		ss = os.str();
	}
	static void convert_font_size(double fsize, std::string &ss) {
		std::stringstream os;
		os << "/Times-Roman findfont" << std::endl;
		os << fsize << " scalefont" << std::endl;
		os << "setfont" << std::endl;
		ss = os.str();
	}

	virtual void draw(PDrawItem pItem, coord_type x0 = 0, coord_type y0 = 0) {
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
		coord_type yup = pParams->height - y0;
		coord_type ydown = yup - h;
		coord_type xf = x0 + w;
		coord_type yf = y0 + h;
		if (this->m_first) {
			this->m_first = false;
			this->m_minx = x0;
			this->m_miny = ydown;
			this->m_maxx = xf;
			this->m_maxy = yup;
		} else {
			if (x0 < this->m_minx) {
				this->m_minx = x0;
			}
			if (ydown < this->m_miny) {
				this->m_miny = ydown;
			}
			if (xf > this->m_maxx) {
				this->m_maxx = xf;
			}
			if (yup > this->m_maxy) {
				this->m_maxy = yup;
			}
		}
		std::stringstream &os = *(this->m_os);
		switch (aType) {
		case MatCellType::indCell: {
			std::string s = info_2s(pItem->text());
			if (!s.empty()) {
				double x = x0 + (w / 2) + (pParams->indivFontSize / 2);
				double y = ydown;
				std::string sfont;
				convert_font_size(pParams->indivFontSize, sfont);
				os << "gsave" << std::endl;
				os << sfont;
				os << x << " " << y << " moveto" << std::endl;
				os << "75 rotate" << std::endl;
				os << "(" << s << ") show" << std::endl;
				os << "grestore" << std::endl;
			} // s
		}
			break;
		case MatCellType::varCell: {
			std::string s = info_2s(pItem->text());
			if (!s.empty()) {
				double y = ydown + (h / 2) + (pParams->variableFontSize / 2);
				std::string sfont;
				convert_font_size(pParams->variableFontSize, sfont);
				os << sfont;
				os << x0 << " " << y << " moveto" << std::endl;
				os << "(" << s << ") show" << std::endl;
			} // s
		}
			break;
		case MatCellType::histogCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double height = h * v;
				os << "newpath" << std::endl;
				os << x0 << " " << ydown << " moveto" << std::endl;
				os << w << " 0 rlineto" << std::endl;
				os << "0 " << height << " rlineto" << std::endl;
				os << (-w) << " 0 rlineto" << std::endl;
				os << "closepath" << std::endl;
				STRINGTYPE sFill;
				convert_color(pParams->downcolor, sFill);
				os << sFill << std::endl;
				os << "fill" << std::endl;
			}
		}
			break;
		case MatCellType::plainCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				os << "newpath" << std::endl;
				os << x0 << " " << ydown << " moveto" << std::endl;
				os << w << " 0 rlineto" << std::endl;
				os << "0 " << h << " rlineto" << std::endl;
				os << (-w) << " 0 rlineto" << std::endl;
				os << "closepath" << std::endl;
				os << v << " setgray" << std::endl;
				os << "fill" << std::endl;
			} // v
		}
			break;
		case MatCellType::summaryVarCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double hx = (h < w) ? h : w;
				double r = hx * std::sqrt(v);
				double x = x0 + (w / 2);
				double y = ydown + (h / 2);
				std::string sFill;
				convert_color(pParams->sumvarcolor, sFill);
				os << "newpath" << std::endl;
				os << x << " " << y << " " << r << " 0 360 arc" << std::endl;
				os << sFill << std::endl;
				os << "fill" << std::endl;
			}
		}
			break;
		case MatCellType::summaryIndCell: {
			double v = (double) pItem->value();
			if ((v >= 0.0) && (v <= 1.0)) {
				double hx = (h < w) ? h : w;
				double r = hx * std::sqrt(v);
				double x = x0 + (w / 2);
				double y = ydown + (h / 2);
				std::string sFill;
				convert_color(pParams->sumindcolor, sFill);
				os << "newpath" << std::endl;
				os << x << " " << y << " " << r << " 0 360 arc" << std::endl;
				os << sFill << std::endl;
				os << "fill" << std::endl;
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
///////////////////////////////
}// namespace info
#endif /* EPS_EPS_MATRICE_H_ */

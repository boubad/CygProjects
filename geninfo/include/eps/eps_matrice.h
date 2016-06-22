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
	class EPSDrawContext : public DrawContext<STRINGTYPE, FLOATTYPE> {
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
		virtual void save(std::ostream &os, const std::string &stitle = std::string()) {
			const DrawContextParams *pParams = this->draw_params();
			std::time_t result = std::time(nullptr);
			std::string ssx = std::asctime(std::localtime(&result));
			os << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
			os << "%%Title: " << stitle << std::endl;
			os << "%%Creator: GenInfo stats suite" << std::endl;
			os << "%%CreationDate: " << ssx;
			os << "%%LanguageLevel: 2" << std::endl;
			os << "%%BoundingBox: " << ((int) this->m_minx) << " "
				<< ((int) this->m_miny);
			os << " " << ((int)(this->m_maxx + 1)) << " "
				<< ((int)(this->m_maxy + 1)) << std::endl;
			os << "%%DocumentNeededFonts: Times-Roman" << std::endl;
			os << "%%EndComments" << std::endl;
			os << "/w "  << pParams->dx << "  def" << std::endl;
			os << "/n {newpath} bind def" << std::endl;
			os << "/rg { setrgbcolor} bind def" << std::endl;
			os << "/sf { scalefont setfont} bind def" << std::endl;
			os << "/i { moveto gsave 75 rotate show grestore } bind  def" << std::endl;
			os << "/v { moveto show } bind def" << std::endl;
			os << "/s { 0 360 arc fill }  bind def" << std::endl;
			os << "/h { moveto w 0 rlineto 0 exch rlineto 0 w sub 0 rlineto closepath fill } bind def" << std::endl;
			std::string ss = this->m_os->str();
			os << ss;
			os << "%%EOF" << std::endl;
		} // save
		virtual void save(std::wostream &wos, const std::wstring &stitle = std::wstring()) {
			std::string s1 = info_2s(stitle);
			std::stringstream os;
			this->save(os, s1);
			std::string s2 = os.str();
			std::wstring sz = info_2ws(s2);
			wos << sz;
		}// save
		void save(std::wostream &wos) {
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
			std::wofstream os(filename.c_str());
			this->save(os, filename);
		} // save
		virtual void set_indivs_font(void) {
			std::stringstream &os = *(this->m_os);
			const DrawContextParams *pParams = this->draw_params();
			os << "/Times-Roman findfont" << std::endl;
			os << pParams->indivFontSize << " sf" << std::endl;
		}//set_indivs_font
		virtual void set_variables_font(void) {
			std::stringstream &os = *(this->m_os);
			const DrawContextParams *pParams = this->draw_params();
			os << "/Times-Roman findfont" << std::endl;
			os << pParams->variableFontSize << " sf" << std::endl;
		}//set_indivs_font
		virtual void set_variables_font_color(void) {
			std::stringstream &os = *(this->m_os);
			os << "0.0 0.0 0.0 rg" << std::endl;
		}//set_variables_font_color
		virtual void set_indivs_summary_color(void) {
			std::stringstream &os = *(this->m_os);
			const DrawContextParams *pParams = this->draw_params();
			InfoColor c = pParams->sumindcolor;
			os << (c.red / 255.0) << " " << (c.green / 255.0) << " " << (c.blue / 255.0) << " rg" << std::endl;
		}// set_indivs_summary_color
		virtual void set_variables_summary_color(void) {
			std::stringstream &os = *(this->m_os);
			const DrawContextParams *pParams = this->draw_params();
			InfoColor c = pParams->sumvarcolor;
			os << (c.red / 255.0) << " " << (c.green / 255.0) << " " << (c.blue / 255.0) << " rg" << std::endl;
		}// set_variables_summary_color
		virtual void set_histog_color(void) {
			std::stringstream &os = *(this->m_os);
			const DrawContextParams *pParams = this->draw_params();
			InfoColor c = pParams->downcolor;
			os << (c.red / 255.0) << " " << (c.green / 255.0) << " " << (c.blue / 255.0) << " rg" << std::endl;
		}// set_histog_color
		virtual void set_plain_color(double f) {
			const DrawContextParams *pParams = this->draw_params();
			std::stringstream &os = *(this->m_os);
			InfoColor c = pParams->downcolor;
			os << f << " " << f << " " << f << " rg" << std::endl;
		}// set_plain_color
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
			double w = (double)pParams->dx;
			double h = (double)pParams->dy;
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
			}
			else {
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
					os << "(" << s << ") " << x << " " << y << " i" << std::endl;
				} // s
			}
									   break;
			case MatCellType::varCell: {
				std::string s = info_2s(pItem->text());
				if (!s.empty()) {
					double y = ydown + (h / 2) + (pParams->variableFontSize / 2);
					os << "(" << s << ") " << x0 << " " << y << " v" << std::endl;
				} // s
			}
									   break;
			case MatCellType::histogCell: {
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					double height = h * v;
					os << "n " << height << " " <<  x0 << " " << ydown << " h" << std::endl;
				}
			}
										  break;
			case MatCellType::plainCell: {
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					this->set_plain_color(v);
					os << "n " << h << " " << x0 << " " << ydown << " h" << std::endl;
				} // v
			}
										 break;
			case MatCellType::summaryVarCell: {
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					double hx = (h < w) ? h : w;
					double r = hx * std::sqrt(v);
					double x = x0 + (w / 2);
					double y = ydown + (h / 2);
					os << x << " " << y << " " << r << " s" << std::endl;
				}
			}
											  break;
			case MatCellType::summaryIndCell: {
				double v = (double)pItem->value();
				if ((v >= 0.0) && (v <= 1.0)) {
					double hx = (h < w) ? h : w;
					double r = hx * std::sqrt(v);
					double x = x0 + (w / 2);
					double y = ydown + (h / 2);
					os << x << " " << y << " " << r << " s" << std::endl;
				}
			}
											  break;
			default:
				break;
			} // aType
		} // draw
	};
	// class SVGDrawContext
	///////////////////////////////
}// namespace info
#endif /* EPS_EPS_MATRICE_H_ */

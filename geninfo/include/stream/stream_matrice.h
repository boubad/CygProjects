#pragma once
#ifndef __STREAM_MATRICE_H__
#define __STREAM_MATRICE_H__
////////////////////////////////////////
#include "../info_drawcontext.h"
/////////////////////////////////////////
namespace info {
	//////////////////////////////////////
	template<typename STRINGTYPE, typename FLOATTYPE>
	class StreamDrawContext : public DrawContext<STRINGTYPE, FLOATTYPE> {
		using coord_type = DrawContextParams::coord_type;
		using dist_type = DrawContextParams::dist_type;
		using DrawItem = BaseDrawItem<STRINGTYPE, FLOATTYPE>;
		using PDrawItem = DrawItem *;
		using BaseType = DrawContext<STRINGTYPE, FLOATTYPE>;
		using ContextType = StreamDrawContext<STRINGTYPE, FLOATTYPE>;
	private:
		std::unique_ptr<std::stringstream> m_os;
	public:
		StreamDrawContext(DrawContextParams *params = nullptr) : BaseType(params) {
			this->initialize();
		}
		virtual ~StreamDrawContext() {
		}
	private:
		void initialize(void) {
			this->m_os.reset(new std::stringstream());
		} // initialize
	public:
		virtual void set_separator(void) {
			std::stringstream & out = *(this->m_os);
			out << ", ";
		}
		virtual void set_endline(void) {
			std::stringstream & out = *(this->m_os);
			out << std::endl;
		}
		virtual void save(std::ostream &os, const std::string & stitle = std::string()) {
			std::stringstream & out = *(this->m_os);
			if (!stitle.empty()) {
				os << stitle << std::endl;
			}
			const DrawContextParams *pParams = this->draw_params();
			assert(pParams != nullptr);
			os << pParams->nCols << ", " << pParams->nRows << std::endl;
			//
			std::string s = out.str();
			os << s;
		} // save
		virtual void draw(PDrawItem pItem, coord_type x0 = 0, coord_type y0 = 0) {
			if (pItem == nullptr) {
				return;
			}
			MatCellType aType = pItem->type();
			if (aType == MatCellType::noCell) {
				return;
			}
			std::stringstream & os = *(this->m_os);
			switch (aType) {
			case MatCellType::indCell: {
				std::string s = info_2s(pItem->text());
				os << s;
			}
									   break;
			case MatCellType::varCell: {
				std::string s = info_2s(pItem->text());
				os << s;
			}
									   break;
			case MatCellType::histogCell: {
				double v = (double)pItem->value();
				os << v;
			}
										  break;
			case MatCellType::plainCell: {
				double v = (double)pItem->value();
				os << v;
			}
										 break;
			case MatCellType::summaryVarCell: {
				double v = (double)pItem->value();
				os << v;
			}
											  break;
			case MatCellType::summaryIndCell: {
				double v = (double)pItem->value();
				os << v;
			}
											  break;
			default:
				break;
			} // aType
		} // draw
	};
	// class SVGDrawContext
	/////////////////////////////////////////
}// namespace info
 ////////////////////////////////////////
#endif // !__SVG_MATRICE_H__


#pragma once
#ifndef __SVG_ARRANGER_H__
#define __SVG_ARRANGER_H__
/////////////////////////////////
#include "base_arranger.h"
#include "svg/svg_matrice.h"
/////////////////////////////
namespace info {
/////////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
		class SVGMatriceArranger :public BASEMatriceArranger<IDTYPE,DISTANCETYPE,STRINGTYPE,FLOATTYPE>{
		public:
			using BaseType = BASEMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
			using ContextType = SVGDrawContext<STRINGTYPE, FLOATTYPE>;
		public:
			SVGMatriceArranger(const STRINGTYPE &sSigle = STRINGTYPE()) : BaseType(sSigle) {
			}
			virtual ~SVGMatriceArranger() {
			}
		protected:
			virtual ContextType *create_context(void) {
				return (new ContextType());
			}
	};
////////////////////////////////////////////////////
}// namespace info
////////////////////////////////////
#endif // !__SVG8ARRAGER_H__


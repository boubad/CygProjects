#pragma once
#ifndef __STREAM_ARRANGER_H__
#define __STREAM_ARRANGER_H__
/////////////////////////////////
#include "base_arranger.h"
#include "stream/stream_matrice.h"
/////////////////////////////
namespace info {
	/////////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
		class StreamMatriceArranger :public BASEMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE> {
		public:
			using BaseType = BASEMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
			using ContextType = StreamDrawContext<STRINGTYPE, FLOATTYPE>;
		public:
			StreamMatriceArranger(const STRINGTYPE &sSigle = STRINGTYPE()) : BaseType(sSigle) {
			}
			virtual ~StreamMatriceArranger() {
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


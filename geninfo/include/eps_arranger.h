/*
 * eps_arranger.h
 *
 *  Created on: 22 juin 2016
 *      Author: boubad
 */

#ifndef EPS_ARRANGER_H_
#define EPS_ARRANGER_H_
 /////////////////////////////////
#include "base_arranger.h"
#include "eps/eps_matrice.h"
/////////////////////////////
namespace info {
	/////////////////////////////////////////////////
	template<typename IDTYPE, typename DISTANCETYPE, typename STRINGTYPE,
		typename FLOATTYPE>
		class EPSMatriceArranger :public BASEMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE> {
		public:
			using BaseType = BASEMatriceArranger<IDTYPE, DISTANCETYPE, STRINGTYPE, FLOATTYPE>;
			using ContextType = EPSDrawContext<STRINGTYPE, FLOATTYPE>;
		public:
			EPSMatriceArranger(const STRINGTYPE &sSigle = STRINGTYPE()) : BaseType(sSigle) {
			}
			virtual ~EPSMatriceArranger() {
			}
		protected:
			virtual ContextType *create_context(void) {
				return (new ContextType());
			}
	};
	////////////////////////////////////////////////////
}// namespace info
///////////////////////////////////




#endif /* EPS_ARRANGER_H_ */

#pragma once
#ifndef __DISTANCEFIXTURE_H__
#define  __DISTANCEFIXTURE_H__
/////////////////////////////////
#include "indivproviderfixture.h"
///////////////////////////////
#include <crititem.h>
////////////////////////////////////
class DistanceFixture : public IndivProviderFixture {
protected:
	std::unique_ptr<info::IndivDistanceMap> m_odistances;
public:
	DistanceFixture();
	virtual ~DistanceFixture();
};// class  DatabaseFixture
/////////////////////////////////////
#endif /* INDIVPROVIDERFIXTURE_H_ */

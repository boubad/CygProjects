#include "distancefixture.h"
//////////////////////////
using namespace info;
////////////////////////
DistanceFixture::DistanceFixture() {
	m_odistances.reset(new IndivDistanceMap());
	IndivDistanceMap *pDist = m_odistances.get();
	assert(pDist != nullptr);
	IIndivProvider *p = this->m_provider.get();
	assert(p != nullptr);
	info_global_compute_distances(p, *pDist);
}
DistanceFixture::~DistanceFixture() {
	m_odistances.reset();
}

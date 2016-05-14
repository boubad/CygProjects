/*
 * test_distances.cpp
 *
 *  Created on: 13 mai 2016
 *      Author: boubad
 */
#include "indivproviderfixture.h"
#include <storeindivprovider.h>
//////////////////////////
using namespace info;
////////////////////////
IndivProviderFixture::IndivProviderFixture() {
	IStoreHelper *p = this->m_man.get();
	assert(p != nullptr);
	this->m_provider.reset(new StoreIndivProvider(p, this->m_oset));
	assert(this->m_provider.get() != nullptr);
}
IndivProviderFixture::~IndivProviderFixture() {
	this->m_provider.reset();
}
void IndivProviderFixture::write_point(const DbValueMap &oMap, std::string &s) {
	std::stringstream os;
	std::for_each(oMap.begin(), oMap.end(),
			[&](const std::pair<IntType, DbValue> &p) {
				IntType key = p.first;
				const DbValue &v = p.second;
				std::string ss;
				v.string_value(ss);
				os << "(" << key << "," << ss << "), ";
			});
	s = os.str();
}


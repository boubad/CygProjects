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
void IndivProviderFixture::write_indis_map(const info::ints_size_t_map &oMap, std::string &s) {
	std::map<size_t, ints_vector> rMap;
	std::for_each(oMap.begin(), oMap.end(), [&rMap](const std::pair<IntType, size_t> &p) {
		IntType ind = p.first;
		size_t  cluster = p.second;
		auto it = rMap.find(cluster);
		if (it == rMap.end()) {
			ints_vector v;
			v.push_back(ind);
			rMap[cluster] = v;
		}
		else {

			ints_vector &v = (*it).second;
			v.push_back(ind);
		}
	});
	std::stringstream os;
	os << std::endl;
	std::for_each(rMap.begin(), rMap.end(), [&os](const std::pair<size_t, ints_vector> &p) {
		os << "cluster " << p.first << ":`\t";
		const ints_vector &v = p.second;
		for (auto it = v.begin(); it != v.end(); ++it) {
			if (it != v.begin()) {
				os << ", ";
			}
			os << (*it);
		}
		os << std::endl;
	});
	s = os.str();
}//write_indis_map

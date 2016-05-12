#include "../include/indiv.h"
///////////////////////////////
namespace info {
////////////////////////////////////////
extern bool info_global_get_random_indivs(const size_t n,
		IIndivProvider *pProvider,
		info_indivs_vector &oVec){
	assert(n > 0);
	assert(pProvider != nullptr);
	assert(pProvider->is_valid());
	//
	std::srand(unsigned(std::time(0)));
	oVec.clear();
	size_t nCount = 0;
	if (!pProvider->indivs_count(nCount)){
		return (false);
	}
	if (n > nCount){
		return (false);
	}
	std::vector<size_t> indexes(nCount);
	for (size_t i = 0; i < nCount; ++i){
		indexes[i] = i;
	}
	std::random_shuffle(indexes.begin(), indexes.end());
	oVec.resize(n);
	for (size_t i = 0; i < n; ++i){
		size_t pos = indexes[i];
		Indiv oInd;
		pProvider->find_indiv_at(pos,oInd);
		oVec[i] = oInd;
	}// i
	return (true);
}//info_global_get_random_indivs
///////////////////////////////////////////
Indiv::Indiv() {
}
Indiv::Indiv(const IntType aIndex) :
		DBStatIndiv(aIndex) {

}
Indiv::Indiv(const IntType aIndex, const DbValueMap &oMap) :
		DBStatIndiv(aIndex), m_map(oMap) {

}
Indiv::Indiv(const DBStatIndiv &oBaseInd) :
		DBStatIndiv(oBaseInd) {

}
Indiv::Indiv(const Indiv &other) :
		DBStatIndiv(other), m_map(other.m_map) {

}
Indiv & Indiv::operator=(const Indiv &other) {
	if (this != &other) {
		DBStatIndiv::operator=(other);
		this->m_map = other.m_map;
	}
	return (*this);
}
Indiv::~Indiv() {

}
bool Indiv::empty(void) const{
	return (this->m_map.empty());
}
const DbValueMap &Indiv::data(void) const {
	return (this->m_map);
}
bool Indiv::has_variable(const IntType key) const {
	auto it = this->m_map.find(key);
	if (it == this->m_map.end()) {
		return (false);
	}
	const DbValue &v = (*it).second;
	return (!v.empty());
}
void Indiv::set_variable(const IntType key, const DbValue &v) {
	(this->m_map)[key] = DbValue(v);
}
void Indiv::swap(Indiv &other) {
	Indiv t(*this);
	*this = other;
	other = t;
}
void Indiv::get_data(DbValueMap &oMap) const {
	oMap = this->m_map;
}
void Indiv::set_data(const DbValueMap &oMap) {
	this->m_map = oMap;
}
double Indiv::distance(const Indiv &other) const {
	const DbValueMap &m1 = this->data();
	const DbValueMap &m2 = other.data();
	size_t nc = 0;
	double dRet = 0;
	std::for_each(m1.begin(), m1.end(), [m2, &nc, &dRet](const std::pair<IntType, DbValue> &oPair) {
		const IntType key = oPair.first;
		const DbValue &v1 = oPair.second;
		if (!v1.empty()) {
			auto jt = m2.find(key);
			if (jt != m2.end()) {
				const DbValue &v2 = (*jt).second;
				if (!v2.empty()) {
					double t = v1.double_value() - v2.double_value();
					dRet += t * t;
					++nc;
				}// ok
			}// found
		}// v1
	});
	if (nc > 1) {
		dRet /= nc;
	}
	return dRet;
} // distance
//////////////////////////////////////////
}	// namespace info

#include "../include/indiv.h"
////////////////////////////////
namespace info {
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
	DbValueMap m1, m2;
	double dRet = 0;
	this->get_data(m1);
	other.get_data(m2);
	size_t nc = 0;
	BOOST_FOREACH(auto oPair, m1) {
		const IntType key = oPair.first;
		auto jt = m2.find(key);
		if (jt != m2.end()) {
			const DbValue &v1 = oPair.second;
			const DbValue &v2 = (*jt).second;
			double t = v1.double_value() - v2.double_value();
			dRet += t * t;
			++nc;
		} // found
	} // oPair
	if (nc > 1) {
		dRet /= nc;
	}
	return dRet;
} // distance
//////////////////////////////////////////
}	// namespace info

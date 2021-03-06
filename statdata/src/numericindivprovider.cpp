/*
 * numericindivprovider.cpp
 *
 *  Created on: 8 mai 2016
 *      Author: boubad
 */

#include "../include/numericindivprovider.h"
///////////////////////////////
namespace info {
//////////////////////////////
static void filter_indiv(statinfos_map &filter, Indiv &oInd,
		TransformationType fTransf) {
	DbValueMap oRes;
	DbValueMap &src = const_cast<DbValueMap &>(oInd.data());
	typedef std::pair<IntType, StatInfo> MyPair;
	std::for_each(filter.begin(), filter.end(), [&](const MyPair &oPair) {
		DbValue v;
		IntType key = oPair.first;
		auto it = src.find(key);
		if (it != src.end()) {
			DbValue vx = (*it).second;
			if (vx.empty()) {
				const StatInfo &info = oPair.second;
				double vm, vv, vs;
				info.get_mean_var_std(vm, vv, vs);
				v = DbValue(vm);
			}
			else {
				v = DbValue(vx.double_value());
			}
		}
		else {
			const StatInfo &info = oPair.second;
			double vm, vv, vs;
			info.get_mean_var_std(vm, vv, vs);
			v = DbValue(vm);
		}
		switch (fTransf) {
			case TransformationType::normalized:
			{
				const StatInfo &info = oPair.second;
				double vm, vv, vs = 0;
				info.get_mean_var_std(vm, vv, vs);
				if (vs != 0) {
					v = DbValue((v.double_value() - vm) / vs);
				}
			}
			break;
			case TransformationType::recoded:
			{
				const StatInfo &info = oPair.second;
				double vmin, vmax;
				info.get_min_max(vmin, vmax);
				v = DbValue((v.double_value() - vmin) / (vmax - vmin));
			}
			break;
			default:
			break;
		} // fTransf
			oRes[key] = v;
		});
	oInd.set_data(oRes);
} //filter_indiv
////////////////////////////
NumericIndivProvider::NumericIndivProvider(IIndivProvider *pProvider,
		TransformationType fTransf /*= TransformationType::noTransform*/) :
		m_provider(pProvider), m_transf(fTransf) {
	IIndivProvider *pMan = this->m_provider;
	assert(pMan != nullptr);
	assert(pMan->is_valid());
	(void) info::info_global_compute_stats(pMan, this->m_stats);
	typedef std::pair<IntType, StatInfo> MyPair;
	ints_vector &vv = this->m_ids;
	vv.clear();
	std::for_each(this->m_stats.begin(), this->m_stats.end(),
			[&](const MyPair &oPair) {
				vv.push_back(oPair.first);
			});
}

NumericIndivProvider::~NumericIndivProvider() {
}
bool NumericIndivProvider::get_variables_ids(ints_vector &oVec) {
	assert(this->is_valid());
	oVec = this->m_ids;
	return (true);
} //get_variables_ids
bool NumericIndivProvider::find_indiv_at(const size_t pos, IntType &aIndex,
		doubles_vector &data) {
	assert(this->is_valid());
	aIndex = 0;
	data.clear();
	Indiv oInd;
	VariableMode mode = VariableMode::modeNumeric;
	if (!this->find_indiv_at(pos, oInd, mode)) {
		return (false);
	}
	aIndex = oInd.id();
	const DbValueMap &oMap = oInd.data();
	const ints_vector &vv = this->m_ids;
	const size_t n = vv.size();
	data.resize(n);
	for (size_t i = 0; i < n; ++i) {
		const IntType key = vv[i];
		auto it = oMap.find(key);
		assert(it != oMap.end());
		DbValue v = (*it).second;
		data[i] = v.double_value();
	} // i
	return (true);
} //find_indiv_at
bool NumericIndivProvider::find_indiv(const IntType aIndex,
		doubles_vector &data) {
	assert(this->is_valid());
	data.clear();
	Indiv oInd;
	VariableMode mode = VariableMode::modeNumeric;
	if (!this->find_indiv(aIndex, oInd, mode)) {
		return (false);
	}
	const DbValueMap &oMap = oInd.data();
	const ints_vector &vv = this->m_ids;
	const size_t n = vv.size();
	data.resize(n);
	for (size_t i = 0; i < n; ++i) {
		const IntType key = vv[i];
		auto it = oMap.find(key);
		assert(it != oMap.end());
		DbValue v = (*it).second;
		data[i] = v.double_value();
	} // i
	return (true);
} //find_indiv
bool NumericIndivProvider::get_statinfo(const IntType key, StatInfo &oInfo) {
	assert(this->is_valid());
	statinfos_map &oMap = this->m_stats;
	auto it = oMap.find(key);
	if (it != oMap.end()) {
		oInfo = (*it).second;
		return (true);
	}
	return (false);
}
bool NumericIndivProvider::is_valid(void) {
	IIndivProvider *pMan = this->m_provider;
	return ((pMan != nullptr) && pMan->is_valid() && (!this->m_stats.empty()));
}
bool NumericIndivProvider::indivs_count(size_t &nCount) {
	assert(this->is_valid());
	return (this->m_provider->indivs_count(nCount));
}
TransformationType  NumericIndivProvider::transformation(void) const{
	 return (this->m_transf);
}
void  NumericIndivProvider::transormation(TransformationType t){
	this->m_transf = t;
}
bool NumericIndivProvider::get_variables_map(variables_map &oMap) {
	assert(this->is_valid());
	oMap.clear();
	variables_map vars;
	if (!this->m_provider->get_variables_map(vars)) {
		return (false);
	}
	const statinfos_map &filter = this->m_stats;
	typedef std::pair<IntType, DBStatVariable> MyPair;
	std::for_each(vars.begin(), vars.end(), [&](const MyPair &oPair) {
		const IntType key = oPair.first;
		if (filter.find(key) != filter.end()) {
			oMap[key] = oPair.second;
		} // ok
		});
	return (true);
}
bool NumericIndivProvider::find_indiv(const IntType aIndex, Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeAll*/) {
	assert(this->is_valid());
	if (!this->m_provider->find_indiv(aIndex, oInd, mode)) {
		return (false);
	}
	filter_indiv(this->m_stats, oInd, this->m_transf);
	return (true);
} //find_indiv
bool NumericIndivProvider::find_indiv_at(const size_t pos, Indiv &oInd,
		const VariableMode mode /*= VariableMode::modeAll*/) {
	assert(this->is_valid());
	if (!this->m_provider->find_indiv_at(pos, oInd, mode)) {
		return (false);
	}
	filter_indiv(this->m_stats, oInd, this->m_transf);
	return (true);
} //find_indiv_at
///////////////////////////
} /* namespace info */

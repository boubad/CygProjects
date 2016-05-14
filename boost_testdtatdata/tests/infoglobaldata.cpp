#include "infoglobaldata.h"
////////////////////////////
#include "infotestdata.h"
/////////////////////////////
using namespace info;
////////////////////////////////////
InfoGlobalInit::InfoGlobalInit() {
	this->init_data();
	this->fill_mortal_data();
	//this->fill_conso_data();
} // init
InfoGlobalInit::~InfoGlobalInit() {
	this->data_teardown();
}
void  InfoGlobalInit::data_teardown(void) {
	this->m_man.reset();
}// data_teardown
void InfoGlobalInit::fill_mortal_data(void) {
	std::string name;
	size_t nRows = 0, nCols = 0;
	std::vector<int> gdata;
	std::vector<std::string> rowNames, colNames;
	InfoTestData::get_mortal_data(name, nRows, nCols, gdata, rowNames,
		colNames);
	assert(!name.empty());
	assert(nRows > 2);
	assert(nCols > 2);
	assert(colNames.size() >= nCols);
	assert(rowNames.size() >= nRows);
	assert(gdata.size() >= (size_t)(nCols * nRows));
	this->import(name, nRows, nCols, gdata, rowNames, colNames);
}// fill_mortal_data
void InfoGlobalInit::fill_conso_data(void) {
	std::string name;
	size_t nRows = 0, nCols = 0;
	std::vector<int> gdata;
	std::vector<std::string> rowNames, colNames;
	InfoTestData::get_conso_data(name, nRows, nCols, gdata, rowNames,
		colNames);
	assert(!name.empty());
	assert(nRows > 2);
	assert(nCols > 2);
	assert(colNames.size() >= nCols);
	assert(rowNames.size() >= nRows);
	assert(gdata.size() >= (size_t)(nCols * nRows));
	this->import(name, nRows, nCols, gdata, rowNames, colNames);
}// fill_mortal_data
void InfoGlobalInit::init_data(void) {
	std::string filename;
	InfoTestData::get_database_filename(filename);
	assert(!filename.empty());
	m_man.reset(new SQLiteStatHelper(filename));
	IStoreHelper *p = this->m_man.get();
	assert(p != nullptr);
	assert(p->is_valid());
}// init_data
void InfoGlobalInit::import(const std::string &name, size_t nRows, size_t nCols,
	const std::vector<int> &data,
	const std::vector<std::string> &rowNames,
	const std::vector<std::string> &colNames) {
	IStoreHelper *p = this->m_man.get();
	assert(p != nullptr);
	assert(p->is_valid());
	DBStatDataset oSet(name);
	bool bRet;
	if (!p->find_dataset(oSet)) {
		bRet = p->maintains_dataset(oSet);
		assert(bRet);
	}
	assert(oSet.id() != 0);
	variables_vector oVars;
	for (size_t i = 0; i < nCols; ++i) {
		std::string sigle = colNames[i];
		DBStatVariable v(oSet, sigle);
		if (!p->find_variable(v)) {
			oVars.push_back(v);
		}
	} // i
	if (!oVars.empty()) {
		bRet = p->maintains_variables(oVars);
		assert(bRet);
	}
	indivs_vector oInds;
	for (size_t i = 0; i < nRows; ++i) {
		std::string sigle = rowNames[i];
		DBStatIndiv v(oSet, sigle);
		if (!p->find_indiv(v)) {
			oInds.push_back(v);
		}
	} // i
	if (!oInds.empty()) {
		bRet = p->maintains_indivs(oInds);
		assert(bRet);
	}
	oInds.clear();
	bRet = p->find_dataset_indivs(oSet, oInds, 0, (int)nRows);
	assert(bRet);
	assert(nRows == oInds.size());
	oVars.clear();
	bRet = p->find_dataset_variables(oSet, oVars, 0, (int)nCols);
	assert(bRet);
	assert(nCols == oVars.size());
	//
	std::map<std::string, DBStatVariable *> pVars;
	std::for_each(colNames.begin(), colNames.end(),
		[&](const std::string &s) {
		std::string sigle = s;
		std::string rsigle;
		DBStatVariable ovar(oSet, sigle);
		ovar.get_sigle(rsigle);
		DBStatVariable *p = nullptr;
		for (size_t i = 0; i < oVars.size(); ++i) {
			DBStatVariable &vv = oVars[i];
			std::string sx;
			vv.get_sigle(sx);
			if (sx == rsigle) {
				p = &vv;
				break;
			}
		} // i
		assert(p != nullptr);
		pVars[sigle] = p;
	});
	std::map<std::string, DBStatIndiv *> pInds;
	std::for_each(rowNames.begin(), rowNames.end(),
		[&](const std::string &s) {
		std::string sigle = s;
		std::string rsigle;
		DBStatIndiv ovar(oSet, sigle);
		ovar.get_sigle(rsigle);
		DBStatIndiv *p = nullptr;
		for (size_t i = 0; i < oInds.size(); ++i) {
			DBStatIndiv &vv = oInds[i];
			std::string sx;
			vv.get_sigle(sx);
			if (sx == rsigle) {
				p = &vv;
				break;
			}
		} // i
		assert(p != nullptr);
		pInds[sigle] = p;
	});
	values_vector oVals;
	for (size_t i = 0; i < nRows; ++i) {
		std::string sigleind = rowNames[i];
		DBStatIndiv *pInd = pInds[sigleind];
		assert(pInd != nullptr);
		for (size_t j = 0; j < nCols; ++j) {
			std::string siglevar = colNames[j];
			DBStatVariable *pVar = pVars[siglevar];
			assert(pVar != nullptr);
			DBStatValue val(*pVar, *pInd);
			if (!p->find_value(val)) {
				float f = (float)data[i * nCols + j];
				DbValue vv(f);
				val.set_value(vv);
				oVals.push_back(val);
			}
		} // j
	} // i
	if (!oVals.empty()) {
		bRet = p->maintains_values(oVals);
		assert(bRet);
	}
} // import

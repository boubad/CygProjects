///////////////////////////////
#include "memorystorefixture.h"
////////////////////////////
#include "infotestdata.h"
/////////////////////////////
using namespace info;
////////////////////////////////////
void MemoryStoreFixture::init_data(void) {
	m_man.reset(new MemoryStoreType());
	MemoryStoreType *p = this->m_man.get();
	assert(p != nullptr);
	assert(p->is_valid());
}// init_data
MemoryStoreFixture::IStoreType *MemoryStoreFixture::get_store(void) {
	IStoreType *p = this->m_man.get();
	assert(p != nullptr);
	assert(p->is_valid());
	return(p);
}
MemoryStoreFixture::MemoryStoreFixture():m_nbrows(0),m_nbcols(0) {
	this->init_data();
	this->fill_mortal_data();
	this->fill_conso_data();
	std::string sigle;
	InfoTestData::get_mortal_name(sigle);
	this->m_oset.sigle(sigle);
	IStoreType *p = this->m_man.get();
	assert(p != nullptr);
	bool bRet = p->find_dataset(this->m_oset);
	assert(bRet);
	assert(this->m_oset.id() != 0);
} // init
MemoryStoreFixture::~MemoryStoreFixture() {
	this->data_teardown();
}
void  MemoryStoreFixture::data_teardown(void) {
	this->m_man.reset();
}// data_teardown
void MemoryStoreFixture::fill_mortal_data(void) {
	std::string name;
	size_t nRows = 0, nCols = 0;
	std::vector<int> gdata;
	std::vector<std::string> rowNames, colNames;
	InfoTestData::get_mortal_data(name, nRows, nCols, gdata, rowNames,
		colNames);
	assert(!name.empty());
	assert(nRows > 2);
	assert(nCols > 2);
	this->m_nbrows = nRows;
	this->m_nbcols = nCols;
	assert(colNames.size() >= nCols);
	assert(rowNames.size() >= nRows);
	assert(gdata.size() >= (size_t)(nCols * nRows));
	this->import(name, nRows, nCols, gdata, rowNames, colNames);
}// fill_mortal_data
void MemoryStoreFixture::fill_conso_data(void) {
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

void MemoryStoreFixture::import(const std::string &name,
	size_t nRows, size_t nCols,
	const std::vector<int> &data,
	const std::vector<std::string> &rowNames,
	const std::vector<std::string> &colNames) {
	MemoryStoreType *p = this->m_man.get();
	assert(p != nullptr);
	assert(p->is_valid());
	std::string stype("float");
	bool bRet = p->import(name, nRows, nCols, data, rowNames, colNames, stype);
	assert(bRet);
} // import

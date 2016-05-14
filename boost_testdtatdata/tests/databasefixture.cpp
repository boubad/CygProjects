#include "databasefixture.h"
#include "infotestdata.h"
/////////////////////////////
using namespace std;
using namespace info;
//////////////////////////////
DatabaseFixture::DatabaseFixture() :m_nbcols(0), m_nbrows(0) {
	string filename;
	InfoTestData::get_database_filename(filename);
	assert(!filename.empty());
	this->m_man.reset(new SQLiteStatHelper(filename));
	IStoreHelper *p = this->m_man.get();
	assert(p != nullptr);
	assert(p->is_valid());
	string name;
	InfoTestData::get_mortal_name(name, m_nbrows, m_nbcols);
	assert(!name.empty());
	this->m_oset.set_sigle(name);
	bool bRet = p->find_dataset(this->m_oset);
	assert(bRet);
	assert(this->m_oset.id() != 0);
};
DatabaseFixture::~DatabaseFixture() {
	this->m_man.reset();
};

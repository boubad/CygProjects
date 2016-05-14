#pragma once
/////////////////////////////
#include <sqlitestathelper.h>
////////////////////////////////
class DatabaseFixture {
protected:
	size_t	 m_nbcols;
	size_t	 m_nbrows;
	std::unique_ptr<info::SQLiteStatHelper> m_man;
	info::DBStatDataset m_oset;
public:
	DatabaseFixture();
	virtual ~DatabaseFixture();
};// class  DatabaseFixture
  //

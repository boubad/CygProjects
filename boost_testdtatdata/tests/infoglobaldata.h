#pragma once
////////////////////////
#include <sqlitestathelper.h>
/////////////////////////////
class InfoGlobalInit {
protected:
	std::unique_ptr<info::SQLiteStatHelper> m_man;
public:
	InfoGlobalInit();
	virtual ~InfoGlobalInit();
private:
	void  data_teardown(void);
	void fill_mortal_data(void);
	void fill_conso_data(void);
	void init_data(void);
	void import(const std::string &name, size_t nRows, size_t nCols,
		const std::vector<int> &data,
		const std::vector<std::string> &rowNames,
		const std::vector<std::string> &colNames);
};

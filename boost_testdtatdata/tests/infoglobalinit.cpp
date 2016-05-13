/*
 * infoglobalinit.cpp
 *
 *  Created on: 13 mai 2016
 *      Author: boubad
 */
#include <boost/test/unit_test.hpp>
///////////////////////////////
#include <sqlitestathelper.h>
#include "infotestdata.h"
/////////////////////////////
#include <boost/foreach.hpp>
//////////////////////////////
using namespace info;
////////////////////////////////
class InfoGlobalInit {
private:
	std::unique_ptr<SQLiteStatHelper> m_man;
	std::string m_filename;
public:
	InfoGlobalInit() {
		InfoTestData::get_database_filename(m_filename);
		BOOST_REQUIRE(!m_filename.empty());
		m_man.reset(new SQLiteStatHelper(m_filename));
		IStoreHelper *p = this->m_man.get();
		BOOST_REQUIRE(p != nullptr);
		BOOST_REQUIRE(p->is_valid());
		std::string name;
		size_t nRows = 0, nCols = 0;
		std::vector<int> gdata;
		std::vector<std::string> rowNames, colNames;
		InfoTestData::get_mortal_data(name, nRows, nCols, gdata, rowNames,
				colNames);
		BOOST_REQUIRE(!name.empty());
		BOOST_REQUIRE(nRows > 2);
		BOOST_REQUIRE(nCols > 2);
		BOOST_REQUIRE(colNames.size() >= nCols);
		BOOST_REQUIRE(rowNames.size() >= nRows);
		BOOST_REQUIRE(gdata.size() >= (size_t ) (nCols * nRows));
		this->import(name, nRows, nCols, gdata, rowNames, colNames);
	} // init
	~InfoGlobalInit() {
	}
private:
	void import(const std::string &name, size_t nRows, size_t nCols,
			const std::vector<int> &data,
			const std::vector<std::string> &rowNames,
			const std::vector<std::string> &colNames) {
#if defined(MYTOTO)
		IStoreHelper *p = this->m_man.get();
		BOOST_REQUIRE(p != nullptr);
		BOOST_REQUIRE(p->is_valid());
		DBStatDataset oSet(name);
		bool bRet;
		if (!p->find_dataset(oSet)) {
			bRet = p->maintains_dataset(oSet);
			BOOST_REQUIRE(bRet);
		}
		BOOST_REQUIRE(oSet.id() != 0);
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
			BOOST_REQUIRE(bRet);
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
			BOOST_REQUIRE(bRet);
		}
		oInds.clear();
		bRet = p->find_dataset_indivs(oSet, oInds, 0, nRows);
		BOOST_REQUIRE(bRet);
		BOOST_REQUIRE(nRows == oInds.size());
		oVars.clear();
		bRet = p->find_dataset_variables(oSet, oVars, 0, nCols);
		BOOST_REQUIRE(bRet);
		BOOST_REQUIRE(nCols == oVars.size());
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
					BOOST_REQUIRE(p != nullptr);
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
					BOOST_REQUIRE(p != nullptr);
					pInds[sigle] = p;
				});
		values_vector oVals;
		for (size_t i = 0; i < nRows; ++i) {
			std::string sigleind = rowNames[i];
			DBStatIndiv *pInd = pInds[sigleind];
			BOOST_REQUIRE(pInd != nullptr);
			for (size_t j = 0; j < nCols; ++j) {
				std::string siglevar = colNames[j];
				DBStatVariable *pVar = pVars[siglevar];
				BOOST_REQUIRE(pVar != nullptr);
				DBStatValue val(*pVar, *pInd);
				if (!p->find_value(val)) {
					float f = (float) data[i * nCols + j];
					DbValue vv(f);
					val.set_value(vv);
					oVals.push_back(val);
				}
			} // j
		} // i
		if (!oVals.empty()) {
			bRet = p->maintains_values(oVals);
			BOOST_REQUIRE(bRet);
		}
#endif
	} // import
};
BOOST_GLOBAL_FIXTURE(InfoGlobalInit);

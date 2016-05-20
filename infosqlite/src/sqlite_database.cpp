/*
* database.cpp
*
*  Created on: 16 sept. 2013
*      Author: Boubacar
*/
#include "./sqlite/sqlite3.h"
////////////////////////
#include <stringconvert.h>
#include "../include/sqlite_database.h"
#include "../include/sqlite_statement.h"
/////////////////////////////
namespace info_sqlite {
	using namespace info;
	///////////////////////////////////////////////
	static const char *SQL_COMMIT_TRANSACTION = "COMMIT TRANSACTION";
	static const char *SQL_BEGIN_TRANSACTION = "BEGIN TRANSACTION";
	static const char *SQL_ROLLBACK_TRANSACTION = "ROLLBACK TRANSACTION";
	////////////////////////////////////////////
	SQLite_Database::SQLite_Database() : m_pDb(nullptr) {
	}
	SQLite_Database::SQLite_Database(const char *pszFilename) : m_pDb(nullptr) {
		std::string s(pszFilename);
		this->open(s);
	} // SQLite_Database
	SQLite_Database::SQLite_Database(const wchar_t *pszFilename) : m_pDb(nullptr) {
		std::wstring ss(pszFilename);
		std::string s = StringConvert::ws2s(ss);
		this->open(s);
	} // SQLite_Database
	SQLite_Database::SQLite_Database(const std::string &filename) : m_pDb(nullptr) {
		this->open(filename);
	} // SQLite_Database
	SQLite_Database::SQLite_Database(const std::wstring &filename) : m_pDb(nullptr) {
		std::string s = StringConvert::ws2s(filename);
		this->open(filename);
	} // SQLite_Database
	void SQLite_Database::internal_get_error(void) {
		::sqlite3 *p = this->m_pDb;
		if (p != nullptr) {
			int rc = ::sqlite3_errcode(p);
			const char *pSrc = ::sqlite3_errmsg(p);
			if (pSrc != nullptr) {
				{
					std::string message = pSrc;
					throw sqlite_error(rc, message);
				}
			}
		}
	} // internal_get_error
	bool SQLite_Database::is_open(void) const {
		return (this->m_pDb != nullptr);
	} // is_open
	bool SQLite_Database::has_error(void) const {
		int rc = 0;
		std::string s;
		if (this->get_last_error(rc, s)) {
			return (rc != 0);
		}
		return (false);
	} // has_error
	bool SQLite_Database::get_last_error(int &rc, std::string &sErr) const {
		rc = 0;
		sErr.clear();
		::sqlite3 *p = this->m_pDb;
		if (p != nullptr) {
			rc = ::sqlite3_errcode(p);
			const char *pSrc = ::sqlite3_errmsg(p);
			if (pSrc != nullptr) {
				{
					sErr = pSrc;
					return (true);
				}
			}
		}
		return (false);
	} // get_last_error
	bool SQLite_Database::get_last_error(int &rc, std::wstring &sErr) const {
		std::string s;
		if (this->get_last_error(rc, s)) {
			sErr = StringConvert::s2ws(s);
			return (true);
		}
		return (false);
	} // get_last_error
	bool SQLite_Database::open(const char *pszFilename) {
		::sqlite3 *p = this->m_pDb;
		if (p != nullptr) {
			if (!this->close()) {
				return (false);
			}
		}
		int rc = ::sqlite3_open_v2(pszFilename, &p,
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
		if (rc != SQLITE_OK) {
			this->internal_get_error();
		}
		this->m_pDb = p;
		return (rc == SQLITE_OK);
	} // open
	bool SQLite_Database::open(const wchar_t *pszFilename) {
		std::wstring ss(pszFilename);
		std::string s = StringConvert::ws2s(ss);
		return this->open(s.c_str());
	} // open
	bool SQLite_Database::open(const std::string &filename) {
		return this->open(filename.c_str());
	} // open
	bool SQLite_Database::open(const std::wstring &filename) {
		return this->open(filename.c_str());
	} // open
	SQLite_Database::~SQLite_Database() {
		this->close();
	}
	bool SQLite_Database::close(void) {
		bool bRet = true;
		::sqlite3 *p = this->m_pDb;
		if (p != nullptr) {
			if (::sqlite3_close_v2(p) != SQLITE_OK) {
				this->internal_get_error();
				bRet = false;
			}
			else {
				this->m_pDb = nullptr;
			}
		}
		return (bRet);
	} // close
	  ///////////////////////////////
	bool SQLite_Database::exec_sql(const wchar_t *pszSQL) {
		std::wstring ss(pszSQL);
		std::string s = StringConvert::ws2s(ss);
		return (this->exec_sql(s.c_str()));
	} // exec_sql
	bool SQLite_Database::exec_sql(const char *pszSQL) {
		::sqlite3 *p = this->m_pDb;
		if (p == nullptr) {
			return (false);
		}
		char *error = nullptr;
		int rc = ::sqlite3_exec(p, pszSQL, nullptr, nullptr, &error);
		if (error != nullptr) {
			std::string message = error;
			::sqlite3_free(error);
			throw sqlite_error(rc, message);
		}
		return (rc == SQLITE_OK);
	} // exec_sql
	bool SQLite_Database::exec_sql(const std::string &s) {
		return this->exec_sql(s.c_str());
	} // exec_sql
	bool SQLite_Database::exec_sql(const std::wstring &s) {
		return this->exec_sql(s.c_str());
	} // exec_sql
	void SQLite_Database::begin_transaction(void) {
		this->exec_sql(SQL_BEGIN_TRANSACTION);
	}
	void SQLite_Database::commit_transaction(void) {
		this->exec_sql(SQL_COMMIT_TRANSACTION);
	}
	void SQLite_Database::rollback_transaction(void) {
		this->exec_sql(SQL_ROLLBACK_TRANSACTION);
	}
	////////////////////////////
} /* namespace sqlite */

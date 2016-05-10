#pragma once
#ifndef __SQLITE_DATABASE_H__
#define __SQLITE_DATABASE_H__
/////////////////////////////////
#include "info_constants.h"
////////////////////////////////////
typedef struct sqlite3 sqlite3;
////////////////////////////////////////
namespace info {
	///////////////////////////////////////
	class sqlite_error : public std::runtime_error {
	public:
		sqlite_error(int nCode,const std::string &what_arg) :std::runtime_error(what_arg),_code(nCode) {}
		sqlite_error(int nCode,const char *what_arg):std::runtime_error(what_arg),_code(nCode){}
		sqlite_error(const sqlite_error &other):std::runtime_error(other),_code(other._code){}
		sqlite_error & operator=(const sqlite_error &other) {
			if (this != &other) {
				std::runtime_error::operator=(other);
				this->_code = other._code;
			}
			return (*this);
		}
		virtual ~sqlite_error(){}
		int code(void) const {
			return (this->_code);
		}
	private:
		int _code;
	};// class sqlite_error
	///////////////////////////////////////////
	class SQLite_Statement;
	/////////////////////////////////////////
	class SQLite_Database : private boost::noncopyable {
		friend class SQLite_Statement;
	private:
		::sqlite3 *m_pDb;
	public:
		SQLite_Database();
		SQLite_Database(const char *pszFilename);
		SQLite_Database(const wchar_t *pszFilename);
		SQLite_Database(const std::string &filename);
		SQLite_Database(const std::wstring &filename);
		virtual ~SQLite_Database();
	public:
		bool open(const char *pszFilename);
		bool open(const wchar_t *pszFilename);
		bool open(const std::string &filename);
		bool open(const std::wstring &filename);
		bool is_open(void) const;
		bool has_error(void) const;
		bool get_last_error(int &rc,std::string &sErr) const;
		bool get_last_error(int &rc,std::wstring &sErr) const;
		bool close(void);
		bool exec_sql(const char *pszSQL);
		bool exec_sql(const wchar_t *pszSQL);
		bool exec_sql(const std::string &s);
		bool exec_sql(const std::wstring &s);
		void exec(const std::string &s) {
			this->exec_sql(s);
		}
		void exec(const std::wstring &s) {
			this->exec_sql(s);
		}
		void exec(const char *s) {
			this->exec_sql(s);
		}
		void exec(const wchar_t *s) {
			this->exec_sql(s);
		}
	public:
		void begin_transaction(void);
		void commit_transaction(void);
		void rollback_transaction(void);
	protected:
		void internal_get_error(void);
	}; // class SQLite_Database
	/////////////////////////////////////////
}// namespace info
//////////////////////////////////////
#endif // !__SQLITE_DATABASE_H__

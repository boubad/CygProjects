/*
* statement.cpp
*
*  Created on: 17 sept. 2013
*      Author: Boubacar
*/
#include "./sqlite/sqlite3.h"
//////////////////////////////
#include <infovalue.h>
#include <stringconvert.h>
#include "../include/sqlite_database.h"
#include "../include/sqlite_statement.h"
//////////////////////////////////
namespace info_sqlite {
	using namespace info;
	////////////////////////////////////////////
	void SQLite_Statement::init(SQLite_Database &oBase, const char *pszSQL) {
		assert(oBase.is_open());
		assert(pszSQL != nullptr);
		//
		this->m_pBase = &oBase;
		this->m_pstmt = nullptr;
		this->m_lastcode = SQLITE_OK;
		::sqlite3 *pdb = this->m_pBase->m_pDb;
		assert(pdb != nullptr);
		const char *pszTail = nullptr;
		int nbytes = (int)(::strlen(pszSQL) + 1);
		::sqlite3_stmt *pRes = nullptr;
		int rc = ::sqlite3_prepare_v2(pdb, pszSQL, nbytes, &pRes, &pszTail);
		if ((rc == SQLITE_OK) && (pRes != nullptr)) {
			this->m_pstmt = pRes;
		}
		else {
			this->m_pBase->internal_get_error();
		}
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database *pBase, const char *pszSQL) {
		this->init(*pBase, pszSQL);
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database &oBase, const wchar_t *pszSQL) {
		std::wstring ss(pszSQL);
		std::string s = StringConvert::ws2s(ss);
		this->init(&oBase, s.c_str());
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database *pBase, const wchar_t *pszSQL) {
		std::wstring ss(pszSQL);
		std::string s = StringConvert::ws2s(ss);
		this->init(pBase, s.c_str());
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database &oBase, const std::string &s) {
		this->init(&oBase, s.c_str());
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database *pBase, const std::string &s) {
		this->init(pBase, s.c_str());
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database &oBase, const std::wstring &ss) {
		this->init(&oBase, ss.c_str());
	} // SQLite_Statement
	void SQLite_Statement::init(SQLite_Database *pBase, const std::wstring &ss) {
		this->init(pBase, ss.c_str());
	} // SQLite_Statement
	  ////////////////////////////////////////////////
	SQLite_Statement::SQLite_Statement(SQLite_Database &oBase, const char *pszSQL) :
		m_first(false),m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(oBase, pszSQL);
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database *pBase, const char *pszSQL) :
		m_first(false), m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(pBase, pszSQL);
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database &oBase, const wchar_t *pszSQL) :
		m_first(false), m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(&oBase, pszSQL);
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database *pBase, const wchar_t *pszSQL) :
		m_first(false), m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(pBase, pszSQL);
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database &oBase, const std::string &s) :
		m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(&oBase, s.c_str());
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database *pBase, const std::string &s) :
		m_first(false), m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(pBase, s.c_str());
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database &oBase, const std::wstring &ss) :
		m_first(false), m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(&oBase, ss.c_str());
	} // SQLite_Statement
	SQLite_Statement::SQLite_Statement(SQLite_Database *pBase, const std::wstring &ss) :
		m_first(false), m_pBase(nullptr), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
		this->init(pBase, ss.c_str());
	} // SQLite_Statement
	bool SQLite_Statement::is_valid(void) const {
		return ((this->m_pBase != nullptr) && m_pBase->is_open()
			&& (this->m_pstmt != nullptr));
	} // is_valid
	int SQLite_Statement::cols(void) const {
		return ((int) this->m_vals.size());
	}
	bool SQLite_Statement::has_values(void) const {
		return (this->m_vals.size() > 0);
	}
	bool SQLite_Statement::col_name(int icol, std::string &sRet) const {
		bool bRet = false;
		const strings_vector &vv = this->m_names;
		if ((icol >= 0) && ((size_t)icol < vv.size())) {
			sRet = vv[icol];
			bRet = true;
		}
		return (bRet);
	} // col_name
	bool SQLite_Statement::col_name(int icol, std::wstring &sRet) const {
		std::string s;
		if (!this->col_name(icol, s)) {
			return (false);
		}
		sRet = StringConvert::s2ws(s);
		return (true);
	}
	bool SQLite_Statement::col_value(int icol, InfoValue &val) const {
		bool bRet = false;
		const values_vector &vv = this->m_vals;
		if ((icol >= 0) && ((size_t)icol < vv.size())) {
			val = vv[icol];
			bRet = true;
		}
		return (bRet);
	} // col_value
	bool SQLite_Statement::col_value(const std::string &sname, InfoValue &val) const {
		bool bRet = false;
		const strings_vector &names = this->m_names;
		const int n = (int)names.size();
		for (int i = 0; i < n; ++i) {
			const std::string & s = names[i];
			if (s == sname) {
				return (this->col_value(i, val));
			}
		} // i
		return (bRet);
	} // col_value
	bool SQLite_Statement::col_value(const std::wstring &sname, InfoValue &val) const {
		std::string s = StringConvert::ws2s(sname);
		return this->col_value(s, val);
	}
	bool SQLite_Statement::exec(void) {
		this->m_first = false;
		values_vector &vals = this->m_vals;
		vals.clear();
		strings_vector &names = this->m_names;
		names.clear();
		::sqlite3_stmt *p = this->m_pstmt;
		this->m_lastcode = SQLITE_OK;
		int rc = ::sqlite3_step(p);
		this->m_lastcode = rc;
		if (rc == SQLITE_DONE) {
			return this->reset();
		}
		else if (rc != SQLITE_ROW) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		else {
			int nCount = ::sqlite3_column_count(p);
			if (nCount < 1) {
				return (true);
			}
			this->m_first = true;
			names.resize(nCount);
			vals.resize(nCount);
			for (int icol = 0; icol < nCount; ++icol) {
				const char *pn = ::sqlite3_column_name(p, icol);
				if (pn != nullptr) {
					names[icol] = pn;
				}
				int ntype = ::sqlite3_column_type(p, icol);
				switch (ntype) {
				case SQLITE_INTEGER: {
					int ival = ::sqlite3_column_int(p, icol);
					vals[icol] = InfoValue(ival);
				}
									 break;
				case SQLITE_FLOAT: {
					double dval = ::sqlite3_column_double(p, icol);
					vals[icol] = InfoValue(dval);
				}
								   break;
				case SQLITE_TEXT: {
					std::string sval;
					const unsigned char *ptext = ::sqlite3_column_text(p, icol);
					if (ptext != nullptr) {
						const char *ps = reinterpret_cast<const char *>(ptext);
						size_t n = ::strlen(ps);
						std::string ss(n, ' ');
						std::copy(ps, ps + n, ss.begin());
						sval = ss;
					}
					vals[icol] = InfoValue(sval);
				}
								  break;
				case SQLITE_BLOB: {
					
				}
								  break;
				default:
					break;
				} // nType
			} // icol
		}
		return (true);
	} // exec
	bool SQLite_Statement::next(void) {
		if (this->m_first) {
			this->m_first = false;
			return (true);
		}
		int rc = this->m_lastcode;
		if (rc != SQLITE_ROW) {
			return (false);
		}
		::sqlite3_stmt *p = this->m_pstmt;
		rc = ::sqlite3_step(p);
		this->m_lastcode = rc;
		if (rc != SQLITE_ROW) {
			return (false);
		}
		int nCount = ::sqlite3_column_count(p);
		if (nCount < 1) {
			return (true);
		}
		values_vector &vals = this->m_vals;
		if (vals.size() != (size_t)nCount) {
			vals.resize(nCount);
		}
		for (int icol = 0; icol < nCount; ++icol) {
			InfoValue v;
			vals[icol] = v;
			int ntype = ::sqlite3_column_type(p, icol);
			switch (ntype) {
			case SQLITE_INTEGER: {
				int ival = ::sqlite3_column_int(p, icol);
				vals[icol] = InfoValue(ival);
			}
								 break;
			case SQLITE_FLOAT: {
				double dval = ::sqlite3_column_double(p, icol);
				vals[icol] = InfoValue(dval);
			}
							   break;
			case SQLITE_TEXT: {
				std::string sval;
				const unsigned char *ptext = ::sqlite3_column_text(p, icol);
				if (ptext != nullptr) {
					const char *ps = reinterpret_cast<const char *>(ptext);
					size_t n = ::strlen(ps);
					std::string ss(n, ' ');
					std::copy(ps, ps + n, ss.begin());
					sval = ss;
				}
				vals[icol] = InfoValue(sval);
			}
							  break;
			case SQLITE_BLOB: {
				
			}
							  break;
			default:
				break;
			} // nType
		} // icol
		return (true);
	} // next
	int SQLite_Statement::get_parameters_count(void) {
		int nRet = 0;
		::sqlite3_stmt *p = this->m_pstmt;
		if (p != nullptr) {
			nRet = ::sqlite3_bind_parameter_count(p);
		}
		return (nRet);
	}// get_parameters_count
	bool SQLite_Statement::close(void) {
		bool bRet = true;
		::sqlite3_stmt *p = this->m_pstmt;
		if (p != nullptr) {
			int rc = ::sqlite3_finalize(p);
			if (rc != SQLITE_OK) {
				bRet = false;
				this->m_pBase->internal_get_error();
			}
			else {
				this->m_pstmt = nullptr;
			}
		}
		return (bRet);
	} // close
	bool SQLite_Statement::reset(void) {
		::sqlite3_stmt *p = this->m_pstmt;
		assert(p != nullptr);
		if (::sqlite3_reset(p) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		this->m_names.clear();
		this->m_vals.clear();
		return true;
	} // reset
	bool SQLite_Statement::set_parameter_null(int iParam) {
		::sqlite3_stmt *p = this->m_pstmt;
		if (::sqlite3_bind_null(p, iParam) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		return (true);
	} // set_parameter_null
	bool SQLite_Statement::set_parameter(int iParam, int ivalue) {
		::sqlite3_stmt *p = this->m_pstmt;
		if (::sqlite3_bind_int(p, iParam, ivalue) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		return (true);
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, long ivalue) {
		::sqlite3_stmt *p = this->m_pstmt;
		if (::sqlite3_bind_int(p, iParam, ivalue) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		return (true);
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, unsigned long ivalue) {
		::sqlite3_stmt *p = this->m_pstmt;
		if (::sqlite3_bind_int(p, iParam, ivalue) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		return (true);
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, double dval) {
		::sqlite3_stmt *p = this->m_pstmt;
		if (::sqlite3_bind_double(p, iParam, dval) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		return (true);
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, const char *pszVal) {
		::sqlite3_stmt *p = this->m_pstmt;
		int nbytes = -1;
		if (::sqlite3_bind_text(p, iParam, pszVal, nbytes,
			SQLITE_TRANSIENT) != SQLITE_OK) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		return (true);
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, const wchar_t *pwszVal) {
		std::wstring ss(pwszVal);
		std::string s = StringConvert::ws2s(ss);
		const char *px = s.c_str();
		return this->set_parameter(iParam, px);
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, const std::string &sVal) {
		return this->set_parameter(iParam, sVal.c_str());
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, const std::wstring &sVal) {
		return this->set_parameter(iParam, sVal.c_str());
	} // set_parameter
	bool SQLite_Statement::set_parameter(int iParam, const InfoValue &val) {
		if (val.is_empty()) {
			return (this->set_parameter_null(iParam));
		}
		const boost::any &v = val.value();
		if ((v.type() == typeid(double)) || (v.type() == typeid(float)) || (v.type() == typeid(long double))) {
			double dval = val.double_value();
			return (this->set_parameter(iParam, dval));
		}
		else if ((v.type() == typeid(short)) || (v.type() == typeid(unsigned short)) || (v.type() == typeid(int)) ||
			(v.type() == typeid(unsigned int)) || (v.type() == typeid(long)) || (v.type() == typeid(unsigned long)) ||
			(v.type() == typeid(long long))) {
			unsigned long lval = val.unsigned_long_value();
			return (this->set_parameter(iParam, lval));
		}
		else if (v.type() == typeid(bool)) {
			bool b = val.bool_value();
			std::string s = (b) ? "T" : "F";
			return (this->set_parameter(iParam, s));
		}
		else {
			std::string s;
			if (val.string_value(s)) {
				return (this->set_parameter(iParam, s));
			}
		}
		return (false);
	}// set_parameter
	  ///////////////////////////////
	bool SQLite_Statement::set_parameter(const std::string &sname, const InfoValue &val) {
		::sqlite3_stmt *p = this->m_pstmt;
		int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
		if (iParam < 1) {
			return (false);
		}
		return this->set_parameter(iParam, val);
	} // set_parameter
	bool SQLite_Statement::set_parameter(const std::wstring &sname, const InfoValue &val) {
		std::string ss = StringConvert::ws2s(sname);
		return this->set_parameter(ss, val);
	} // set_parameter
	bool SQLite_Statement::set_parameter_null(const std::string &sname) {
		::sqlite3_stmt *p = this->m_pstmt;
		int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
		if (iParam < 1) {
			return (false);
		}
		return this->set_parameter_null(iParam);
	} // set_paraemter_null
	bool SQLite_Statement::set_parameter_null(const std::wstring &sname) {
		std::string ss = StringConvert::ws2s(sname);
		return this->set_parameter_null(ss);
	} // set_parameter_null
	bool SQLite_Statement::set_parameter(const std::string &sname, int ivalue) {
		::sqlite3_stmt *p = this->m_pstmt;
		int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
		if (iParam < 1) {
			return (false);
		}
		return this->set_parameter(iParam, ivalue);
	} // set_parameter
	bool SQLite_Statement::set_parameter(const std::wstring &sname, int ivalue) {
		std::string ss = StringConvert::ws2s(sname);
		return this->set_parameter(ss, ivalue);
	}
	bool SQLite_Statement::set_parameter(const std::string &sname, double dval) {
		::sqlite3_stmt *p = this->m_pstmt;
		int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
		if (iParam < 1) {
			return (false);
		}
		return this->set_parameter(iParam, dval);
	}
	bool SQLite_Statement::set_parameter(const std::wstring &sname, double dval) {
		std::string ss = StringConvert::ws2s(sname);
		return this->set_parameter(ss, dval);
	}
	bool SQLite_Statement::set_parameter(const std::string &sname,
		const std::string &sval) {
		::sqlite3_stmt *p = this->m_pstmt;
		int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
		if (iParam < 1) {
			return (false);
		}
		return this->set_parameter(iParam, sval);
	}
	bool SQLite_Statement::set_parameter(const std::wstring &sname,
		const std::wstring &sval) {
		::sqlite3_stmt *p = this->m_pstmt;
		std::string ss = StringConvert::ws2s(sname);
		int iParam = ::sqlite3_bind_parameter_index(p, ss.c_str());
		if (iParam < 1) {
			return (false);
		}
		std::string sv = StringConvert::ws2s(sval);
		return this->set_parameter(iParam, sv);
	}
	///////////////////////////////////
	SQLite_Statement::~SQLite_Statement() {
		this->close();
	}

} /* namespace sqlite */

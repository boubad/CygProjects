/*
* infovalue.cpp
*/
///////////////////////////
#include <sstream>
//////////////////////////////////
#include "infovalue.h"
#include "stringconvert.h"
////////////////////////////////
namespace info
{
	//////////////////////////////////////////////////
	void InfoValue::get_value(boost::any &v) const {
		v = this->value();
	}
	void InfoValue::get_value(bool &v) const {
		v = this->bool_value();
	}
	void InfoValue::get_value(char &v) const {
		v = this->char_value();
	}
	void InfoValue::get_value(unsigned char &v) const {
		v = this->unsigned_char_value();
	}
	void InfoValue::get_value(wchar_t &v) const {
		v = this->wchar_value();
	}
	void InfoValue::get_value(short &v) const {
		v = this->short_value();
	}
	void InfoValue::get_value(unsigned short &v) const {
		v = unsigned_short_value();
	}
	void InfoValue::get_value(int &v) const {
		v = this->int_value();
	}
	void InfoValue::get_value(unsigned int &v) const {
		v = this->unsigned_int_value();
	}
	void InfoValue::get_value(long &v) const {
		v = this->long_value();
	}
	void InfoValue::get_value(unsigned long &v) const {
		v = this->unsigned_long_value();
	}
	void InfoValue::get_value(long long &v) const {
		v = this->long_long_value();
	}
	void InfoValue::get_value(float &v) const {
		v = this->float_value();
	}
	void InfoValue::get_value(double &v) const {
		v = this->double_value();
	}
	void InfoValue::get_value(long double &v) const {
		v = this->long_double_value();
	}
	void InfoValue::get_value(std::string &v) const {
		this->string_value(v);
	}
	void InfoValue::get_value(std::wstring &v) const {
		this->string_value(v);
	}
	//////////////////////////////////////////////////
	bool InfoValue::is_empty(void) const {
		return (this->empty());
	}
	bool InfoValue::is_integral(void) const {
		if (this->m_val.empty()) {
			return (false);
		}
		if (this->m_val.type() == typeid(int)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(short)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(long)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(long long)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(unsigned int)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(unsigned short)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(unsigned long)) {
			return (true);
		}
		return (false);
	}
	bool InfoValue::is_floating(void) const {
		if (this->m_val.empty()) {
			return (false);
		}
		if (this->m_val.type() == typeid(float)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(double)) {
			return (true);
		}
		else if (this->m_val.type() == typeid(long double)) {
			return (true);
		}
		return	(false);
	}
	bool InfoValue::is_boolean(void) const {
		if (this->m_val.empty()) {
			return (false);
		}
		return (this->m_val.type() == typeid(bool));
	}
	bool InfoValue::is_text(void) const {
		if (this->m_val.empty()) {
			return (false);
		}
		return ((this->m_val.type() == typeid(std::string)) || (this->m_val.type() == typeid(std::wstring)));
	}
	bool InfoValue::is_numerical(void) const {
		return (this->is_floating() || this->is_integral());
	}
	///////////////////////////////////////////////
	InfoValue::InfoValue() {}
	InfoValue::InfoValue(const boost::any &v) {
		if (!v.empty()) {
			if (v.type() == typeid(char)) {
				this->m_val = boost::any_cast<char>(v);
			}
			else if (v.type() == typeid(unsigned char)) {
				this->m_val = boost::any_cast<unsigned char>(v);
			}
			else if (v.type() == typeid(wchar_t)) {
				this->m_val = boost::any_cast<wchar_t>(v);
			}
			else if (v.type() == typeid(short)) {
				this->m_val = boost::any_cast<short>(v);
			}
			else if (v.type() == typeid(unsigned short)) {
				this->m_val = boost::any_cast<unsigned short>(v);
			}
			else if (v.type() == typeid(int)) {
				this->m_val = boost::any_cast<int>(v);
			}
			else if (v.type() == typeid(unsigned int)) {
				this->m_val = boost::any_cast<unsigned int>(v);
			}
			else if (v.type() == typeid(long)) {
				this->m_val = boost::any_cast<long>(v);
			}
			else if (v.type() == typeid(unsigned long)) {
				this->m_val = boost::any_cast<unsigned long>(v);
			}
			else if (v.type() == typeid(long long)) {
				this->m_val = boost::any_cast<long long>(v);
			}
			else if (v.type() == typeid(float)) {
				this->m_val = boost::any_cast<float>(v);
			}
			else if (v.type() == typeid(double)) {
				this->m_val = boost::any_cast<double>(v);
			}
			else if (v.type() == typeid(long double)) {
				this->m_val = boost::any_cast<long double>(v);
			}
			else if (v.type() == typeid(std::string)) {
				this->m_val = boost::any_cast<std::string>(v);
			}
			else if (v.type() == typeid(std::wstring)) {
				this->m_val = boost::any_cast<unsigned char>(v);
			}
		}// not empty
	}
	InfoValue::InfoValue(long long ival) : m_val(ival) {

	}
	InfoValue::InfoValue(long double dval) : m_val(dval) {

	}
	InfoValue::InfoValue(bool bval) :
		m_val(bval)
	{
	}
	InfoValue::InfoValue(char bval) :
		m_val(bval)
	{
	}
	InfoValue::InfoValue(unsigned char bval) :
		m_val(bval)
	{
	}
	InfoValue::InfoValue(wchar_t bval) :
		m_val(bval)
	{
	}
	InfoValue::InfoValue(short ival) :
		m_val(ival)
	{
	}
	InfoValue::InfoValue(unsigned short ival) :
		m_val(ival)
	{
	}
	InfoValue::InfoValue(int ival) :
		m_val(ival)
	{
	}
	InfoValue::InfoValue(unsigned int ival) :
		m_val(ival)
	{
	}
	InfoValue::InfoValue(long ival) :
		m_val(ival)
	{
	}
	InfoValue::InfoValue(unsigned long ival) :
		m_val(ival)
	{
	}
	InfoValue::InfoValue(float fval) :
		m_val(fval)
	{
	}
	InfoValue::InfoValue(double dval) : m_val(dval)
	{
	}
	InfoValue::InfoValue(const std::string &sval) :
		m_val(sval)
	{
	}
	InfoValue::InfoValue(const std::wstring &wsval) :
		m_val(wsval)
	{
	}
	InfoValue::InfoValue(const InfoValue &other) : m_val(other.m_val)
	{
	}
	InfoValue & InfoValue::operator=(const InfoValue &other)
	{
		if (this != &other)
		{
			this->m_val = other.m_val;
		}
		return (*this);
	}
	InfoValue::~InfoValue()
	{
	}
	InfoValue & InfoValue::operator=(const boost::any &v) {
		if (v.empty()) {
			this->m_val.clear();
		}
		else if (v.type() == typeid(char)) {
			this->m_val = boost::any_cast<char>(v);
		}
		else if (v.type() == typeid(unsigned char)) {
			this->m_val = boost::any_cast<unsigned char>(v);
		}
		else if (v.type() == typeid(wchar_t)) {
			this->m_val = boost::any_cast<wchar_t>(v);
		}
		else if (v.type() == typeid(short)) {
			this->m_val = boost::any_cast<short>(v);
		}
		else if (v.type() == typeid(unsigned short)) {
			this->m_val = boost::any_cast<unsigned short>(v);
		}
		else if (v.type() == typeid(int)) {
			this->m_val = boost::any_cast<int>(v);
		}
		else if (v.type() == typeid(unsigned int)) {
			this->m_val = boost::any_cast<unsigned int>(v);
		}
		else if (v.type() == typeid(long)) {
			this->m_val = boost::any_cast<long>(v);
		}
		else if (v.type() == typeid(unsigned long)) {
			this->m_val = boost::any_cast<unsigned long>(v);
		}
		else if (v.type() == typeid(long long)) {
			this->m_val = boost::any_cast<long long>(v);
		}
		else if (v.type() == typeid(float)) {
			this->m_val = boost::any_cast<float>(v);
		}
		else if (v.type() == typeid(double)) {
			this->m_val = boost::any_cast<double>(v);
		}
		else if (v.type() == typeid(long double)) {
			this->m_val = boost::any_cast<long double>(v);
		}
		else if (v.type() == typeid(std::string)) {
			this->m_val = boost::any_cast<std::string>(v);
		}
		else if (v.type() == typeid(std::wstring)) {
			this->m_val = boost::any_cast<unsigned char>(v);
		}
		return (*this);
	}
	InfoValue & InfoValue::operator=(bool b)
	{
		this->m_val = boost::any(b);
		return *this;
	}
	InfoValue & InfoValue::operator=(char ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(unsigned char ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(wchar_t ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(short ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(unsigned short ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(int ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(unsigned int ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(long ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(long long ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(unsigned long ival)
	{
		this->m_val = boost::any(ival);
		return *this;
	}
	InfoValue & InfoValue::operator=(float fval)
	{
		this->m_val = boost::any(fval);
		return *this;
	}
	InfoValue & InfoValue::operator=(double dval)
	{
		this->m_val = boost::any(dval);
		return *this;
	}
	InfoValue & InfoValue::operator=(long double dval)
	{
		this->m_val = boost::any(dval);
		return *this;
	}
	InfoValue & InfoValue::operator=(const std::string &s)
	{
		std::string ss(s);
		this->m_val = boost::any(ss);
		return *this;
	}
	InfoValue & InfoValue::operator=(const std::wstring &s)
	{
		std::wstring ss(s);
		this->m_val = boost::any(ss);
		return *this;
	}
	const std::type_info & InfoValue::type(void) const
	{
		return (this->m_val).type();
	}
	const boost::any & InfoValue::value(void) const
	{
		return (this->m_val);
	}
	bool InfoValue::is_valid(void) const
	{
		return (!this->m_val.empty());
	}
	bool InfoValue::empty(void) const
	{
		return (this->m_val.empty());
	}
	void InfoValue::value(const boost::any &v) {
		if (v.empty()) {
			this->m_val.clear();
			return;
		}
		if (v.type() == typeid(char)) {
			this->m_val = boost::any_cast<char>(v);
		}
		else if (v.type() == typeid(unsigned char)) {
			this->m_val = boost::any_cast<unsigned char>(v);
		}
		else if (v.type() == typeid(wchar_t)) {
			this->m_val = boost::any_cast<wchar_t>(v);
		}
		else if (v.type() == typeid(short)) {
			this->m_val = boost::any_cast<short>(v);
		}
		else if (v.type() == typeid(unsigned short)) {
			this->m_val = boost::any_cast<unsigned short>(v);
		}
		else if (v.type() == typeid(int)) {
			this->m_val = boost::any_cast<int>(v);
		}
		else if (v.type() == typeid(unsigned int)) {
			this->m_val = boost::any_cast<unsigned int>(v);
		}
		else if (v.type() == typeid(long)) {
			this->m_val = boost::any_cast<long>(v);
		}
		else if (v.type() == typeid(unsigned long)) {
			this->m_val = boost::any_cast<unsigned long>(v);
		}
		else if (v.type() == typeid(long long)) {
			this->m_val = boost::any_cast<long long>(v);
		}
		else if (v.type() == typeid(float)) {
			this->m_val = boost::any_cast<float>(v);
		}
		else if (v.type() == typeid(double)) {
			this->m_val = boost::any_cast<double>(v);
		}
		else if (v.type() == typeid(long double)) {
			this->m_val = boost::any_cast<long double>(v);
		}
		else if (v.type() == typeid(std::string)) {
			this->m_val = boost::any_cast<std::string>(v);
		}
		else if (v.type() == typeid(std::wstring)) {
			this->m_val = boost::any_cast<unsigned char>(v);
		}
	}
	//////////////////////////////////////////
	bool InfoValue::bool_value(void) const
	{
		bool oRet = false;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(bool)) {
				return boost::any_cast<bool>(this->m_val);
			}
		} // not empty
		return (oRet);
	} // bool_value
	char InfoValue::char_value(void) const
	{
		char oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(char)) {
				return boost::any_cast<char>(this->m_val);
			}
		} // not empty
		return (oRet);
	} // char_value
	unsigned char InfoValue::unsigned_char_value(void) const
	{
		unsigned char oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(unsigned char)) {
				return boost::any_cast<unsigned char>(this->m_val);
			}
		} // not empty
		return (oRet);
	} // unsigned_char_value
	wchar_t InfoValue::wchar_value(void) const
	{
		wchar_t oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(wchar_t)) {
				return boost::any_cast<wchar_t>(this->m_val);
			}
		} // not empty
		return (oRet);
	} // wchar_value
	short InfoValue::short_value(void) const
	{
		short  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(short)) {
				return boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (short)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (short)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (short)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (short)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (short)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (short)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (short)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (short)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (short)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				short r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				short r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (short)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // short_value
	unsigned short InfoValue::unsigned_short_value(void) const
	{
		unsigned short  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(short)) {
				return (unsigned short)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (unsigned short)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (unsigned short)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (unsigned short)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (unsigned short)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (unsigned short)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (unsigned short)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (unsigned short)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (unsigned short)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				unsigned short r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				unsigned short r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (unsigned short)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // unsigned_short_value
	int InfoValue::int_value(void) const
	{
		int  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(int)) {
				return boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (int)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (int)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (int)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (int)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (int)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (int)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (int)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (int)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (int)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				int r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				int r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (int)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // int_value
	unsigned int InfoValue::unsigned_int_value(void) const
	{
		unsigned int  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(int)) {
				return (unsigned int)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (unsigned int)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (unsigned int)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (unsigned int)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (unsigned int)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (unsigned int)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (unsigned int)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (unsigned int)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (unsigned int)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				unsigned int r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				unsigned int r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (unsigned int)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // unsigned_int_value
	long InfoValue::long_value(void) const
	{
		long  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(long)) {
				return boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (long)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (long)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (long)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (long)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (long)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (long)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (long)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (long)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (long)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				long r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				long r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (long)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // long_value
	long long InfoValue::long_long_value(void) const
	{
		long long  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(long)) {
				return (long long)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (long long)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (long long)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (long long)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (long long)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (long long)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (long long)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (long long)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (long long)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				long long r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				long long r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (long long)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // long_long_value
	unsigned long InfoValue::unsigned_long_value(void) const
	{
		unsigned long  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(long)) {
				return (unsigned long)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (unsigned long)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (unsigned long)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (unsigned long)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (unsigned long)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (unsigned long)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (unsigned long)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(double)) {
				return (unsigned long)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (unsigned long)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				unsigned long r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				unsigned long r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (unsigned long)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // unsigned_long_value
	float InfoValue::float_value(void) const
	{
		float  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(double)) {
				return (float)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (float)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (float)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (float)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (float)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (float)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (float)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (float)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (float)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (float)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				float r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				float r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (float)((boost::any_cast<bool> (this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // float_value
	double InfoValue::double_value(void) const
	{
		double  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(double)) {
				return boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (double)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (double)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (double)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (double)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (double)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (double)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (double)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (double)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return (double)boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				double r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				double r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (double)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // double_value
	long double InfoValue::long_double_value(void) const
	{
		long double  oRet = 0;
		if (!this->m_val.empty())
		{
			if (this->m_val.type() == typeid(double)) {
				return (long double)boost::any_cast<double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				return (long double)boost::any_cast<unsigned short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(short)) {
				return (long double)boost::any_cast<short>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				return (long double)boost::any_cast<unsigned int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(int)) {
				return (long double)boost::any_cast<int>(this->m_val);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				return (long double)boost::any_cast<unsigned long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long long)) {
				return (long double)boost::any_cast<long long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(float)) {
				return (long double)boost::any_cast<float>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long)) {
				return (long double)boost::any_cast<long>(this->m_val);
			}
			else if (this->m_val.type() == typeid(long double)) {
				return boost::any_cast<long double>(this->m_val);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(this->m_val);
				std::stringstream in(s);
				long double r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(std::string)) {
				std::wstring s = boost::any_cast<std::wstring>(this->m_val);
				std::wstringstream in(s);
				long double r = 0;
				in >> r;
				return (r);
			}
			else if (this->m_val.type() == typeid(bool)) {
				return (long double)((boost::any_cast<bool>(this->m_val)) ? 1 : 0);
			}
		} // not empty
		return (oRet);
	} // long_double_value
	bool InfoValue::string_value(std::string &s) const
	{
		s.clear();
		if (!this->m_val.empty()) {
			boost::any v = this->m_val;
			if (this->m_val.type() == typeid(boost::any)) {
				v = boost::any_cast<boost::any>(this->m_val);
				if (v.empty()) {
					return (false);
				}
			}
			if (v.type() == typeid(std::string)) {
				s = boost::any_cast<std::string>(v);
				return (true);
			}
			else if (v.type() == typeid(std::wstring)) {
				std::wstring ss = boost::any_cast<std::wstring>(v);
				s = StringConvert::ws2s(ss);
				return (true);
			}
			else if (v.type() == typeid(int)) {
				int x = boost::any_cast<int>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(short)) {
				short x = boost::any_cast<short>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(long)) {
				long x = boost::any_cast<long>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(float)) {
				float x = boost::any_cast<float>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(double)) {
				double x = boost::any_cast<double>(this->m_val);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(long long)) {
				long long x = boost::any_cast<long long>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(long double)) {
				long double x = boost::any_cast<long double>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				unsigned short x = boost::any_cast<unsigned short>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(unsigned int)) {
				unsigned int x = boost::any_cast<unsigned int>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(unsigned long)) {
				unsigned long x = boost::any_cast<unsigned long>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (v.type() == typeid(bool)) {
				bool x = boost::any_cast<bool>(v);
				if (x) {
					s = "T";
				}
				else {
					s = "F";
				}
				return (true);
			}
		}
		return (false);
	} // string_value
	bool InfoValue::string_value(std::wstring &s) const
	{
		s.clear();
		if (!this->m_val.empty()) {
			if (this->m_val.type() == typeid(std::string)) {
				std::string ss = boost::any_cast<std::string>(this->m_val);
				s = StringConvert::s2ws(ss);
				return (true);
			}
			else if (this->m_val.type() == typeid(std::wstring)) {
				s = boost::any_cast<std::wstring>(this->m_val);
				return (true);
			}
			else if (this->m_val.type() == typeid(int)) {
				int x = boost::any_cast<int>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(short)) {
				short x = boost::any_cast<short>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(long)) {
				long x = boost::any_cast<long>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(float)) {
				float x = boost::any_cast<float>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(double)) {
				double x = boost::any_cast<double>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(long long)) {
				long long x = boost::any_cast<long long>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(long double)) {
				long double x = boost::any_cast<long double>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(unsigned short)) {
				unsigned short x = boost::any_cast<unsigned short>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(unsigned int)) {
				unsigned int x = boost::any_cast<unsigned int>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(unsigned long)) {
				unsigned long x = boost::any_cast<unsigned long>(this->m_val);
				std::wstringstream os;
				os << x;
				s = os.str();
				return (true);
			}
			else if (this->m_val.type() == typeid(bool)) {
				bool x = boost::any_cast<bool>(this->m_val);
				if (x) {
					s = L"true";
				}
				else {
					s = L"false";
				}
				return (true);
			}
		}
		return (false);
	} // string_value

	InfoValueType InfoValue::get_infovalue_type(void) const {
		InfoValueType bRet = InfoValueType::emptyType;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bRet = InfoValueType::boolType;
			}
			else if (t == typeid(char))
			{
				bRet = InfoValueType::charType;
			}
			else if (t == typeid(unsigned char))
			{
				bRet = InfoValueType::unsignedCharType;
			}
			else if (t == typeid(wchar_t))
			{
				bRet = InfoValueType::wcharType;
			}
			else if (t == typeid(short))
			{
				bRet = InfoValueType::shortType;
			}
			else if (t == typeid(unsigned short))
			{
				bRet = InfoValueType::unsignedShortType;
			}
			else if (t == typeid(int))
			{
				bRet = InfoValueType::intType;
			}
			else if (t == typeid(unsigned int))
			{
				bRet = InfoValueType::unsignedIntType;
			}
			else if (t == typeid(long))
			{
				bRet = InfoValueType::longType;
			}
			else if (t == typeid(long long))
			{
				bRet = InfoValueType::longLongType;
			}
			else if (t == typeid(unsigned long))
			{
				bRet = InfoValueType::unsignedLongType;
			}
			else if (t == typeid(float))
			{
				bRet = InfoValueType::floatType;
			}
			else if (t == typeid(double))
			{
				bRet = InfoValueType::doubleType;
			}
			else if (t == typeid(long double))
			{
				bRet = InfoValueType::longDoubleType;
			}
			else if (t == typeid(std::string))
			{
				bRet = InfoValueType::stringType;
			}
			else if (t == typeid(std::wstring))
			{
				bRet = InfoValueType::wstringType;
			}
		} // not empty
		return (bRet);
	}//get_dbvalue_type
	 /////////////////////////////////////////////
} /* namespace info */

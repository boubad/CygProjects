/*
* infovalue.cpp
*/
///////////////////////////
#include <sstream>
//////////////////////////////////
#include "../include/infovalue.h"
#include "../include/stringconvert.h"
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
		const InfoValueType t = this->get_infovalue_type();
		switch (t) {
		case InfoValueType::charType:
		case InfoValueType::unsignedCharType:
		case InfoValueType::wcharType:
		case InfoValueType::shortType:
		case InfoValueType::unsignedShortType:
		case InfoValueType::intType:
		case InfoValueType::unsignedIntType:
		case InfoValueType::longType:
		case InfoValueType::unsignedLongType:
		case InfoValueType::longLongType:
			return (true);
		default:
			return (false);
		}
	}
	bool InfoValue::is_floating(void) const {
		const InfoValueType t = this->get_infovalue_type();
		return ((t == InfoValueType::floatType) || (t == InfoValueType::doubleType) || (t == InfoValueType::longDoubleType));
	}
	bool InfoValue::is_boolean(void) const {
		return (this->get_infovalue_type() == InfoValueType::boolType);
	}
	bool InfoValue::is_text(void) const {
		const InfoValueType t = this->get_infovalue_type();
		return ((t == InfoValueType::stringType) || (t == InfoValueType::wstringType));
	}
	bool InfoValue::is_numerical(void) const {
		return (this->is_floating() || this->is_integral());
	}
	///////////////////////////////////////////////
	InfoValue::InfoValue() {}
	InfoValue::InfoValue(const boost::any &v) :m_val(v) {
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
	//////////////////////////////////////////
	bool InfoValue::bool_value(void) const
	{
		bool oRet = false;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				oRet = boost::any_cast<bool>(v);
			}
			else if (t == typeid(char))
			{
				char c = boost::any_cast<char>(v);
				oRet = (c == 't') || (c == '1') || (c == 'v') || (c == 'o') ||
					(c == 'T') || (c == 'V') || (c == 'O') || (c == 'Y');
			}
			else if (t == typeid(unsigned char))
			{
				char c = (char)boost::any_cast<unsigned char>(v);
				oRet = (c == 't') || (c == '1') || (c == 'v') || (c == 'o') ||
					(c == 'T') || (c == 'V') || (c == 'O') || (c == 'Y');
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t c = boost::any_cast<wchar_t>(v);
				oRet = (c == L't') || (c == L'1') || (c == L'v') || (c == L'o') ||
					(c == L'T') || (c == L'V') || (c == L'O') || (c == L'Y');
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (x != 0.0f) ? true : false;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (x != 0.0) ? true : false;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (x != 0.0) ? true : false;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = info_trim_lower(x);
				if (!xx.empty())
				{
					auto c = *(xx.begin());
					oRet = (c == 't') || (c == '1') || (c == 'v') || (c == 'o') ||
						(c == 'T') || (c == 'V') || (c == 'O') || (c == 'Y');
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = info_trim_lower(x);
				if (!xx.empty())
				{
					auto c = *(xx.begin());
					oRet = (c == L't') || (c == L'1') || (c == L'v') || (c == L'o') ||
						(c == L'T') || (c == L'V') || (c == L'O') || (c == L'Y');
				} // not empty
			}
		} // not empty
		return (oRet);
	} // bool_value
	char InfoValue::char_value(void) const
	{
		char oRet = (char)0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 'T' : 'F';
			}
			else if (t == typeid(char))
			{
				oRet = boost::any_cast<char>(v);
			}
			else if (t == typeid(unsigned char))
			{
				oRet = (char)boost::any_cast<unsigned char>(v);
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t c = boost::any_cast<wchar_t>(v);
				std::wstring ss;
				ss += c;
				if (!ss.empty())
				{
					std::string s = StringConvert::ws2s(ss);
					if (!s.empty())
					{
						oRet = *(s.begin());
					}
				}
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (char)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (char)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (char)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (char)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (char)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (char)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (char)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (char)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (char)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (char)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				if (!x.empty())
				{
					oRet = *(x.begin());
				}
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				if (!x.empty())
				{
					std::string s = StringConvert::ws2s(x);
					if (!s.empty())
					{
						oRet = *(s.begin());
					}
				}
			}
		} // not empty
		return (oRet);
	} // char_value
	unsigned char InfoValue::unsigned_char_value(void) const
	{
		unsigned char oRet = (unsigned char)0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				char x = (b) ? 'T' : 'F';
				oRet = (unsigned char)x;
			}
			else if (t == typeid(char))
			{
				char c = boost::any_cast<char>(v);
				oRet = (unsigned char)c;
			}
			else if (t == typeid(unsigned char))
			{
				oRet = boost::any_cast<unsigned char>(v);
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t c = boost::any_cast<wchar_t>(v);
				std::wstring ss;
				ss += c;
				if (!ss.empty())
				{
					std::string s = StringConvert::ws2s(ss);
					if (!s.empty())
					{
						oRet = (unsigned char)(*(s.begin()));
					}
				}
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				std::stringstream os;
				os << x;
				std::string s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				if (!x.empty())
				{
					oRet = (unsigned char)(*(x.begin()));
				}
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				if (!x.empty())
				{
					std::string s = StringConvert::ws2s(x);
					if (!s.empty())
					{
						oRet = (unsigned char)(*(s.begin()));
					}
				}
			}
		} // not empty
		return (oRet);
	} // unsigned_char_value
	wchar_t InfoValue::wchar_value(void) const
	{
		wchar_t oRet = (wchar_t)0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? L'T' : L'F';
			}
			else if (t == typeid(char))
			{
				char c = boost::any_cast<char>(v);
				std::string s;
				s += c;
				if (!s.empty())
				{
					std::wstring ss = StringConvert::s2ws(s);
					if (!ss.empty())
					{
						oRet = *(ss.begin());
					}
				}
			}
			else if (t == typeid(unsigned char))
			{
				char c = boost::any_cast<unsigned char>(v);
				std::string s;
				s += c;
				if (!s.empty())
				{
					std::wstring ss = StringConvert::s2ws(s);
					if (!ss.empty())
					{
						oRet = *(ss.begin());
					}
				}
			}
			else if (t == typeid(wchar_t))
			{
				oRet = boost::any_cast<wchar_t>(v);
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				std::wstringstream os;
				os << x;
				std::wstring s = os.str();
				if (!s.empty())
				{
					oRet = *(s.begin());
				}
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				if (!x.empty())
				{
					std::wstring ss = StringConvert::s2ws(x);
					if (!ss.empty())
					{
						oRet = *(ss.begin());
					}
				}
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				if (!x.empty())
				{
					oRet = *(x.begin());
				}
			}
		} // not empty
		return (oRet);
	} // wchar_value
	short InfoValue::short_value(void) const
	{
		short oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (short)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				oRet = (short)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (short)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (short)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (short)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (short)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (short)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (short)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (short)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (short)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (short)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (short)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // short_value
	unsigned short InfoValue::unsigned_short_value(void) const
	{
		unsigned short oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (unsigned short)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // unsigned_short_value
	int InfoValue::int_value(void) const
	{
		int oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (int)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<char>(v);
				oRet = (int)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (int)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (int)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (int)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (int)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (int)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (int)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (int)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (int)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (int)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (int)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // int_value
	unsigned int InfoValue::unsigned_int_value(void) const
	{
		unsigned int oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<char>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(unsigned int))
			{
				oRet = boost::any_cast<unsigned int>(v);
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (unsigned int)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // unsigned_int_value
	long InfoValue::long_value(void) const
	{
		long oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (long)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<char>(v);
				oRet = (long)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (long)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (long)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (long)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (long)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (long)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (long)x;
			}
			else if (t == typeid(long))
			{
				oRet = boost::any_cast<long>(v);
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (long)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (long)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (long)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (long)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // long_value
	long long InfoValue::long_long_value(void) const
	{
		long long oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<char>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (long long)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // long_long_value
	unsigned long InfoValue::unsigned_long_value(void) const
	{
		unsigned long oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<char>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(unsigned long))
			{
				oRet = boost::any_cast<unsigned long>(v);
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (unsigned long)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // unsigned_long_value
	float InfoValue::float_value(void) const
	{
		float oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0f : 0.0f;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (float)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				oRet = (float)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (float)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (float)x;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (float)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (float)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (float)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (float)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (float)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (float)x;
			}
			else if (t == typeid(float))
			{
				oRet = boost::any_cast<float>(v);
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (float)x;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (float)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // float_value
	double InfoValue::double_value(void) const
	{
		double oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0 : 0.0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (double)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				oRet = (double)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (double)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (double)x;;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (double)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (double)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (double)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (double)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (double)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (double)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (double)x;
			}
			else if (t == typeid(double))
			{
				oRet = boost::any_cast<double>(v);
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				oRet = (double)x;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // double_value
	long double InfoValue::long_double_value(void) const
	{
		long double oRet = 0;
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0 : 0.0;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				oRet = (double)x;;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				oRet = (long double)x;
			}
			else if (t == typeid(long double))
			{
				oRet = boost::any_cast<long double>(v);
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty())
				{
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}

		} // not empty
		return (oRet);
	} // long_double_value
	bool InfoValue::string_value(std::string &s) const
	{
		bool bRet = false;
		s.clear();
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				s = (b) ? "true" : "false";
				bRet = true;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				std::wstringstream os;
				os << x;
				std::wstring ss = os.str();
				s = StringConvert::ws2s(ss);
				bRet = true;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				s = x;
				bRet = true;
			}
			else if (t == typeid(std::wstring))
			{
				std::wstring x = boost::any_cast<std::wstring>(v);
				s = StringConvert::ws2s(x);
				bRet = true;
			}
		} // not empty
		return (bRet);
	} // string_value
	bool InfoValue::string_value(std::wstring &s) const
	{
		bool bRet = false;
		s.clear();
		const boost::any &v = this->m_val;
		if (!v.empty())
		{
			const std::type_info & t = v.type();
			if (t == typeid(bool))
			{
				bool b = boost::any_cast<bool>(v);
				s = (b) ? L"true" : L"false";
				bRet = true;
			}
			else if (t == typeid(char))
			{
				char x = boost::any_cast<char>(v);
				std::stringstream os;
				os << x;
				std::string ss = os.str();
				s = StringConvert::s2ws(ss);
				bRet = true;
			}
			else if (t == typeid(unsigned char))
			{
				unsigned char x = boost::any_cast<unsigned char>(v);
				std::stringstream os;
				os << x;
				std::string ss = os.str();
				s = StringConvert::s2ws(ss);
				bRet = true;
			}
			else if (t == typeid(wchar_t))
			{
				wchar_t x = boost::any_cast<wchar_t>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(short))
			{
				short x = boost::any_cast<short>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned short))
			{
				unsigned short x = boost::any_cast<unsigned short>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(int))
			{
				int x = boost::any_cast<int>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned int))
			{
				unsigned int x = boost::any_cast<unsigned int>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(long))
			{
				long x = boost::any_cast<long>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(long long))
			{
				long long x = boost::any_cast<long long>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(unsigned long))
			{
				unsigned long x = boost::any_cast<unsigned long>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(float))
			{
				float x = boost::any_cast<float>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(double))
			{
				double x = boost::any_cast<double>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(long double))
			{
				long double x = boost::any_cast<long double>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(std::string))
			{
				std::string x = boost::any_cast<std::string>(v);
				s = StringConvert::s2ws(x);
				bRet = true;
			}
			else if (t == typeid(std::wstring))
			{
				s = boost::any_cast<std::wstring>(v);
				bRet = true;
			}

		} // not empty
		return (bRet);
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

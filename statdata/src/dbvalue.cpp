/*
 * dbvalue.cpp
 */

#include "../include/dbvalue.h"
#include "../include/stringconvert.h"
////////////////////////////////
namespace info
{
	////////////////////////////////////////////
	extern bool info_global_compute_distances(const DbValueMap &m1, const DbValueMap &m2,double &dRes) {
		dRes = 0;
		size_t nc = 0;
		auto iend = m2.end();
		std::for_each(m1.begin(), m1.end(), [&](const std::pair<IntType, DbValue> &p) {
			const DbValue &v1 = p.second;
			if (!v1.empty()) {
				const IntType key = p.first;
				auto it = m2.find(key);
				if (it != iend) {
					const DbValue &v2 = (*it).second;
					if (!v2.empty()) {
						const double t = v1.double_value() - v2.double_value();
						dRes += t * t;
						++nc;
					}// not empty
				}// found
			}// v1
		});
		if (nc > 1) {
			dRes /= nc;
		}
		return (nc > 0);
	}//info_global_compute_distances
///////////////////////////////////////////////
DbValue::DbValue() {}
DbValue::DbValue(bool bval) :
    m_val(bval)
{
}
DbValue::DbValue(char bval) :
    m_val(bval)
{
}
DbValue::DbValue(unsigned char bval) :
    m_val(bval)
{
}
DbValue::DbValue(wchar_t bval) :
    m_val(bval)
{
}
DbValue::DbValue(short ival) :
    m_val(ival)
{
}
DbValue::DbValue(unsigned short ival) :
    m_val(ival)
{
}
DbValue::DbValue(int ival) :
    m_val(ival)
{
}
DbValue::DbValue(unsigned int ival) :
    m_val(ival)
{
}
DbValue::DbValue(long ival) :
    m_val(ival)
{
}
DbValue::DbValue(unsigned long ival) :
    m_val(ival)
{
}
DbValue::DbValue(float fval) :
    m_val(fval)
{
}
DbValue::DbValue(double dval) : m_val(dval)
{
}
DbValue::DbValue(const std::string &sval) :
    m_val(sval)
{
}
DbValue::DbValue(const std::wstring &wsval) :
    m_val(wsval)
{
}
DbValue::DbValue(const DbValue &other) :m_val(other.m_val)
{
}
DbValue & DbValue::operator=(const DbValue &other)
{
    if (this != &other)
    {
        this->m_val = other.m_val;
    }
    return (*this);
}
DbValue::~DbValue()
{
}
void DbValue::swap(DbValue &other)
{
    boost::any t = this->m_val;
    this->m_val = other.m_val;
    other.m_val = t;
}
DbValue & DbValue::operator=(bool b)
{
    this->m_val = boost::any(b);
    return *this;
}
DbValue & DbValue::operator=(char ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(unsigned char ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(wchar_t ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(short ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(unsigned short ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(int ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(unsigned int ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(long ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(unsigned long ival)
{
    this->m_val = boost::any(ival);
    return *this;
}
DbValue & DbValue::operator=(float fval)
{
    this->m_val = boost::any(fval);
    return *this;
}
DbValue & DbValue::operator=(double dval)
{
    this->m_val = boost::any(dval);
    return *this;
}
DbValue & DbValue::operator=(const std::string &s)
{
    std::string ss(s);
    this->m_val = boost::any(ss);
    return *this;
}
DbValue & DbValue::operator=(const std::wstring &s)
{
    std::wstring ss(s);
    this->m_val = boost::any(ss);
    return *this;
}
const std::type_info & DbValue::type(void) const
{
    return (this->m_val).type();
}
const boost::any & DbValue::value(void) const
{
    return (this->m_val);
}
bool DbValue::is_valid(void) const
{
    return (!this->m_val.empty());
}
bool DbValue::empty(void) const
{
    return (this->m_val.empty());
}
//////////////////////////////////////////
bool DbValue::operator<(const DbValue &other) const
{
    long l1 = this->long_value();
    long l2 = other.long_value();
    return (l1 < l2);
}
bool DbValue::operator==(const DbValue &other) const
{
    long l1 = this->long_value();
    long l2 = other.long_value();
    return (l1 == l2);
}
////////////////////////////////////////////
bool DbValue::bool_value(void) const
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
char DbValue::char_value(void) const
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
unsigned char DbValue::unsigned_char_value(void) const
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
wchar_t DbValue::wchar_value(void) const
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
short DbValue::short_value(void) const
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
unsigned short DbValue::unsigned_short_value(void) const
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
int DbValue::int_value(void) const
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
unsigned int DbValue::unsigned_int_value(void) const
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
long DbValue::long_value(void) const
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
unsigned long DbValue::unsigned_long_value(void) const
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
float DbValue::float_value(void) const
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
double DbValue::double_value(void) const
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
            oRet = (float)x;
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
bool DbValue::string_value(std::string &s) const
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
bool DbValue::string_value(std::wstring &s) const
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

DbValueType DbValue::get_dbvalue_type(void) const{
    DbValueType bRet = DbValueType::emptyType;
    const boost::any &v = this->m_val;
    if (!v.empty())
    {
        const std::type_info & t = v.type();
        if (t == typeid(bool))
        {
            bRet = DbValueType::boolType;
        }
        else if (t == typeid(char))
        {
            bRet = DbValueType::charType;
        }
        else if (t == typeid(unsigned char))
        {
            bRet = DbValueType::unsignedCharType;
        }
        else if (t == typeid(wchar_t))
        {
            bRet = DbValueType::wcharType;
        }
        else if (t == typeid(short))
        {
            bRet = DbValueType::shortType;
        }
        else if (t == typeid(unsigned short))
        {
            bRet = DbValueType::unsignedShortType;
        }
        else if (t == typeid(int))
        {
            bRet = DbValueType::intType;
        }
        else if (t == typeid(unsigned int))
        {
            bRet = DbValueType::unsignedIntType;
        }
        else if (t == typeid(long))
        {
            bRet = DbValueType::longType;
        }
        else if (t == typeid(unsigned long))
        {
            bRet = DbValueType::unsignedLongType;
        }
        else if (t == typeid(float))
        {
            bRet = DbValueType::floatType;
        }
        else if (t == typeid(double))
        {
            bRet = DbValueType::doubleType;
        }
        else if (t == typeid(std::string))
        {
            bRet = DbValueType::stringType;
        }
        else if (t == typeid(std::wstring))
        {
            bRet = DbValueType::wstringType;
        }
    } // not empty
    return (bRet);
}//get_dbvalue_type
/////////////////////////////////////////////
} /* namespace sqlite */

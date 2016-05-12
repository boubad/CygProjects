#ifndef DBVALUE_H
#define DBVALUE_H
///////////////////////
#include "info_constants.h"
////////////////////////////
namespace info
{
//////////////////////////////////
typedef unsigned char byte;

////////////////////////////////////
enum class DbValueType : short
{
    emptyType,boolType, charType,unsignedCharType,wcharType,shortType,unsignedShortType,intType,
    unsignedIntType, longType,unsignedLongType,floatType,doubleType,stringType,wstringType
};
//////////////////////////////////
class DbValue
{
private:
    boost::any m_val;
public:
    DbValue();
    explicit DbValue(bool bval);
    explicit DbValue(char bval);
    explicit DbValue(unsigned char bval);
    explicit DbValue(wchar_t bval);
    explicit DbValue(short ival);
    explicit DbValue(unsigned short ival);
    explicit DbValue(int ival);
    explicit DbValue(unsigned int ival);
    explicit DbValue(long ival);
    explicit DbValue(unsigned long ival);
    explicit DbValue(float fval);
    explicit DbValue(double dval);
    explicit DbValue(const std::string &sval);
    explicit DbValue(const std::wstring &wsval);
    DbValue(const DbValue &other);
    DbValue & operator=(const DbValue &other);
    void swap(DbValue &other);
    bool operator<(const DbValue &other) const;
    bool operator==(const DbValue &other) const;
    ~DbValue();
    DbValue & operator=(bool b);
    DbValue & operator=(char ival);
    DbValue & operator=(unsigned char ival);
    DbValue & operator=(wchar_t ival);
    DbValue & operator=(short ival);
    DbValue & operator=(unsigned short ival);
    DbValue & operator=(int ival);
    DbValue & operator=(unsigned int ival);
    DbValue & operator=(long ival);
    DbValue & operator=(unsigned long ival);
    DbValue & operator=(float fval);
    DbValue & operator=(double dval);
    DbValue & operator=(const std::string &s);
    DbValue & operator=(const std::wstring &s);
    DbValueType get_dbvalue_type(void) const;
    const std::type_info & type(void) const;
    const boost::any & value(void) const;
    bool is_valid(void) const;
    bool empty(void) const;
    bool bool_value(void) const;
    char char_value(void) const;
    wchar_t wchar_value(void) const;
    unsigned char unsigned_char_value(void) const;
    short short_value(void) const;
    unsigned short unsigned_short_value(void) const;
    int int_value(void) const;
    unsigned int unsigned_int_value(void) const;
    long long_value(void) const;
    unsigned long unsigned_long_value(void) const;
    float float_value(void) const;
    double double_value(void) const;
    bool string_value(std::string &s) const;
    bool string_value(std::wstring &s) const;
};
//////////////////////////////////////////
typedef std::map<IntType, DbValue> DbValueMap;
typedef std::vector<DbValue> dbvalues_vector;
//////////////////////////////////////
} /* namespace info */
/////////////////////////////////////////
inline void swap(info::DbValue &v1, info::DbValue &v2)
{
    v1.swap(v2);
}// swap
///////////////////////////////////
#endif // DBVALUE_H

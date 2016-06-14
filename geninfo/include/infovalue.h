/*
 * infovalue.h
 *
 *  Created on: 14 juin 2016
 *      Author: boubad
 */

#ifndef INFOVALUE_H_
#define INFOVALUE_H_
#include "info_includes.h"
////////////////////////////
namespace info
{
	////////////////////////////////////
	enum InfoValueType : short
	{
		emptyType, boolType, charType, unsignedCharType, wcharType, shortType, unsignedShortType, intType,
		unsignedIntType, longType, unsignedLongType, longLongType, floatType, doubleType, longDoubleType, stringType, wstringType,endType
	};
	//////////////////////////////////
	class InfoValue
	{
	private:
		boost::any m_val;
	public:
		InfoValue();
		explicit InfoValue(bool bval);
		explicit InfoValue(char bval);
		explicit InfoValue(unsigned char bval);
		explicit InfoValue(wchar_t bval);
		explicit InfoValue(short ival);
		explicit InfoValue(unsigned short ival);
		explicit InfoValue(int ival);
		explicit InfoValue(unsigned int ival);
		explicit InfoValue(long ival);
		explicit InfoValue(unsigned long ival);
		explicit InfoValue(long long ival);
		explicit InfoValue(float fval);
		explicit InfoValue(double dval);
		explicit InfoValue(long double dval);
		explicit InfoValue(const std::string &sval);
		explicit InfoValue(const std::wstring &wsval);
		explicit InfoValue(const boost::any &v);
		InfoValue(const InfoValue &other);
		InfoValue & operator=(const InfoValue &other);
		~InfoValue();
		InfoValue & operator=(bool b);
		InfoValue & operator=(char ival);
		InfoValue & operator=(unsigned char ival);
		InfoValue & operator=(wchar_t ival);
		InfoValue & operator=(short ival);
		InfoValue & operator=(unsigned short ival);
		InfoValue & operator=(int ival);
		InfoValue & operator=(unsigned int ival);
		InfoValue & operator=(long ival);
		InfoValue & operator=(unsigned long ival);
		InfoValue & operator=(long long ival);
		InfoValue & operator=(float fval);
		InfoValue & operator=(double dval);
		InfoValue & operator=(long double dval);
		InfoValue & operator=(const std::string &s);
		InfoValue & operator=(const std::wstring &s);
		InfoValue & operator=(const boost::any &v);
		InfoValueType get_infovalue_type(void) const;
		const std::type_info & type(void) const;
		const boost::any & value(void) const;
		void value(const boost::any &v);
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
		long long long_long_value(void) const;
		float float_value(void) const;
		double double_value(void) const;
		long double long_double_value(void) const;
		bool string_value(std::string &s) const;
		bool string_value(std::wstring &s) const;
		//
		bool is_empty(void) const;
		bool is_integral(void) const;
		bool is_floating(void) const;
		bool is_boolean(void) const;
		bool is_text(void) const;
		bool is_numerical(void) const;
		//
		void get_value(boost::any &v) const;
		void get_value(bool &v) const;
		void get_value(char &v) const;
		void get_value(unsigned char &v) const;
		void get_value(wchar_t &v) const;
		void get_value(short &v) const;
		void get_value(unsigned short &v) const;
		void get_value(int &v) const;
		void get_value(unsigned int &v) const;
		void get_value(long &v) const;
		void get_value(unsigned long &v) const;
		void get_value(long long &v) const;
		void get_value(float &v) const;
		void get_value(double &v) const;
		void get_value(long double &v) const;
		void get_value(std::string &v) const;
		void get_value(std::wstring &v) const;
	};
	//////////////////////////////////////////
} /* namespace info */
  /////////////////////////////////////////
#endif /* INFOVALUE_H_ */

#pragma once
#ifndef __STRINGCONVERT_H__
#define __STRINGCONVERT_H__
/////////////////////////////////
#include <string>
#include <locale>
#include <codecvt>
///////////////////////////////////
#include <boost/algorithm/string.hpp>
//////////////////////////////////////
namespace info {
/////////////////////////////////////
class StringConvert {
public:
	inline static std::wstring s2ws(const std::string& str) {
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return converterX.from_bytes(str);
	}

	inline static std::string ws2s(const std::wstring& wstr) {
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return converterX.to_bytes(wstr);
	}
};
//////////////////////////////////////
template<class T>
inline T info_trim(const T &s) {
	return (boost::trim_copy(s));
}
template<class T>
inline T info_trim_upper(const T &s) {
	return boost::to_upper_copy(boost::trim_copy(s));
}
template<class T>
inline T info_trim_lower(const T &s) {
	return boost::to_lower_copy(boost::trim_copy(s));
}
inline std::string info_2s(const std::string &s) {
	return (s);
}
inline std::string info_2s(const std::wstring &s) {
	return (StringConvert::ws2s(s));
}
inline std::wstring info_Zws(const std::string &s){
	return (StringConvert::s2ws(s));
}
inline std::wstring info_Zws(const std::wstring &s){
	return (s);
}
////////////////////////////////////////
}// namespace sqlite
////////////////////////////////////
#endif // !__STRINGCONVERT_H__


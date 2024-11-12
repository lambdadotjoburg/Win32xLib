/**
  * @file 		StrConverter.cpp
  * @brief 		Implemenation of StrConverter utility class
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details 	This file contains the definition/implemenation
  *             for the StrConverter class' functionality
  */

#ifndef STRCONVER_CPP
#define STRCONVER_CPP
#endif

#include "./StrConverter.h"

/// @brief      method for converting a narrow string to a wide string
/// @param[in]  s ~ narrow string to be converted to a wide string
/// @details    This method creates a wide string by copying data from a narrow string
/// @return		copy of a wide string converted from a narrow
std::wstring
StrConverter::StringToWString(const std::string& s) {
    // append L before string literal to be returned
    std::wstring temp(s.length(), L' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp;
}

/// @brief      method for converting a wide string to a narrow string
/// @param[in]  ws wide string to be converted to a narrow string
/// @details    This method creates a narrow string by copying data from a wide string
/// @return		copy of a narrow string converted from a wide
std::string
StrConverter::WStringToString(const std::wstring& ws) {
    std::string temp(ws.length(), ' ');
    std::copy(ws.begin(), ws.end(), temp.begin());
    return temp;
}

/**
 * DEMO USAGE:
 * 
 * string => wstring
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * std::string s = "This is a std::string object";
 * std::wstring s_to_ws = StrConverter::StringToWString(s);
 * std::wcout << s_to_ws << std::endl;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * wstring => string
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * std::wstring ws = L"This is a std::wstring object";
 * std::string ws_to_s = StrConverter::WStringToString(ws);
 * std::cout << ws_to_s << std::endl;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 */
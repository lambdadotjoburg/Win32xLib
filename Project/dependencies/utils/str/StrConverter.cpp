/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		StrConverter.cpp
  * @brief 		Implemenation of StrConverter utility class
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * 
  * @details 	This file contains the definition/implemenation
  *           for the StrConverter class' functionality
  */

/// @brief begin of STR_CPP implementation
#ifndef STR_CPP
#define STR_CPP

#include "./StrConverter.h"

/**
 * @example StringToWString
 * 
 * string => wstring
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * std::string s = "This is a std::string object";
 * std::wstring s_to_ws = StrConverter::StringToWString(s);
 * std::wcout << s_to_ws << std::endl;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @see StrConverter::StringToWString
 */

/// @param[in]  s ~ narrow string to be converted to a wide string
/// @details    This method creates a wide string by copying data from a narrow string
/// @return		  copy of a wide string converted from a narrow
std::wstring
StrConverter::StringToWString(const std::string& s) {
  // append L before string literal to be returned
  std::wstring temp(s.length(), L' ');
  std::copy(s.begin(), s.end(), temp.begin());
  return temp;
}

// for use by the below ...
// #include <locale>
// #include <codecvt>

// alternate "recommended" C++ way ...
// std::wstring convertToWideString(const std::string& str) {
//   std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//   return converter.from_bytes(str);
// }

/**
 * @example WStringToString
 * 
 * wstring => string
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * std::wstring ws = L"This is a std::wstring object";
 * std::string ws_to_s = StrConverter::WStringToString(ws);
 * std::cout << ws_to_s << std::endl;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @see StrConverter::WStringToString
 */

/// @param[in]  ws ~ wide string to be converted to a narrow string
/// @details    This method creates a narrow string by copying data from a wide string
/// @return		  copy of a narrow string converted from a wide
std::string
StrConverter::WStringToString(const std::wstring& ws) {
  std::string temp(ws.length(), ' ');
  std::copy(ws.begin(), ws.end(), temp.begin());
  return temp;
}

/**
 * @example StrConverter::VectorToWideString
 * 
 * std::vector<std::string> => std::vector<std::wstring>
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * std::vector<std::string> narrow_vector = { "This is a narrow ... " };
 * std::vector<std::string> wide_vector = StrConverter::VectorToWideString(ws);
 * // do something with wide_vector ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @see StrConverter::VectorToWideString
 */

/// @brief
/// method to convert a vector of (narrow)
/// `ASCI` strings to wide (`UNICODE`) strings
/// @param narrow_vec ~ const reference of container to convert
/// @return copy of vector containing wide (`UNICODE`) strings
std::vector<std::wstring> StrConverter::VectorToWideString(
  const std::vector<std::string>& narrow_vec
) {
  std::vector<std::wstring> wide_vec;
  // reserve space to avoid AMORTIZATION
  wide_vec.reserve(narrow_vec.size());
  for (const auto& str : narrow_vec) {
      wide_vec.push_back(StringToWString(str));
  }
  return wide_vec;
}

/// @brief
/// method to convert a vector of (wide)
/// `UNICODE` strings to narrow (`ASCI`) strings
/// @param wide_vec ~ const reference of container to convert
/// @return copy of vector containing narrow (`ASCI`) strings
std::vector<std::string> StrConverter::VectorToNarrowString(
  const std::vector<std::wstring>& wide_vec
) {
  std::vector<std::string> narrow_vec;
  // reserve space to avoid AMORTIZATION
  narrow_vec.reserve(wide_vec.size());
  for (const auto& wstr : wide_vec) {
      narrow_vec.push_back(WStringToString(wstr));
  }
  return narrow_vec;
}

#endif // end of STR_CPP
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
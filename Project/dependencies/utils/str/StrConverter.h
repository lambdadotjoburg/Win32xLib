/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		StrConverter.h
  * @brief 		Declaration of StrConverter utility class
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * 
  * @details 	This file contains the method signatures for the StrConverter class
  * 
  */

#pragma once

/// @brief begin of STR_H declaration
#ifndef STR_H
#define STR_H

#include <string>
#include <algorithm> // std::copy
// #include <windows.h> // if using `wcstombs`
#include <vector> // for converting containers ...

/**
 * @class   StrConverter
 * @brief   class containing string utilitiy functions
 * @details This class contains static interface methods
 *          for converting between `string` & `wstring` objects
 */
class StrConverter {
    
public:

  /// @brief method for converting a narrow string to a wide string
  static std::wstring
  StringToWString(const std::string& s);

  /// C++ style "substitute" for `wcstombs`
  /// @brief method for converting a wide string to a narrow string
  static std::string
  WStringToString(const std::wstring& ws);

  /// @brief method to convert a container
  /// of (narrow) `ASCI` strings to wide (`UNICODE`)
  static std::vector<std::wstring>
  VectorToWideString(
    const std::vector<std::string>& narrow_vec
  );

  /// @brief method to convert a container
  /// of (wide) `UNICODE` strings to narrow (`ASCI`)
  static std::vector<std::string>
  VectorToNarrowString(
    const std::vector<std::wstring>& wide_vec
  );
};

#endif // end of STR_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
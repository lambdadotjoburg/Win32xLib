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

#ifndef STRCONVER_H
#define STRCONVER_H
#endif

#include <windows.h>
#include <string>
#include <algorithm> // std::copy

class StrConverter {
    
public:

  static std::wstring
  StringToWString(const std::string& s);

  static std::string
  WStringToString(const std::wstring& ws);

  // std::string convert(LPCTSTR input, int s = 64) {
  //   // input can be LPCSTR (ANSII) or LPCWSTR (UNICODE)
  //   // if input is of type LPCSTR then use std::string(input)
  //   std::string result = std::string(input);
  //   // if input is of type LPCWSTR then use `wcstombs`
  //   char res[s]; // declare a char buffer of sufficient size (default 64)
  //   int n = wcstombs(res, input, s); // number of wide characters converted
  // }

};
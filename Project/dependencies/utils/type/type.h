/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		type.h
  * @brief 		Declares the type library
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file declares the type library (typelib)
  */

#pragma once

/// @brief begin of TYPE_H declaration
#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <typeinfo>

/// @brief gets human-readable type/name
std::string demangle(const char* name);

// template function for use with any type object
/** 
 * @brief   template function to get human-readable type/name
 * @details template function (use with any type object) for
 *          returning human-readble type name of a given input
 *          var during program runtime
 */
template <class T>
std::string type(const T& t) {
  return demangle(typeid(t).name());
};

#endif // end of TYPE_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

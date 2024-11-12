/**
  * @file 		type.cpp
  * @brief 		Defines the type library
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file defines the type library (typelib)
  */

#ifndef TYPE_CPP
#define TYPE_CPP

#include "./type.h"

// if g++
#ifdef __GNUC__
/**
 * @def __GNUC__
 * @brief check if GNUC compiler
 */

// import for using __cxa_demangle()
#include <cxxabi.h>

/**
 * @brief     gets human-readable type/name
 * @param[in] name receives a constant character pointer to the variable
 * @return    returns a `string` representative of var type/name
 * 
 * @details   function for returning human-readble type name
 *            of a given input var during program runtime
 * 
 * @note      If `__GNUC__` is defined, then `demangle` uses `__cxa_demangle` otherwize returns the name as `typeid(var).name()`
 * @remark    see [libstdc++ for info about `__cxa_demangle`](https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html)
 */
std::string demangle(const char* name) {

    int status = -4;
    char* readable_name = abi::__cxa_demangle(name, NULL, NULL, &status);
    std::string res = std::string(readable_name);
    free(readable_name);
    return (status == 0) ? res : name;
}
#else // if not g++
std::string demangle(const char* name) {
    return name;
}
#endif
#endif
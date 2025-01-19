/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		SystemException.cpp
  * @brief 		Defines SystemException class (implementation)
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file defines `SystemException` class
  */

/// @brief begin of SYSEX_CPP definition
#ifndef SYSEX_CPP
#define SYSEX_CPP

#include "./SystemException.h"

/// @param[in] dscrptn ~ string message to display when exception is thrown
SystemException::SystemException(const std::string& dscrptn)
    : _dscrptn(dscrptn) {
    // nothing to do ...
}
/// @details method for showing `SystemException` message content ~ `_dscrptn`
const char* SystemException::what() const noexcept {
    // declare a static string to persist `const char *`
    // thread-safe => no racing-conditions!
    static std::string result;
    // Using `type(*this)` to get human-readable name for this exception class
    std::string msg = std::string(type(*this)) + ": " + _dscrptn;
    // assign `msg` string to `static std::string` object
    result = msg;
    // return a c-style string pointer of valid `static std::string` object
    return result.c_str();
}

/// @details `RuntimeException` extends `SystemException`
///           to be used for throwing runtime errors
RuntimeException::RuntimeException(const std::string& dscrptn)
    : SystemException(dscrptn) {}

#endif // end of SYSEX_CPP
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		SystemException.cpp
  * @brief 		Defines SystemException class (implementation)
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file defines SystemException class
  */

#ifndef SYSEX_CPP
#define SYSEX_CPP

#include "./SystemException.h"

/// @brief string holding the message to be displayed when exception throws
std::string msg; // keep the message for the throw exception as global ...

/// @brief parameterless constructor
SystemException::SystemException() {}

/// @brief parameterized constructor
/// @param[in] dscrptn ~ string message to display when exception is thrown
SystemException::SystemException(const std::string& dscrptn)
    : _dscrptn(dscrptn) {
    // nothing to do ...
}

/// @brief throws-clause for exception class
const char* SystemException::what() const throw() {
    msg = std::string(type(*this)) + ": " + _dscrptn;
    return msg.c_str();
}

/// @brief virtual class destructor
SystemException::~SystemException() {
    // nothing to do
}

/// @brief `RutimeException` class implementation
RuntimeException::RuntimeException(const std::string& dscrptn)
    : SystemException(dscrptn) {}
RuntimeException::~RuntimeException() {}

#endif
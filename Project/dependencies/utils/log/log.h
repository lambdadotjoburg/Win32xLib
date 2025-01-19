/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		log.h
  * @brief 		Defines global interface log's
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file defines a global `LOG(...)` macro function
  *     for reporting important log info to developers
  */

#pragma once

/// @brief begin of LOG_H global interface
#ifndef LOG_H
#define LOG_H

#ifdef NDEBUG // switch/toggle `NDEBUG` in makefile for "release" mode

/// @brief    variadic macro LOG function
/// @details  `LOG(...)` is defined whenever `NDEBUG` is NOT defined,
///            i.e. when building in no-debug mode!
#define LOG(...) // do (replace/expand) nothing ...

#else // if NDEBUG is NOT defined

#include <sstream>  // include sstream to simply stream buffer content
#include <iostream> // include iostream for std::cerr

/**
 * @brief          print debug info!
 * @details       `LOG(...)` method to print debug info to `std::cerr` if `NDEBUG` is NOT defined
 * @param[in] ...  variadic macro ` __VA_ARGS__ `
 * 
 * @note          If `NDEBUG` is NOT defined, then `LOG(...)` is expanded/replaced by NOTHING
 * @remark        see <a href="https://stackoverflow.com/questions/14251038/debug-macros-in-c" target="_blank">stackoverflow @ DEBUG MACRO in C++</a>
 * 
*/
#define LOG(...) {\
    char msg[256];\
    sprintf(msg, __VA_ARGS__);\
    std::stringstream ss;\
    ss\
    << "[" << "file: " << __FILE__     << "]"\
    << "[" << "fn: "   << __FUNCTION__ << "]"\
    << "[" << "line: " << __LINE__     << "]"\
    << "[" << "msg: "  << msg << "]"   << std::endl;\
    std::cerr << ss.str() << std::flush;\
};\

#endif // end of LOG

#endif // end of LOG_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
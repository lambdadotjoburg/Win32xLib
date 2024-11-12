#pragma once

#ifndef LOG_H
#define LOG_H

#ifdef NDEBUG // switch on NDEBUG in makefile for "release" mode

#define LOG(...) // do (replace/expand) nothing ...

#else // if NDEBUG is NOT defined

// include iostream for std::cerr
// #include <iostream>
#include <sstream>

/**
 * @brief         print debug info!
 * @details       LOG method to print debug info to std::cerr if NDEBUG is NOT defined
 * @param[in] ... variadic macro __VA_ARGS__
 * 
 * @note          If `NDEBUG is NOT defined, then the `LOG(...)` `#define` is expanded/replaced by NOTHING
 * @remark        see [stackoverflow @ DEBUG MACRO in C++](https://stackoverflow.com/questions/14251038/debug-macros-in-c)
*/
#define LOG(...) {\
    char msg[100];\
    sprintf(msg, __VA_ARGS__);\
    std::stringstream ss;\
    ss\
    << "[" << "file: " << __FILE__ << "]"\
    << "[" << "fn: " << __FUNCTION__ << "]"\
    << "[" << "line: " << __LINE__ << "]"\
    << "[" << "msg: " << msg << "]" << std::endl;\
    std::cerr << ss.str() << std::flush;\
};\

#endif

#endif
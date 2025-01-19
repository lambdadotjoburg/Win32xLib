/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		macro.h
  * @brief 		Defines global interface macro's
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file defines global macro's
  *     for simplifying some developer work for
  *     building `UNICODE` or `ANSI`/`ASCII` projects
  */

#pragma once

/// @brief begin of MACRO_H global interface
#ifndef MACRO_H
#define MACRO_H

#include <iostream>

// The "correct" using MinGW compiler,
// to suppress warnings for unused parameters,
// is using the `__UNUSED_PARAM(x)`

/// @brief MACRO for nullifying/void unused arguments to suppress warnings ...
#define UNUSED(x) (void)(x) // C-stlye cast any variable/arg/param to void
/// @brief MACRO for suppressing unused arguments warnings
///        by assiging a variable to itself ...
#define IMPLICIT(x) { x = x; } // assign variable to itself

/// @brief   define MACRO for printing `string` (ANSI) or `wstring` (UNICODE)
/// @details Depending on `BUILDMODE` during compilation `TPRINT(x)`
///          expands as either ... <br/>
///          <br/>
///             `std::cout << x << std::endl;` <br/>
///           or ...
///           <br/>
///             `std::wcout << x << std::endl;` <br/>
#if defined(_UNICODE) && defined(UNICODE)
#define TPRINT(x) std::wcout << (x) << std::endl;
#else
#define TPRINT(x) std::cout << (x) << std::endl;
#endif

/// @brief   define MACRO for reporting build mode (UNICODE/ANSI)
/// @details `BUILDMODE` is determined during compilation
///           using the appropriate `makefile` recipe using
///           either `make` (ANSI/ASCII) or `make win` (UNIDOCE).
///           `BUILDMODE` is defined as "UNICODE" if both `UNICODE` & `_UNICODE` are defined.
///            In contrast, `BUILDMODE` is defined as "ANSI/ASCII" if either one of `UNICODE` or `UNICODE` are not defined.
/// @note     *DO NOT* define `UNICODE` & `_UNICODE` in program script ~ these are defined according to `makefile` recipes: <br/>
///            <br/>
///            `make` => `ANSI/ASCII` build <br/>
///            `make win` => `UNICODE` build <br/>
#if defined(_UNICODE) && defined(UNICODE)
#define BUILDMODE "UNICODE"
#else
#define BUILDMODE "ANSI/ASCII"
#endif

#endif // end of MACRO_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
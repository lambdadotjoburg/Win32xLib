#pragma once

#ifndef MACRO_H
#define MACRO_H

#include <iostream>

// define MACRO for nullifying/void unused arguments to suppress warnings ...
#define UNUSED(x) (void)(x) // C-stlye cast any variable/arg/param to void

// define MACRO for printing string (ANSII) or wstring (UNICODE) to the console
#if defined(_UNICODE) && defined(UNICODE)
#define TPRINT(x) std::wcout << (x) << std::endl;
#else
#define TPRINT(x) std::cout << (x) << std::endl;
#endif

// define MACRO for showing/displaying build info/mode (UNICODE/ANSII)
#if defined(_UNICODE) && defined(UNICODE)
#define BUILDMODE "UNICODE"
#else
#define BUILDMODE "ANSII"
#endif

#endif
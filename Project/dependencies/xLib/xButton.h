/**
 * @file    xButton.h
 * @author  &lambda;ambda
 * @date    \showdate "%Y-%m-%d"
 * @brief   `xButton` class declaration & implemenation header file
 * @details xButton.h defines the client-code class interface for creating `xButton`s
 */
#pragma once

#ifndef xBUTTON_H
#define xBUTTON_H

// No need to include windows header file here ...
// since xWidget.h includes windows header file ...
// which is included before xButton is defined ...
// #include <windows.h>

#include "./xControl.h"

/**
 * @class    xButton
 * @brief    `xButton` derives from abstract class template `xControl<xButton>`
 * @details  `xButton` provides the client-code interface
 *           for instantiating `xWidget` objects of type `xButton`
 *           given specific parameters, like:
 * 
 *           - position ~ \f( (x, y) \f)
 *           - dimensions ~ \f( (w, h) \f)
 *           - text ~ `const std::string&`
 *           - `append(&frame)`
 *           - etc ...
 */
class xButton : public xControl<xButton> {
public:
    // xButton code here ...
    xButton() {/* ... */}
};

#endif
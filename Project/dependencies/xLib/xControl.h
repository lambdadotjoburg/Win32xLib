/**
 * @file    xControl.h
 * @author  &lambda;ambda
 * @date    \showdate "%Y-%m-%d"
 * 
 * @brief   `xControl` & `xWindow` have a similar/nearly-identical
 *          class/interface declarations & implemention/definitions
 * 
 * @details This design decision has the following trade-offs:
 * 
 * - Code-repetition in TWO `xControl`/`xWindow` classes
 * - Gain access to a `this` instance pointer via `GWLP_USERDATA`
 * - Avoid circular dependency between `xWidget` & `xManager`,
 *   since `xControl` & `xWindow` is responsible for interfacing with `xManager`
 */

#pragma once

#ifndef xCONTROL_H
#define xCONTROL_H

#include "./xWidget.h"

/**
 * @class   xControl
 * @brief   `xControl` is an abstract class template, deriving from `xWidget`
 * @details `xControl` is responsible for registering & creating
 *          `xControl<xButton>`/`xControl<xTextBox>`/`xControl<xLable>`
 *          derived type instances & performs message dispatch
 *          for all of it's derived types
 */
template <class T>
class xControl : public xWidget {
public:
    // xControl code here ...
    xControl() {
        // test (to be used elsewhere) ...
        // `xControl` has access to `xApp`
        xApp::get().hGlobalInstance;
    };
};

#endif
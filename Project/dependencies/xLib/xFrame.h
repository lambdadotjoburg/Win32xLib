/**
 * @file    xFrame.h
 * @author  &lambda;ambda
 * @date    \showdate "%Y-%m-%d"
 * @brief   `xFrame` class declaration & implemenation header file
 * @details xFrame.h defines the client-code class interface for creating `xFrame`s
 */

#pragma once

#ifndef xFRAME_H
#define xFRAME_H

// No need to include windows header file here ...
// since xWidget.h includes windows header file ...
// which is included before xFrame is defined ...
// #include <windows.h>

#include "./xWindow.h"

/**
 * @class    xFrame
 * @brief    `xFrame` derives from abstract class template `xWindow<xFrame>`
 * @details  `xFrame` provides the client-code interface
 *           for instantiating `xWidget` objects of type `xFrame`
 *           given specific parameters, like:
 * 
 *           - position ~ \f( (x, y) \f)
 *           - dimensions ~ \f( (w, h) \f)
 *           - title (text) ~ `const std::string&`
 *           - `show()`/`hide()` ~ (true/false)
 *           - etc ...
 */
class xFrame : public xWindow<xFrame> {
public:
    
    /// @brief parameterless/default constructor
    xFrame() {
        // ...
        // test (to be used elsewhere) ...
        // `xWindow` has access to `xApp`
        // xApp::get().hGlobalInstance;
        // Set style of xWidget window class struct
        mStyle = WS_OVERLAPPEDWINDOW;
    };

    /// @brief parameterized constructor accepting no parent `xWidget*` pointer
    /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
    /// @param[in] x ~ the horizontal position of the `xWidget`
    /// @param[in] y ~ the vertical position of the `xWidget`
    /// @param[in] w ~ the width of the `xWidget`
    /// @param[in] h ~ the height of the `xWidget`
    xFrame(const std::string& text, int x, int y, int w, int h)
        : xWindow(text, x, y, w, h)
        {
        // ...
        // Set style of xWidget window class struct
        mStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS;
    };

    /// @brief parameterized constructor accepting a parent `xWidget*` pointer
    /// @param[in] parent ~ `xWidget*` pointer to parent
    /// @param[in] text ~ the text to be displayed in the `xFrame` (`xWidget`)
    /// @param[in] x ~ the horizontal position of the `xFrame`
    /// @param[in] y ~ the vertical position of the `xFrame`
    /// @param[in] w ~ the width of the `xFrame`
    /// @param[in] h ~ the height of the `xFrame`
    xFrame(xWidget* parent, const std::string& text, int x, int y, int w, int h)
        : xWindow(parent, text, x, y, w, h)
        {
        // ...
        // Set style of xWidget window class struct
        // WS_CHILD indicates that the xFrame is a sub-window of its parent
        // This behaviour may be undesirable for most purposes => to be adjusted for common use
        mStyle = WS_OVERLAPPEDWINDOW | WS_CHILD | WS_CLIPSIBLINGS;
    };

    /// @brief final override method that returns the classname of this widget window class
    /// @return returns a long pointer to a constant `T` string representative of the widget class
    LPCTSTR ClassName() const override {
        return TEXT("xFrame"); // ANY NAME YOU WANT
    }

    /// @brief final override method that returns the typename of this widget window type
    /// @return returns a long pointer to a constant `T` string representative of the widget type
    /// @details In the context of xButtons, this value is capitals "BUTTON" corresponding to the built-in windows class
    ///          With common controls (commctrl.h) header file included & visual styles enabled, use `WC_BUTTON`
    LPCTSTR TypeName() const {
        return TEXT("xFrame"); // ANY NAME YOU WANT
    }

    /// @brief     Message Handler for specific xWidget derived type/class
    ///
    /// @details   Pure virtual abstract method to enforce derived class override
    ///            Each derived class type is responsible to override `HandleMessage`
    ///            This is to ensure that each derived class handles message events
    ///            appropriately/according to its intended purpose/use!
    ///            This method remains virtual in case client-code needs interfacing
    /// 
    /// @param[in] msg ~ `uint` message value received from `WndProc`
    ///
    /// @param[in] wParam ~ a number the size of a pointer (meaning depends on msg)
    /// @param[in] lParam ~ a number the size of a pointer (meaning depends on msg)
    /// 
    /// @return    integer value returned to Windows OS
    ///            (program's response to particular msg)
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
        // ...
        return DefWindowProc(mhWnd, msg, wParam, lParam);
    }

};

#endif
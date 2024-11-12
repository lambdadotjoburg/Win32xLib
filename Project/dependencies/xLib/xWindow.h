/**
 * @file    xWindow.h
 * @author  &lambda;ambda
 * @date    \showdate "%Y-%m-%d"
 * 
 * @brief   `xWindow` & `xControl` have a similar/nearly-identical
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

#ifndef xWINDOW_H
#define xWINDOW_H

#include "./xWidget.h"

/**
 * @class   xWindow
 * @brief   `xWindow` is an abstract class template, deriving from `xWidget`
 * @details `xWindow` is responsible for registering & creating
 *          `xWindow<xFrame>` derived type instances
 *          & performs message dispatch for all of it's derived types
 */
template <class T>
class xWindow : public xWidget {

// public class members ...
public:

    // define xWindow CALLBACK procedure ...
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        // window procedure code ...
        
        // return default window procedure ...
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    
    /// @brief parameterless/default constructor
    xWindow() {
        // ...
        // test (to be used elsewhere) ...
        // `xWindow` has access to `xApp`
        // xApp::get().hGlobalInstance;
    };

    /// @brief parameterized constructor accepting no parent `xWidget*` pointer
    /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
    /// @param[in] x ~ the horizontal position of the `xWidget`
    /// @param[in] y ~ the vertical position of the `xWidget`
    /// @param[in] w ~ the width of the `xWidget`
    /// @param[in] h ~ the height of the `xWidget`
    xWindow(const std::string& text, int x, int y, int w, int h)
        : xWidget(text, x, y, w, h)
        {
        // ...
    };

    /// @brief parameterized constructor accepting a parent `xWidget*` pointer
    /// @param[in] parent ~ `xWidget*` pointer to parent
    /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
    /// @param[in] x ~ the horizontal position of the `xWidget`
    /// @param[in] y ~ the vertical position of the `xWidget`
    /// @param[in] w ~ the width of the `xWidget`
    /// @param[in] h ~ the height of the `xWidget`
    xWindow(xWidget* parent, const std::string& text, int x, int y, int w, int h)
        : xWidget(parent, text, x, y, w, h)
        {
        // ...
    };

// protected class members ...
protected:

    /// @brief   gets the classname of the defined widget
    /// @return  long pointer to a constant `T` string
    /// @details pure virtual abstract method
    ///          to enforce derived class to define
    ///          a classname for the widget it defines
    ///          to be used when registering the window class struct
    virtual LPCTSTR ClassName() const = 0;

    /// @brief   gets the typename of the defined widget
    /// @return  long pointer to a constant `T` string
    /// @details
    ////         
    ///          pure virtual abstract method
    ///          to enforce derived class to define
    ///          a typename for the widget it defines
    ///          to be used when creating the window class struct
    ///
    ///          In the context of a button, this MUST be captitals "BUTTON"
    ///          & to correspond to the pre-defined windows class names!
    ///          Use WC_BUTTON whenever visual-styles is enabled
    virtual LPCTSTR TypeName() const = 0;

    /// @brief   Pure virtual abstract method for handling main window procedure dispatch
    ///
    /// @details   Pure virtual abstract method to enforce derived class override
    ///            Each derived class type is responsible to override `HandleMessage`
    ///            This is to ensure that each derived class handles message events
    ///            appropriately/according to its intended purpose/use!
    /// 
    /// @param[in] msg ~ `uint` message value received from `WndProc`
    ///
    /// @param[in] wParam ~ a number the size of a pointer (meaning depends on msg)
    /// @param[in] lParam ~ a number the size of a pointer (meaning depends on msg)
    /// 
    /// @return    integer value returned to Windows OS
    ///            (program's response to particular msg)
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

// protected members ...
protected:

    /// @brief method for registering window class struct `WNDCLASSEX wx`
    /// @return boolean flag indicating whether window registration success/fail
    /// @details this method never returns false, instead invokes `exit(-1)` to abort program
    bool RegisterWindow() {
        
        // register the window class
        LOG("RegisterWindow()");
        
        // set the window class attribute/properties
        wx.cbSize = sizeof(WNDCLASSEX);
        wx.style = 0;
        wx.cbClsExtra = 0;
        wx.hInstance = xApp::get().hGlobalInstance; // GetModuleHandle(NULL)
        wx.hIcon = NULL; // LoadIcon(NULL, IDI_APPLICATION); // IDI => short for Icon ID
        wx.hCursor = LoadCursor(NULL, IDC_ARROW); // default cursor
        wx.hbrBackground = (HBRUSH)(COLOR_WINDOW); // 
        wx.lpszMenuName = NULL; //

        // special class attributes
        wx.lpfnWndProc = T::WndProc; // static LRESULT CALLBACK
        wx.lpszClassName = ClassName(); // pure abstract virtual override returns LPSTSTR

        // before registering window class, first check if already exists
        if (xManager::get().isRegisered(mClassName)) {
            LOG("xWindow class already registered; proceeding ...");
            return true;
        }

        // try to register the window class struct
        try {
            // `RegisterClassEx` accepts reference to WNDCLASSEX wx struct
            if (!RegisterClassEx(&wx)) {
                // if class registration fails, store last error from OS
                int error = GetLastError();
                // concatenate to string message
                std::string message = "xWindow registration failed: " + std::to_string(error);
                LOG(message.c_str()); // LOG message
                throw xWidgetException(message);
            }
        } catch(xWidgetException &ex) {
            // stream exception message to std::cerr
            std::cerr << ex.what() << std::endl;
            exit(-1); // abort
        }

        // store the classname in `xManager`
        xManager::get().setClassName(mClassName);
        LOG("xWindow registration successful!");
        return true;
    }

    /// @brief method to create the window class after registering window class struct
    /// @return ~ true/false => whether or not `CreateWindowEx` method assigns a `HWDN` to `mhWnd`
    bool create() {
        
        // register the windows class ...
        RegisterWindow();
        
        LOG("create()");

        // try to create the window class ...
        try {
            // assign `CreateWindowEx` return value to internal `mhWnd` class member
            mhWnd = CreateWindowEx(
                mExStyle, // extra styles
                TypeName(), // type name received from derived class override
                mText.c_str(), // `ANSII` vs. `UNICODE`
                mStyle, // This specifies that the `xWidget` is an `xFrame`
                m_x, m_y, m_w, m_h, // dimensions & coordinates
                (parent != nullptr ? parent->Handle() : NULL), // handle to parent window (if any)
                NULL, // handle to menu (if any)
                xApp::get().hGlobalInstance, // GetModuleHandle(NULL)
                this // this can be a `LPVOID` to any defined type to be recovered via `GWLP_USERDATA`
            );

            // if `mhWnd` is `NULL`, i.e. `if CreateWindowEx` fails
            if (!mhWnd) {
                // then throw exception & abort ...
                int error = GetLastError();
                std::string message = "xWindow create failed: " + std::to_string(error);
                LOG(message.c_str());
                throw xWidgetException(message);
            }
        } catch(xWidgetException &ex) {
            std::cerr << ex.what() << std::endl;
            exit(-1); // terminate // abort
        } // otherwize, window class instance created success

        // if this xWidget is a child widget,
        // then add it to its parent's children-list `xChildren`
        if (parent != nullptr) {
            parent->setChild(this);
        }
        
        // mark widget as `exists`
        exists = true;
        LOG("xWindow created successfully!");

        return (mhWnd ? true : false);
    }

    /// @brief parameter to toggle between "show"/"hide" state
    int mState;

// public members ...
public:

    /// @brief     method to toggle the state of display
    /// @param[in] state ~ `boolean` flag whether or not
    ///            to show/hide/display the window
    ///
    /// @details   This method is responsible for invoking
    ///            `create()` on the `xWidget` data
    ///            depending on the `boolean` value `exists`
    ///            Client code MUST invoke `show()` or `hide()`
    ///            to trigger `create()` if they wish to adjust
    ///            any of the `xWidget` fields/properties/parameters
    void setVisible(bool state) {

        mState = state;
        
        // invoke create here for the first time if not exists
        if (!exists) {
            create();
        }

        // check state flag
        if (mState) {
            // if true, update internal member mStyle
            mStyle |= SW_SHOW | WS_VISIBLE;
            ShowWindow(mhWnd, SW_SHOW);
        } else { // if false, remove visibility from mStyle
            mStyle &= ~SW_HIDE & ~WS_VISIBLE;
            ShowWindow(mhWnd, SW_HIDE);
        }

        UpdateWindow(mhWnd);
    }

    /// @brief minimal interface method ~ invoking `setVisible(true)`
    void show() {
        // ...
        setVisible(true);
    }

    /// @brief minimal interface method ~ invoking `setVisible(false)`
    void hide() {
        // ...
        setVisible(false);
    }

    /// @brief 
    void setMaximize() {
        // ...
    }

    /// @brief 
    void maximize() {
        // ...
    }

    /// @brief 
    void minimize() {
        // ...
    }

    // ...

    // etc ...

    // ...

};

#endif
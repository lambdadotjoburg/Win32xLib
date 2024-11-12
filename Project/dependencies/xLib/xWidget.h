/**
 * @file    xWidget.h
 * @author  &lambda;ambda
 * @date    \showdate "%Y-%m-%d"
 * @brief   `xWidget` class declaration & implemenation header file
 * @details xWidget.h defines the `xWidget` base class for ALL derived types
 */

#pragma once

#ifndef xWIDGET_H
#define xWIDGET_H

// include windows header file here ...
#include <windows.h>
#include <commctrl.h>

// undefine unicode macro
// makefile is responsible for defining
// -DUNICODE & -D_UNICODE flags during g++ compilation
// #undef UNICODE
// #undef _UNICODE

// include exception handling utilities ...
#include "../utils/ex/SystemException.h"

/**
 * @class   xWidget
 * @brief   `xWidget` is an abstract base class for ALL
 *          `xWidget` derived classes/types
 * 
 * @details `xWidget` is implemented to ensure that the derived classes
 *          can store instance pointers & enforces the type-hierarchy
 *          for its derived types/classess
 */
class xWidget {

protected:

    /**
     * @class xWidgetException
     * @brief `xWidgetException` is a subclass of xWidget
     * @details `xWidgetException` is used during
     *          window class registration & creation
     *          `xWidgetException` is protected, since
     *          `xWindow` & `xControl` derives from it
     */
    class xWidgetException : public SystemException {
    public:
        /// @brief   xWidgetException constructor
        /// @details xWidgetException is derived from SystemException
        /// @throws  xWidgetException
        xWidgetException(const std::string& dscrptn)
            : SystemException(dscrptn) {}
        /// @brief xWidgetException destructor
        ~xWidgetException() {}
    };

// protected class members ...
public:

    /// @brief window class struct associated to the `xWidget`
    WNDCLASSEX wx = { };

    /// @brief handle to the `xWidget` instance
    HWND mhWnd;

    /// @brief pointer to the `xWidget`s parent (if any)
    xWidget* parent = nullptr;

protected:

    /// @brief string representative of the `xWidget` registered window class
    std::string mClassName;

    /**
     * @brief    `ANSII` `std::string` versus `UNICODE` `std::wstring`
     *           text of the `xWidget` [`std::wstring`] => `UNICODE` build
     *           text of the `xWidget` [`std::string`] => `ANSII` build
     *      
     * @details 
     *           To ensure that the `xWidget` `mText` property can be
     *           set correctly, a MACRO is used to determine the string
     *           type during program compilation
     * 
     *           In the context of `xButton` & other `xControl` widgets,
     *           the (`std::string`) text represents the button's actual text
     *           & in the context of `xFrame` widgets, the text represents
     *           the frame/window title
     * 
     *           In the context of `xButton` & other `xControl` widgets,
     *           the (`std::wstring`) text represents the button's actual text
     *           & in the context of `xFrame` widgets, the text represents
     *           the frame/window title
     */
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstring mText;
    #else
    std::string mText;
    #endif

    /// @brief dimensions & position of `xWidget`
    int m_x, m_y, m_w, m_h;

    /// @brief window class struct styles associated to the `xWidget`
    DWORD mStyle = (DWORD)NULL;
    /// @brief window class struct extra styles associated to the `xWidget`
    DWORD mExStyle = (DWORD)NULL;

    /// @brief menu ID associate to the `xWidget`/window class struct
    ///        used to identify during message-dispatch/event-handling
    HMENU mID = NULL;
    
    /// @brief handle to the `Icon` associated to
    ///        the `xWidget`/window class struct
    HICON mIcon = NULL;

    /// @brief handle to the `cursor` associated to
    ///        the `xWidget`/window class struct
    HCURSOR mCursor = NULL;

    /// @brief field to mark xWidget as "exists" or NOT ~ (true/false)
    bool exists;

    /// @brief container storing `xWidget*` to `xWidget` instance
    std::vector<xWidget*> xChildren;

public:

    /// @brief method to append a child element to the list of child widgets
    void setChild(xWidget* child) {
        xChildren.push_back(child);
    }

protected:

    // Come back to this ...
    // comment-out for now, since otherwize compilation errors ...
    // derived classes MUST override pure virtual abstract base class methods ...

    /// @brief   Pure virtual abstract method for handling main window procedure dispatch
    ///
    /// @details pure virtual abstract method to enforce derived class override
    ///          Each derived class type is responsible to override `HandleMessage`
    ///          This is to ensure that each derived class handles message events
    ///          appropriately/according to its intended purpose/use!
    ///
    /// @return  integers value returned to Windows OS
    ///          (program's response to particular msg)
    // virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

// public class members ...
public:
    
    /// @brief parameterless/default constructor
    xWidget() {
        // ...
        // provide public interface to set `xWidget` members externally ...
        // such as text, parent, dimensions & position ...
    };

    /// @brief parameterized constructor accepting no parent `xWidget*` pointer
    /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
    /// @param[in] x ~ the horizontal position of the `xWidget`
    /// @param[in] y ~ the vertical position of the `xWidget`
    /// @param[in] w ~ the width of the `xWidget`
    /// @param[in] h ~ the height of the `xWidget`
    xWidget(const std::string& text, int x, int y, int w, int h)
        : m_x{x}, m_y{y}, m_w{w}, m_h{h}
        {
        // ...
        // convert the text to appropriate encoding
        // ANSII std::string vs. UNICODE std::wstring
        convert(text);
    };

    /// @brief parameterized constructor accepting a parent `xWidget*` pointer
    /// @param[in] parent ~ `xWidget*` pointer to parent
    /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
    /// @param[in] x ~ the horizontal position of the `xWidget`
    /// @param[in] y ~ the vertical position of the `xWidget`
    /// @param[in] w ~ the width of the `xWidget`
    /// @param[in] h ~ the height of the `xWidget`
    xWidget(xWidget* parent, const std::string& text, int x, int y, int w, int h)
        : parent{parent}, m_x{x}, m_y{y}, m_w{w}, m_h{h}
        {
        // ...
        // convert the text to appropriate encoding
        // ANSII std::string vs. UNICODE std::wstring
        convert(text);
    };

    /// @brief     method to retrieve a child `xWidget*`
    /// @param[in] i ~ `int` index
    /// @return    pointer to an `xWidget` child
    /// @details   method to retrieve the `xWidget`s child element
    ///            at a specified `int` index `i` from `xChildren` vector
    xWidget* child(int i) {
        return xChildren[i];
    }

    /// @brief   method to retrieve `xWidget`s parent pointer
    /// @return `xWidget*` pointer to its parent
    xWidget* Parent() {
        return parent;
    }

    /// @brief  method to return the handle of the `xWidget`s window
    /// @return handle of the `xWidget` window
    HWND Handle() {
        return mhWnd;
    }

    /// @brief  direct retrieval method for the parent window handle
    /// @return handle to parent `xWidget`
    HWND hParent() {
        return parent->Handle();
    }

    /// @brief    
    ///         (helper) method to set `xWidget` text-data
    ///         depending on `ANSII` [`std::string`]
    ///         vs. `UNICODE` [`std::wstring`] build
    /// 
    /// @param[in] text ~ `std::string` `C++` style string input
    void convert(const std::string& text) {
        #if defined(UNICODE) && defined(_UNICODE)
        // copy the `const std::string&` data from `text`
        // into a new `std::wstring` variable `wstr`
        std::wstring wstr(text.begin(), text.end());
        mText = wstr; // assign the copied data to mText
        #else
        // just copy the data over into mText directly
        mText = text;
        #endif
    }

    /// @brief 
    /// @param x 
    /// @param y 
    void setText(const std::string& text) {
        // set the xWidget mText to text & update the window class text ...
    }

    /// @brief 
    /// @param x 
    /// @param y 
    void setHorizontalPosition(int x) {
        // ...
    }

    /// @brief 
    /// @param x 
    /// @param y 
    void setVerticalPosition(int y) {
        // ...
    }
    
    /// @brief 
    /// @param x 
    /// @param y 
    void setWidth(int w) {
        // ...
    }

    /// @brief 
    /// @param x 
    /// @param y 
    void setHeight(int h) {
        // ...
    }

    /// @brief 
    /// @param x 
    /// @param y 
    void center(int x = 0, int y = 0) {
        // ...
    }

    // etc ...

};

#endif
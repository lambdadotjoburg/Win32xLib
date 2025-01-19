/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xControl.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xControl` class declaration & implemenation
  * @details  xControl.h defines the `xControl` class
  *           having immediate offspring:
  *           `xButton`, `xLabel`, `xRadioButton`,
  *           `xCheckBox`, `xDropDown` & `xTreeView`
  */

#pragma once

/// @brief begin of xCONTROL_H implementation
#ifndef xCONTROL_H
#define xCONTROL_H

#include "./xWidget.h"

/** 
 * @class    xControl
 * @brief   `xControl` is a pure abstract class derived from `xWidget`
 * @details `xControl` is implemented to enfore the `xLib` type hierarchy
 *           & provides the public interface for control-related methods
 *           from which other control-type classes are derived such as
 *           `xButton` & `xLabel` & `xTextBox`, xDropDown, etc ...
 */
class xControl : public xWidget, public iClickEventListener {

public:

  /**
   * @brief     static/shared subclass procedure (`ControlSubclassProc`)
   * @param[in] hWnd ~ handle of the widget/window receiving the `msg`
   * @param[in] msg ~ `uint` message ID to process
   * @param[in] wParam ~ message data (interpretation depends on the `msg`)
   * @param[in] lParam ~ message data (interpretation depends on the `msg`)
   * @details   Subclass Callback Procedure through which
   *            xControl click events are dispatched/re-directed
   *            to the relevant derived type `HandleMessage(...)` routine
   * @return   `LRESULT` ~ integer value returned to OS (program response to particular `msg`)
   */
  static LRESULT CALLBACK ControlSubclassProc(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,
    UINT_PTR uIdSubclass, DWORD_PTR dwRefData
  ) {

    // LOG("ControlSubclassProc::WM_DESTROY");
    
    // get a pointer of the widget sending the message ...
    xWidget* pWidget = xWidgetManager::get().getWidget(hWnd);

    IMPLICIT(uIdSubclass); // suppress unused param
    IMPLICIT(dwRefData); // suppress unused param

    switch(msg) {

      // redirect all the below cases to `xControl`s
      // message handler event dispatch `HandleMessage(...)`

      // Notification Messages ...
      case WM_SETFOCUS:
      case WM_KILLFOCUS:
      // case NM_CUSTOMDRAW:
      // Window Messages ...
      case WM_NOTIFY:
      // drawing-related routines ...
      case WM_PAINT:
      case WM_NCPAINT:
      case WM_CHAR:
      case WM_ERASEBKGND:
      case WM_CTLCOLORBTN:
      case WM_CTLCOLOREDIT:
      case WM_CTLCOLORSTATIC: {
        // std::cout << "WM_RBUTTONDBLCLK" << std::endl;
        if (pWidget) {
          pWidget->HandleMessage(msg, wParam, lParam);
        }
        break;
      }

      case WM_CONTEXTMENU: {
        /// @todo define mechanism to display/show context menu for `xWidget`s
        /// TrackPopupMenu(...) ~ similar to `xFrame`s version ...
        return DefWindowProc(pWidget->mhWnd, msg, wParam, lParam);
      }

      case WM_DESTROY: {
        // ensure that the derive type object's
        // resources are destroyed correctly ...
        pWidget->destroy();
        break;
      }

      // move this to `xControl` for ALL derived types ...
      case WM_KEYDOWN: {
        // std::cout << "WM_KEYDOWN" << std::endl;
        // Key key = static_cast<Key> (wParam);
        Key key(static_cast<UINT>(wParam));

        if (pWidget->mOnKeyPress) {
          pWidget->mOnKeyPress(pWidget, key);
        }
        break;
      }

      // move this to `xControl` for ALL derived types ...
      case WM_KEYUP: {
        // std::cout << "WM_KEYUP" << std::endl;
        // Key key = static_cast<Key> (wParam);
        Key key(static_cast<UINT>(wParam));

        if (pWidget->mOnKeyRelease) {
          pWidget->mOnKeyRelease(pWidget, key);
        }
        break;
      }
      
      // MouseEvent::EXIT
      case WM_MOUSELEAVE: {

        // toggle hover-state ...
        pWidget->isHovering = false;

        if (pWidget->mOnMouseHover) {
          pWidget->mOnMouseHover(pWidget, MouseEvent::EXIT);
          SendMessage(pWidget->mhWnd, WM_PAINT, 0, 0);
        }
        break;
      }
     
      // Hover ...
      // MouseEvent::ENTER
      case WM_MOUSEMOVE: {

        if (!pWidget->isHovering) {
          TRACKMOUSEEVENT tme;
          tme.cbSize = sizeof(TRACKMOUSEEVENT);
          tme.dwFlags = TME_LEAVE;
          tme.hwndTrack = hWnd;
          tme.dwHoverTime = 0;
          TrackMouseEvent(&tme);
          // toggle hover-state ...
          pWidget->isHovering = true;
        }
        
        if (pWidget->mOnMouseHover) {
          pWidget->mOnMouseHover(pWidget, MouseEvent::ENTER);
        }
        break;
      }

      // Clicks & Double Clicks ...
      case WM_LBUTTONDOWN: {
        if (pWidget->mOnMouseClick != nullptr) {
          pWidget->mOnMouseClick(pWidget, MouseKey::LeftButton);
        }
        break;
      }

      case WM_LBUTTONUP: {
        if (pWidget->mOnMouseRelease != nullptr) {
          pWidget->mOnMouseRelease(pWidget, MouseKey::LeftButton);
        }
        break;
      }

      case WM_LBUTTONDBLCLK: {
        if (pWidget->mOnMouseDoubleClick != nullptr) {
          pWidget->mOnMouseDoubleClick(pWidget, MouseKey::LeftButton);
        }
        break;
      }

      case WM_RBUTTONDOWN: {
        if (pWidget->mOnMouseClick != nullptr) {
          pWidget->mOnMouseClick(pWidget, MouseKey::RightButton);
        }
        break;
      }

      case WM_RBUTTONUP: {
        if (pWidget->mOnMouseRelease != nullptr) {
          pWidget->mOnMouseRelease(pWidget, MouseKey::RightButton);
        }
        break;
      }

      case WM_RBUTTONDBLCLK: {
        if (pWidget->mOnMouseDoubleClick != nullptr) {
          pWidget->mOnMouseDoubleClick(pWidget, MouseKey::RightButton);
        }
        break;
      }

      case WM_MBUTTONDOWN: {
        if (pWidget->mOnMouseClick != nullptr) {
          pWidget->mOnMouseClick(pWidget, MouseKey::MiddleButton);
        }
        break;
      }

      case WM_MBUTTONUP: {
        if (pWidget->mOnMouseRelease != nullptr) {
          pWidget->mOnMouseRelease(pWidget, MouseKey::MiddleButton);
        }
        break;
      }

      case WM_MBUTTONDBLCLK: {
        if (pWidget->mOnMouseDoubleClick != nullptr) {
          pWidget->mOnMouseDoubleClick(pWidget, MouseKey::MiddleButton);
        }
        break;
      }

      case WM_XBUTTONDOWN: {
        if (pWidget->mOnMouseClick != nullptr) {
          pWidget->mOnMouseClick(pWidget, MouseKey::ExtraButton);
        }
        break;
      }

      case WM_XBUTTONUP: {
        if (pWidget->mOnMouseRelease != nullptr) {
          pWidget->mOnMouseRelease(pWidget, MouseKey::ExtraButton);
        }
        break;
      }

      case WM_XBUTTONDBLCLK: {
        if (pWidget->mOnMouseDoubleClick != nullptr) {
          pWidget->mOnMouseDoubleClick(pWidget, MouseKey::ExtraButton);
        }
        break;
      }
    }

    // Handle default cases ...
    return DefSubclassProc(hWnd, msg, wParam, lParam);
  }

protected:

  // button-related only => not common controls => `WM_COMMAND`
  // i.e. setOnClick & setOnDoubleClick for `xLabel`, `xTextBox`, etc ...
  virtual void setOnClick(event_onClick callback) override = 0;
  virtual void setOnDoubleClick(event_onDoubleClick callback) override = 0;

public:

  /// @brief   abstract class default/parameterless constructor
  /// @details cannot instantiate an `xControl` abstract class type object
  xControl()
    : xWidget()
    {
    m_x = 0, m_y = 0;
    // owner-draw/constrained to parent/owner
    isSubWidget = true;
  }

  /// @brief     abstract class parameterized constructor
  /// @details   cannot instantiate an `xControl` abstract class type object
  /// @param[in] text ~ the text to be displayed in the `xControl` (`xButton`/`xLabel`, etc ...)
  /// @param[in] w ~ the width of the `xControl` (no default specified)
  /// @param[in] h ~ the height of the `xControl` (no default specified)
  /// @param[in] x ~ the horizontal position of the `xControl`
  /// @param[in] y ~ the vertical position of the `xControl`
  /// @note no default specified for width & height params,
  /// since each `xControl` object may have different defaults
  xControl(const std::string& text, int w, int h, int x=0, int y=0)
    : xWidget(text, w, h, x, y)
    {
    // ...
    // owner-draw/constrained to parent/owner
    isSubWidget = true;
  }

  /// @brief     abstract class parameterized constructor accepting parent pointer
  /// @details   cannot instantiate an `xControl` abstract class type object
  /// @param[in] parent ~ `xWidget*` pointer to parent widget
  /// @param[in] text ~ the text to be displayed in the `xControl` (`xButton`/`xLabel`, etc ...)
  /// @param[in] w ~ the width of the `xControl`    (no default specified)
  /// @param[in] h ~ the height of the `xControl`   (no default specified)
  /// @param[in] x ~ the horizontal position of the `xControl` (default 0)
  /// @param[in] y ~ the vertical position of the `xControl`   (default 0)
  /// @note no default specified for width & height params,
  /// since each `xControl` object may have different defaults
  xControl(xWidget* parent, const std::string& text, int w, int h, int x = 0, int y = 0)
    : xWidget(parent, text, w, h, x, y)
    {
    // ...
    // owner-draw/constrained to parent/owner
    isSubWidget = true;
  }

protected:

  /// @brief   pure virtual abstract method to enforce derived-class override for derived-class specific class name
  /// @return `LPCTSTR` ~ a c-style string to be used when registering window class
  virtual LPCTSTR ClassName() const override = 0;

  /// @brief   pure virtual abstract method to enforce derived-class override for derived-class specific type name
  /// @return `LPCTSTR` ~ a c-style string to be used when creating window instance
  /// @remark
  /// In the context of `xFrame` both `ClassName()` & `TypeName()` returns `TEXT("xFrame")` <br/>
  /// In the context of `xButton` `TypeName()` returns `WC_BUTTON` if visual styles         <br/>
  /// common controls library is loaded or TEXT("BUTTON") otherwize,                        <br/>
  /// while `ClassName()` returns `TEXT("xButton")`                                         <br/>
  virtual LPCTSTR TypeName() const override = 0;

public:

  /// @brief  pure virtual abstract method to enforce derived-type specific message handling
  /// @param  msg ~ `uint` message value received from `WndProc`
  /// @param  wParam ~ a number the size of a pointer (meaning depends on `msg`)
  /// @param  lParam ~ a number the size of a pointer (meaning depends on `msg`)
  /// @return LRESULT callback ~ `int` value returned to OS [program's response to `msg`]
  virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override = 0;

public:

  /// @brief pure virtual abstract method for drawing `xWidget`s
  virtual LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) override = 0;

};

#endif // end of xCONTROL_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
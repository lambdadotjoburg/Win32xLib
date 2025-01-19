/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xButton.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xButton` class declaration & implemenation
  * @details  xButton.h defines the `xButton` class
  */

#pragma once

/// @brief begin of xBUTTON_H implementation
#ifndef xBUTTON_H
#define xBUTTON_H

#include "./xControl.h"

/**
 * @class    xButton
 * @brief   `xButton` provides the interface for creating button controls
 * @details `xButton` defines button-related methods for creating
 * & manipulating `xButton` objects derived from `xControl`
 */
class xButton : public xControl {

  /*%*%*%*%*%*%*%*%*%*/
  /*  Button Control */
  /*%*%*%*%*%*%*%*%*%*/

  /// @brief global constant storing the default xButton width
  #define DEFAULT_BUTTON_WIDTH 63

  /// @brief global constant storing the default xButton height
  #define DEFAULT_BUTTON_HEIGHT 24

public:

  /// @brief provide client-friendly name for setting button border => invokes `setWidgetBorder()`
  /// @param[in] pBorder ~ pointer to the `xBorder` object to set
  void setButtonBorder(xBorder* pBorder) {
    setWidgetBorder(pBorder); // just a renaming convention for ease of use ...
  }

  /// @brief  method to retrieve the xBorder* associated to the button
  /// @return pointer to the `xBorder` object
  xBorder* getButtonBorder() {
    return pWidgetBorder;
  }

protected:

  // move the draw-routines to xControl base class ...

  /// @brief   code-interface method to modulazie the drawing stages of a control
  /// @details This method encapsulates the drawing stages of the control!
  ///          The fill-region takes place in the border drawing stage ...
  /// @note
  /// The `xWidget`s background & text WILL be updated
  /// EACH time that this notification message is processed
  /// whether or not client-code invokes `setBackgroundcolor(...)`
  /// or not & regardless of whether or not client code invokes
  /// `setTextColor()` or any other border/xWidget related styles ...
  virtual void draw(xWidget* pWidget) override;

  /// @brief code-interface method to camouflage the control's background
  /// @details 
  /// The background behind the control is colored using the parent frame's
  /// background color, since if rounded edges are specified, the background
  /// may still be obviously visible, specifically in the corners of a square rectangle
  void camouflage(xWidget* pWidget) override;

  /// @brief   method to write the text inside of the control with applied styles
  /// @details The write method is to be invoked last
  /// when writing text to a control, to avoid painting
  /// over the text at a later stage, obscuring the data
  void write(xWidget* pWidget) override;

public:

  // move this code to xControl base class ...

  /// @brief method to draw `xButton`s
  LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) override {
    
    // LOG("xButton::CustomDraw(...)");

    LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW) lParam;
    this->setDrawContext(lpnmCD->hdc);
    this->setDrawRect(lpnmCD->rc);

    switch(lpnmCD->dwDrawStage) {

      case CDDS_PREPAINT: {
        // std::cout << "CDDS_ITEMPREPAINT" << std::endl;
        // If the border is not defined,
        // this will also not change the text,
        // To fix this, `xLib` defines default borders ...
        // image button that have no border defined will skip this stage,
        // thus, for image buttons, `xLib` provides a default `xEmptyBorder` ...
        if (!pWidgetBorder) {
          return FALSE;
        };
        // start drawing routines ...
        draw(this);
        // ensure that the default text
        // & default focus border is NOT drawn
        return CDRF_SKIPDEFAULT;
      } // end of `CDDS_PREPAINT`

      case CDDS_POSTPAINT: {
        // If the control was drawn successfully ...
        return CDRF_NOTIFYITEMDRAW;
      } // end of `CDDS_POSTPAINT`

    } // end of `switch(lpnmCD->dwDrawStage)`

    // handle other window procedures ...
    return DefWindowProc(mhWnd, msg, wParam, lParam);
  }

public:

  /// @brief   parameterless/default constructor
  /// @remark
  /// The only reason this constructor exists is
  /// to provide flexibility to client-code, i.e.
  /// to decide the parameters during runtime
  /// <br/>
  /// client-code may not immediately "know" any of the preferred
  /// parameters upon instantiation, however, the object can only
  /// be created when for the first time when the parent is defined
  xButton()
    : xControl()
    {
    // set default coordinates for `xButton`
    // using parameterless/default constructor
    m_w=63, m_h=24, m_x=0, m_y=0;
    // set attributes for "default" owner-drawn buttons ...
    appendWindowStyle(WS_CHILD | BS_NOTIFY | WS_TABSTOP | WS_CLIPSIBLINGS /* | WS_CLIPCHILDREN */);
    setHoverColor(RGB(203, 230, 239)); // light blue ...
    initDefaultHandlers();
  }

  /// @brief     (preferred) parametrized constructor
  /// @param[in] text ~ the text to be displayed in the `xButton`
  /// @param[in] w ~ the width of the `xButton`               (default 63)
  /// @param[in] h ~ the height of the `xButton`              (default 24)
  /// @param[in] x ~ the horizontal position of the `xButton` (default 0)
  /// @param[in] y ~ the vertical position of the `xButton`   (default 0)
  /// @details   provides a client-code constructor interface for
  /// instantiating `xButton` objects by provinding `xButton` params,
  /// including a reference pointer to a parent widgets
  xButton(
    const std::string& text,
    int w = DEFAULT_BUTTON_WIDTH,
    int h = DEFAULT_BUTTON_HEIGHT,
    int x = 0, int y = 0
  ) : xControl(text, w, h, x, y) {
    // ...
    // set attributes for "default" owner-drawn buttons ...
    appendWindowStyle(WS_CHILD | BS_NOTIFY | WS_TABSTOP | WS_CLIPSIBLINGS /* | WS_CLIPCHILDREN */);
    setHoverColor(RGB(203, 230, 239)); // light blue ...
    initDefaultHandlers();
  }

  /// @brief  fully-parametrized public constructor
  /// @param[in] parent ~ `xWidget*` pointer to parent widget
  /// @param[in] text ~ the text to be displayed in the `xButton`
  /// @param[in] w ~ the width of the `xButton`               (default 63)
  /// @param[in] h ~ the height of the `xButton`              (default 24)
  /// @param[in] x ~ the horizontal position of the `xButton` (default 0)
  /// @param[in] y ~ the vertical position of the `xButton`   (default 0)
  /// @details   provides a client-code constructor interface for
  /// instantiating `xButton` objects by provinding `xButton` params,
  /// including a reference pointer to a parent widgets
  /// @remark
  /// The only reason this constructor exists is
  /// to provide flexibility to client-code, i.e.
  /// to decide the parameters during runtime
  /// <br/>
  /// client-code may not immediately "know" the owner/parent,
  /// however, it can only be created when the parent is defined
  xButton(
    xWidget* parent,
    const std::string& text,
    int w = DEFAULT_BUTTON_WIDTH,
    int h = DEFAULT_BUTTON_HEIGHT,
    int x = 0, int y = 0
  ) : xControl(parent, text, w, h, x, y) {
    // ...
    // set attributes for "default" owner-drawn buttons ...
    appendWindowStyle(WS_CHILD | BS_NOTIFY | WS_TABSTOP | WS_CLIPSIBLINGS /* | WS_CLIPCHILDREN */);
    setHoverColor(RGB(203, 230, 239)); // light blue ...
    initDefaultHandlers();
  }

protected:

  /**
   * @name Default Event Handlers 
   * @brief default static/shared mouse click / release / hover / focus events ...
   * @{ */
  static void defaultOnFocusHandler(xWidget* pWidget, FocusEvent event);
  static void defaultOnMouseClickHandler(xWidget* pWidget, MouseKey key);
  static void defaultOnMouseReleaseHandler(xWidget* pWidget, MouseKey key);
  static void defaultOnMouseHoverHandler(xWidget* pWidget, MouseEvent event);
  /** @} */

  /// @brief override method to initialize default event handlers associated to `xButton` objects
  virtual void initDefaultHandlers() override {
    
    // check whether client-code had already associated
    // an event handler, otherwize provide the
    // default version ...

    if (!mOnMouseClick) {
      setOnMouseClick(defaultOnMouseClickHandler);
    }

    if (!mOnMouseRelease) {
      setOnMouseRelease(defaultOnMouseReleaseHandler);
    }

    if (!mOnFocus) {
      setOnFocus(defaultOnFocusHandler);
    }

    if (!mOnMouseHover) {
      setOnMouseHover(defaultOnMouseHoverHandler);
    }
  }

  /// @brief method to attach a mouse-click event handler to the `xButton` control
  /// @param callback ~ callback function to invoke upon mouse-click detected
  virtual void setOnMouseClick(event_onMouseClick callback) override {
    // if callback function is a `nullptr` 
    // then attach the `defaultOnMouseClickHandler`
    // do the same for other `xControl`s that require default handlers ...
    if (callback == nullptr) {
      mOnMouseClick = std::move(defaultOnMouseClickHandler);
      return;
    }
    mOnMouseClick = std::move(callback);
  };

  /// @brief method to attach a mouse-release event handler to the `xButton` control
  /// @param callback ~ callback function to invoke upon mouse-release detected
  virtual void setOnMouseRelease(event_onMouseRelease callback) override {
    // if callback function is a `nullptr` 
    // then attach the `defaultOnMouseReleaseHandler`
    // do the same for other `xControl`s that require default handlers ...
    if (callback == nullptr) {
      mOnMouseClick = std::move(defaultOnMouseClickHandler);
      return;
    }
    mOnMouseRelease = std::move(callback);
  }

protected:

  /**
   * @remark
   * The class name is `xLib`-specific, to ensure that each
   * window class receives a unique class name upon registration
   * @note This method MUST NOT be overridden if client-code
   *       extends `xButton` thus it is marked as `final`
   * @see `ClientDemoClass`
   */
  LPCTSTR ClassName() const final override;

  /**
   * @remark
   * The type name is Win32-specific, to ensure that each
   * window class receives the correct built-in class type,
   * upon registration, such as `WC_BUTTON`, except for `xFrames`
   * @note This method MUST NOT be overridden if client-code
   *       extends `xButton` thus it is marked `final`
   * @see `ClientDemoClass`
   */
  LPCTSTR TypeName() const final override;

public:

  /**
   * @remark This method is marked `virtual` to allow client-code the opportunity
   *         of overriding for further customizations by extension/inheritance
   * @note   This method *MUST be overridden* if client-code extends `xButton`
   *         to ensure derived-type specific functionality/behaviour/message-handling
   */
  virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

public:

  /*%*%*%*%*%*/
  /* cleanup */
  /*%*%*%*%*%*/

  /// @brief  `~xButton` is `virtual` by virtue of the fact
  /// that the base class destructor `~xWidget` is virtual
  /* virtual */ ~xButton() {
    // nothing to do,
    // since most/all resources
    // are held in `xWidget` base class
    // LOG("~xButton()");
  }

public:

  /// @brief method to attach a button-click event handler to the `xButton` control
  /// @param callback ~ the function to invoke upon button-click detected
  virtual void setOnClick(event_onClick callback) override {
    mOnClick = std::move(callback);
  }

  /// @brief method to attach a button double click event handler to the `xButton` control
  /// @param callback ~ the function to invoke upon button double click detected
  virtual void setOnDoubleClick(event_onDoubleClick callback) override {
    mOnDoubleClick = std::move(callback);
  }
};

/// @brief  method retrieving the `xButton`s class name when requested by `xWidget` base class `RegisterWidget()` method
/// @return returns `LPCTSTR` class name TEXT("xButton") ~ `ANSI`/`ASCII` & `UNICODE` compatible
LPCTSTR xButton::ClassName() const {
  return TEXT("xButton");
};

/// @brief  method retrieving the `xButton`s type name when requested by `xWidget` base class `create()` method
/// @return returns `LPCTSTR` type name WC_BUTTON/TEXT("BUTTON") ~ `ANSI`/`ASCII` & `UNICODE` compatible
LPCTSTR xButton::TypeName() const {
  return WC_BUTTON; // common controls vizual styles enabled!
  // WC_BUTTON equivalent TEXT("BUTTON")
  // return TEXT("BUTTON");
};

/// @brief    `xButton`s `HandleMessage` overrided method, to handle `msg`s for `xButton` objects
/// @details   Implements the message handler for the `xButton` type, i.e. `xButton` `msg`s are handled here!
/// @param[in] msg ~ `uint` message value received from `WndProc` redirected/filtered according to `mhWnd`
/// @param[in] wParam ~ a number the size of a pointer (meaning depends on `msg`)
/// @param[in] lParam ~ a number the size of a pointer (meaning depends on `msg`)
LRESULT xButton::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {

  switch(msg) {

    /// dummy `WM_PAINT`
    /// to allow for base class message redirecting ...
    /// specifically, to handle `WM_PAINT` in other xControl derived types ...
    case WM_PAINT: {
      // do not paint the client area of the button ...
      return 0;
    }

    /// dummy `WM_NCPAINT`
    /// to allow for base class message redirecting ...
    /// specifically, to handle `WM_PAINT` in other xControl derived types ...
    case WM_NCPAINT: {
      // do not paint the non-client area of the button ...
      return 0;
    }

    // handle `TrackPopupMenu(...)`s `WM_COMMAND` messages ...
    case WM_COMMAND: {
      
      switch(HIWORD(wParam)) {
        
        /// @todo
        /// DIFFICULT! <br/>
        /// define a mechanism that distinguishes double clicks from single clicks by
        /// using a clock function to determine the elapsed time since the last single
        /// click then process the event on a different thread as single click only if
        /// no new click message is sent within elapsed time, otherwize process
        /// the `BN_DBLCLK` as per usual => do this for all double click events!

        // on double click ...
        case BN_DBLCLK: {
          if (mOnDoubleClick != nullptr) {
            mOnDoubleClick(this);
          }
          break;
        }
        
        // on left click ...
        case BN_CLICKED: {
          if (mOnClick) {
            mOnClick(this);
          }
          break;
        }

        // on focus gained ...
        case BN_SETFOCUS: {
          this->setFocus(true);
          if (mOnFocus != nullptr) {
            mOnFocus(this, FocusEvent::GAINED);
          }
          break;
        }

        // on focus lost ...
        case BN_KILLFOCUS: {
          this->setFocus(false);
          // this->mOnFocusLost();
          if (mOnFocus != nullptr) {
            mOnFocus(this, FocusEvent::LOST);
          }
          break;
        }
      } // end of `switch(HIWORD(wParam))`
      
      break;
    } // end of `WM_COMMAND`

    case WM_NOTIFY: {

      // std::cout << "xButton::WM_NOTIFY" << std::endl;

      // "Long Pointer Notification Message Draw Routine"
      LPNMHDR lpnmhdr = (LPNMHDR) lParam;

      // check which notification `msg`
      // is received by inspecting `code`
      switch(lpnmhdr->code) {

        // in case of NM_CUSTOMDRAW message,
        // handle custom drawing of button window
        case NM_CUSTOMDRAW: {

          // pWidget is gotten from a child control ...
          return this->CustomDraw(msg, wParam, lParam);
        } // end of NM_CUSTOMDRAW

      } // end of switch(lpnmhdr->code)

      break;
    } // end of `WM_NOTIFY`
    
    /// ...
  }

  // continue handling control's messages ...
  return DefWindowProc(mhWnd, msg, wParam, lParam);
} // end of `switch(msg)`

/*%*%*%*%*%*%*%*%*%*/
/* implementation  */
/*%*%*%*%*%*%*%*%*%*/

/// @brief   implementation of `xButton::camouflage`
/// @details to hide the window rect corners that may "spill" over
void xButton::camouflage(xWidget* widget) {
  // create a solid brush
  // from parent's background color
  HBRUSH hBrush = CreateSolidBrush(
    widget->Parent()->getBackgroundColor()
  );
  // paint the rectangular region to clear
  // any "spill" of rectangle corners
  FillRect(
    widget->getDrawContext(),
    &widget->getDrawRect(),
    hBrush
  );
  // free handle of brush resource
  DeleteObject(hBrush);
}

/// @brief   implementation of `xButton::write`
///          for writing text in the control
/// @details This method is to be invoked last
///          within the `xWidget`s `draw(...)`
///          method, to avoid the case that
///          the `camouflage(...)` method 
///          to potentially obscure the text
void xButton::write(xWidget* widget) {
  // set the text color based on
  // the control's text color

  HDC hDC = widget->getDrawContext();

  SetTextColor(
    hDC,
    widget->getTextColor()
  );
  // set the background mode to TRANSPARENT
  SetBkMode(hDC, TRANSPARENT);
  // write the text in the control
  DrawText(
    hDC,
    #if defined(UNICODE) && defined(_UNICODE)
    StrConverter::StringToWString(widget->getText()).c_str(),
    #else
    widget->getText().c_str(),
    #endif
    -1,
    &widget->getDrawRect(),
    widget->getTextAlignment()
  );
}

/// @brief   implementation of `xButton::draw(...)` routine
/// @details Encapsulates the various draw stages of the control ...
void xButton::draw(xWidget* widget) {
  
  // invoke camouflage method ...
  widget->camouflage(widget);

  xBorder* pBorder = widget->getWidgetBorder();
  if (pBorder) {
    // invoke the border-drawing routine ...
    pBorder->draw(this);
  }
  
  xBorder* pFocusBorder = widget->getFocusBorder();
  if (pFocusBorder) {
    // invoke the focus-border drawing routine ...
    pFocusBorder->draw(this);
  }

  // only write the button's text once
  // the area fill has been applied
  // to avoid painting over the text
  widget->write(widget);
}

#endif // end of xBUTTON_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xWindow.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xWindow` class declaration & implemenation
  * @details  xWindow.h defines the `xWindow` class
  *           having immediate offspring `xFrame` & `xPanel`
  */

#pragma once

/// @brief begin of xWINDOW_H implementation
#ifndef xWINDOW_H
#define xWINDOW_H

#include "./xWidget.h"

/**
 * @class    xWindow
 * @brief   `xWindow` is a pure abstract class derived from `xWidget`
 * @details `xWindow` is implemented to enfore the `xLib` type hierarchy
 *           & provides the public interface for window-related methods
 *           from which other window-type classes are derived such as
 *           `xFrame` & `xPanel` & `xPane`, etc ...
 */
class xWindow : public xWidget {

public:

  /// @brief pure virtual abstract method for drawing `xWindow`s
  virtual LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) override = 0;

protected:

  /// @brief   pure virtual abstract method to enforce derived-class override for derived-class specific class name
  /// @return `LPCTSTR` ~ a c-style string to be used when registering window class
  virtual LPCTSTR ClassName() const = 0;

  /// @brief  pure virtual abstract method to enforce derived-class override for derived-class specific type name
  /// @return `LPCTSTR` ~ a c-style string to be used when creating window instance
  /// @remark
  /// In the context of `xFrame` both `ClassName()` & `TypeName()` returns `TEXT("xFrame")` <br/>
  /// In the context of `xButton` `TypeName()` returns `WC_BUTTON` if visual styles         <br/>
  /// common controls library is loaded or TEXT("BUTTON") otherwize,                        <br/>
  /// while `ClassName()` returns `TEXT("xButton")`                                         <br/>
  
  virtual LPCTSTR TypeName() const = 0;
  /// @brief  virtual method to encourage derived-type specific message handling
  ///         but some `xFrame` subclass `msg`s are redirected to this `xWindow` parent type
  ///         for better code readability, specifically for the `xButton` `WM_CUSTOMDRAW` `msg`s
  /// @param  msg ~ `uint` message value received from `WndProc`
  /// @param  wParam ~ a number the size of a pointer (meaning depends on `msg`)
  /// @param  lParam ~ a number the size of a pointer (meaning depends on `msg`)
  /// @return LRESULT callback ~ `int` value returned to OS [program's response to `msg`]
  virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam); // no longer pure abstract

protected:

  /// @brief   abstract class default/parameterless constructor
  /// @details cannot instantiate an `xWindow` abstract class type object
  xWindow()
    : xWidget()
    {
    // ...
  }

  /// @brief     abstract class parameterized constructor
  /// @details   cannot instantiate an `xWindow` abstract class type object
  /// @param[in] text ~ the text to be displayed in the `xWindow` (`xFrame`/`xPanel`, etc ...)
  /// @param[in] w ~ the width of the `xWindow`
  /// @param[in] h ~ the height of the `xWindow`
  /// @param[in] x ~ the horizontal position of the `xWindow`
  /// @param[in] y ~ the vertical position of the `xWindow`
  xWindow(const std::string& text, int w, int h, int x, int y)
    : xWidget(text, w, h, x, y)
    {
    // ...
  }

  /// @brief     abstract class parameterized constructor accepting parent pointer
  /// @details   cannot instantiate an `xWindow` abstract class type object
  /// @param[in] parent ~ `xWidget*` pointer to parent widget
  /// @param[in] text ~ the text to be displayed in the `xWindow` (`xFrame`/`xPanel`, etc ...)
  /// @param[in] w ~ the width of the `xWindow`
  /// @param[in] h ~ the height of the `xWindow`
  /// @param[in] x ~ the horizontal position of the `xWindow`
  /// @param[in] y ~ the vertical position of the `xWindow`
  xWindow(xWidget* parent, const std::string& text, int w, int h, int x, int y)
    : xWidget(parent, text, w, h, x, y)
    {
    // ...
  }

public:

  /*%*%*%*%*%*%*%*%*%*%*/
  /* Show/Hide states  */
  /*%*%*%*%*%*%*%*%*%*%*/
  
  /// @brief method to make the `xWindow` object visible on the screen
  void show() {
    // toggle show/hide state
    // toggle the `visible` base class member to `true`
    setVisible(true); // invoke `xWidget` base class `setVisible(true)`
  }

  /// @brief method to make the `xWindow` object invisible on the screen
  void hide() {
    // toggle show/hide state
    // toggle the `visible` base class member to `false`
    setVisible(false); // invoke `xWidget` base class `setVisible(true)`
  }

  /// @brief   method for restoring the state of an `xWindow`
  /// @details
  /// if hidden & `restore()` is invoked, then the `xWindow` will be shown <br/>
  /// if shown & `restore()` is invoked, then the `xWindow` will be hidden <br/>
  void restore() {
    // if widget does not exist ...
    if (!exists) {
      return; // cannot send `SW_RESTORE` message
    }
    // restore show/hide state of window
    ShowWindow(mhWnd, SW_RESTORE);
  }

  /*%*%*%*%*%*%*%*%*%*%*/
  /* Minimize/Maximize */
  /*%*%*%*%*%*%*%*%*%*%*/

  /// @brief method to enlarge the `xWindow`
  /// @details takes-up the desktop/screen space
  /// @remark NOT equivalent to "full-screen mode"
  /// @param state ~ boolean flag indicating whether
  /// (`true`) or not (`false`) to maximize the `xWindow`
  /// (default `true`)
  void setMaximize(bool state = true) {

    // if the `xWindow` does not exist ...
    if (!exists) {
      create(); // create (regardless of `state`) ...
    }
    // guaranteed to exist ...
    ShowWindow(mhWnd, state ? SW_MAXIMIZE : SW_RESTORE);
  }

  /// @brief method to shrink the `xWindow
  /// @details shows the window's system menu (if any)
  /// in left-bottom corner of desktop/screen
  /// @param[in] state ~ boolean flag indicating whether
  /// (`true`) or not (`false`) to maximize the `xWindow`
  /// (default `true`)
  void setMinimize(bool state = true) {

    // if the `xWindow` does not exist ...
    if (!exists) {
      create(); // create (regardless of `state`) ...
    }

    ShowWindow(mhWnd, state ? SW_MINIMIZE : SW_RESTORE);
  }

  /// @brief shorthand notation for `widget.setMaximize(true);`
  void maximize() {
    setMaximize(true);
  }

  /// @brief shorthand notation for `widget.setMinimize(true);`
  void minimize() {
    setMinimize(true);
  }

  /*%*%*%*%*%*%*%*/
  /* System Menu */
  /*%*%*%*%*%*%*%*/

  /// @brief     method to disable/enable the close button on the `xWindow`
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true`) or not (`false`)
  ///            to disable/enable the close button
  ///            (default `true`)
  /// @todo      initialize this within derived class `create(...)`
  void disableClose(bool flag = true) {
    
    // get the handle of the `xWindow`s menu
    HMENU hMenu = GetSystemMenu(mhWnd, FALSE);

    if (!hMenu) {
      return;
    }

    if (flag) { // true
      // disable the close button
      EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    } else { // false
      // enable the close button
      EnableMenuItem(hMenu, SC_CLOSE, MF_ENABLED);
    }
  }

  /// @brief     method to remove/add the minimize button on the `xWindow`
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true`) or not (`false`)
  ///            to remove/add the minimize button
  ///            (default `true`)
  /// @todo      initialize this within derived class `create(...)` method
  void disableMinimize(bool flag = true) {
    // window may not exist at this point ...
    // temporary ...
    if (!exists) {
      return; // change this later ...
    }
    // GWL_XXX => cannot use `appendWindowStyle(...)`/`removeWindowStyle(...)` here ...
    long style = GetWindowLongPtr(mhWnd, GWL_STYLE);
    if (flag) {
      // remove minimize box style ...
      style &= ~WS_MINIMIZEBOX;
    } else {
      // append minimize box style ...
      style |= WS_MINIMIZEBOX;
    }
    SetWindowLongPtr(mhWnd, GWL_STYLE, style);
    render();
  }

  /// @brief     method to remove/add the maximize button on the `xWindow`
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true`) or not (`false`)
  ///            to remove/add the maximize button
  ///            (default `true`)
  /// @todo      initialize this within derived class `create(...)` method
  void disableMaximize(bool flag = true) {
    // window may not exist at this point ...
    // temporary ...
    if (!exists) {
      return; // change this later ...
    }
    // GWL_XXX => cannot use appendWindowStyle(...)/removeWindowStyle(...) here ...
    long style = GetWindowLongPtr(mhWnd, GWL_STYLE);
    if (flag) {
      // remove minimize box style ...
      style &= ~WS_MAXIMIZEBOX;
    } else {
      // append minimize box style ...
      style |= WS_MAXIMIZEBOX;
    }
    SetWindowLongPtr(mhWnd, GWL_STYLE, style);
    render();
  }

  /// @brief     method to enable/disable re-size-ing of `xWindow`
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true`) or not (`false`)
  ///            the `xWindow` is re-size-able (default `true`)
  void disableResize(bool flag = true) {
    if (flag) {
      removeWindowStyle(WS_THICKFRAME);
    } else {
      appendWindowStyle(WS_THICKFRAME);
    }    
    render();
  }

protected:

  /// @brief variable indicating whether (`true`)
  /// or not (`false`) an `xWindow` can be moved
  bool moveable = true;

public:

  /// @brief     method to enable/disable a `xWindow` from being move-able/"dragged"
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true`) or not (`false`)
  ///            an `xWindow` can be moved (default `true`)
  void disableMove(bool flag = true) {
    moveable = !flag; // flip boolean => disable means to "switch off" in this context ...
  }

  /// @brief     method to remove/add System Menu & associated buttons & operations from the `xWindow`
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true) or not (`false`)
  ///            to remove the System Menu (default `true`)
  void removeSystemMenu(bool flag = true) {
    if (flag) {
      removeWindowStyle(WS_SYSMENU);
    } else {
      appendWindowStyle(WS_SYSMENU);
    }
    render();
  }

  /// @brief     method to remove/add Caption/"title-bar" from the `xWindow`
  /// @param[in] flag ~ boolean flag indicating
  ///            whether (`true) or not (`false`)
  ///            to remove the Caption/"title-bar" (default `true`)
  void removeCaption(bool flag = true) {
    if (flag) {
      removeWindowStyle(WS_CAPTION);
    } else {
      appendWindowStyle(WS_CAPTION);
    }
    render();
  }

protected:

  /*%*%*%*%*%*%*%*%*%*/
  /* FULLSCREEN MODE */
  /*%*%*%*%*%*%*%*%*%*/

  /// @brief boolean flag to indicate fullscreen mode
  bool fullscreen = false;

public:

  /// @brief     method to toggle `xWindow` fullscreen mode
  /// @param[in] flag ~ boolean flag to toggle the fullscreen state
  /// @todo      Provide hot-key `F11` fullscreen toggle
  ///            <br/>
  ///            This should really be a user/client choice, 
  ///            but common for windows apps ...
  /// @todo      initialize this in derived class `create(...)` method
  void FullScreenMode(bool flag) {

    // assign boolean flag ...
    fullscreen = flag;

    if (fullscreen) {
      
      // request from `xGDI`
      int w = Monitor::ScreenWidth();
      int h = Monitor::ScreenHeight();

      removeWindowStyle(WS_OVERLAPPEDWINDOW); // no borders, no system menu, etc ...

      // window may not exist at this point ...
      if (!exists) {
        return; // temporary solution ...
      }
      SetWindowPos(mhWnd, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);

    } else {
      
      appendWindowStyle(WS_OVERLAPPEDWINDOW); // add back borders system menu, etc ...

      // window may not exist at this point ...
      if (!exists) {
        return; // temporary solution ...
      }
      SetWindowPos(mhWnd, HWND_TOP, m_x, m_y, m_w, m_h, SWP_FRAMECHANGED);
    }

    // ensure that the disable/enable state of the frame persists ...
    // `SetWindowPos(...)` may affect the window's styles, losing disable/enable state ...
    isInert(isIdle()); // set active based on modality of children & client preference
  }

protected:

  /*%*%*%*%*%*%*%*%*%*/
  /*  "Modal state"  */
  /*%*%*%*%*%*%*%*%*%*/

  /// @brief boolean field for keeping track of `inert` state toggle ...
  bool inert = false;

  /**
   * @brief     method to toggle window `inert` state => active/inactive
   * @param[in] flag ~ boolean flag to toggle `inert` state
   * @details   This method is used to interface with the code,
   *            specifically when assigning/removing a modal
   *            state from any of its children.
   * 
   * @remark    similar to `isIdle(...)` <br/>
   * 
   * `idle` is a client preference while `inert` is dictated by internal code machnisms <br/>
   *  This is done to enforce the mutually exclusive relationship between `idle` & `inert` <br/>
   * `idle` is applicable to all `xWidget`s & client code for enabling/disabling the interactive state of a widget <br/>
   *  This is not the case for `inert` which is only applicabl to `xWindow`s for internal use to toggle active/inactive
   *  states of a window based on the modality of its children
   * @todo initialize this within derived class `create(...)` method
   */
  void isInert(bool flag) {

    // assign flag input arg
    inert = flag;

    // window may not exist at this point ...
    if (!exists) {
      return; // temporary solution ...
    }

    // if true, disble window
    if (inert) {
      EnableWindow(this->Handle(), FALSE);
    } else {
      EnableWindow(this->Handle(), TRUE);
    }
  }

  /// @brief boolean field for keeping track of modal state toggle ...
  bool modal = false; // default false for all `xWindow`s

public:

 /**
  * @brief     define method to toggle `modal` state,
  * @details   i.e. make parent widget inactive/inert,
  *            until `inert` state is removed or if
  *            the widget itself is destroyed!
  * @param[in] flag ~ boolean parameter to toggle modal state of `xWindow`
  *            <br/>
  *            No default param provided, to force client-code to decide
  *            appropriate "modal state" => default param may cause
  *            unnecessary confusion
  * @todo initialize this within derived class `create(...)` method
  */
  void isModal(bool flag) {

    // check if `xWindow` has a parent,
    // otherwize this feature wouldn't make sense
    if (!parent) {
      return; // in which case return immediately
    }

    // accept the input & assign modal member variable
    modal = flag;

    if (modal) {
      // similar to `parent->disable();`
      static_cast<xWindow*>(parent)->isInert(true);
      // EnableWindow(parent->Handle(), FALSE); // disable parent
    } else {
      // similar to `parent->enable();`
      static_cast<xWindow*>(parent)->isInert(false);
      // EnableWindow(parent->Handle(), TRUE); // enable parent
    }
  }

public:

  /**
   * @brief   method to check whether an `xWindow`
   *          should be set "active"/"inactive"
   * @details This routine checks whether ALL
   *          children are NOT in modal state
   *          using the syntax:
   *          <br/>
   *          `static_cast<xWindow*>(parent)->isIdle()`
   * @return  returns boolean `||` between children
   *          modal state & this window's `inert` state
   * @remark  If any child xWindow is in modal state,
   *          then this method returns false <br/>
   *          If the `xWindow` itself is disabled,
   *          then this method also returns false,
   *          otherwize, this method returns true
   * @todo initialize this within derived class `create(...)` method
   */
  bool isIdle() override {

    //  Ensure the correct override version
    // `xWindow::isIdle()` is being invoked ...
    // LOG("xWindow::isIdle()");
    
    // initialize return variable
    bool flag = false;

    std::vector<xWidget*> childWindows = getChildrenByClassName("xFrame");
    for (size_t i = 0; i < childWindows.size(); i++) {
      // static down-cast from base class type `xWidget*` to `xWindow*`
      // to access the modal state which is only applicable to `xWindow`
      if (static_cast<xWindow*>(childWindows[i])->modal) {
        flag = true;
        break; // break from the loop
      }
    }

    // boolean `||` combine `flag` user preference `idle`
    return (flag || idle);
  }

protected:

  /*%*%*%*%*/
  /* Icon  */
  /*%*%*%*%*/

  /// @todo 
  /// Move icon-related stuff to derived `xFrame` class
  /// since not all `xWindow` derived types require `xIcon`

  /// @brief  pointer to a small icon [.ico/.png]
  /// @remark
  /// In the context of a `xWindow`/`xFrame`
  /// small & large icons can be different:
  /// <br/>
  /// Small => frame caption/menu <br/>
  /// Large => task-bar           <br/>
  xIcon* pSmallIcon = nullptr;
  
  /// @brief  pointer to a large icon [.ico/.png]
  /// @remark
  /// In the context of a `xWindow`/`xFrame`
  /// small & large icons can be different:
  /// <br/>
  /// Small => frame caption/menu <br/>
  /// Large => task-bar           <br/>
  xIcon* pLargeIcon = nullptr;

public:

  /// @brief     method to set both small & large icons simultaneously
  /// @param[in] path ~ the path of the icon file
  /// @remark    This method is marked `virtual` to allow `xControl`s override capability
  /// since `xContol`s generally only require a single icon, i.e. no distinction between small/large.
  /// If we are to segragate the code, so that `xWidgets` have a single icon but `xFrame`/`xWindow`
  /// has an additional icon, the code may be more difficult to read & free-ing the correct pointer
  /// may have additional overhead involved, thus it is decided that we keep the design as-is,
  /// at little extra cost of storing an additional pointer for an icon that may never
  /// actually be used in the context of `xContol`s
  virtual void setIcon(const std::string& path) {
    setSmallIcon(path);
    setLargeIcon(path);
  }

  /// @brief     method to set the small icon for an `xWidget`
  /// @param[in] path ~ the path of the icon file
  void setSmallIcon(const std::string& path) {
    // delete pointer if set to replace
    // only delete if icons are not the same pointer
    if ((pSmallIcon != nullptr) && (pSmallIcon != pLargeIcon)) {
      delete pSmallIcon;
      pSmallIcon = nullptr;
    }
    // set new icon
    pSmallIcon = new xIcon(path);
    // send window message to update/set icon
    pSmallIcon->setIcon(mhWnd, ICON_SMALL);
  }

  /// @brief     method to set the large icon for an `xWidget`
  /// @param[in] path ~ the path of the icon file
  void setLargeIcon(const std::string& path) {
    // delete pointer (if set) to replace previous
    // only delete if icons are not the same pointer
    // The latter case may occur if client code interfaces with `setIcon(...)`
    if ((pLargeIcon != nullptr) && (pLargeIcon != pSmallIcon)) {
      delete pLargeIcon;
      pLargeIcon = nullptr;
    }
    // set new icon
    pLargeIcon = new xIcon(path);
    // send window message to update/set icon
    pLargeIcon->setIcon(mhWnd, ICON_BIG);
  }

  /// @brief   method to get a pointer to the small icon for an `xWidget`
  /// @return `xIcon*` pointer to the small `xIcon`
  xIcon* getSmallIcon() {
    return pSmallIcon;
  }

  /// @brief   method to get a pointer to the large icon for an `xWidget`
  /// @return `xIcon*` pointer to the small `xIcon`
  xIcon* getLargeIcon() {
    return pLargeIcon;
  }

  /// @brief  method to get the string path of the small icon file
  /// @return string value representative of the small icon file path
  std::string getSmallIconPath() {
    try {

      if (!pSmallIcon) { // if invalid
        std::string message = "xIcon* not set";
        throw xIcon::xIconException(message);
      }

    } catch(xIcon::xIconException &ex) {
      
      std::cerr << ex.what() << std::endl;
      // return empty string ...
      return "";
      
    }
    return pSmallIcon->getIconPath();
  }

  /// @brief  method to get the string path of the large icon file
  /// @return string value representative of the large icon file path
  std::string getLargeIconPath() {
    try {

      if (!pLargeIcon) { // if invalid
        std::string message = "xIcon* not set";
        throw xIcon::xIconException(message);
      }

    } catch(xIcon::xIconException &ex) {
      
      std::cerr << ex.what() << std::endl;
      // return empty string ...
      return "";
      
    }    
    return pLargeIcon->getIconPath();
  }

protected:

  /**
   * 
   * @brief
   * pure virtual abstract method to enforce override in `xFrame`
   * @details
   * This method is declared to allow base-derived interfacing
   * <br/>
   * `xFrame` to invoke base class implementation, thus
   * avoid the need to expose `parent` base class member 
   * i.e. no implementation for `setParent(...)` required,
   * yet still provides private interface to set `parent`
   * from within derived-version's `add(...)` method
   */
  virtual void add(xWidget* widget) override = 0;

public:

  /*%*%*%*%*%*/
  /* cleanup */
  /*%*%*%*%*%*/

  /// @brief  `~xWindow` is `virtual` by virtue of the fact
  /// that the base class destructor `~xWidget` is virtual
  /*virtual*/ ~xWindow() {
    // nothing to do,
    // since all resources
    // are held in `xWidget` base class
    // LOG("~xWindow()");
    this->destroy(); // clean up xIcon resoureces ...
  }

  /// @brief override method to perform additional work on creation of the `xWindow`
  virtual bool create() override {

    // invoke base class method ...
    bool res = xWidget::create();

    // do some additional work ...

    // set the icons for the frame ...
    pSmallIcon->setIcon(mhWnd, ICON_SMALL);
    pLargeIcon->setIcon(mhWnd, ICON_BIG);

    /// @todo initialize the following during `create()`: <br/>
    /// <br/>
    /// 1) `FullScreenMode`, <br/>
    /// 2) `setMinimized`    <br/>
    /// 3) `setMaximized`    <br/>
    /// 4) `disableClose`    <br/>
    /// 5) `disableMaximize` <br/>
    /// 6) `disableMinimize` <br/>
    /// 7) `setVisible`      <br/>

    return res; // whether success
  }

protected:

  /// @brief derived class override method to properly
  /// destroy `xWindow` related resources, specifically
  /// for cleaning up `xWindow` icons (Small/Large)
  virtual void destroy() override {

    if (this->pLargeIcon) {
      delete this->pLargeIcon;
      this->pLargeIcon = nullptr;
    }
    
    if (this->pSmallIcon) {
      delete this->pSmallIcon;
      this->pSmallIcon = nullptr;
    }

    // invoke base class destroy ...
    xWidget::destroy();
  }

  // This method may NOT be a good idea for client-code
  // specifically if client assigns a parent that is not `nullptr`
  // for its top-level window, then this method will actually always recurrr ...
  // xWidget* getTopLevelParentWindow() {
  //   // recursively invoke getParent() until the parent does not have a parent,
  //   // in which case it the last non-nullptr widget is the top-level window ...
  //   // this could be dangerous, if the top-level window assigns a parent,
  //   // such as the desktop for example, or if the parent-child hierarchy
  //   // for the client-code implementation is a bit of a mess ...
  //   // a safer approach would be to invoke WM_DESTROY on all widgets
  //   // by class name "xFrame"
  //   return nullptr;
  // }

protected:

  /// @brief method to restore the focus of an `xWindow`
  ///        when ALL of its child frames lose modal state
  void restoreFocus() {

    // restore focus to parent ...
    if (parent) {

      // check if modal state & boolean flip ...
      if (modal) { // all modal frames have parent windows ...

        modal = !modal; // flip boolean
      }

      LOG(("modal: " + std::to_string(modal) + "\0").c_str());
      LOG(("idle: " + std::to_string(parent->isIdle())).c_str());
      LOG(("inert: " + std::to_string(static_cast<xWindow*>(parent)->inert)).c_str());

      if (!parent->isIdle()) { // parent not idle
        static_cast<xWindow*>(parent)->isInert(false); // make window active!
      }

      // @todo  Do some "magic" code to bring the frame forward,
      //        even if in idle state by client preference <br/>
      //        A possible mechanism would be to enable the frame
      //        then `SetFocus(...)` on it, & then disable the
      //        frame again, but that may be unnecessary work ...
      SetFocus(parent->Handle());
    }
  }

  // ...
};

/// @details implementation of `xWindow::HandleMessage(...)` message dispatch handler
/// @remark
/// This method handles ALL the messages received from `xFrame::HandleMessage(...)`
/// i.e. many messages that are `xWindow`-related may be easily handled here,
/// to preserve code quality & readbility in the `xFrame` derived implementation
/// @note
/// This method handles `xFrame` default messages,
/// i.e. those that are NOT processed by the `xFrame`
/// derived class implementation, propagates here!
LRESULT xWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {

  switch(msg) {

    case WM_DRAWITEM: {
      
      // cast lParam to LPDRAWITEMSTRUCT
      LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
      
      // // check whether the message was sent from an owner drawn button ...
      // if (lpDIS->CtlType == ODT_BUTTON) {
      //   // get the handle of the window/widget that sent the message
      //   xWidget* pWidget = xWidgetManager::get().getWidget(lpDIS->hwndItem);
      //   if (pWidget) {
      //     pWidget->CustomDraw(msg, wParam, lParam);
      //   }
      // }

      xWidget* pWidget = xWidgetManager::get().getWidget(lpDIS->hwndItem);
      if (pWidget) {
        pWidget->CustomDraw(msg, wParam, lParam);
      }

      return DefWindowProc(mhWnd, msg, wParam, lParam);
    }

    // This case is never reached, but instead
    // processed within `xFrame`s message handler ...
    // essentially equalivalent to right click ...
    // case WM_CONTEXTMENU: {
    //   // std::tuple<int, int> point = xGDI::get().MouseCoordinates(lParam);
    //   // HWND hFrom = xGDI::get().HandleRelativeToPoint(point);
    //   // std::cout << "hFrom: " << hFrom << std::endl;
    //   // std::cout << "xWindow::WM_CONTEXTMENU" << std::endl;
    //   // handle frame's `WM_CONTEXTMENU`
    //   // this->showContextMenu();
    //   break;
    // }

    // prefer to treat `WM_SYSCOMMAND` over `WM_MOVE`
    // for reserving later `onMoveEvent(...)`
    // implementation, in which case, `WM_MOVE` is
    // "free"/"available" to define separate case ...
    case WM_SYSCOMMAND: {

      // Possible to process `SC_CLOSE `message similarly, but we will NOT do it this way.
      // Instead, we'll just alter the System Menu styles see `disableClose()`
      // We'll leave this code here for clarity & back-reference:
      // if ((wParam & 0xFFF0) == SC_CLOSE) {
      //   // return 0; // makes NOT "closable" ...
      //   std::cout << "SC_CLOSE" << std::endl;
      // }

      // LOG("xWindow::WM_SYSCOMMAND");

      // process SC_MOVE MESSAGE
      if ((wParam & 0xFFF0) == SC_MOVE) {

        // if not moveable
        if (!moveable) {

          // do nothing ...
          return 0;

        } // othewize don't do anything ...
      }

      // previously not handled ...
      // Allow default processing for other system commands
      return DefWindowProc(mhWnd, msg, wParam, lParam);
    }

    // non-common control related commands
    // button clicks, etc ...
    case WM_COMMAND: {
      xWidget* pWidget = xWidgetManager::get().getWidget((HWND)lParam);
      // handle other `WM_COMMAND` messages in derived class `HandleMessage(...)`
      if (pWidget) {
        return pWidget->HandleMessage(msg, wParam, lParam);
      }
      break;
    }

    // common-control-related messages ....
    case WM_NOTIFY: {

      // "Long Pointer Notification Message Draw Routine"
      LPNMHDR lpnmhdr = (LPNMHDR) lParam;

      // get the handle of the button/window
      // from which the (notification) `msg`
      // is sent, extracted from `lpnmhdr`
      // HWND hWidget = lpnmhdr->hwndFrom;
      // get the button pointer from `xWidgetManager`
      // xWidget* pWidget = xWidgetManager::get().getWidget(hWidget);
      
      xWidget* pWidget = xWidgetManager::get().getWidget(lpnmhdr->hwndFrom);

      if (!pWidget) {
        // handle other `WM_NOTIFY` messages in derived class `HandleMessage(...)`
        return DefWindowProc(mhWnd, msg, wParam, lParam);
      }

      return pWidget->HandleMessage(msg, wParam, lParam);

      break;
    } // end of WM_NOTIFY

    // used for changing the background color of an `xFrame`
    // NOT applicable to button controls, i.e. `WC_BUTTON`/"BUTTON"
    // which are handled by Commctrl.h library
    case WM_ERASEBKGND: {

      // LOG("xWindow::WM_ERASEBKGND");

      HDC hDC = (HDC) wParam;
      RECT rect;
      GetClientRect(mhWnd, &rect);
      HBRUSH hBrush = CreateSolidBrush(this->colorBG);
      FillRect(hDC, &rect, hBrush);
      DeleteObject(hBrush);

      return 1; // TRUE => indicates background erase
    }

    // invoked when `xWindow` object is destroyed ...
    // handles cleaning up resources:
    // `pSmallIcon`, `pLargeIcon`, `pFont`
    case WM_DESTROY: {

      // ...

      // more code ...

      // ...

      // invoked when the `xWindow` is destroyed to manage
      // resources & properly free-ing held data ...
      // destroyRecursive(this);
      destroy(); // override method ...

      // check whether the `xWidgetManager` is empty,
      // in which case, post a quit message,
      // since no more `xWidget`s are alive
      LOG(("xWidgetManager::get().count() = " + std::to_string(xWidgetManager::get().count())).c_str());
      // ... 
      if (xWidgetManager::get().count() == 0) {
        // shut down the app ...
        PostQuitMessage(0);
        LOG("Terminating ...");
      }

      break;
    }

    // ...
  }

  return DefWindowProc(mhWnd, msg, wParam, lParam);
}

#endif // end of xWINDOW_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

/// @todo include demonstration/example graphics/images for each of the below ...

/** 
 * @example Size
 * 
 * Instantiate a `xFrame` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Set/get the width of the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setWidth(800); // updates the frame's width
 * std::cout << frame.getWidth() << std::endl; // gets & prints the frame's width
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Set/get the height of the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setHeight(600); // updates the frame's height
 * std::cout << frame.getHeight() << std::endl; // gets & prints the frame's height
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ... or set the size of the frame, which updates both width & height simultaneously ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setSize(800, 600); // updates the frame's width & height simultaneously ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Set the frame's horizontal position ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setHorizontalPosition(250); // sets the frame's horizontal x-position
 * std::cout << frame.getHorizontalPosition() << std::endl; // gets the frame's x-position
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Set the frame's vertical position ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setVerticalPosition(300); // sets the frame's vertical y-position
 * std::cout << frame.getVerticalPosition() << std::endl; // gets the frame's y-position
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ... or set the frame's position, which updates both horizontally & vertically ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setPosition(250, 300); // sets the frame's horizontal & vertical position ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To center the frame relative to the center of the desktop/screen ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.center(); // center's the frame relative to the desktop/screen
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * To center the frame relative to its parent ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.center(&parentFrame); // center's the frame relative to it's parent widget
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @example Attributes
 * 
 * Instantiate a `xFrame` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * manipulate the `xFrame` ...
 * 
 * To pervent the frame from being moved ...
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.disableMove(); // disables the window drag/move feature
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To pervent the frame from being closed ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.disableClose(); // disables the window drag/move feature
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To pervent the frame from being resized ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.disableResize(); // disables the window from being resized
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To pervent the frame from being maximized ...
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.disableMaximize(); // disables the window from being maximized/enlarged
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To pervent the frame from being minimized ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.disableMinimize(); // disables the window from being minimized/shrunk
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To remove the system menu from the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.removeSysMenu(); // removes the System Menu from the window ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To remove the title-bar from the frame entirely ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.removeCaption(); // removes the caption from the window, renders border ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Each of the above methods accepts an optional
 * boolean `true`/`false` parameter, which is useful
 * for explicit/verbose code style & useful to restore
 * previous option/toggle
 * 
 */ 

/**
 * @example state
 * 
 * Instantiate a `xFrame` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * use any of the following to change the frame state ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.show(); // shows the frame graphics
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Make the frame invisible ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.hide(); // hides the frame graphics
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * To restore the original position of the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.restore(); // restores the frame to initial state!
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Alternative interface method to manipulate the frame's visibility ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setVisible(true/false); // shows/hides the frame graphics
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Short-hand method to maximize the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.maximize(); // maximizes/enlarges the frame over the desktop region
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Short-hand method to minimize the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.minimize(); // minimizes/shrinks the frame to a menu-like state
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Alternative method to maximize the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setMiximize(true/false); // similar to the above ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Alternative method to minimize the frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setMinimize(true/false); // similar to the above ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @example Color
 * 
 * Instantiate a `xFrame` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Use `RGB(...)` macro's to change the background color of an `xFrame`
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setBackgroundColor(RGB(123, 213, 231));
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Use constants to change the background color of an `xFrame`
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setBackgroundColor(WIN_95_BG);
 * frame.setBackgroundColor(WIN_2000_BG);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Use Win32 `COLORREF` data type to change the background color of an `xFrame`
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * COLORREF colorRef = RGB(123, 213, 132);
 * frame.setBackgroundColor(colorRef);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @example Modality
 * 
 * Instantiate a `xFrame` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * xFrame child("Child Frame", 100, 200, 300, 400);
 * xFrame sibling("Sibling Frame", 400, 300, 200, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Set the modality for child windows/frames
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * childFrame.isModal(true); // must have a parent, else nothing ...
 * siblingFrame.isModal(true); // multiple modal child frames ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @example Fullscreen
 * 
 * Instantiate a `xFrame` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Enter `fullscreen` state ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.FullScreenMode(true); // toggle true/false
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xFrame.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xFrame` class declaration & implemenation
  * @details  xFrame.h defines the `xFrame` class
  */

#pragma once

/// @brief begin of xFRAME_H implementation
#ifndef xFRAME_H
#define xFRAME_H

#include "./xWindow.h"

/**
 * @class    xFrame
 * @brief    A class for creating `xFrame` objects that are representative of an "Application Window"
 * @details `xFrame` provides a client-code constructor-based interface to create `xFrame` objects <br/>
 *           by provding only the essential parameters such as the title, dimensions & coordinates <br/>
 *           as well as a pointer to a parent widget if the client-code so desires ... <br/>
 * @see     `ClientDemoClass` for an example of how to extend/inherit from this `xFrame` class
 */
class xFrame : public xWindow, public iCloseEventListener {

public:

  /// @brief method to draw `xFrame`s => currently no custom drawing for `xFrames`
  LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) override {
    // ...

    // THis method is triggered on WM_PAINT for frame objects ...

    // LOG("xFrame::CustomDFraw()");

    switch(msg) {

      case WM_PAINT: {
        // std::cout << "xFrame::WM_PAINT" << std::endl;
        PAINTSTRUCT ps;
        HDC hdc  = BeginPaint(mhWnd, &ps);
        RECT rect;
        GetClientRect(mhWnd, &rect);
        HBRUSH hBrush = CreateSolidBrush(this->getBackgroundColor());
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        EndPaint(mhWnd, &ps);
        return 0; // indicates that the window has been painted ...
      }
    }

    // ...
    // Handle custom draw drawing
    // ...

    /// @todo define the logic for custom drawing the non-client area,
    ///       to allow for customizing the title bar & menubar region
    ///       <br/>
    ///       This is generally a complicated task => investigate!
    
    return DefWindowProc(mhWnd, msg, wParam, lParam);
  };

public:

  /// @brief   public default/parameterless constructor
  /// @details provides a client-code constructor interface for
  ///          instantiating `xFrame` objects having no pre-set params
  xFrame()
    : xWindow()
    {
    init(-1, -1);
  }

  /// @brief     public parametrized constructor
  /// @param[in] text ~ the text to be displayed in the `xFrame`s caption
  /// @param[in] w ~ the width of the `xFrame`
  /// @param[in] h ~ the height of the `xFrame`
  /// @param[in] x ~ the hor7izontal position of the `xFrame`
  ///            (default -1 indicates center screen horizontal)
  /// @param[in] y ~ the vertical position of the `xFrame`
  ///            (default -1 indicates center screen vertical)
  /// @details   provides a client-code constructor interface for
  ///            instantiating `xFrame` objects by provinding `xFrame` params
  xFrame(const std::string& text, int w=900, int h=600, int x=-1, int y=-1)
    : xWindow(text, w, h, x, y)
    {
    init(x, y);
  }

  /// @brief     fully-parametrized public constructor
  /// @param[in] parent ~ `xWidget*` pointer to parent widget
  /// @param[in] text ~ the text to be displayed in the `xFrame`s caption
  /// @param[in] w ~ the width of the `xFrame`
  /// @param[in] h ~ the height of the `xFrame`
  /// @param[in] x ~ the horizontal position of the `xFrame`
  /// @param[in] y ~ the vertical position of the `xFrame`
  /// @param[in] subwindow ~ boolean flag to accommodate the need
  ///            for an explicit subwindow (`default false`)
  /// @details   provides a client-code constructor interface for
  ///            instantiating `xFrame` objects by provinding `xFrame` params,
  ///            including a reference pointer to a parent widget
  xFrame(xWidget* parent, const std::string& text, int w=900, int h=600, int x=-1, int y=-1, bool subwindow = false)
    : xWindow(parent, text, w, h, x, y)
    {
    init(x, y);
    // additional toggle for specifying
    // whether explicit (subwindow) child
    // or merely pointer (C++) child
    // default value false
    if (subwindow) {
      appendWindowStyle(WS_CHILD); // if subwindow == true
      isSubWidget = subwindow; // store boolean true
    }
  }

  void init(int x, int y) {
    // appendWindowStyle(WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    appendWindowStyle(WS_OVERLAPPEDWINDOW);
    appendClassStyle(CS_HREDRAW | CS_VREDRAW);
    // to allow for adjusting the alpha/transparency
    // appendExWindowStyle(WS_EX_LAYERED);

    // if x < 0 then default horizontal position center
    if (x < 0) {
      m_x = (Monitor::ScreenWidth() - getWidth())/2;
    }
    // if y < -1 then default vertical position center
    if (y < 0) {
      m_y = (Monitor::ScreenHeight() - getHeight())/2;
    }

    // default background color ...
    if (!colorBG) {
      setBackgroundColor(RGB(32, 140, 113));
    }
  }

protected:

  
  /**
   * @remark
   * The class name is `xLib`-specific, to ensure that each
   * window class receives a unique class name upon registration
   * @note This method MUST NOT be overridden if client-code
   *       extends `xFrame` thus it is marked as `final`
   * @see `ClientDemoClass`
   */
  LPCTSTR ClassName() const final override; // note `final`
  
  /**
   * @remark
   * The type name is Win32-specific, to ensure that each
   * window class receives the correct built-in class type,
   * upon registration, such as `WC_BUTTON`, except for `xFrames`
   * @note This method MUST NOT be overridden if client-code
   *       extends `xFrame` thus it is marked `final`
   * @see `ClientDemoClass`
   */
  LPCTSTR TypeName() const final override; // note `final`

public:

  /**
   * @remark This method is marked `virtual` to allow client-code the opportunity
   *         of overriding for further customizations by extension/inheritance
   * @note   This method *MUST be overridden* if client-code extends `xFrame`
   *         to ensure derived-type specific functionality/behaviour/message-handling
   */
  virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

public:

  /// @brief mechanism to add buttons & other `xComponent`s
  ///        directly to parnet `xWindow`/`xFrame`    <br/>
  ///        Parent to handle logic for `create()`
  ///        on each child, i.e. the invokin `widget`
  /// 
  /// @param[in] widget ~ the widget which to add to the `xFrame`
  /// @note `widget` will NOT `exist`, until it is `create()`-ed
  /// i.e. `xWidget` base to invoke `RegisterWidget(...)`
  /// followed by `create()` on the `widget` instance.
  /// Once the `widget` is `create()`-ed only then
  /// it is added to `this` instance's children
  /// as per the usual runtime logic <br/>
  /// `widget`s having parameterless/default
  /// constructors will assume `this` as its `parent`
  void add(xWidget* widget) override {
    xWidget::add(widget); // invoke base class version
  }

public:

  /*%*%*%*%*%*/
  /* cleanup */
  /*%*%*%*%*%*/

  /// @brief  `~xFrame` is `virtual` by virtue of the fact
  /// that the base class destructor `~xWidget` is virtual
  /*virtual*/ ~xFrame() {
    // nothing to do,
    // since all resources
    // are held in `xWidget` base class
    // LOG("~xFrame()");
  }

public:

  // transparency/alpha-blend & extending non-client area

  /**
   * @see
   * <a href="https://learn.microsoft.com/en-us/previous-versions/ms997507(v=msdn.10)?redirectedfrom=MSDN" target="_blank">
   *  SetLayeredWindowAttributes function
   * </a>
   */

  /// @brief margins for the non-client area
  /// @details Classic borders don't have an obvious non-client area inset/margin
  /// DEFAULT theme 
  /// setting the margins to -1, -1, -1, -1 
  MARGINS margins = {0,0,0,0};

  /// @brief method to set the margins of the non-client area for a frame
  void setMargins(int left, int right, int top, int bottom) {
    margins.cxLeftWidth = left;
    margins.cxRightWidth = right;
    margins.cyTopHeight = top;
    margins.cyBottomHeight = bottom;
    redraw();
  }

  /// @brief method to adjust the left margin of the frame's non-client area's border
  /// @param left ~ integere representative of the left margin
  void setLeftMargin(int left) {
    margins.cxLeftWidth = left;
    redraw();
  }

  /// @brief method to adjust the right margin of the frame's non-client area's border
  /// @param rigth ~ integere representative of the right margin
  void setRightMargin(int right) {
    margins.cxRightWidth = right;
    redraw();
  }

  /// @brief method to adjust the top margin of the frame's non-client area's border
  /// @param top ~ integere representative of the top margin
  void setTopMargin(int top) {
    margins.cyTopHeight = top;
    redraw();
  }

  /// @brief method to adjust the bottom margin of the frame's non-client area's border
  /// @param bottom ~ integere representative of the bottom margin
  void setBottomMargin(int bottom) {
    margins.cyBottomHeight = bottom;
    redraw();
  }

protected:

  /// @brief alpha/transparency of a frame => between 0-255
  BYTE alpha = 255; // completely opaque

  // boolean blur = true; // boolean flag for toggling blur behind
  // DWM_BLURBEHIND bb = { 0, 0, 0, 0 }; // blur-behind effect
  
  /// @brief transparency color/overlay value to apply
  COLORREF transparency = RGB(255,255,255); // default

public:

  // void setBlur(boolean b) {
  //   blur = b;
  //   // bb.dwFlags = DWM_BB_ENABLE;
  //   // bb.fEnable = blur; // true/false
  //   // bb.hRgnBlur = NULL; // apply blur to entire window
  //   DwmEnableBlurBehindWindow(mhWnd, &bb);
  // }

  /// @brief method to set the opacity/transparency/alpha-scaling of a frame
  /// @param a ~ the alpha-value to apply for the opacity
  /// @param key ~ color key for the transparency effect
  void setOpacity(BYTE a = 255, COLORREF key = RGB(255,255,255)) {
    
    alpha = a;
    
    // blur = b;

    // bb.dwFlags = DWM_BB_ENABLE;
    // bb.fEnable = blur; // true/false
    // bb.hRgnBlur = NULL; // apply blur to entire window

    transparency = key;
    
    if (
         margins.cxLeftWidth  != 0
      && margins.cxRightWidth != 0
      && margins.cyTopHeight  != 0
      && margins.cyTopHeight  != 0
      && theme != xTheme::Theme::DEFAULT
    ) {
      margins = {-1, -1, -1, -1};
    }

    redraw();
  }

protected:

  /// @brief helper method to apply the frame's transparency & blur effects ...
  void applyFrameStyles() {

    // DwmEnableBlurBehindWindow(mhWnd, &bb);
    
    try {
      
      HRESULT hrECA = DwmExtendFrameIntoClientArea(mhWnd, &margins);

      if (FAILED(hrECA)) {
        throw RuntimeException("cannot apply margins");
      }

    } catch (RuntimeException &ex) {
      LOG(ex.what());
      std::cerr << ex.what() << std::endl;
    }

    try {

      HRESULT hrLWA = SetLayeredWindowAttributes(
        mhWnd,
        transparency,
        alpha,
        LWA_ALPHA // | (transparency != RGB(255,255,255) ? LWA_COLORKEY : 0)
      );

      if (FAILED(hrLWA)) {
        throw RuntimeException("cannot apply transparency/alpha-coloring");
      }

    } catch(RuntimeException &ex) {
      LOG(ex.what());
      std::cerr << ex.what() << std::endl;
    }
  }

  /// @brief derived type (`xFrame`) protected code-interface
  /// "wrapper" method to `InvalidateRect(...)`/re-paint a window
  virtual void redraw() {    
    if (exists) {
      applyFrameStyles();
      InvalidateRect(mhWnd, NULL, TRUE);
    }
  }

protected:

  /// @brief default close operation enum constant ...
  CloseEvent defaultCloseOperation = CloseEvent::CLOSE; // default ...
  
public:

  /// @brief     method to set the default close operation when closing an `xFrame`
  /// @param[in] e ~ CloseEvent enum constant (default `CloseEvent::CLOSE`)
  void setDefaultCloseOperation(CloseEvent e) {
    defaultCloseOperation = e;
  }

  /// @brief     method to associate a pre-close operation for `an xFrame`
  /// @param[in] callback ~ a function pointer for an action to perform on close
  virtual void setOnClose(event_onClose callback) override {
    mOnClose = callback;
  }

protected:

  /// @brief     helper method to distinguish the different close-operations
  /// @param[in] e ~ CloseEvent enum constant received from `WM_CLOSE` switch case
  LRESULT InvokeCloseOperation(CloseEvent e, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (e == CloseEvent::DO_NOTHING) {
      
      LOG("CloseEvent::DO_NOTHING");
      return 0;

    } else if (e == CloseEvent::DISPOSE) {
      
      LOG("CloseEvent::DISPOSE");
      // isModal(false); // not necessary
      dispose();
      return 0; // return 0 not to close the frame, i.e. don't destroy

    } else if (e == CloseEvent::CLOSE) {

      LOG("CloseEvent::CLOSE");
      // close();
      this->restoreFocus();
      // don't return, use `DefWindowProc(...)`
      return xWindow::HandleMessage(msg, wParam, lParam);

    } else if (e == CloseEvent::TERMINATE) {

      // close the application gracefully ...
      LOG("CloseEvent::TERMINATE");

      /// @remark
      /// The below technique is safer than to get the top-level window
      /// having parent `nullptr`, because client-code may misuse
      /// the `xFrame` constructor, i.e. the client code may 
      /// do something silly for the *top-level* window, like:
      /// 
      ///   `xFrame(&someFrame, ...);`
      /// 
      /// i.e. provide a parent, in which case recursively
      /// looking for top-level window such that it's parent
      /// is `nullptr`, will not necessarily work ...
      std::unordered_map<HWND, xWidget*> widgets = xWidgetManager::get().getWidgets();
      /// so instead, iterating all the widgets
      /// & sending a `WM_DESTROY` message to each is safer ...
      std::unordered_map<HWND, xWidget*>::iterator i;

      // loop over all the widgets to destroy ...
      for (i = widgets.begin(); i != widgets.end(); i++) {
        SendMessage(i->first, WM_DESTROY, 0, 0);
      }
      
      // now safely quit the app ...
      PostQuitMessage(0);
      // don't return anything ...
      // just do `DefWindowProc(...)` below ...
    }

    // after all the above operations done ....
    // return xWindow::HandleMessage(msg, wParam, lParam);
    return DefWindowProc(mhWnd, msg, wParam, lParam);    
  }

public:

  /// @brief method to close the `xFrame`
  /// & clean up the resources associated with it
  void close() {
    SendMessage(mhWnd, WM_CLOSE, 0, TRUE);
  }

  /// @todo  define `dispose()` method
  /// @brief method to dispose of a window/`xWidget` along with all of its children
  virtual void dispose() {
    this->restoreFocus();
    this->hide();
  }

  /*%*%*%*%*/
  /* Menu  */
  /*%*%*%*%*/

protected:

  /// @brief variable to store pointer associated to this frame
  xMenu* pMenuBar = nullptr;

public:

  /// @brief   method to retrieve the pointer of the `xMenu` associated to the `xFrame`
  /// @return `xMenu*` ~ pointer of the menu associated to the frame
  xMenu* getMenu() {
    return pMenuBar;
  }

  /// @brief method to set/attach/associate an `xMenu` to the `xFrame`
  /// @param menu ~ pointer of the menu to attach to the frame
  void setMenu(xMenu* menu) {
    // check if the same menu used or `nullptr`
    /// @todo
    /// Handle exceptions/throw to inform client-code
    /// that the menu pointer is null or points to
    /// the same menu as currently being used ...
    if (menu == pMenuBar || menu == nullptr) { return; }
    // check whether the menu is already in use ...
    /// @todo
    /// Handle exceptions/throw to inform client-code
    /// that the menu is ALREADY in USE!
    if (xMenuManager::checkMenu(menu)) { return; }
    // otherwize, hold on to the previous menu ...
    xMenu* pPrevMenuBar = pMenuBar;
    // set the new menu for the frame ....
    pMenuBar = menu;
    // store the new menu in `xMenuManager` ...
    xMenuManager::storeMenu(pMenuBar);
    // delete the previous menu, if it exists ...
    if (pPrevMenuBar) {
      // remove this from the parent of previous menu ...
      pPrevMenuBar->owner = nullptr;
      xMenuManager::removeMenu(pPrevMenuBar);
    }
    // assign this widget as the owner of the menu ...
    menu->owner = this;
    // set the menu for this frame ...
    SetMenu(mhWnd, pMenuBar->hMenu);
  }

  /// @brief method to detatch/remove a menu from the frame
  void removeMenu() {

    if (pMenuBar) {
      // std::cout << "xFrame::removeMenu()" << std::endl;
      // pMenuBar->owner = nullptr;
      xMenuManager::removeMenu(pMenuBar);
      pMenuBar = nullptr;
    }

    // redraw();
    DrawMenuBar(mhWnd); // FORCE UPDATE
  }

protected:

  /// @brief derived class override method to properly
  /// destroy `xFrame` related resources, specifically
  /// for cleaning up the `xFrame`s `xMenu*`
  virtual void destroy() override {

    // ensure the menu is destroyed
    // & removed from `xMenuManager`
    removeMenu();

    // invoke base class destroy ...
    xWindow::destroy();
  }
};

/// @brief  method retrieving the `xFrame`s class name when requested by `xWidget` base class `RegisterWidget()` method
/// @return returns `LPCTSTR` class name TEXT("xFrame") ~ `ANSI`/`ASCII` & `UNICODE` compatible
LPCTSTR xFrame::ClassName() const {
  return TEXT("xFrame");
};

/// @brief  method retrieving the `xFrame`s type name when requested by `xWidget` base class `create()` method
/// @return returns `LPCTSTR` type name TEXT("xFrame") ~ `ANSI`/`ASCII` & `UNICODE` compatible
LPCTSTR xFrame::TypeName() const {
  return TEXT("xFrame");
};

/// @brief    `xFrame`-specific `HandleMessage` overrided method, to handle `msg`s for `xFrame` objects
/// @details   This method implements the message dispatch handler for the `xFrame` type, i.e. `xFrame` `msg`s are handled here!
/// @param[in] msg ~ `uint` message value received from `WndProc` redirected/filtered according to `mhWnd`
/// @param[in] wParam ~ a number the size of a pointer (meaning depends on `msg`)
/// @param[in] lParam ~ a number the size of a pointer (meaning depends on `msg`)
LRESULT xFrame::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {

  /// @note* it is already "known" at this point that the window exists!!!
  switch(msg) {

    case WM_KEYDOWN: {
      // std::cout << "WM_KEYDOWN" << std::endl;      
      // Key key = static_cast<Key> (wParam);
      Key key(static_cast<UINT>(wParam));
      if (mOnKeyPress) {
        mOnKeyPress(this, key);
      }
      break;
    }

    case WM_KEYUP: {
      // std::cout << "WM_KEYUP" << std::endl;
      // Key key = static_cast<Key> (wParam);
      Key key(static_cast<UINT>(wParam));
      if (mOnKeyRelease) {
        mOnKeyRelease(this, key);
      }
      break;
    }

    case WM_LBUTTONUP: {
      // std::cout << "xFrame::WM_LBUTTONUP" << std::endl;
      // this->mOnLeftButtonRelease();
      if (mOnMouseRelease != nullptr) {
        mOnMouseRelease(this, MouseKey::LeftButton);
      }
      break;
    }
    case WM_RBUTTONUP: {
      // std::cout << "xFrame::WM_RBUTTONUP" << std::endl;
      // this->mOnRightButtonRelease();
      if (mOnMouseRelease != nullptr) {
        mOnMouseRelease(this, MouseKey::RightButton);
      }
      break;
    }
    case WM_LBUTTONDOWN: {
      // std::cout << "xFrame::WM_LBUTTONDOWN" << std::endl;
      // this->mOnLeftClick();
      if (mOnMouseClick != nullptr) {
        mOnMouseClick(this, MouseKey::LeftButton);
      }
      break;
    }
    case WM_RBUTTONDOWN: {
      // std::cout << "xFrame::WM_RBUTTONDOWN" << std::endl;
      // this->mOnRightClick();
      if (mOnMouseClick != nullptr) {
        mOnMouseClick(this, MouseKey::RightButton);
      }
      break;
    }
    case WM_LBUTTONDBLCLK: {
      // std::cout << "xFrame::WM_LBUTTONDBLCLK" << std::endl;
      // this->mOnLefDoubletClick();
      if (mOnMouseDoubleClick != nullptr) {
        mOnMouseDoubleClick(this, MouseKey::LeftButton);
      }
      break;
    }
    case WM_RBUTTONDBLCLK: {
      // std::cout << "xFrame::WM_RBUTTONDBLCLK" << std::endl;
      // this->mOnRightDoubleClick();
      if (mOnMouseDoubleClick != nullptr) {
        mOnMouseDoubleClick(this, MouseKey::RightButton);
      }
      break;
    }

    case WM_MOUSEMOVE: {
      if (mOnMouseHover) {
        mOnMouseHover(this, MouseEvent::ENTER);
      }
      break;
    }

    case WM_MOUSELEAVE: {
      if (mOnMouseHover) {
        mOnMouseHover(this, MouseEvent::EXIT);
      }
      break;
    }

    // redirect to `xWindow` parent type
    case WM_NOTIFY: {

      /// @note
      /// `WC_BUTTON`s that have the `BS_NOTIFY` attribute
      ///  redirect notification `WM_NOTIFY` messages
      ///  to parent windows, meaning that if a button
      ///  is clicked, the message is NOT processed in
      ///  `xButton`s `HandleMessage(...)` function,
      ///  instead, it is processed here
      ///  in `xFrame` or in `xWindow`

      ///  In this case, we redirect the message to `xWindow`
      return xWindow::HandleMessage(msg, wParam, lParam);
    }

    case WM_PAINT: {
      // invoke `CustomDraw(...)` method on `xFrame` object
      return this->CustomDraw(msg, wParam, lParam);
    }

    // redirect to `xWindow` parent type
    case WM_ERASEBKGND: {
      return xWindow::HandleMessage(msg, wParam, lParam);
    }

    case WM_DRAWITEM: {
      return xWindow::HandleMessage(msg, wParam, lParam);
    }

    case WM_CTLCOLORSTATIC: {
      
      // std::cout << "WM_CTLCOLOREDIT" << std::endl;
      xWidget* pWidget = xWidgetManager::get().getWidget((HWND)lParam);

      if (pWidget) {
        pWidget->HandleMessage(msg, wParam, lParam);
      }

      break;
    }

    case WM_CTLCOLOREDIT: {
      
      // std::cout << "WM_CTLCOLOREDIT" << std::endl;
      xWidget* pWidget = xWidgetManager::get().getWidget((HWND)lParam);

      if (pWidget) {
        pWidget->HandleMessage(msg, wParam, lParam);
      }

      break;
    }

    case WM_CTLCOLORBTN: {
      // std::cout << "WM_CTLCOLORBTN" << std::endl;

      // std::cout << "WM_CTLCOLOREDIT" << std::endl;
      xWidget* pWidget = xWidgetManager::get().getWidget((HWND)lParam);

      if (pWidget) {
        pWidget->HandleMessage(msg, wParam, lParam);
      }

      break;
    }

    // redirect to xWindow parent type
    case WM_SYSCOMMAND: {
      // LOG("xFrame::WM_ERASEBKGND");
      return xWindow::HandleMessage(msg, wParam, lParam);
    }

    /// @todo
    /// define a mechanism that ensures the menu is no hidden/closed 
    /// whenever a checkbox item or radio item is clicked!
    case WM_CONTEXTMENU: {
      // std::cout << "WM_CONTEXTMENU" << std::endl;
      // get the coordinates of the cursor, to track the popup menu ...
      POINT p = Mouse::PointFromMouseCoordinates(lParam);
      // get the pointer of the widget that is closest to the cursor ...
      xWidget* pWidget = Mouse::WidgetRelativeToPoint(p);
      // this check ensures the context menu
      // does not pop up when clicking a button
      // or any other xControl object ...
      if (pWidget == this) {        
        // check whether widget has a context menu associated to it ...
        // only then, track the popup/context menu, if any ...
        if (pContextMenu) {
          // this sends a `WM_COMMAND` message to the parent of the popup menu,
          // thus, handle the `WM_COMMAND` message received as any other menu item ID
          // std::cout << "TrackPopupMenu(...)" << std::endl;
          TrackPopupMenu(pContextMenu->hMenu, 0, p.x, p.y, 0, mhWnd, NULL);
        }
      }
      // else {
      //   return pWidget->HandleMessage(msg, wParam, lParam);
      // }
      // default processing ...
      return DefWindowProc(mhWnd, msg, wParam, lParam);
    }

    case WM_COMMAND: {
      // std::cout << "WM_COMMAND" << std::endl;
      // check if a menu/context menu item is being interacted with ...
      // i.e. `HIWORD` of `wParam` is 0 & `lParam` itself is also 0
      if (HIWORD(wParam) == 0 && lParam == 0) {
        // `LOWORD` of `wParam` carries the menu item ID
        int itemID = LOWORD(wParam);
        // std::cout << "menuID: " << menuID << std::endl;
        xItemManager::triggerMenuItemClickEvent(itemID, this);
        return DefWindowProc(mhWnd, msg, wParam, lParam);
      } // otherwize, if not menu interaction, then trampoline to `xWindow` ...
      return xWindow::HandleMessage(msg, wParam, lParam);
    }

    case WM_CLOSE: {
      // do client-defined operations before invoking `close()`
      if (mOnClose) {
        mOnClose(this);
      }
      // close the frame according to the `defaultCloseOperation` enum constant ...
      return InvokeCloseOperation(defaultCloseOperation, msg, wParam, lParam);
    }

    case WM_DESTROY: {
      return xWindow::HandleMessage(msg, wParam, lParam);
    }

    // ... other more interesting msg's to handle, if any ...
    // ... REASON: keep code neat/readable => BIG REASON!
  }

  // handle unprocessed messages ...
  return DefWindowProc(mhWnd, msg, wParam, lParam);
};

// implementation after xControl & xFrame ...
bool xWidget::create() {

  // check widget already exists
  // avoids abuse of usage like
  // multiple invokations of `create()`
  if (exists) {
    return true; // return early ...
  }

  // if widget has parent, but parent not created ...
  try {

    // check if for isSubWidget => MUST HAVE PARENT!!!
    if (isSubWidget && !parent) {
      std::string message = "A subwidget must have a parent!";
      throw RuntimeException(message);
    }

    // prevent a widget from being created
    // if it's parent does not yet exist ...
    if (parent && !parent->Handle()) {
      
      std::string message = "xWidget parent does not exist!";
      throw RuntimeException(message);
    }

  } catch(RuntimeException &ex) {

    LOG(ex.what());
    std::cerr << ex.what() << std::endl;

    exit(-1); // abort/terminate
  }

  // register window class before creating ...
  RegisterWidget();

  // after registration, create window/widget
  try {

    mhWnd = CreateWindowEx(
      mExWindowStyle,  // extra styles, like `WM_EX_XXX`
      TypeName(),  // type name received from overridden derived-class-specific pure virtual abstract method
      mText.c_str(),  // convert the `mText` to c-style string (regardless of `ANSI`/`ASCII` or `UNICODE`)
      mWindowStyle,
      // Depending on the "type" of window being created => `appendWindowStyle(...)`
      // usually (`WS_CLIPSIBLINGS | WS_CLIPCHILDREN`) Win32 MACRO constants
      m_x, m_y, m_w, m_h, // dimensions & coordinates in `int`
      // EVERY child window MUST have a parent => crash if NOT!
      ((parent) ? parent->Handle() : NULL), // handle to parent window (if any)
      ((parent) ? ( isSubWidget ? (HMENU) parent->children.size() : NULL ) : NULL),
      /// @note HMENU handle of menu (if any) provided as `int` based
      /// on children count (if any) to ensure *unique* ID      <br/>
      /// DO NOT provide HMENU handle for "top-level" windows until the menu resource is created ...
      xApp::get().hGlobalInstance,  // module handle => invoking .exe script file path ...
      this  // ANY pointer => `xLib` uses a pointer to `this` instance
    );

    // check if `mhWnd` is `NULL`, i.e. whether `CreateWindowEx` fails
    if (!mhWnd) {

      // get last error/crash code ...
      int error = GetLastError();

      std::string translate = xError::errorTranslate(error); // translate ...
      std::string message = "xWidget registration failed: " + translate;
      
      throw RuntimeException(message);
    }

  } catch(RuntimeException &ex) {

    // log error message ...
    LOG(ex.what());
    // report exception ...
    std::cerr << ex.what() << std::endl;
    
    exit(-1); // abort/terminate
  } // otherwize widget successfully registered ...

  // mark widget as `exsits`
  // set exists = false, when destroying (optional)
  exists = true;

  // if widget is a child of another widget,
  // then add it to its parent's children list `children`
  if (parent) {
    // set *unique* `HMENU` handle based on next index in `children` container
    mID = (HMENU) parent->children.size();
    parent->setChild(this); // add widget to list of children ...
  }

  // add widget to the list of existing widgets held in resources by `xWidgetManager`
  xWidgetManager::get().storeWidget(mhWnd, this);
  LOG("xWidget created successfully!");

  // show/hide widget based on `mState`
  ShowWindow(mhWnd, visible ? SW_SHOW : SW_HIDE);
  
  // post-creation stage ...

  // update font if user set font before create ...
  // first check if `pFont*` exists ...
  if (pFont) {
    updateFont();
  }

  // apply the theme to the widget upon creation
  if (xTheme::globalThemeSelected) {
    SetWidgetTheme(xTheme::globalTheme);
  } else {
    SetWidgetTheme(this->theme);
  }

  /// move this to `xFrame`/`xWindow`
  if (dynamic_cast<xFrame*>(this)) {
    appendExWindowStyle(WS_EX_LAYERED);
  }

  // if the widget is a button or textbox or
  // any derived type of xControl, then subclass ...
  // Subclass the button to handle right-clicks ...
  if (dynamic_cast<xControl*>(this)) {
    SetWindowSubclass(mhWnd, xControl::ControlSubclassProc, 1, 0);
  }

  /// some controls may require default event handlers
  initDefaultHandlers();

  // always return `true` or abort/terminate
  return true;
}

#endif // end of xFRAME_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

/// @todo include demonstration/example graphics/images for each of the below ...

/**
 * @example HandleMessage
 * 
 * If client-code wishes to extend/inherit from this class,
 * but (for some reason) decides NOT to implement message-handling
 * for their derived class, then the technique for overriding `HandleMessage(...)`
 * can be redirected to it's parent class, i.e. `xFrame` like so ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {
 *    return xFrame::HandleMessage(msg, wParam, lParam);
 * }; // end of `HandleMessage` `override` demo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @see `ClienDemoClass`
 */
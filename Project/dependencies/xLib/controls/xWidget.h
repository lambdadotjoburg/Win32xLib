/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xWidget.h
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 		contains `xWidget` class declaration & implemenation
  * @details 	xWidget.h defines the `xWidget` base class
  *          `xWindow` & `xControl` to inherit from `xWidget` base class
  */

#pragma once

/// @brief begin of xWIDGET_H implementation
#ifndef xWIDGET_H
#define xWIDGET_H

// class xWindow;
class xFrame;
class xControl;
// class xButton;
// class xTextBox;

/**
 * @class    xWidget
 * @brief   `xWidget` is a pure abstract base class for ALL `xWidget` derived classes/types
 * 
 * @details `xWidget` is implemented to enfore the `xLib` type hierarchy
 *           & provides the public interface for the main message dispatch
 *           window procedure `WndProc(...)` which re-directs events
 *           to ALL relevant `xWidget` according to their handles `hWnd`
 * 
 * @implements iMouseEventListener
 * @implements iFocusListener
 */
class xWidget :
  public iMouseEventListener,
  public iFocusEventListener,
  public iKeyEventListener
{

  /// @brief allow friend-level access to `xBorderFactory`
  ///        for manipulating the widget's border data
  friend class xBorderFactory;
  
  // provide friend-level access to xControl for accessing xWidget data members ...
  friend class xControl;

// events ...

public:

  /// @brief method to attach a mouse-hover callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnMouseHover(event_onMouseHover callback) override {
    mOnMouseHover = std::move(callback);
  };

  /// @brief method to attach a mouse-click callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnMouseClick(event_onMouseClick callback) override {
    mOnMouseClick = std::move(callback);
  };

  /// @brief method to attach a mouse-release callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnMouseRelease(event_onMouseRelease callback) override {
    mOnMouseRelease = std::move(callback);
  }

  /// @brief method to attach a mouse double click callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnMouseDoubleClick(event_onMouseDoubleClick callback) override {
    mOnMouseDoubleClick = std::move(callback);
  };

  /// @brief method to attach a focus-gained callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnFocus(event_onFocus callback) {
    mOnFocus = std::move(callback);
  }

  /// @brief method to attach a key-press callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnKeyPress(event_onKeyPress callback) {
    mOnKeyPress = std::move(callback);
  }

  /// @brief method to attach a key-release callback function to an `xWidget`
  /// @param callback ~ function to execute once the event is triggered
  virtual void setOnKeyRelease(event_onKeyRelease callback) {
    mOnKeyRelease = std::move(callback);
  }

public:

  /**
   * @brief     static/shared main Window Procedure (`WndProc`)
   * @param[in] hWnd ~ handle of the widget/window receiving the `msg`
   * @param[in] msg ~ `uint` message ID to process
   * @param[in] wParam ~ message data (interpretation depends on the `msg`)
   * @param[in] lParam ~ message data (interpretation depends on the `msg`)
   * @details   Main Window Callback Procedure through which ALL
   *            messages are dispatched/re-directed to the relevant
   *            widget/window for processing by `hWnd`
   * @return   `LRESULT` ~ integer value returned to OS (program response to particular `msg`)
   */
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    // check if widget exists using `xWidgetManager` map of `hWnd`
    // since widget/window may still be processing `WM_NCCREATE`
    if (xWidgetManager::get().exists(hWnd)) {

      // translate messages in debug mode ...
      #define NDEBUG 1 // to suppress message translation pump ...
      #ifndef NDEBUG
      xTRANSLATE(msg); // translate `UINT`
      #endif
      #undef NDEBUG

      // re-direct message to appropriate window handle `hWnd` for further processing ...
      xWidget* pWidget = xWidgetManager::get().getWidget(hWnd);
      if (pWidget) {
        return pWidget->HandleMessage(msg, wParam, lParam);
      }
    }

    // return default window procedure ...
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }

public:

  /*%*%*%*%*/
  /* Color */
  /*%*%*%*%*/

  #define WIN_95_BG RGB(32, 140, 113)
  #define WIN_2000_BG RGB(56, 108, 160)

  // assign default colors in derived classes if NULL

  /// @brief default background color => NULL
  COLORREF colorBG = (COLORREF) NULL;
  /// @brief default foreground/text color => NULL
  COLORREF colorFG = (COLORREF) NULL;

  // more default data to be defined here ...
  // specifically default borders ...

public:

  // BYTE => unsigned char => range between 0 & 255
  /// @brief     method to change/update/set `xWidget`s background color
  /// @details   pure virtual abstract method to allow
  ///            for derived-class-specific functionality ...
  /// @param[in] R ~ Red   => derived class to provide default
  /// @param[in] G ~ Green => derived class to provide default
  /// @param[in] B ~ Blue  => derived class to provide default
  virtual void setBackgroundColor(BYTE R, BYTE G, BYTE B) {
    colorBG = RGB(R, G, B);
    if (exists) {
      redraw();
    }
  }

  // BYTE => unsigned char => range between 0 & 255
  /// @brief     method to change/update/set `xWidget`s text color
  /// @details   pure virtual abstract method to allow
  ///            for derived-class-specific functionality ...
  /// @param[in] R ~ Red   => derived class to provide default
  /// @param[in] G ~ Green => derived class to provide default
  /// @param[in] B ~ Blue  => derived class to provide default
  virtual void setTextColor(BYTE R, BYTE G, BYTE B) {
    colorFG = RGB(R, G, B);
    if (exists) {
      redraw();
    }
  }

  /// @brief overload method for `setBackgroundColor()` accepting hexadecimal like `0xF18EA32` or similar
  virtual void setBackgroundColor(COLORREF colorRef) {
    colorBG = colorRef;
    if (exists) {
      redraw();
    }
  }

  /// @brief overload method for `setTextColor()` accepting hexadecimal like `0xF18EA32` or similar
  virtual void setTextColor(COLORREF colorRef) {
    colorFG = colorRef;
    if (exists) {
      redraw();
    }
  }

public:

  /// @brief  method to retrieve the background color of an `xWidget`
  /// @return `COLORREF` hexadecimal value representative of the `xWidget`s background color
  COLORREF getBackgroundColor() {
    return colorBG;
  }

  /// @brief  method to retrieve the text color of an `xWidget`
  /// @return `COLORREF` hexadecimal value representative of the `xWidget`s text color
  COLORREF getTextColor() {
    return colorFG;
  }

protected:

  /// @brief variable to store whether a widget is in a hover state
  bool isHovering = false;

public:

  /// @brief  method to retrieve `isHovering` state of a widget
  /// @return boolean flag representative of whether the widget is in a hover-state or NOT
  bool getHoverState() {
    return isHovering;
  }

protected:

  /// @brief   default Hover Background Color (light blue)
  /// @details Each `xWidget` derived-type may require
  ///          it's own hover background color
  COLORREF hoverBG = (COLORREF) NULL;

public:
  
  /// @brief   Method to change/update/set the
  /// xButton control's hover background color
  /// accepting `COLORREF` color value ...
  /// @details
  /// The hover color mechanism is distinct from the `mOnMouseHover`
  /// event handler, to allow for more client flexibility, i.e.
  /// not interfering with the drawing/color effects...
  void setHoverColor(COLORREF colorRef) {
    hoverBG = colorRef;
  }

  /// @brief   Overload method to change/update/set
  /// the `xWidget`s hover background color,
  /// accepting R, G, B `BYTE` values ...
  /// @details This interface method is provided,
  /// so as to provide client-flexibility, i.e.
  /// so that client-code is not forced/required to define
  /// the logic for hover-effects in the client-defined
  /// `mOnMouseHover` callback function, but to use the
  /// pre-defined hover-effect interface API for cleaner code.
  void setHoverColor(BYTE R, BYTE G, BYTE B) {
    setHoverColor(RGB(R, G, B));
  }

  /// @brief  Method to retrieve the `xWidget`s hover color
  /// @return `COLORREF` representative of the `xWidget`s hover-state background color
  COLORREF getHoverColor() {
    return hoverBG;
  }

protected:

  /// @brief variable storing a button's text alignment
  DWORD textalignment = DT_CENTER | DT_VCENTER | DT_SINGLELINE; // default

public:

  // these don't work for static control & edit control ...

  /**
   * @name  Text Alignment Constants
   * @brief macro constants for client usability to select button's text alignment
   * @todo  wrap these constants in a TextAlignment enum
   * @{ */
  #define LEFT DT_LEFT
  #define RIGHT DT_RIGHT
  #define TOP DT_TOP
  #define BOTTOM DT_BOTTOM
  #define CENTER DT_CENTER
  #define VERTICAL DT_VCENTER
  #define SINGLELINE DT_SINGLELINE
  /** @} */

protected:

  /// @brief
  /// variable to store the text alignment of a widget  <br/>
  /// default is `CENTER | VERTICAL | SINGLELINE`       <br/>
  /// This may be different for each derived-type       <br/>
  DWORD textAlignment = CENTER | VERTICAL | SINGLELINE;

public:

  /// @brief     method to set/update/change a button's text alignment
  /// @param[in] a ~ `DWORD` representative of the button's text alignment
  virtual void setTextAlignment(DWORD a) {
    textalignment = a;
  }

  /// @brief method to retrieve a button's text alignment
  DWORD getTextAlignment() {
    return textalignment;
  }

public:

  /// @brief draw context handle member
  HDC mhDC = nullptr;

  /// @brief method to retrieve the reference to the draw context's handle
  HDC& getDrawContext() {
    return mhDC;
  }

  /// @brief method to persist the draw context handle for an `xWidget` control
  /// @param hDC ~ handle of the draw context to store
  void setDrawContext(HDC hDC) {
    mhDC = hDC; // don't delete hDC, since created by another process ...
  }

  /// @brief draw context rectangle member
  RECT rc = {0,0,0,0}; // initialize rc RECT

  /// @brief method to persiste a draw context rectangle in the object's memory
  /// @param rect ~ the draw context rectangle to stored for later retrieval
  void setDrawRect(RECT rect) {
    rc = rect;
    // rc.left = rect.left;
    // rc.top = rect.top;
    // rc.right = rect.right;
    // rc.bottom = rect.bottom;
  }

  /// @brief  method to retrieve a ref of the draw context rectangle
  /// @return reference of the draw context rectangle `xWidget` control's class member
  RECT& getDrawRect() {
    return rc;
  }

protected:

  /// @brief variable holding the focus state of a control
  bool mHasFocus = false;

public:

  /// @brief     method to set the control as having focus
  /// @param[in] flag ~ boolean flag indicating the focus state of the control
  void setFocus(bool flag) {
    mHasFocus = flag;
    // InvalidateRect(mhWnd, NULL, TRUE);
  }

  /// @brief  method to retrieve the focus state of the control
  /// @return boolean flag representative of the control's focus state
  bool getFocus() {
    return mHasFocus;
  }

public:

  /// @brief helper method for toggling click state for a widget control
  /// @param pWidget ~ pointer of the widget to draw
  void toggleClickState(xWidget* pWidget, bool state);

protected:

  /// @brief variable holding the click/release state of a control
  bool mClickState = false;

public:

  /// @brief     method to set the control as having click/release state
  /// @param[in] flag ~ boolean flag indicating the click/release state of the control
  void setClickState(bool flag) {
    mClickState = flag;
    // InvalidateRect(mhWnd, NULL, TRUE);
  }

  /// @brief  method to retrieve the click state of the control
  /// @return boolean flag representative of the control's click state
  bool getClickState() {
    return mClickState;
  }

public:

  /// @brief pure virtual abstract method for drawing `xWidget`s
  /// @param msg, wParam, lParam ~ these are the same parameters as received by `HandleMessage(...)`
  virtual LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

  /// @brief virtual method to encapsulate the drawing stages of a widget
  /// @param widget ~ pointer of the widget to draw
  virtual void draw(xWidget* widget) { UNUSED(widget); } /*=0*/;

  /// @brief method to fill the "leaking"/"spilled" rectangle regions
  /// when drawing a custom border, specifically a border having rounded corners
  virtual void camouflage(xWidget* widget) { UNUSED(widget); } /*=0*/;

  /// @brief method to write the text of the control,
  /// once all the prior drawing stages have been completed,
  /// i.e. only write the text in the control AFTER
  /// fill has been applied to avoid painting over the text
  virtual void write(xWidget* widget) { UNUSED(widget); } /*=0*/;

protected:

  /// remove `pWidgetBorder` from `xBorderFactory` when destroying this `xWidget`
  /// @brief variable to keep track of the border associated to the `xWidget`
  xBorder* pWidgetBorder = nullptr; // nullptr

public:

  /// @brief  method to retrieve the widget/controls custom border
  /// @return pointer to the `xBorder` object associated to the control
  xBorder* getWidgetBorder() {
    return pWidgetBorder;
  }

  /// @brief   interface method to assign an `xBorder` object to an `xWidget` control
  /// @param   border ~ pointer to the `xBorder` object holding the parameters of the border
  /// @details This method interfaces with the `xBorderFactory` class to ensure that `xBorder`
  /// memory resources are managed correctly throughout the app instance life cycle,
  /// by maintaining control over the borders used by existing controls.
  /// @note
  /// check whether the given border is already contained in `xWidgetManager::borders`  <br/>
  /// if contained, then increment the `xBorder`s ref count ...                   <br/>
  /// if not, create & increment it's count by 1                                  <br/>
  /// if count of old border drops to 0, delete it from `xBorderFactory::borders` <br/>
  void setWidgetBorder(xBorder* border) {

    // setting/storing a `new xBorder` is handled by
    // `xWidgetManager`s internal mechanisms to take care
    // of reference count & actually storing
    // the element (if need be)
    xBorderFactory::get().storeBorder(border);

    // store local reference to the previous border ...
    xBorder* pPrevBorder = pWidgetBorder;

    // persist to memory, the new border ...
    // DO NOT free/delete the pointer ...
    // the resource management is done by `xBorderFactory`
    pWidgetBorder = border;
    
    // remove `pPrevBorder` from `xBorderFactory::borders`
    // `xBorderFactory`s internal mechanisms to take care
    // of reference count & actually removing
    // the element (if need be)
    xBorderFactory::get().removeBorder(pPrevBorder);
  }

protected:

  /// remove `pFocusBorder` from `xBorderFactory` when destroying this `xWidget`
  /// @brief variable to keep track of the focus border associated to the `xWidget`
  xBorder* pFocusBorder = nullptr; // nullptr

public:

  /// @brief  method to retrieve the widget/controls custom focus border
  /// @return pointer to the `xBorder` object associated to the control
  xBorder* getFocusBorder() {
    return pFocusBorder;
  }

  /// @brief   interface method to assign a focus `xBorder` object to an `xButton` control
  /// @param   border ~ pointer to the `xBorder` object holding the parameters of the focus border
  /// @details This method interfaces with the `xBorderFactory` class to ensure that `xBorder`
  /// memory resources are managed correctly throughout the app instance life cycle,
  /// by maintaining control over the borders used by existing controls.
  /// @note
  /// check whether the given border is already contained in `xWidgetManager::borders`  <br/>
  /// if contained, then increment the `xBorder`s ref count ...                         <br/>
  /// if not, create & increment it's count by 1                                        <br/>
  /// if count of old border drops to 0, delete it from `xBorderFactory::borders`       <br/>
  void setFocusBorder(xBorder* border) {

    // setting/storing a `new xBorder` is handled by
    // `xWidgetManager`s internal mechanisms to take care
    // of reference count & actually storing
    // the element (if need be)
    xBorderFactory::get().storeBorder(border);

    // store local reference to the previous border ...
    xBorder* pPrevBorder = pFocusBorder;

    // persist to memory, the new border ...
    // DO NOT free/delete the pointer ...
    // the resource management is done by `xBorderFactory`
    pFocusBorder = border;
    
    // remove `pPrevBorder` from `xBorderFactory::borders`
    // `xBorderFactory`s internal mechanisms to take care
    // of reference count & actually removing
    // the element (if need be)
    xBorderFactory::get().removeBorder(pPrevBorder);
  }

protected:

  /// @brief   `xWidget` parameterless/default base class constructor
  /// @details  cannot instantiate an `xWidget` abstract base class type object
  xWidget() {
    // ...
  }

  /// @brief    `xWidget` parameterized base class constructor
  /// @details   cannot instantiate an `xWidget` abstract base class type object
  /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
  /// @param[in] w ~ the width of the `xWidget`
  /// @param[in] h ~ the height of the `xWidget`
  /// @param[in] x ~ the horizontal position of the `xWidget`
  /// @param[in] y ~ the vertical position of the `xWidget`
  xWidget(const std::string& text, int w, int h, int x, int y)
    : m_w{w}, m_h{h}, m_x{x}, m_y{y}
    {
    setText(text);
  }

  /// @brief    `xWidget` parameterized base class constructor accepting `xWidget*` parent pointer
  /// @details   cannot instantiate an `xWidget` abstract base class type object
  /// @param[in] parent ~ `xWidget*` pointer to parent widget
  /// @param[in] text ~ the text to be displayed in the `xWidget` (`xButton`/`xFrame`)
  /// @param[in] w ~ the width of the `xWidget`
  /// @param[in] h ~ the height of the `xWidget`
  /// @param[in] x ~ the horizontal position of the `xWidget`
  /// @param[in] y ~ the vertical position of the `xWidget`
  xWidget(xWidget* parent, const std::string& text, int w, int h, int x, int y)
    : parent{parent}, m_w{w}, m_h{h}, m_x{x}, m_y{y}
    {
    setText(text);
  }

protected:

  /// @brief window class struct holding window
  /// data of registered window C-style object
  WNDCLASSEX wx = { };

  /// @brief handle of the window registered by the `xWidget`
  HWND mhWnd = NULL;

  /// @brief `xWidget*` parent pointer
  xWidget* parent = nullptr;

  /// @brief string storing the class name of the `xWidget`
  std::string mClassName;

public:

  /// @brief  method to retrieve `mClassName` member variable
  /// @return `std::string` copy representative of `mClassName`
  std::string getClassName() {
    return mClassName;
  }

protected:

  /// @brief string storing the text of a widget
  /// @details
  /// In the context of `xFrame` this stores the frame title  <br/>
  /// In the context of `xButton` this stores the button text <br/>
  #if defined(UNICODE) && defined(_UNICODE)
  std::wstring mText;
  #else
  std::string mText;
  #endif

  /// @brief coordinates & dimensions of the `xWidget`
  int m_w, m_h, m_x, m_y;

protected:

  /// @brief styles to be used when registering the `xWidget`/window 
  /// @details
  /// The main styles associated to each `xWidget`/window class         <br/>
  /// These styles may be altered during program runtime, in which case <br/>
  /// redraw-ing/render-ing of the widget object may be required ...    <br/>
  /// @note
  /// Some window-specific styles include:                   <br/>
  /// `WS_OVERLAPPED`, `WS_OVERLAPPEDWINDOW`, `WC_BUTTON`    <br/>
  /// but the shared/default attributes are as indicated ... <br/>
  /**
   * @see
   *  <a href="https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles" target="_blank">
   *    Extended Window Styles
   *  </a>
   */
  DWORD mWindowStyle = WS_VISIBLE;

protected:

  /// @brief variable for keeping track of class styles.
  /// @details
  /// Mainly used for registering `xWidget`s for detecting double-clicks.
  /// To ensure a window class can detect `WM_LBUTTONDBLCLK` & `WM_RBUTTONDBLCLK`
  /// messages, it is necessary to provide the `CS_DBLCLKS` attribute upon registration ...
  /**
   * @see
   *  <a href="https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles" target="_blank">
   *    Extended Window Styles
   *  </a>
   * also @see
   *  <a href="https://learn.microsoft.com/en-us/windows/win32/dwm/dwm-overview" target="_blank">
   *    Desktop Window Manager
   *  </a>
   * 
   */
  DWORD mClassStyle = CS_DBLCLKS;

protected:

  /**
   * @brief  These are extended windows styles
   * @details
   * Mainly used for enabling alpha-transparency & extending non-client-area margins into the client-area => border margins!
   * @see
   * <a href="https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles" target="_blank">
   *  Extended Window Styles
   * </a>
   */
  DWORD mExWindowStyle = (DWORD)NULL;

protected:

  /// @brief   Handle to the menu associated with the window/`xWidget`
  /// @details `mID` is generated based on the number of children
  /// of its parent widget, like `parent->children.size();` <br/>
  /// This technique is helpful when assigning menu items & button ID's
  /// @todo provide/implement interface method to set a new menu for `xFrame`s
  HMENU mID = NULL; // unique ID based on parent's children count

  /// @brief cursor to be associated to the `xWidget`,
  /// usually associated to on-hover/mouse-enter/mouse-leave events
  /// @todo provide/implement interface method to change cursor
  HCURSOR mCursor = NULL; 

  /// @brief   boolean flag whether an `xWidget` has been registered/exists
  /// @details
  /// Used internally for private interface members & `xWidgetManager` class
  /// to determine whether or not a certain internal action may/should be performed
  bool exists = false; // initialize this to false

public:

  /// @brief     method to append window attributes/styles to an existing `xWidget`
  /// @param[in] flags ~ list of flags to append in the form `WS_XXX | WS_YYY`
  void appendWindowStyle(DWORD flags) {
    mWindowStyle |= flags; // append to mStyle class member
    SetWindowLongPtr(mhWnd, GWL_STYLE, mWindowStyle); // update window style
  }

  /// @brief     method to remove window attributes/styles from an existing `xWidget`
  /// @param[in] flags ~ list of flags to remove in the form `WS_XXX | WS_YYY`
  void removeWindowStyle(DWORD flags) {
    mWindowStyle &= ~flags; // remove from mStyle class member
    SetWindowLongPtr(mhWnd, GWL_STYLE, mWindowStyle); // update window style
  }

public:

  /// @brief     method to append class attributes/styles to an existing `xWidget`
  /// @param[in] flags ~ list of flags to append in the form `CS_XXX | CS_YYY`
  void appendClassStyle(DWORD flags) {
    mClassStyle |= flags; // append to `mClassStyle` class member
    SetWindowLongPtr(mhWnd, GCL_STYLE, mClassStyle); // update class style
  }

  /// @brief     method to remove class attributes/styles from an existing `xWidget`
  /// @param[in] flags ~ list of flags to remove in the form `CS_XXX | CS_YYY`
  void removeClassStyle(DWORD flags) {
    mClassStyle &= ~flags; // remove from `mClassStyle` class member
    SetWindowLongPtr(mhWnd, GCL_STYLE, mClassStyle); // update class style
  }

public:

  /// @brief     method to append extra window attributes/styles to an existing `xWidget`
  /// @param[in] flags ~ list of flags to append in the form `WS_EXXXX | WS_EXYYY`
  void appendExWindowStyle(DWORD flags) {
    mExWindowStyle |= flags; // append to `mExStyle` class member
    SetWindowLongPtr(mhWnd, GWL_EXSTYLE, mExWindowStyle); // update window style
  }

  /// @brief     method to remove extra window attributes/styles from an existing `xWidget`
  /// @param[in] flags ~ list of flags to remove in the form `WS_EXXXX | WS_EXYYY`
  void removeExWindowStyle(DWORD flags) {
    mExWindowStyle &= ~flags; // remove from `mExStyle` class member
    SetWindowLongPtr(mhWnd, GWL_EXSTYLE, mExWindowStyle); // update window style
  }

protected:

  /// @brief     method for setting the class name of the `xWidget`
  ///            used internally during registration of the window
  /// @param[in] classname ~ the class name to use for registering the window
  /// @details   The class name is derived-type specific & thus retrieved using `ClassName()`
  /// @see       `ClassName`
  void setClassName(LPCTSTR classname) {
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstringstream wss;
    wss << classname;
    mClassName = StrConverter::WStringToString(wss.str());
    #else
    mClassName = classname;
    #endif
  }

public:

  /// @brief     method for setting/updating the text of an `xWidget`
  /// @param[in] text ~ std::string reference of the new text
  /// @note      Designed to be `UNICODE` & `ANSI/ASCI` compatible
  virtual void setText(const std::string& text) {
    #if defined(UNICODE) && defined(_UNICODE)
    // copy the `const std::string&` data from `text`
    // into a new `std::wstring` variable `wstr`
    std::wstring wstr(text.begin(), text.end());
    mText = wstr; // assign the copied data to `mText`
    #else
    // just copy the data directly into `mText`
    mText = text;
    #endif
    // update window text if the window exists ...
    if (exists) {
      SetWindowText(mhWnd, mText.c_str());
    }
  }

  /// @brief   method for retrieving the text of a widget
  /// @details
  /// In the context of `xButton` this retrieves the button text <br/>
  /// In the context of `xFrame` this retrieves the frame/ title <br/>
  std::string getText() {
    // return mText;
    #if defined(UNICODE) && defined(_UNICODE)
    return StrConverter::WStringToString(mText);
    #else
    return mText;
    #endif
  }

protected:

  /// @brief container storing `xWidget*` pointers of children for `this` instance
  std::vector<xWidget*> children;

  /// @brief     method for adding child to container of `xWidget*` children
  /// @param[in] child ~ pointer to `xWidget` child, i.e. `xWidget*` pointer
  void setChild(xWidget* child) {
    children.push_back(child);
  }

  /// @brief     method for retrieving a child from `children` container
  /// @param[in] hWnd ~ handle of the child `xWidget` to retrieve
  /// @return    `xWidget*` of the child element or `nullptr` if not found
  xWidget* getChild(HWND hWnd) {
    for (size_t i = 0; i < children.size(); i++) {
      if (children[i]->Handle() == hWnd) {
        return children[i];
      }
    }
    return nullptr;
  }

public:

  /// @brief   method to retrieve all the children of an `xWidget`
  /// @return `std::vector<xWidget*>` ~ a vector of pointers to ALL `xWidget`s
  std::vector<xWidget*> getChildren() {
    return children;
  }

  public:

  /// @brief   method to get all children of an `xWidget` by `classname`
  /// @details This method filters the `children` vector to retrieve only
  ///          those children having a specified `classname`
  /// @return  `std::vector<xWidget*>` ~ a vector of pointers to `xWidgets` having specified `classname`
  std::vector<xWidget*> getChildrenByClassName(const std::string& classname) {
    // container to return once populated ...
    std::vector<xWidget*> filter;
    // filter the `xWidget`s children by `mClassName` "xFrame"
    for (size_t i = 0; i < children.size(); i++) {
      if (children[i]->getClassName() == classname) {
        // if child widget is an `xFrame` by class name,
        // then add it to the vector to return to client
        filter.push_back(children[i]);
      }
    }
    // return the populated container ...
    return filter; // if empty, obviously criteria unmatched!
  }

  /// @brief    method to retrieve a child element according to `int` index
  /// @return  `xWidget*` ~ pointer to an `xWidget` child
  /// @details  May not be super useful, but kept
  ///           for iterating children if need be ...
  xWidget* child(int index) {
    return children[index];
  }

  /// @brief   method to return the C-style private handle of a window
  /// @return `mhWnd` of type `HWND` ~ handle of the `xWidget`s window
  HWND Handle() {
    return mhWnd;
  }

  /// @brief   method to return a handle on the parent of this `xWidget`
  /// @return `parent->Handle()` ~ parent's `mhWnd` of type `HWND`
  HWND hParent() {
    // return parent->Handle();
    // alternatively
    return parent->mhWnd;
  }

  /// @brief   method to return a pointer to this `xWidget`s parent
  /// @return `xWidget*` ~ pointer to parent of invoking `xWidget`
  xWidget* Parent() {
    return parent;
  }

protected:

  virtual void add(xWidget* pWidget) {
    if (!pWidget) {
      return;
    }
    // assign `this` as `parent` to `widget`
    pWidget->parent = this;
    // create the `widget`
    pWidget->create();
    // `create()` to insert widget as child for widget->parent
  }

protected:

  /**
   * @brief   pure virtual abstract method to retrieve class name
   * @return  long pointer to a constant `T` string
   * @details pure virtual abstract method
   *          to enforce each derived-class
   *          returns an appropriate class name
   * @note    class name is `xLib`-dependent,
   *          for example, button windows are
   *          identified by the class name "xButton"
   *          & frame windows are identified by the
   *          the class name "xFrame" <br/>
   *          distinct/subtly different from type name
   */
  virtual LPCTSTR ClassName() const = 0;

  /**
   * @brief   pure virtual abstract method to retrieve type name
   * @return  long pointer to a constant `T` string
   * @details pure virtual abstract method
   *          to enforce each derived class
   *          returns an appropriate type name
   * @note    type name is window-class dependent,
   *          for example, button windows are registered
   *          using the type name `WC_BUTTON` <br/>
   *          distinct/subtly different from class name
   */  
  virtual LPCTSTR TypeName() const = 0;

public:

  /**
   * @brief      Pure virtual abstract method to be overridden by derived types for class-specific message handling
   * @return    `LRESULT` ~ integer value returned to OS (program response to particular `msg`)
   * @param[in]  msg ~ `UINT` message value received from `WndProc(...)` in reponse to redirecting/dispatch
   * @param[in]  wParam ~ a number the size of a pointer (meaning depends on msg received by window/widget)
   * @param[in]  lParam ~ a number the size of a pointer (meaning depends on msg received by window/widget)
   */ 
  virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

  /**
   * @brief    method for registering a new window/`xWidget` class
   * @return   boolean flag whether or not registration was successful => never used!
   *           returns `true` or abort/terminate `exit(-1)`
   * @details  This protected method is used to register new window classes <br/>
   *           by encapsulating all the "low-level" C-style Win32 API       <br/>
   *           class registration procedures in a single method body        <br/>
   *           & "swapping-out" some of the parameters depending            <br/>
   *           on the type of window being registered.
   */
  bool RegisterWidget() {

    // set class name ...
    setClassName(ClassName());

    wx.cbSize = sizeof(WNDCLASSEX);                    // allocate memory for class
    wx.style = mClassStyle;                            // styles/attributes `DWORD`
    wx.cbClsExtra = 0;                                 // extra styles/extra attributes `DWORD`
    wx.hInstance = xApp::get().hGlobalInstance;        // global module handle executable file name `hInstance`
    wx.hIcon = NULL;                                   // icon `HICON` => updated on a per-window-basis after registration
    wx.hCursor = LoadCursor(NULL, IDC_ARROW);          // cursor `HCURSOR` => updated on a per-event-basis => `onHover`, `onClick`, `onDoubleClick`
    wx.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);      // background color `HBRUSH` => updated on a per-window-basis after registration
    wx.lpszMenuName = NULL;                            // menu name `HMENU` => updated on a per-window-basis
    wx.lpfnWndProc = WndProc;                          // pointer to `WndPoc` function => static/shared main message handler ...
    wx.lpszClassName = ClassName();                    // pure abstract virtual override returns `LPCTSTR`

    // before registering widget, check if already registered ...
    if (xWidgetManager::get().isRegistered(mClassName)) {
      LOG("xWidget class already registered => proceeding ...");
      return true; // return early, i.e. no need to register
    }

    try {
      // try to register class ...
      if (!RegisterClassEx(&wx)) {
        // get last error/crash code ...
        int error = GetLastError();

        std::string translate = xError::errorTranslate(error); // translate
        std::string message = "xWidget registration failed: " + translate;
        
        throw RuntimeException(message);
      }
    } catch(RuntimeException &ex) {

      // log error message ...
      LOG(ex.what());
      // report exception ...
      std::cerr << ex.what() << " ... " << "ABORT/TERMINATE!" << std::endl;

      exit(-1); // abort/termintate
      // return false; // no return after abort
    }

    // if registration successfull, then store the class name ...
    xWidgetManager::get().setClassName(mClassName);
    LOG("xWidget registration successfull");

    // always true or abort/terminate
    return true;
  }

public:

  /**
   * @brief   method for creating the window/`xWidget`
   * @return  boolean flag whether or not registration was successful => never used!
   *          returns `true` or abort/terminate `exit(-1)`
   * @details method to create a window based on an existing/registered window class
   * 
   * @remark
   * Associating a parent to a widget, for which the parent has not yet been created
   * does not make sense, since the parent may be responsible for creating/drawing
   * its child components & also creating a widget before its parent may not add
   * the widget to the parent's children, leading to unnecessary complications ...
   * 
   * @note
   * `ShowWindow(mhWnd, visible ? SW_SHOW : SW_HIDE);` is invoked at the end of `create()`
   * <br/>
   * In the context of `xWindow`/`xFrame` the show/hide state is determined by
   * any of the interface methods that directly manipulate `xWindow` data, such as
   * `minimize()`, `maximize()`, `setHeight(...)`, `setSize(...)` , etc.
   * but it is generally preferred to use `show()`/`hide()`/`create()`
   * <br/>
   * In the context of `xButton` & other `xControl`s, the show state
   * will be set to `true` by default, but can be toggled using
   * `setVisible(true/false)`
   */
  virtual bool create(); // implementation aftex `xFrame` & `xControl` ...

protected:

  /// @brief method to initialize the default event handlers for an `xWidget` object
  virtual void initDefaultHandlers() { } // override in derived classes, if required

protected:

  /// @brief variable to mark `xWidget` as a "subwindow",
  /// mainly for positioning purposes <br/>
  /// By default, all `xComponent`s will be "subwidgets"
  /// only `xFrame` implements the special case ...
  bool isSubWidget = false;

  /// @brief show/hide state of `xWindow` ~ default `true`
  bool visible = true; // default

public:

  /// @brief     method for toggling the show/hide state of a xWidget
  /// @details   This method takes over the role of `create()`
  ///            in the contex of `xWindow` if the `xWidget` does
  ///            not exist, otherwize show/hide the `xWindow` as per normal <br/>
  ///            The `create()` method is invoked on the `xWidget` *regardless* of state
  /// @param[in] state ~ whether or not to display the window after `create()`
  void setVisible(bool state) {
    // set the show/hide state of the `xWindow`
    visible = state;

    // if the `xWindow` does not exist ...
    if (!exists) {
      create(); // create `xWidget` (*regardless* of `state`) ...
    }
    
    if (visible) {
      appendWindowStyle(WS_VISIBLE);
    } else {
      removeWindowStyle(WS_VISIBLE);
    }

    // update window graphic appearance ...
    render();
  }

  /*%*%*%*%*%*%*%*/
  /* Dimensions  */
  /*%*%*%*%*%*%*%*/

  /// @brief     method to update the width of an `xWidget`
  /// @param[in] w ~ `int` value representative of the new width for the `xWidget`
  virtual void setWidth(int w) {
    // create if not exists
    if (!exists) {
      create();
    }

    m_w = w; // update/set `xWidget` width
    // render => ensure that size is updated => provide appropriate flags/attributes
    render(SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER);
  }

  /// @brief     method to update the height of an `xWidget`
  /// @param[in] h ~ `int` value representative of the new height for the `xWidget`
  virtual void setHeight(int h) {
    // create if not exists
    if (!exists) {
      create();
    }

    m_h = h; // update `xWidget` height
    // render => ensure that size is updated => provide appropriate flags/attributes
    render(SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER);
  }

  /// @brief     method to update the width & height of an `xWidget`
  /// @param[in] w ~ `int` value representative of the new width for the `xWidget`
  /// @param[in] h ~ `int` value representative of the new height for the `xWidget`
  void setSize(int w, int h) {
    setWidth(w);
    setHeight(h);
  }

  /// @brief   method for retrieving the width of an `xWidget`
  /// @return `int` value representative of the `xWidget` width
  int getWidth() {
    return m_w;
  }
  /// @brief   method for retrieving the height of an `xWidget`
  /// @return `int` value representative of the `xWidget` height
  int getHeight() {
    return m_h;
  }

  // alternative methods to get the width & height
  // of a window using Win32 API `RECT`-technique

  // int getWindowWidth() {
  //   RECT wndRect;
  //   GetWindowRect(mhWnd, &wndRect);
  //   return wndRect.right - wndRect.left;
  // }
  
  // int getWindowHeight() {
  //   RECT wndRect;
  //   GetWindowRect(mhWnd, &wndRect);
  //   return wndRect.bottom - wndRect.top;
  //   return 0;
  // }

  /*%*%*%*%*%*%*%*%*%*%*%*%*/
  /* Coordinates/Position  */
  /*%*%*%*%*%*%*%*%*%*%*%*%*/

  /// @brief     method to update the horizontal x-position/coordinate of an `xWidget`
  /// @param[in] x ~ `int` value representative of the new horizontal x-position/coordinate- for the `xWidget`
  void setHorizontalPosition(int x) {
    // create if not exists
     if (!exists) {
      create();
    }

    // update widget horizontal position
    m_x = x;
    // render => ensure that position (move) is updated => provide appropriate flags/attributes
    render(SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
  }

  /// @brief     method to update the vertical y-position/coordinate of an `xWidget`
  /// @param[in] y ~ `int` value representative of the new vertical y-position/coordinate for the `xWidget`
  void setVerticalPosition(int y) {
    // create if not exists
     if (!exists) {
      create();
    }

    // update widget vertical position
    m_y = y;
    // render => ensure that position (move) is updated => provide appropriate flags/attributes
    render(SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
  }

  /// @brief     method to update the position/coordinates of an `xWidget`
  /// @param[in] x ~ `int` value representative of the new vertical x-position/coordinate for the `xWidget`
  /// @param[in] y ~ `int` value representative of the new vertical y-position/coordinate for the `xWidget`
  void setPosition(int x, int y) {
    setHorizontalPosition(x);
    setVerticalPosition(y);
  }

  /// @brief   method for retrieving the horizontal x-coordinate of an `xWidget`
  /// @return `int` value representative of the `xWidget` horizontal position
  int getHorizontalPosition() {
    return m_x;
  }
  /// @brief   method for retrieving the vertical y-coordinate of an `xWidget`
  /// @return `int` value representative of the `xWidget` vertical position
  int getVerticalPosition() {
    return m_y;
  }

  /**
   * @todo define interface method to manipulate/adjust `xFrames` z-order ...
   */

protected:

  /// @brief     protected method for re-drawing an `xWidget` whenever graphical changes have taken place
  /// @details   The Win32 API function `SetWindowPos(...)`
  ///            is encapsulated within the `render(...)` xWidget interface method,
  ///            & is responsible for adjusting appearance-related aspects of a window, such
  ///            as z-order, coordinates, dimensions as well as the show/hide state of a window
  /// @note
  /// `SWP_XXX` ~ `SET WINDOW POSITION`
  /// @remark
  /// The render() interface method encapsulates the `SetWindowPos()` Win32 API function
  /// The `SWP_XXX` attributes/flags are to be taken into consireation upon invoking `SetWindowPos(...)`
  /// @param[in] attributes ~ `SWP_XXX` flags specifying the appropriate changes to be made
  /// @param[in] widget ~ `xWidget*` after which the updated widget must appear in z-order (optional param)
  void render(
    DWORD attributes
      = SWP_FRAMECHANGED
      | SWP_NOSIZE
      | SWP_NOMOVE
      | SWP_NOZORDER,
    xWidget* widget = nullptr
  ) {
    // second argument is the pointer of the `xWidget` to insert this widget after, in terms of z-order ...
    // The handle (mhWnd) is to be extracted from the given `xWidget*` (if not null) ...
    SetWindowPos(mhWnd, widget ? widget->Handle() : nullptr , m_x, m_y, m_w, m_h, attributes);
  }

  /// @brief method to invalidate the entire client-area of a window, thus re-painting the window!
  void update() {

    // check whether the `xWidget` exists
    if (!exists) {
      return; // return if not
    }

    // ... invalidate client rect ...
    // NULL ~ specifies that the entire client area is to be repainted
    // TRUE ~ specifies that the background should be erased
    InvalidateRect(mhWnd, NULL, TRUE);
  }

public:

  /// @brief     method to center a widget, relative to another
  /// @param[in] widget ~ used as relative coordinate <br/>
  ///            if `nullptr` then center relative to screen/desktop
  /// @todo      fix `center(...)` for `xButton` objects! [REVISIT]
  void center(xWidget* widget = nullptr) {

    // create if not exists
    if (!exists) {
      create();
    }

    // check if `nullptr`
    if (!widget) {
      
      // if widget == this
      // center relative to itself
      // if (widget == this) {
      //   // handle case ...
      //   // nothing to do ...
      // }

      // center relative to screen/desktop
      setHorizontalPosition((Monitor::ScreenWidth() - getWidth())/2);
      setVerticalPosition((Monitor::ScreenHeight() - getHeight())/2);

    } else if (widget != nullptr) {

      try {
        
        // check whether widget NOT exists
        // cannot center this widget relative
        // to a widget that does not exist

        if (!widget->exists) {
          std::string message = "cannot center widget relative to another that does not exist!";
          throw RuntimeException(message);
        }

      } catch(RuntimeException &ex) {
        // LOG(ex.what());
        std::cerr << ex.what() << std::endl;
        return;
      }

      /// @note if `widget == this->parent`
      /// then the position is relative
      /// to the coordinates of it's parent
      
      int otherX = 0;
      int otherY = 0;

      if (isSubWidget) {
      
        // case: `widget != this->parent`
        if (widget != this->parent) {
          // get other widget coordinates/position
          otherX = widget->getHorizontalPosition();
          otherY = widget->getVerticalPosition();
        };

      } else {
        // get other widget coordinates/position
        otherX = widget->getHorizontalPosition();
        otherY = widget->getVerticalPosition();
      }

      setHorizontalPosition(otherX + widget->getWidth()/2 - this->getWidth()/2);
      setVerticalPosition(otherY + widget->getHeight()/2 - this->getHeight()/2);
    }
  }

protected:

  /*%*%*%*%*/
  /* Font  */
  /*%*%*%*%*/

  /// @brief pointer to `xFont` object used by the `xWidget`
  /// @todo define a global/shared font mechanism
  xFont* pFont = nullptr;
  // /// @brief store ref to previous font for cleanup
  // xFont* prevFont = nullptr; // takes memory resources ...

public:

  /// @brief   interface method to set the font for an `xWidget`
  /// @details 
  /// `xFont` constructor to be accessible by `xWidget` <br/>
  /// `xFont` objects to be manufactured by `xWidget`   <br/>
  /// `xWidget` to manage `xFont` resources to avoid    <br/>
  /// improper use of `xFont` objects by client-code
  /// @param[in] font ~ `string` value representative of the font name to use (default "Tahoma")
  /// @param[in] size ~ `int` value representative of the font-size in pixels (default 10)
  /// @param[in] italic ~ `bool` flag whether (`true`) or not (`false`) to set the text as italic/slant (default `false`)
  /// @param[in] bold ~ `bool` flag whether (`true`) or not (`false`) to set the text as bold/`FW_BOLD` (default `false`)
  /// @param[in] underline ~ `bool` flag whether (`true`) or not (`false`) to underline the text (default `false`)
  /// @param[in] strikeout ~ `bool` flag whether (`true`) or not (`false`) to strike out the text (default `false`)
  void setFont(
    const std::string& font = "Tahoma", // default font name
    int size = 10,
    bool italic = false,
    bool bold = false,
    bool underline = false,
    bool strikeout = false
  ) {
    
    // variable for previous font
    xFont* prevFont = pFont; // copy pointer
    
    // update font to a new value ...
    pFont = new xFont(font, size, italic, bold, underline, strikeout);
    
    // update font if exists
    if (exists) {
      updateFont();
    } // otherwize wait for client to create ...

    // cleanup previous font, etc ...
    // note* each xWidget has ref to own `xFont*` instance
    // thus, we can delete the previous `xFont*` held by `xWidget`
    // `prevFont*` not in use by other widgets, unless client-code
    // does something extreme, i.e. not using the provided interface
    if (prevFont) {
      delete prevFont; // crash if `prevFont` local variable ???
      prevFont = nullptr;
    } // free `prevFont*`
  }

protected:

  /// @brief protected code-interface/"wrapper" method
  ///        to update the font of a window
  virtual void updateFont() {
    if (exists) {
      SendMessage(mhWnd, WM_SETFONT, (WPARAM) pFont->hFont, TRUE);
      redraw();
    }
  }

  /// @brief protected code-interface/"wrapper" method
  ///        to `InvalidateRect(...)`/re-paint a window
  virtual void redraw() {
    if (exists) {
      InvalidateRect(mhWnd, NULL, TRUE);
    }
  }

protected:

  /// @brief boolean flag to toggle disable/enable mode for an `xWidget`
  bool idle = false; // default false

public:

  /// @brief method to enable/disable an `xWidget`
  /// @param[in] flag ~ boolean flag to toggle enable/disable state
  void setEnable(bool flag = true) {
    // re-use below setDisable with switch flag
    setDisable(!flag);
  }

  /// @brief method to disable/enable an `xWidget`
  /// @param[in] flag ~ boolean flag to toggle disable/enable state
  /// @todo also invoke this method from widthin derived class during `create(...)`
  /// @todo defined `mOnDisable(...)` event handler for client-code control
  void setDisable(bool flag = true) {

    // assign flag input arg
    idle = flag;

    if (!exists) {
      return; // cannot send window message if mhWnd is NULL
    }

    // if true, disable window
    if (idle) {
      EnableWindow(this->Handle(), FALSE);
    } else {
      EnableWindow(this->Handle(), TRUE);
    }
  }

  /// @brief shorthand notation for `setDisable(true)`/`setEnable(false)`
  void disable() {
    setDisable(true);
  }

  /// @brief shorthand notation for `setEnable(true)`/`setDisable(false)`
  void enable() {
    setDisable(false);
  }

  /// @brief   method to retrieve the `idle` state of an `xWidget`
  /// @return  boolean flag indicating whether the `xWidget` is `idle`
  /// @details This method is marked `virtual` to allow `override` in `xWindow`
  ///          <br/>
  ///          `xWindow` requires to `override` this method since, it's `idle`
  ///          state is determined by whether or not all of it's children
  ///          `xWindow`s are not in a "modal state" & client `idle` state
  virtual bool isIdle() {
    // base class member `xWindow::isIdle()`
    // LOG("xWidget::isIdle()");
    // return idle state ...
    return idle;
  }

public:

  /*%*%*%*%*%*/
  /* cleanup */
  /*%*%*%*%*%*/

  /// @brief   shorthand method to recursively destroy
  ///          an `xWidget` along with all of its children
  /// @details method to destroy resources for this class,
  ///          whenever `WM_DESTROY` `msg` is received by
  ///         `HandleMessage()` or whenever the base class
  ///         `xWidget` destructor is invoked
  /// @todo    make protected to hide from client-code
  /// @remark  don't use this method to destroy a `xFrame`
  ///          instead use `close()` which will correctly
  ///          Handle the xFrame's `mOnClose(...)` operation!
  virtual void destroy() {
    // LOG("destroy()");
    destroyRecursive(this);
    // Logging this info recursively, could be helpful, but confusing to read ...
    // LOG(("xWidgetManager::get().count() = " + std::to_string(xWidgetManager::get().count())).c_str());
  }

protected:

  /// @brief protected code-interface method to recursively remove
  ///        child elements & destroy `xWidget` resources correctly
  /// @param[in] widget ~ pointer for the widget on which to act recursively!
  /// @remark
  /// We CANNOT use `parent` as variable name => `parent` reserved! <br/>
  /// we HAVE to use a param name for `removeRecursive(...)`        <br/>
  /// because it's not operating on `this` instance ...             <br/>
  /// Rather, it operates on a widget pointer                       <br/>
  /// & recursively acts on its children ...                        <br/>
  static void destroyRecursive(xWidget* widget) {

    if (!widget) {
      return;
    }

    // Remove children & `.this` instance from `xWidgetManager` map ...
    // loop over children & invoke this method recursively on each
    for (size_t i = 0; i < widget->children.size(); i++) {

      xWidget* child = widget->children[i];

      if (child) {
        // child->destroy(); // destroy child widget
        DestroyWindow(child->mhWnd);
        destroyRecursive(child); // invoke on child
        // delete child; // client code to delete (perhaps right after mOnClose)
      }
    }

    // returns bool, but for no real reason ...
    xWidgetManager::get().remove(widget->mhWnd);
    
    // The below is false error reporting & will not be implemented ...
    // try {
    //   // remove widget from map ...
    //   bool success = xWidgetManager::get().remove(widget->mhWnd);

    //   /// This most likely NOT the case ...
    //   if (!success) {
    //     std::string message = "xWidget not found in `xWidgetManager` => potential memory leak!!!";
    //     throw RuntimeException(message);
    //   }

    // } catch(RuntimeException &ex) {
    //   // if this exception is encountered,
    //   // client may have a memory leak,
    //   // but quite unlikely ...
    //   LOG(ex.what());
    //   std::cerr << ex.what() << std::endl;      
    //   // exit(-1); // don't abort
    // }

    // @note redundant:
    // causes issues in the erase-from-map process!
    // remove widget from its own parent's children!
    // This reduce unnecessary exceptions, but handle
    // the exceptional case in which the widget
    // is *actually not removed correctly*
    // from the map, but quite unlikely ...
    // if (widget->parent) {
    //   //     Not necessary to check here ...
    //   // ... because check is done in
    //   //    `removeFromOwnParent(...)`
    //   removeFromOwnParent(widget);
    //   // `clear()` widget's children vector
    // }

    widget->children.clear();

    // destroy icons & font & other instance-held resources ...
    destroyResources(widget);
    // delete widget;
  }

  /// @brief     method to destroy the resources associated to an `xWidget`
  /// @param[in] widget ~ pointer to the widget being destroyed
  static void destroyResources(xWidget* widget) {

    // ...
    // Evolving code-base may require more
    // memory management to be handled here!
    // ...

    /// @note
    /// don't delete the widget's `xBorder` object,
    /// since this is done by `xBorderFactory`
    /// depending on the ref count for the border ...

    // if widget is not null ...
    if (!widget) {
      return; // return early (nothing to do ...)
    }

    // check if this widget's `pWidgetBorder` exists, i.e. not `nullptr`
    if (widget->pWidgetBorder) {
      // then check if the border is stored `xBorderFactory` ...
      bool status = xBorderFactory::get().checkBorder(widget->pWidgetBorder);
      if (status) {
        // std::cout << "pWidgetBorder count: " << pBorder->getCount() << std::endl;
        // if pBorder is contained in `xBorderFactory`, then remove it
        xBorderFactory::get().removeBorder(widget->pWidgetBorder);
      } // otherwize, it has already been removed ...
      widget->pWidgetBorder = nullptr;
    }

    // similar to above ...
    if (widget->pFocusBorder) {
      // then check if the border is stored in `xBorderFactory` ...
      bool status = xBorderFactory::get().checkBorder(widget->pFocusBorder);
      if (status) {
        // std::cout << "pFocusBorder count: " << pBorder->getCount() << std::endl;
        // if `pBorder` is contained in `xBorderFactory`, then remove it
        xBorderFactory::get().removeBorder(widget->pFocusBorder);
      } // otherwize, it has already been removed ...
      widget->pFocusBorder = nullptr;
    }

    // destroy handle for Draw Context ...
    if (widget->mhDC) {
      DeleteDC(widget->mhDC);
    }

    // free the `xFont` object resources ...
    if (widget->pFont) {
      delete widget->pFont;
      widget->pFont = nullptr;
    }

    // destroy the widget's context menu ...
    widget->removeContextMenu();
  }

protected:

  // method that can be used for a widget
  // to remove itself from it's parent's children
  // which is not currently being used in the `xLib` system ...
  // /// @brief     protected code-interface method for removing 
  // //             a widget from its own parent's children
  // /// @param[in] widget ~ pointer of the widget to be removed
  // /// @remark    this method is currently not used
  // void removeFromOwnParent(xWidget* widget) {
    
  //   // check if the widget is not nullptr
  //   if (!widget) {
  //     return;
  //   }

  //   // This interface method is sensible
  //   // only if widget has a parent
  //   if (!widget->parent) {
  //     return; // do nothing ...
  //   }

  //   // remove widget from its own parent's children ...
  //   for (size_t i = 0; i < widget->parent->children.size(); i++) {
  //     if (widget->parent->children[i] == widget) {
  //       widget->parent->children.erase(
  //         widget->parent->children.begin() + i
  //       );
  //     }
  //   }
  // }

public:

  /// @brief   `xWidget` abstract base class virtual destructor
  /// @details `xWidget` is a pure abstract class base class <br/>
  ///           To ensure that the class inheritance hierarchy
  ///           respects the invokation of derived class destructors,
  ///           this abstract base class destructor is marked `virtual`
  virtual ~xWidget() {
    // LOG("~xWidget");
    // destroy();
    destroyRecursive(this);
  }

protected:

  /// @brief friend class to access theme member
  friend class xTheme;

  /// @brief
  /// default theme when registering window,
  /// unless `globalThemeSelected == true` ...
  xTheme::Theme theme = xTheme::Theme::DEFAULT;

public:
  
  /// @brief     Interface method to apply the selected theme to `this` widget
  /// @param[in] t ~ selected theme from the enum `xTheme::Theme` => `CLASSIC`/`MODERN`
  void SetWidgetTheme(xTheme::Theme t) {
    // No need to set theme for instance,
    // as it is done in xTheme interface method
    // Apply the selected theme to the widget ...
    xTheme::SetWidgetTheme(this, t);
  }

  /// @brief     Interface method to apply the selected theme recursively to `this` widget
  /// @param[in] t ~ selected theme from the enum `xTheme::Theme` => `CLASSIC`/`MODERN`
  void SetWidgetThemeRecursive(xTheme::Theme t) {
    // No need to set theme for instance,
    // as it is done in xTheme interface method
    // Recursively apply the selected theme to the widget ...
    xTheme::SetWidgetThemeRecursive(this, t);
  }

  /*%*%*%*%*/
  /* Menu  */
  /*%*%*%*%*/

protected:

  /// @brief  variable storing a pointer for the widget's context menu
  xPopupMenu* pContextMenu = nullptr;

public:

  /// @brief   method to retrieve the pointer of the `xPopupMenu` associated to the `xWidget`
  /// @return `xPopupMenu*` ~ pointer of the context menu associated to the widget
  xPopupMenu* getContextMenu() {
    return pContextMenu;
  }

  /// @brief method to set/attach/associate an `xPopupMenu` to the `xWidget`
  /// @param popup ~ pointer of the popup/context/submenu to attach to the widget
  void setContextMenu(xPopupMenu* popup) {
    // check if the same menu used or `nullptr`
    /// @todo
    /// Handle exceptions/throw to inform client-code
    /// that the menu pointer is null or points to
    /// the same menu as currently being used ...
    if (popup == pContextMenu || popup == nullptr) { return; }
    // check whether the menu is already in use ...
    /// @todo
    /// Handle exceptions/throw to inform client-code
    /// that the menu is ALREADY in USE!
    if (xMenuManager::checkMenu(popup)) { return; }
    // otherwize, hold on to the previous menu ...
    xMenu* pPrevContextMenu = pContextMenu;
    // set the new menu for the frame ....
    pContextMenu = popup;
    // store the new menu in xMenuManager ...
    xMenuManager::storeMenu(pContextMenu);
    // delete the previous menu, if it exists ...
    if (pPrevContextMenu) {
      // remove this from the parent of previous menu ...
      pPrevContextMenu->owner = nullptr;
      xMenuManager::removeMenu(pPrevContextMenu);
    }
    // assign this widget as the owner of the menu ...
    popup->owner = this;
    // // set the menu for this frame ...
    // SetMenu(mhWnd, pContextMenu->hMenu);
    // TrackPopupMenu via `WM_CONTEXTMENU` & handle `WM_COMMAND`
  }

  /// @brief method to detatch/remove a context menu from the widget
  void removeContextMenu() {

    if (pContextMenu) {
      // std::cout << "xWidget::removeContextMenu()" << std::endl;
        // pContextMenu->owner = nullptr;
        xMenuManager::removeMenu(pContextMenu);
        pContextMenu = nullptr;
    }

    // no force redraw necessary ...
  }

}; // end of xWidget class

/// @brief   method to get all widgets by `classname`
/// @details This method filters the unordered_map to retrieve only
///          those widgets having a specified `classname`
/// @return  `std::vector<xWidget*>` ~ a vector of pointers to `xWidgets` having specified `classname`
std::vector<xWidget*> xWidgetManager::getWidgetsByClassName(const std::string& classname) {
  // container to return once populated ...
  std::vector<xWidget*> filter;
  std::unordered_map<HWND, xWidget*> widgets = xWidgetManager::get().getWidgets();
  // filter the `xWidget`s by `mClassName` "xFrame"
  std::unordered_map<HWND, xWidget*>::iterator i;
  for (i = widgets.begin(); i != widgets.end(); i++) {
    if (i->second->getClassName() == classname) {
      filter.push_back(i->second);
    }
  }        
  // return the populated container ...
  return filter; // if empty, obviously criteria unmatched!
}

void xWidgetManager::clear() {
  // destroy widgets held in xWdidgets ...
  std::unordered_map<HWND, xWidget*>::iterator i;
  for (i = xWidgets.begin(); i != xWidgets.end(); i++) {
      i->second->destroy();
  }
  xWidgets.clear();
}

/// @brief implementation for `xBorderFactory`s `clear()` method ...
/// @note  implementation after `xWidget`
void xBorderFactory::clear() {
  // for each xWidget, invoke removeBorder();
  // check if key hWnd exists before remove ...
  std::unordered_map<HWND, xWidget*> widgets = xWidgetManager::getWidgets();
  std::unordered_map<HWND, xWidget*>::iterator i;
  for (i = widgets.begin(); i != widgets.end(); i++) {
    removeBorder(i->second->pWidgetBorder);
  }
  borders.clear();
}

/// @brief implementation for `xBorderFactory`s `clearTemp()` method ...
/// @note  implementation after `xWidget`
void xBorderFactory::clearTemp() {
  // ...
  // these borders are not yet associated to a widget, just delete ...
  temp.clear();
}

/// @brief     method to set the theme of an `xWidget` control
/// @param[in] widget ~ the widget for which to apply the theme
/// @param[in] t ~ the theme to be applied to the widget
void xTheme::SetWidgetTheme(xWidget* widget, xTheme::Theme t) {
  widget->theme = t;
  if (t == xTheme::Theme::CLASSIC) {
    // classic
    SetWindowTheme(widget->Handle(), L" ", L" ");
  } else if (t == xTheme::Theme::DEFAULT) {
    // modern/default
    SetWindowTheme(widget->Handle(), L"Exoplorer", NULL);
  }
  SendMessage(widget->Handle(), WM_THEMECHANGED, 0, 0);
  // widget->redraw(); // ... ?
}

/// @brief     method to recursively set the theme of an `xWidget` control & its children
/// @param[in] widget ~ the root widget for which to apply the theme
/// @param[in] t ~ the theme to be applied to the widget & its children
void xTheme::SetWidgetThemeRecursive(xWidget* widget, xTheme::Theme t) {
  for (size_t i = 0; i < widget->children.size(); i++) {
    xTheme::SetWidgetThemeRecursive(widget->children[i], t);
  }
  xTheme::SetWidgetTheme(widget, t);
}

/// @brief     method to apply a global them to all `xWidget`s in the app
/// @param[in] t ~ the theme to be applied to the widget
/// @details   apply the selected Theme to ALL the `xWidget`
///            objects in the `xWidgetManager::xWidgets` container
void xTheme::SetGlobalTheme(xTheme::Theme t) {
  xTheme::globalTheme = t; // commit the global theme selected ...
  xTheme::globalThemeSelected = true; // flag global theme set ...
  // enumerate all the widget & apply the theme ...
  std::unordered_map<HWND, xWidget*>::iterator i;
  for (i = xWidgetManager::getWidgets().begin(); i != xWidgetManager::getWidgets().end(); i++) {
    // if the list of widgets is empty, the theme will not be applied when registering => FIX!
    xTheme::SetWidgetTheme(i->second, xTheme::globalTheme);
  }
}

/// @brief method to dispatch menu item click events to appropriate menu items via `xItemManager`
/// @param pItem ~ pointer of the item/source from which the click event originated
/// @param pWidget ~ pointer of the widget to which the menu is attched containing the item
void xMenuItem::defaultMenuItemClickEvent(xMenuItem* pItem, xWidget* pWidget) {
  LOG(("pItem->getID() => " + std::to_string(pItem->getID())).c_str());
  LOG(("pWidget->getText() => " + pWidget->getText()).c_str()); // UNUSED(pWidget);
}

/// @brief   method to dispatch radio item click events to appropriate radio items via `xItemManager`
/// @param   pItem ~ pointer of the item/source from which the click event originated
/// @param   pWidget ~ pointer of the widget to which the menu is attched containing the item
/// @details This method is implemented to ensure that all radio items are toggled correctly
///          all the while triggering menu-change events correctly to associated menus objects ...
void xRadioItem::defaultRadioItemClickEvent(xMenuItem* pItem, xWidget* pWidget) {
  // std::cout << "defaultRadioButtonClickEvent" << std::endl;

  IMPLICIT(pWidget); // unused ...
  // Note*: not to be removed, since `defaultRadioItemclickEvent` should 
  // have the same signature as `defaultMenuItemClickEvent` ...
  // since it's using mOnMenuItemClick function pointer interface member ...

  // cast `pItem` to derived type ...
  xRadioItem* pRadioItem = static_cast<xRadioItem*>(pItem);
  // get the (parent) menu of the radio item ...
  xRadioMenu* pRadioMenu = static_cast<xRadioMenu*>(pRadioItem->parent);

  // pass details to protected `selectRadioItem` method ...
  pRadioMenu->selectRadioItem(pRadioItem);
  // after processing radio item select,
  // HOW to allow client to attach this?
  // `defaultMenuItemClickEvent(pItem, pWidget)`
  // NOT that important, but would be a good feature!
}

#endif // end of xWIDGET_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

/// @todo include demonstration/example graphics/images for each of the below ...

/**
 * @example Handle
 * 
 * Instantiate a `xFrame`/`xButton`/any-other `xWidget` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100); // provide params
 * xButton button(&frame, "OK", 50, 25, 100, 100); // pass by ref => `&frame`
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Get a handle on an `xWidget`:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * HWND hFrame = frame.Handle(); // retrieves the parent's window handle
 * HWND hButton = button.Handle(); // retrieves the button's window handle
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Get the pointer of an `xWidget`
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame* parentPointer = button.Parent(); // retrieves the parent's pointer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * HWDN parentHandle = button.hParent(); // retrieves the button's parent's handle
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */ 

/**
 * @example Text
 * 
 * Instantiate a `xFrame`/`xButton`/any-other `xWidget` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100); // provide params
 * xButton button(&frame, "OK", 50, 25, 100, 100); // provide params
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Manipulating an `xWidget`/control/element ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * frame.setText("client text"); // sets the title of the window/frame
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * button.setText("Cancel"); // sets the text of the button/control
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * std::cout << frame.getText() << std::endl; // gets & prints the frame's title/text
 * std::cout << button.getText() << std::endl; // gets & prints the buttons's title/text
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */  

/**
 * @example Idle
 * 
 * Instantiate a `xFrame`/`xButton`/any-other `xWidget` object as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Parent Frame", 1200, 800, 150, 100);
 * xFrame child("Child Frame", 500, 500, 400, 300);
 * xFrame sibling("Sibling Frame", 500, 500, 400, 300);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Manipulate the `idle` state of an `xWidgets`/controls/elements ...
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * childFrame.isIdle(true); // set the child frame as "inactive"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * siblingFrame.isIdle(false); // set the sibling frame as "active"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		iEvent.h
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * 
  * @brief 		file contains implementation of iEvent interface classes
  * 
  * @details 	iEvent.h defines event listerner interfaces for various event types, 
  *           including mouse click events as well as keyboard events, etc ...
  */

/// @brief begin of xEVENT_H implementation
#ifndef iEVENT_H
#define iEVENT_H

/// @todo implement a timer for properly
/// distinguising single clicks versus double clicks

/// @todo implement move semantics for all function pointers

// include memory for moving events pointers into existing storage
#include <memory>

// include functional for allowing client-code
// to use lambda/anonymous functions for events ...
#include <functional>

/// @remark allow for anonymous/lambda functions, using sytanx:
/// std::function<void(xT* pTObj, xK* pKType)>;

/// forward declaration of xWidget class ...
class xWidget;

/// @brief global enum of mouse-event constants
enum MouseEvent {
  ENTER,
  EXIT,
};

/// @brief global enum of mouse-key constants
enum MouseKey {
  LeftButton   = WM_LBUTTONDOWN,
  RightButton  = WM_RBUTTONDOWN,
  MiddleButton = WM_MBUTTONDOWN,
  ExtraButton  = WM_XBUTTONDOWN,
};

/**
  * @class   iClickEventListener
  * @brief 	 Click-event listener interface class
  *          
  * @details `iClickEventListener` declares function signatures for common control click events
  * 
  */
class iClickEventListener {

protected:

  /// @remark alias for void(*)(...) function pointer signature

  /// @param[in] pWidget ~ pointer of common control which receives the on-click callback trigger
  using event_onClick = std::function<void(xWidget* pWidget)>;

  /// @param[in] pWidget ~ pointer of common control which receives the on--double-click callback trigger
  using event_onDoubleClick = std::function<void(xWidget* pWidget)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onClick mOnClick = nullptr;
  event_onDoubleClick mOnDoubleClick = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a click event (applicable to common controls)
  virtual void setOnClick(event_onClick callback) = 0;

  /// @brief pure virtual abstract interface class method
  /// to attach a double click event (applicable to common controls)
  virtual void setOnDoubleClick(event_onDoubleClick callback) = 0;
};

/**
  * @class   iMouseEventListener
  * @brief 	 Mouse-event listener intreface class
  *          
  * @details `iMouseEventListener` declares function signatures for mouse events
  * 
  */
class iMouseEventListener {

protected:
  
  /**
   * @defgroup events Mouse Events
   * @remark alias for void(*)(...) function pointer signature
   * @{
   */

  /// @param[in] pWidget ~ pointer of widget which receives the on-mouse-hover callback trigger
  /// @param[in] event ~ MouseEvent enum constant to use for client-side comparison
  using event_onMouseHover = std::function<void(xWidget* pWidget, MouseEvent event)>;

  /// @param[in] pWidget ~ pointer of widget which receives the on-mouse-click callback trigger
  /// @param[in] event ~ MouseEvent enum constant to use for client-side comparison
  using event_onMouseClick = std::function<void(xWidget* pWidget, MouseKey key)>;

  /// @param[in] pWidget ~ pointer of widget which receives the on-mouse-release callback trigger
  /// @param[in] event ~ MouseEvent enum constant to use for client-side comparison
  using event_onMouseRelease = std::function<void(xWidget* pWidget, MouseKey key)>;

  /// @param[in] pWidget ~ pointer of widget which receives the on-mouse-double-click callback trigger
  /// @param[in] event ~ MouseEvent enum constant to use for client-side comparison
  using event_onMouseDoubleClick = std::function<void(xWidget* pWidget, MouseKey key)>;

  /*@} */

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onMouseHover mOnMouseHover = nullptr;
  event_onMouseClick mOnMouseClick = nullptr;
  event_onMouseRelease mOnMouseRelease = nullptr;
  event_onMouseDoubleClick mOnMouseDoubleClick = nullptr;

protected:

  /**
   * @defgroup setters Setters
   * @remark alias for void(*)(...) function pointer signature
   * @{
   */

  /// @brief pure virtual abstract interface class method
  /// to attach a mouse hover event (applicable to widgets)
  virtual void setOnMouseHover(event_onMouseHover callback) = 0;

  /// @brief pure virtual abstract interface class method
  /// to attach a mouse click event (applicable to widgets)
  virtual void setOnMouseClick(event_onMouseClick callback) = 0;

  /// @brief pure virtual abstract interface class method
  /// to attach a mouse release event (applicable to widgets)
  virtual void setOnMouseRelease(event_onMouseRelease callback) = 0;

  /// @brief pure virtual abstract interface class method
  /// to attach a mouse double click event (applicable to widgets)
  virtual void setOnMouseDoubleClick(event_onMouseDoubleClick callback) = 0;

  /* @} */

};

/// @brief global enum of focus-event constants
enum FocusEvent {
  GAINED,
  LOST,
};

/**
  * @class   iFocusEventListener
  * @brief 	 Focus-event listener intreface class
  *          
  * @details `iFocusEventListener` declares function signatures for focus events
  * 
  */
class iFocusEventListener {

protected:

  /// @remark    alias for void(*)(...) function pointer signature
  /// @param[in] pWidget ~ pointer of widget which receives the on-focus callback trigger
  /// @param[in] event ~ FocusEvent enum constant to use for client-side comparison
  using event_onFocus = std::function<void(xWidget* pWidget, FocusEvent event)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onFocus mOnFocus = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a focus gained click event (applicable to widgets)
  virtual void setOnFocus(event_onFocus callback) = 0;

};

/// @brief global enum of close-operation constants
enum CloseEvent {
  TERMINATE,  // terminate app/shutdown/quit
  CLOSE,      // close the frame & destroy the resources
  DISPOSE,    // dispose the frame but keep the resources
  DO_NOTHING, // don't do anything, just ignore the close button ...
};

/**
  * @class   iCloseEventListener
  * @brief 	 Close-event listener intreface class
  *          
  * @details `iCloseEventListener` declares function signatures for close events
  * 
  */
class iCloseEventListener {

protected:

  /// @remark    alias for void(*)(...) function pointer signature
  /// @param[in] pWidget ~ pointer of widget for which to invoke the on-close callback
  using event_onClose = std::function<void(xWidget* pWidget)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onClose mOnClose = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a close event (applicable to windows/frames)
  virtual void setOnClose(event_onClose callback) = 0;
};

// forward declaration ...
class xMenuItem;

/**
  * @class   iMenuItemClickEventListener
  * @brief 	 Menu-item-click-event listener intreface class
  *          
  * @details `iMenuItemClickEventListener` declares function signatures for menu item click events
  * 
  */
class iMenuItemClickEventListener {

protected:

  /// @remark    alias for void(*)(...) function pointer signature
  /// @param[in] pItem ~ pointer of the menu item that triggers the menu callback
  /// @param[in] pMenu ~ pointer of the widget to which the (parent) menu belongs
  using event_onMenuItemClick = std::function<void(xMenuItem* pItem, xWidget* pWidget)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onMenuItemClick mOnMenuItemClick = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a menu item click event (applicable to menu items)
  virtual void setOnMenuItemClick(event_onMenuItemClick callback) = 0;
};

// forward declaration ...
class xMenu;

/**
  * @class   iMenuChangeEventListener
  * @brief 	 Menu-change-event listener intreface class
  *          
  * @details `iMenuChangeEventListener` declares function signatures for menu change events
  * 
  * @todo     implement this interface for `xMenuItem` that may have checkbox functionality!
  *           i.e. invoke this method inside of `triggerMenuItemClickEvent`
  *           if its pointer is NOT `nullptr`
  */
class iMenuChangeEventListener {

protected:

  /// @remark    alias for void(*)(...) function pointer signature
  /// @param[in] pItem ~ pointer of the menu item that triggers the menu change
  /// @param[in] pMenu ~ pointer of the menu that "changed"/"updated"
  using event_onMenuChange = std::function<void(xMenuItem* pItem, xMenu* pMenu)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onMenuChange mOnMenuChange = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a change event (applicable to menus)
  virtual void setOnMenuChange(event_onMenuChange callback) = 0;
};

// forward declaration ...
class xControl;

/**
  * @class   iSelectionChangeEventListener
  * @brief 	 Selection-change-event listener intreface class
  *          
  * @details `iSelectionChangeEventListener` declares function signatures for dropdown menu item change events
  * 
  * @todo     Investigate a way to merge this interface with general "change",
  *           for re-usability, since the functionality is similar for all ...
  *  
  */
class iSelectionChangeEventListener {

protected:

  /// @remark    alias for void(*)(...) function pointer signature
  /// @param[in] pItem ~ pointer of the menu item that triggers the menu change
  /// @param[in] pMenu ~ pointer of the menu that "changed"/"updated"
  using event_onSelectionChange = std::function<void(xControl* pControl, int index)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onSelectionChange mOnSelectionChange = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a change event (applicable to dropdown/combobox)
  virtual void setOnSelectionChange(event_onSelectionChange callback) = 0;
};

// forward declaration ...
class xTextBox;

/**
  * @class   iTextChangeEventListener
  * @brief 	 Text-change-event listener intreface class
  *          
  * @details `iTextChangeEventListener` declares function signatures for textbox change events
  * 
  * @todo     Investigate a way to merge this interface with general "change",
  *           for re-usability, since the functionality is similar for all ...
  *  
  */
class iTextChangeEventListener {

protected:

  /// @remark    alias for void(*)(...) function pointer signature
  /// @param[in] pTextBox ~ pointer of the textbox that triggers the text change action
  /// @param[in] text ~ copy of the textbox's current held internal string value
  using event_onTextChange = std::function<void(xTextBox* pTextbox, std::string text)>;

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onTextChange mOnTextChange = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a change event (applicable to textbox)
  virtual void setOnTextChange(event_onTextChange callback) = 0;
};

// #include "../peripherals/Key.h" // included in xApp.h

class iKeyEventListener {

protected:

  /**
   * @defgroup events Key Events
   * @remark alias for void(*)(...) function pointer signature
   * @{
   */

  /// @param[in] pWidget ~ pointer of widget that receives the key-press callback
  /// @param[in] key ~ Key enum constant for client-side comparison
  /// @remarks   Key enums include `ARROW`, `EXTRA`, `F`, `NUMPAD`
  using event_onKeyPress = std::function<void(xWidget* pWidget, Key key)>;

  /// @param[in] pWidget ~ pointer of widget that receives the key-release callback
  /// @param[in] key ~ Key enum constant for client-side comparison
  /// @remarks   Key enums include `ARROW`, `EXTRA`, `F`, `NUMPAD`
  using event_onKeyRelease = std::function<void(xWidget* pWidget, Key key)>;

  /* @} */

protected:

  /// @remark function pointer to client-defined callback method/event triggers
  event_onKeyPress mOnKeyPress = nullptr;
  event_onKeyRelease mOnKeyRelease = nullptr;

protected:

  /// @brief pure virtual abstract interface class method
  /// to attach a close event (applicable to windows/frames)
  virtual void setOnKeyPress(event_onKeyPress callback) = 0;
  virtual void setOnKeyRelease(event_onKeyRelease callback) = 0;
};


#endif // end of xEVENT_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
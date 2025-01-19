/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xPopupMenu.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This file contains the implementation for `xPopupMenu`
  * 
  * @details 	xPopupMenu.h defines the type for
  *             creating submenu/context menu objects
  */

/// @brief begin of xPOPUPMENU_H implementation
#ifndef xPOPUPMENU_H
#define xPOPUPMENU_H

/**
 * @class      xPopupMenu
 * @brief      Factory class to create `xPopupMenu` objects
 * @details    This class implements the factory design pattern
 *             to ensure proper resource management of `xPopupMenu`
 *             resources derived from `xMenu` <br/>
 * 
 *      This class interfaces with `xMenuManager` ...
 */
class xPopupMenu : public xMenu {

    /// friend-level access to `xPopupMenu` data ...
    friend class xMenuManager;

protected:

    /// @brief protected factory class constructor
    xPopupMenu() { hMenu = CreatePopupMenu(); }

    /// @brief proptected factory class destructor
    ~xPopupMenu () { /* destroy(); */ }

    // virtual void destroy() override {
    //     LOG("releasing xPopup resources ...");
    //     xMenu::destroy();
    // }

public:

    /// @brief public factory class interface method to create `xPopupMenu` objects
    /// @param action ~ optional function pointer for a menu-change callback trigger
    static xPopupMenu* create(
        event_onMenuChange action = nullptr
    ) { return xMenuManager::createPopupMenu(action); }
};

/// implementation of `xMenuManager::createPopupMenu`
xPopupMenu* xMenuManager::createPopupMenu(
    event_onMenuChange action = nullptr
) {
    // create the actual popup menu object ...
    xPopupMenu* popup = new xPopupMenu();
    // store the pointer of the new object
    xMenuManager::storeTempMenu(popup);
    // assign the default/provided click event trigger ...
    popup->setOnMenuChange(action);
    // return pointer to client
    return popup;
}

#endif // end of xPOPUPMENU_H
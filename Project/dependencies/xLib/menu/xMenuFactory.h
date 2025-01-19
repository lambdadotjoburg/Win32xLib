/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xMenuFactory.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This file contains the implementation of `xmenuManager`
  */

/// @brief begin of xMENUFACTORY_H implementation
#ifndef xMENUFACTORY_H
#define xMENUFACTORY_H

#pragma once

// containers for items & menus ...
#include "./xItemManager.h"
#include "./xMenuManager.h"

// items ...
#include "./xMenuItem.h"
#include "./xRadioItem.h"

// menus ...
#include "./xMenu.h"
#include "./xPopupMenu.h"
#include "./xRadioMenu.h"

// this class does not do much work, but merely serves
// as a common interface for creating menu-related objects ...

/**
 * @class      xMenuFactory
 * @brief      Factory class to create menu & menu item objects
 * @details    This class implements the factory design pattern to
 *             ensure proper resource management of `xMenu`s & `xMenuItem`s
 * 
 *      This class interfaces with all menu & menu item (derived) classes
 */
class xMenuFactory {

  /// These `using` delcarations are used by the function signatures below ...
  using event_onMenuItemClick = std::function<void(xMenuItem* pItem, xWidget* pWidget)>;
  using event_onMenuChange = std::function<void(xMenuItem* pItem, xMenu* pMenu)>;

public:

    /// @brief public factory class interface method to create `xMenu` objects
    /// @param action ~ optional function pointer of a menu-change callback trigger
    /// @return ~ pointer of the new created `xMenu` object
    static xMenu* createMenu(event_onMenuChange action = nullptr) {
        return xMenuManager::createMenu(action);
    }

    /// @brief public factory class interface method to create `xPopupMenu` objects
    /// @param action ~ optional function pointer of a menu-change callback trigger
    /// @return ~ pointer of the new created `xPopupMenu` object
    static xPopupMenu* createPopupMenu(event_onMenuChange action = nullptr) {
        return xMenuManager::createPopupMenu(action);
    }

    /// @brief public factory class interface method to create `xRadioMenu` objects
    /// @param action ~ optional function pointer of a menu-change callback trigger
    /// @return ~ pointer of the new created `xRadioMenu` object
    static xRadioMenu* createRadioMenu(event_onMenuChange action = nullptr) {
        return xMenuManager::createRadioMenu(action);
    }

    /// @brief public factory class interface method to create `xMenuItem` objects
    /// @param action ~ optional function pointer of a menu-item=click callback trigger
    /// @return ~ pointer of the new created `xMenuItem` object
    static xMenuItem* createMenuItem(
        const std::string& text = "",
        event_onMenuItemClick action = xMenuItem::defaultMenuItemClickEvent
    ) {
        return xItemManager::createMenuItem(text, action);
    }

    /// @brief public factory class interface method to create `xRadioItem` objects
    /// @return ~ pointer of the new created `xRadioItem` object
    static xRadioItem* createRadioItem(const std::string& text) {
        return xItemManager::createRadioItem(text);
    }
};

#endif // end of xMENUFACTORY_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
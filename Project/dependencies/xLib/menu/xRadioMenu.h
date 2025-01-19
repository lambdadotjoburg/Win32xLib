/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xRadioMenu.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This file contains the implementation for `xRadioMenu`
  * 
  * @details 	xPopupMenu.h defines the type for
  *             creating submenu/context menu objects
  */

/// @brief begin of xRADIOMENU_H implementation
#ifndef xRADIOMENU_H
#define xRADIOMENU_H

/**
 * @class      xRadioMenu
 * @brief      Factory class to create `xRadioMenu` objects
 * @details    This class implements the factory design pattern
 *             to ensure proper resource management of `xRadioMenu`
 *             resources derived from `xMenu` <br/>
 * 
 *      This class interfaces with `xMenuManager` ...
 */
class xRadioMenu : public xMenu {

    // firend-level access to `xRadioMenu` data ...
    friend class xMenuManager;

    // friend-level access to make use of `selectedItemID`
    friend class xRadioItem;

    /// @brief variable to store the currently selected item in the radio menu
    int selectedItemID = -1;

protected:

    /// @brief protected factory pattern constructor
    xRadioMenu () { hMenu = CreatePopupMenu(); }

    /// @brief protected factory pattern destructor
    ~xRadioMenu() { /* destroy(); */ }

    // virtual void destroy() override {
    //     LOG("releasing xRadioMenu resources ...");
    //     std::cout << "Clean up radio menu resources" << std::endl;
    //     xMenu::destroy();
    // }

    /// @brief helper method to change/update the currently selected item
    /// @param item ~ pointer of the item receiving the callback/action trigger
    void selectRadioItem(xRadioItem* item) {
        // retrieve the currently selected item, before change ...
        xRadioItem* pPrevItem = getSelectedItem();
        // check that the selected item is not the same
        // as the one being selected & that it exists (not nullptr) ...
        if (pPrevItem && item != pPrevItem) {
            // if so, just toggle the check state OFF
            pPrevItem->setCheckState(false);
        }
        // toggle the state of the new item,
        // i.e. true if false & false if true
        item->toggleCheckState();
        // if the item is selected as true,
        if (item->getCheckState()) {
            // ensure that the item is marked
            // as selected as the currently selected item ...
            selectedItemID = item->getID(); // store the selected ID
        }
        // invoke menu change event ...
        // this is done as a client-callback function ...
        xMenu* p = item->parent;
        
        // trigger mOnMenuChange ...
        // only after all changes have been done to the radio menu ...
        if (p->mOnMenuChange) {
            p->mOnMenuChange(item, item->parent);
        }
    }

public:

    /// @brief public factory class interface method to create `xRadioMenu` objects
    /// @param action ~ optional function pointer for a menu-change callback trigger
    static xRadioMenu* create(
        event_onMenuChange action = nullptr
    ) { return xMenuManager::createRadioMenu(action); }

    // void add(std::vector<xRadioItem*> item, int index = -1) {
    //     // ...
    // }

    /// @brief  method to retrieve the currently selected item
    /// @return pointer of the currently selected item
    xRadioItem* getSelectedItem() {
        // return xItemManager::getItemByID(selectedItemID);
        return static_cast<xRadioItem*>(getItemByID(selectedItemID));
    }

    /// @brief  method to retrieve the ID of the currently selected item
    /// @return integer representative of the item's ID (assigned by item factory)
    int getSelectedItemID() { return selectedItemID; }

    /// @brief   method to set/change/update the check state of a menu item
    /// @details at MOST ONE item can be selected at a time <br/>
    ///          This method is implemented, specifically
    ///          to "override" base class behaviour, i.e.
    ///          to avoid multiple menu-change triggers!
    void setCheckState(bool flag);

    /// @brief public interface method to add multiple items to the menu (all at once)
    /// @param selected ~ index/position of the item selected by default when adding the items
    void add(std::vector<xRadioItem*> radioItems, int selected = -1) {
        for (size_t i = 0; i < radioItems.size(); i++) {
            this->add(radioItems[i]);
            if ((size_t) selected == i) {
                radioItems[i]->setCheckState(true);
            }
        }
    }

    /// @brief public interface method to add a radio item to the menu
    /// @param index ~ the position at which to add the item to the menu
    void add(xRadioItem* radioItem, int index = -1) {
        xMenu::add(radioItem, index);
    }
};

/// implementation of `xMenuManager::createRadioMenu(...)`
xRadioMenu* xMenuManager::createRadioMenu(
    event_onMenuChange action = nullptr
) {
    xRadioMenu* menu = new xRadioMenu();
    xMenuManager::storeTempMenu(menu);
    menu->setOnMenuChange(action);
    return menu;
}

/// implementation of `xRadioItem::setCheckState(...)`
void xRadioItem::setCheckState(bool flag) {

    // invoke "base class method" as normal ...
    // `xMenuItem::setCheckState(flag)`

    checkstate = flag;

    mInfo.fMask |= MIIM_STATE;

    if (checkstate) {
        mInfo.fState |= MFS_CHECKED;
        mInfo.fState &= ~MFS_UNCHECKED;
    } else {
        mInfo.fState &= ~MFS_CHECKED;
        mInfo.fState |= MFS_UNCHECKED;
    }

    // ensure to update the selected item ID for the menu
    xRadioMenu* parentMenu = static_cast<xRadioMenu*>(this->parent);
    
    if (checkstate) {
        // get the currently selected item ...
        xRadioItem* pPrevItem = parentMenu->getSelectedItem();
        // check if not nullptr
        if (pPrevItem) {
            // uncheck the item ...
            pPrevItem->setCheckState(false);
            // pPrevItem->checkstate = false;
        }
        
        // update (parent) menu's selected item ID
        parentMenu->selectedItemID = this->mID;
        
    } else {

        // if the same item is unchecked, ensure
        // that no items are selected in parent menu ...
        parentMenu->selectedItemID = -1;
    }

    // update always triggers `mOnMenuChange(...)`
    // which is exactly why this method
    // is implemented in the first place
    // to bypass double-menu-change triggers ...
    update();
}

// implementation after `xMenu` for access to `hMenu`
void xRadioItem::update() {
    if (!parent) { return; }
    // check if item's parent exists
    // this is necessary, since update()
    // can be triggered before associating to a menu ...
    if (parent->hMenu) {
        // update the Win32 menu
        SetMenuItemInfo(parent->hMenu, mID, FALSE, &mInfo);
    }
    // remove mOnMenuChange from here ...
    // move it to selectRadioItem(...)
}

#endif // end of xRADIOMENU_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
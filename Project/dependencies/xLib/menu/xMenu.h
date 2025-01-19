/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xMenu.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This class contains the implementation for `xMenu`
  * 
  * @details 	xMenu.h contains quite a bit of forward declaration,
  *             friend-class declarations & implementation from other
  *             menu-related classes since these classes interface
  *             with one another!
  */

#pragma once

/// @brief begin of xMENU_H implementation
#ifndef xMENU_H
#define xMENU_H

/**
 * @class    xMenu
 * @brief   `xMenu` defines the class & public interface members
 *           for creating interacting with `xMenu` objects
 * @details  This class implements the factory design pattern
 *           to ensure proper resource management
 *           of `xMenu` resources
 */
class xMenu : public iMenuChangeEventListener {

    // provide friend-level access for using `hMenu`
    friend class xMenuItem;
    
    // provide friend-level access for constructor
    // & other important `xMenu` data ...
    friend class xMenuManager;

    // provide friend-level access for using `mOnMenuChange` ...
    friend class xRadioMenu;

    // provide friend-level access for using `hMenu` ...
    friend class xRadioItem;

    // provide friend-level access to assign frame owner (context menu) ...
    friend class xFrame;

    // friend-level access to assign widget owner (menu bar) ...
    friend class xWidget;

protected:

    /// @brief variable to store the Win32 menu
    /// handle associated to this menu object
    HMENU hMenu = nullptr;

    /// @brief variable to keep track of owner widget (if any)
    xWidget* owner = nullptr;

    /// @brief variable to keep track of parent menu (if any)
    xMenuItem* parent = nullptr;

    /// @brief container to store all items associated to this menu instance
    std::vector<xMenuItem*> items;

    /// @brief protected factory-pattern-style constructor
    xMenu() { hMenu = CreateMenu(); }

    /// @brief protected factory-patten destructor
    virtual ~xMenu() { destroy(); }

    /// @brief destroy method to correctly free/release instance resources
    virtual void destroy();

public:

    /// @brief overloaded indexing operator
    /// to retrieve menu items by index!
    /// @remark Not all that useful, because of the
    /// factory pattern design decision/implementation
    xMenuItem* operator [] (int index) {
        return getItemByIndex(index);
    }

public:

    #ifndef NDEBUG
    /// @brief friend ostream method for streaming menu object data to console (DEBUG)
    friend std::ostream& operator << (std::ostream& os, const xMenu& menu);
    #endif

    /// @brief interface override method to set the menu-click action/callback function
    virtual void setOnMenuChange(event_onMenuChange callback) override {
        mOnMenuChange = std::move(callback);
    }

    #ifndef NDEBUG
    /// @brief derived-class specific override method
    ///        to get the type of this object as string
    virtual std::string getType() const { return type(*this); }
    #endif

    /// @brief  method to get the size/count of the menu,
    ///         i.e. number of items having this as parent
    /// @return integer representative of the number of items
    int size() const { return items.size(); }

    /// @brief     method to check whether an item
    ///            has this menu as a child item
    /// @param[in] item ~ pointer of the item to check
    bool checkItem(xMenuItem* item) {
        if (!item) { return false; }
        std::vector<xMenuItem*>::iterator i;
        i = std::find(items.begin(), items.end(), item);
        if (i != items.end()) {
            return true;
        } else { return false; }
    }

    /// @brief public factory method to create an `xMenu`
    ///        object & return a pointer to client-code
    /// @param action ~ optional function pointer for on-menu-change callback
    /// @return `xMenu*` pointer of the manufactured object for use by client-code
    static xMenu* create(event_onMenuChange action = nullptr) {
        return xMenuManager::createMenu(action);
    }

    /// @brief     method to insert items into this menu, all at once
    /// @param[in] menuItems ~ vector container of the items to insert
    void add(std::vector<xMenuItem*> menuItems) {
        for (size_t i = 0; i < menuItems.size(); i++) {
            this->add(menuItems[i]);
        }
    }

    /// @brief method to insert a single menu item into the container
    /// @param item ~ pointer of the item to insert
    /// @param index ~ optional index/position at which to insert the item
    /// @note  `xMenuItems` having text data "separator" are separators (by default)
    /// @remark This can be undone, by invoking `separator(false)` on the item!
    ///         However, this attribute is useful, since it allows for re-using
    ///         the separator menu item object multiple times (duplicate)!
    void add(xMenuItem* item, int index = -1) {
        // if item is `nullptr`
        if (!item) { return; }
        
        // if "separator" then skip these checks ...
        // since a separator can be duplicated / inserted mulitple times ...
        if (item->getText() != "separator") {
            // check if item is contained in `xItemManager` temp ...
            // since if not, the item was not created correctly, via the manager ...
            if (!xItemManager::checkTempItem(item)) { return; }
            // check if this item has a parent ...
            if (item->parent) { return; }
            // check if this item is already contained in `xItemManager`s items ...
            // since if so, it may not been removed from items
            // & may still be used by another menu ...
            if (xItemManager::checkItem(item)) { return; }
            // check if this item already exists in the menu ...
            // i.e. no duplicate menu item elements allowed ...
            if (this->checkItem(item)) { return; }
        } // otherwize ... 

        // get the size of the menu ...
        int s = this->size();
        // check whether the specified index is sensible & update if not ...
        int i = (index < 0 || index > s ? s : index);
        // insert the item at the specified index 
        items.insert(items.begin() + i, item);
        // ensure that the item is given `this` as parent ...
        item->parent = this;
        // store the item in xItemManager::items ...
        xItemManager::storeItem(item);
        // insert the item into the menu display ...
        InsertMenuItem(
            hMenu,
            index,
            TRUE,
            &(item->mInfo)
        );
    }

    /// @brief method to remove an item from this menu, using
    ///        as argument the pointer* of the menu item
    void remove(xMenuItem* item);

    /// @brief method to remove an item from this menu,
    ///        using as argument the menu item's ID
    void removeItemByID(int ID);

    /// @brief method to remove an item from this menu,
    ///        using as argument its index in the menu
    void removeItemByIndex(int index);

    /// @brief method to remove an item from this menu,
    ///        using as argument the text of the menu item
    /// @param text ~ the text to use to find & remove the item
    void removeItemByText(const std::string& text);

    /// @brief  method to get/retrieve a menu item from this menu by ID
    /// @return pointer of the menu item or `nullptr` if NOT
    xMenuItem* getItemByID(int ID);

    /// @brief  method to get/retrieve a menu item from this menu by index
    /// @return pointer of the menu item or `nullptr` if NOT
    xMenuItem* getItemByIndex(int index);

    /// @brief  method to get/retrieve a menu item from this menu by text
    /// @return pointer of the menu item or `nullptr` if NOT
    xMenuItem* getItemByText(const std::string& text);
};

#ifndef NDEBUG
/// @brief friend ostream operator overload for streaming
///        menu object data to the console (DEBUG)
std::ostream& operator << (std::ostream& os, const xMenu& menu) {

    os << "\n";

    os << "type: " << menu.getType() << "; ";
    os << "size: " << menu.size() << "; ";
    os << "parent: " << (menu.parent != nullptr) << "; ";
    os << "owner: " << (menu.owner != nullptr) << ";";

    os << "\n\n";
    
    os << "items: \n";
    
    for (size_t i = 0; i < menu.items.size(); i++) {
        os << "\t" << *menu.items[i] << "\n";
    }

    return os;
}
#endif

#ifndef NDEBUG
/// @brief friend ostream operator overload for streaming
///        menu item object data to the console (DEBUG)
std::ostream& operator << (std::ostream& os, const xMenuItem& item) {
    
    os << "ID: " << item.mID << "; ";
    os << "index: " << item.getIndex() << "; ";
    os << "text: " << item.getText() << "; ";
    os << "enable: " << item.active << "; ";
    os << "parent: " << (item.parent != nullptr) << "; ";
    os << "click: " << (item.mOnMenuItemClick != nullptr) << "; ";
    os << "check: " << item.checkstate << "; ";

    // fix this when refactoring `xImage` & image-related controls ...
    os << "\n\n";
    os << "\t\t"; item.streamImageData(os, item);

    if (item.pSubMenu) {
        os << "\n\n";
        os << "\t" << "submenu: " << "\n";
        os << "\t\t" << *item.pSubMenu << "\n";
    }

    return os;
}
#endif

/// implementation for `xMenuItem::getIndex(...)`
int xMenuItem::getIndex() const {
    // loop over the menu's vector container of items ...
    for (size_t i = 0; i < parent->items.size(); i++) {
        // compare for item object equality ...
        if (this == parent->items[i]) {
            return i; // index of item in parent's items vector ...
        }
    }
    return -1; // not found
}

/// implementation for `xManager::createMenu(...)`
xMenu* xMenuManager::createMenu(
    event_onMenuChange action = nullptr
) {
    xMenu* menu = new xMenu();
    xMenuManager::storeTempMenu(menu);
    menu->setOnMenuChange(action);
    return menu;
}

/// implementation for `xManager::storeMenu(...)`
void xMenuManager::storeMenu(xMenu* menu) {
    if (!menu) { return; }
    menus.insert(menu);
    removeTempMenu(menu);
}

/// implementation for `xManager::storeTempMenu(...)`
void xMenuManager::storeTempMenu(xMenu* menu) {
    if (!menu) { return; }
    temp.insert(menu);
}

/// implementation for `xManager::removeMenu(...)`
void xMenuManager::removeMenu(xMenu* menu) {
    if (!menu) { return; }
    if (menus.find(menu) != menus.end()) {
        // #ifndef NDEBUG
        // std::cout << *menu << std::endl;
        // #endif
        menu->destroy();
        menus.erase(menu);
    }
}

/// implementation for `xManager::removeTempMenu(...)`
void xMenuManager::removeTempMenu(xMenu* menu) {
    if (!menu) { return; }
    if (temp.find(menu) != temp.end()) {
        temp.erase(menu);
    }
}

/// implementation for `xManager::checkMenu(...)`
bool xMenuManager::checkMenu(xMenu* menu) {
    if (!menu) { return false; }
    if (menus.find(menu) != menus.end()) {
        return true;
    } else { return false; }
}

/// implementation for `xManager::checkTempMenu(...)`
bool xMenuManager::checkTempMenu(xMenu* menu) {
    if (!menu) { return false; }
    if (temp.find(menu) != temp.end()) {
        return true;
    } else { return false; }
}

/// implementation for `xMenuItem::update()`
/// requires access to `hMenu`
void xMenuItem::update() {
    if (!parent) { return; }
    // check if item's parent exists <br/>
    // This is necessary, since `update()`
    // can be triggered (in some cases)
    // before associating to a menu ...
    if (parent->hMenu) {
        // update the Win32 menu ...
        SetMenuItemInfo(parent->hMenu, mID, FALSE, &mInfo);
        // triggere `mOnMenuChange` event handler, if set ...
        if (parent->mOnMenuChange) {
            parent->mOnMenuChange(this, this->parent);
        }
    }
}

/// implementation for `xMenuItem::add(...)`
void xMenuItem::add(xMenu* submenu) {
    // check if the same menu used or `nullptr`
    /// @todo
    /// Handle exceptions/throw to inform client-code
    /// that the menu pointer is null or points to
    /// the same menu as currently being used ...
    if (submenu == pSubMenu || submenu == nullptr) { return; }
    // check whether the menu is already in use ...
    /// @todo
    /// Handle exceptions/throw to inform client-code
    /// that the menu is ALREADY in USE!
    if (xMenuManager::checkMenu(submenu)) { return; }
    // otherwize, hold on to the previous menu ...
    xMenu* pPrevSubMenu = pSubMenu;
    // set the new menu for the frame ....
    pSubMenu = submenu;
    // store the new menu in `xMenuManager` ...
    xMenuManager::storeMenu(pSubMenu);
    // delete the previous menu, if it exists ...
    if (pPrevSubMenu) {
        pPrevSubMenu->parent = nullptr;
        xMenuManager::removeMenu(pPrevSubMenu);
    }
    // ensure the submenu's parent is this ...
    pSubMenu->parent = this;
    // do some win32 stuff ...   
    mInfo.fMask |= MIIM_SUBMENU;
    mInfo.hSubMenu = pSubMenu->hMenu;
    // reflect the changes in the menu ...
    update();
}

/// implementation for `xMenu::destroy(...)`
void xMenu::destroy() {

    // log data for debugging purposes ...
    LOG("releasing xMenu resources ...");

    int size = items.size();

    // std::cout << "items.size() before => " << items.size() << std::endl;
    for (int i = size-1; i >= 0; i--) {
        // items[i]->destroy();
        // std::cout << "index => " << i << std::endl;
        xItemManager::removeItem(items[i]);
        items.pop_back(); // remove from menu's items vector ...
    }

    if (this->owner) {
        // std::cout << "this->owner" << std::endl;
        this->owner = nullptr;
    }

    if (this->parent) {
        // std::cout << "this->parent" << std::endl;
        this->parent = nullptr;
    }

    // only destroy if once submenus are destroyed
    if (this->hMenu) {
        // std::cout << "this->hMenu" << std::endl;
        DestroyMenu(hMenu);
    }
}

/// implementation for `xMenuItem::destroy(...)`
void xMenuItem::destroy() {    
    
    // Log data for debugging purposes ...
    LOG("releasing xMenuItem resources ...");    

    if (hBitmapCheck) {
        // std::cout << "hBitmapCheck" << std::endl;
        DeleteObject(hBitmapCheck);
    }

    if (hBitmapUncheck) {
        // std::cout << "hBitmapUncheck" << std::endl;
        DeleteObject(hBitmapUncheck);
    }

    // remove the item from the menu ...
    // removes a menu item but does not free resources ...
    RemoveMenu(parent->hMenu, mID, MF_BYCOMMAND);

    // destroys submenu recursively & frees resources ...
    // for Win32 builds only, not applicable to this design ...
    // DeleteMenu(hMenu, );
    
    // destroy submenu ...
    if (pSubMenu) {
        // std::cout << "pSubMenu" << std::endl;
        xMenuManager::removeMenu(pSubMenu);
    }
}

/// implementation for `xMenuItem::remove(...)`
void xMenuItem::remove(xMenu* submenu) {
    
    if (!submenu) { return; }

    xMenuManager::removeMenu(submenu);
    
    // check if menu has parent menu ...
    if (submenu->parent) {

        // check whether menu's parent has submenu ...
        if (submenu->parent->pSubMenu) {
            // in which case, set it to `nullptr`
            // i.e. this (sub)menu's parent no longer
            // refers to this menu as submenu ...
            submenu->parent->pSubMenu = nullptr;
        }
    }

    // set this menu's submenu to `nullptr`
    // since it was already removed above ...
    submenu = nullptr;
}

/// implementation for `xMenu::remove(...)`
void xMenu::remove(xMenuItem* item) {

    if (!item) { return; }

    xItemManager::removeItem(item);

    // remove item from it's parent's items vector ...
    std::vector<xMenuItem*>::iterator i;
    i = std::find(items.begin(), items.end(), item);
    if (i != items.end()) {
        items.erase(i);
    }

    // remove the item's parent pointer ...
    item->parent = nullptr;
    // ensure the item's pointer is set to `nullptr`
    item = nullptr;
}

/// implementation for `xMenu::removeByID(...)`
void xMenu::removeItemByID(int ID) {
    remove(getItemByID(ID));
}

/// implementation for `xMenu::removeByIndex(...)`
void xMenu::removeItemByIndex(int index) {
    remove(getItemByIndex(index));
}

/// implementation for `xMenu::removeByText(...)`
void xMenu::removeItemByText(const std::string& text) {
    remove(getItemByText(text));
}

/// implementation for `xMenu::getItemByIndex(...)`
xMenuItem* xMenu::getItemByIndex(int index) {
    // get the item by ID from this menu's items vector
    return items[index];
}

/// implementation for `xMenu::getItemByID(...)`
xMenuItem* xMenu::getItemByID(int ID) {
    // get the item by ID from this menu's items vector
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]->getID() == ID) {
            return items[i];
        }
    }
    return nullptr;
}

/// implementation for `xMenu::removeByText(...)`
xMenuItem* xMenu::getItemByText(const std::string& text) {
    // get the item by ID from this menu's items vector
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]->getText() == text) {
            return items[i];
        }
    }
    return nullptr;
}

#ifndef NDEBUG
/// implementation for `xMenuManager::EnumerateMenus(...)` [DEBUG]
void xMenuManager::EnumerateMenus() {
    std::set<xMenu*>::iterator i;
    for (i = menus.begin(); i != menus.end(); i++) {
        // dereference the iterator to get the pointer of the `xMenu`
        xMenu* pMenu = *i;
        if (pMenu) {
            // dereference the menu pointer to ostream the data ...
            std::cout << *pMenu << std::endl;
        }
    }
}
#endif

#ifndef NDEBUG
/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
void xMenuManager::EnumerateTempMenus() {
    std::set<xMenu*>::iterator i;
    for (i = temp.begin(); i != temp.end(); i++) {
        // dereference the iterator to get the pointer of the `xMenu`
        xMenu* pMenu = *i;
        if (pMenu) {
            // dereference the menu pointer to ostream the data ...
            std::cout << *pMenu << std::endl;
        }
    }
}
#endif

#endif // end of xMENU_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
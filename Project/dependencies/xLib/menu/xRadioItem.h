/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xMenuItem.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This file contains the implementation for `xMenuItem`
  * 
  * @details 	xMenuItem.h contains quite a bit of forward declaration,
  *             friend-class declarations & implementation from other
  *             menu-related classes since these classes interface
  *             with one another!
  */

/// @brief begin of xRADIOITEM_H implementation
#ifndef xRADIOITEM_H
#define xRADIOITEM_H

/**
 * @class      xRadioItem
 * @brief      Factory class to create `xRadioItem` objects
 * @details    This class implements the factory design pattern
 *             to ensure proper resource management of `xRadioItem`
 *             resources derived from `xMenuItem` <br/>
 * 
 *      This class interfaces with `xItemManager` ...
 *      & implements `iMenuItemClickEventListener`
 */
class xRadioItem : public xMenuItem {

    // friend level access for access to item manager interface methods
    friend class xItemManager;

    // friend level access for access to `mOnMenuChangeEvent` (interface class)
    friend class xRadioMenu;

    // this method is associated to a radio button item by default
    // in addition to client's menu item click event handler ...
    // This event cannot be changed since it's meant to ensure
    // that the radio buttons are toggled correctly ...

    /// @brief   default method associated to radio item click events
    /// @details This method ensures that the radio menu toggle mechanism functions correctly
    /// @param pItem ~ pointer of the menu item triggering the callback/action
    /// @param pWidget ~ pointer of the widget holding the menu containing the item
    static void defaultRadioItemClickEvent(xMenuItem* pItem, xWidget* pWidget);

protected:

    /// @brief override base class method to ensure updates
    ///        are properly handled when item-click detected,
    ///        specifically, to ensure `mOnMenuChange` is invoked
    virtual void update() override;

    /// @brief protected factory class constructor
    xRadioItem(int ID, const std::string& text)
        : xMenuItem(ID, text)
        {
        // ensure the radio button behaves as a radio button ...
        this->mInfo.fType = MFT_RADIOCHECK;
        // give radio buttons default on menu item click event to toggle radio option ...
        // this->setOnMenuItemClick(defaultRadioButtonClickEvent);
    }

    /// @brief protected factory class destructor
    ~xRadioItem() { /* destroy(); */ }

    // virtual void destroy() {
    //     LOG("releasing xRadioItem resources ...");
    //     xMenuItem::destroy();
    // }

public:

    /// @brief shorthand method to toggle the check-state of an item
    /// @todo  move this function declaration to base class `xMenuItem`
    void toggleCheckState() { setCheckState(!getCheckState()); }

    /// @brief derived class specific override method
    ///        to set the check state of a radio item,
    ///        specifically, since additional logic is
    ///        to be performed after toggling the base
    ///        class check state ...
    void setCheckState(bool flag) override;

    /// @brief factory class interface method to create `xRadioItem` objects
    static xRadioItem* create(const std::string& text) {
        return xItemManager::createRadioItem(text);
    }
};

/// implementation of `xItemManager::createRadioItem(...)`
xRadioItem* xItemManager::createRadioItem(const std::string& text) {
    // sum of used & temp items ...
    int size = xItemManager::items.size() + xItemManager::temp.size();

    // increment the size by 1 => mID cannot be 0
    // 0 indicated menu item selection cancelled
    xRadioItem* item = new xRadioItem(++size, text);

    // store the item in temp container ...
    xItemManager::storeTempItem(item);

    // assign default radio-item callback function ...
    item->setOnMenuItemClick(xRadioItem::defaultRadioItemClickEvent);

    // return pointer to client ...
    return item;
}

#endif // end of xRADIOITEM_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
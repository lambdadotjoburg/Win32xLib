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

/// @brief begin of xMENUITEM_H implementation
#ifndef xMENUITEM_H
#define xMENUITEM_H

#include "../events/iEvent.h"

/**
 * @class      xMenuItem
 * @brief      Factory class to create `xRadioItem` objects
 * @details    This class implements the factory design pattern
 *             to ensure proper resource management
 *             of `xMenuItem` resources <br/>
 * 
 *      This class interfaces with `xItemManager` ...
 *      & implements `iMenuItemClickEventListener`
 * 
 * @implements  iMenuItemClickEventListener
 */
class xMenuItem : public iMenuItemClickEventListener {

    // friend level access for access to item manager interface methods
    friend class xItemManager;

    // friend-level access to parent & other `xMenuItem` data
    friend class xMenu;

    // friend level access for access to `mOnMenuChangeEvent` (interface class)
    friend class xRadioMenu;

    // for access to `defaultMenuItemClickEvent`
    friend class xMenuFactory;

protected:

    /// @brief menu item ID
    /// @details
    /// initially -1 but set to appropriate
    /// positive integer value upon `create(...)`
    /// @note -1 indicates that the item has not yet been created!
    /// @remark 0 is reserved by Win32's WM_COMMAND message system
    /// to indicate that a menu item selection has been cancelled
    int mID = -1;

    /// @brief   struct containing Win32 API C-style menu info data
    /// @details
    /// `MENUITEMINFO` struct associated to the menu item
    /// <br/>
    /// Manipulate data depending on the type of menu item
    /// being created, for example, menu items that have
    /// checkbox/radio-buttons/bitmap images associated to it!
    MENUITEMINFO mInfo { };

    /// @brief variable to store boolean state of whether the menu item is enabled/disabled
    bool active = true;

    /// @brief variable to store boolean state of whether the menu item is checked/unchecked
    bool checkstate = false;

    /// @brief vairable to store boolean stat of whether the menu item acts as a separator or not
    bool linebreak = false;

    /// @brief variable storing the pointer of a parent menu,
    ///        i.e. the menu to which this item belongs (if any)
    xMenu* parent = nullptr;

    /// @brief variable storing the submenu associated to this menu item (if any)
    xMenu* pSubMenu = nullptr;

    /// @brief variable holding the menu item's string text
    /// @note UNICODE/ASCII compatible
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstring mText = L""; // default empty wide string
    #else
    std::string mText = ""; // default empty narrow string
    #endif

    /** @defgroup images Image Data */
    /** @{ */
    /// @brief variable holding the image file path associated to the menu item's icon
    /// @note  This member to be removed when refactoring xImage class & image-related controls!
    std::string checkImagePath = "";
    std::string uncheckImagePath = "";
    /// @brief variable holding the check-state bitmap image handle
    HBITMAP hBitmapUncheck = nullptr;
    HBITMAP hBitmapCheck = nullptr;
    /** @} */

protected:

    /// @brief default method associated to `xMenuItem::create(...)`
    /// @details
    /// Enabling client-code to associate an item click callback when
    /// manufacturing the `xMenuItem` object (for convenience & readability) <br/>
    /// if no client-defined callback is provided, then this default method is used!
    static void defaultMenuItemClickEvent(
        xMenuItem* pItem,
        xWidget* pWidget
    );

    /// @brief protected factory-pattern constructor
    xMenuItem(int ID, const std::string& text)
        : mID{ID}
        {
        initMenuInfo();
        setText(text);
        separator(linebreak);
        setEnable(active);
        setCheckState(checkstate);
    }

    /// @brief helper method to initialize menu item's
    /// Win32 C-style `MENUITEMINFO` struct data
    void initMenuInfo() {
        mInfo.cbSize = sizeof(MENUITEMINFO);
        mInfo.fMask = MIIM_TYPE | MIIM_ID;
        mInfo.wID = mID;
    }

    /// @brief methot to trigger menu-item updates for the menu's view
    virtual void update(); // implementation after `xMenu`

    /// @brief protected factory-pattern destructor
    virtual ~xMenuItem() { destroy(); }

    /// @brief method to destroy the menu item
    /// along with all its associated resources
    virtual void destroy();

    /// @brief helper (internal) method to load an image
    /// associated to the menu item, given its path & dimensions
    bool LoadImage(
        const std::string& path,
        HBITMAP& hBitmap,
        int w, int h
    ) {
        
        // load the bitmap image from a file ...
        Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(
            StrConverter::StringToWString(path).c_str()
        );

        // extract the bitmap from Gdiplus::Bitmap* with transparent background ...
        pBitmap->GetHBITMAP(Gdiplus::Color(0,0,0,0), &hBitmap);

        // if any of w or h -1 then use original image dimensions, otherwize ...
        // create a new bitmap image using provided dimensions & overwrite previous ...
        if (w != -1 || h != -1) {
            
            // create a new image with scaled dimensions w & h ...
            Gdiplus::Bitmap* scaledImage = new Gdiplus::Bitmap(w, h, PixelFormat32bppARGB);

            // prepare for overwriting the image ...
            Gdiplus::Graphics graphics(scaledImage);
            // using high quality bicubic bitmap object ...
            graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

            // overwrite the original bitmap object ...
            graphics.DrawImage(pBitmap, 0, 0, w, h);

            // get the handle of the new (overwritten) bitmap image
            // & save this handle into the referenced bitmap ...
            scaledImage->GetHBITMAP(Gdiplus::Color(0,0,0,0), &hBitmap);

            // free the scaled bitmap resources ...
            delete scaledImage;
            scaledImage = nullptr;
        }

        // free the original bitmap resources ...
        delete pBitmap;
        pBitmap = nullptr;

        return (hBitmap ? true: false);
    }

public:

    /// @brief  implementation `getIndex()` of a menu item
    /// @return integer representative of the index of the item
    ///         (relative to its parent)
    int getIndex() const; // implementation after `xMenu`

    /// @brief     interface method for adhering to factory pattern
    /// @details   The menu item is stored in the temporary
    ///            (unordered) map, until an `xMenu`/`xMenuItem`
    ///            object adds it to it's items list
    /// @param[in] text ~ optional text to use for the menu item
    ///            default text value is "" (empty string
    /// @param[in] action ~ optional callback function to
    ///            trigger when menu item click detected
    /// @return   `xPopupMenu*` pointer to the menu object
    /// @remark    if text is empty string then
    ///           `setText(...)` is skipped <br.>
    ///            if text is "separator" then
    ///            the menu item is set as a
    ///            menu item separator/line break
    ///            by default. This can be toggled
    static xMenuItem* create(
        const std::string& text = "",
        event_onMenuItemClick action = xMenuItem::defaultMenuItemClickEvent
    ) {
        /// redirect method call to `xItemManager`s version of create ...
        return xItemManager::createMenuItem(text, action);
    }

    /// @brief iMenuItemClickEventListener override method
    ///        for attaching a menu-item-click-event callback/action
    virtual void setOnMenuItemClick(event_onMenuItemClick callback) {
        mOnMenuItemClick = std::move(callback);
    }

    /// @brief  method to get the parent menu of this menu item
    /// @todo   check naming-conventions for `getParent()` versus `Parent()`
    /// @return pointer of the item's `xMenu` parent
    xMenu* getParent() { return parent; }

    /// @brief method to retrieve the ID of a menu item object,
    ///        i.e. the ID which is assigned by factory pattern
    ///        `xItemManager` upon creating the menu item object
    int getID() { return mID; }

    /// @brief method to update/set/change the text of a menu item object
    void setText(const std::string& text) {
        
        /// check whether the menu item
        /// is to be used as a separator
        if (text == "separator") {
            separator(true);
        }

        /// Enusre UNICODE/ASCII conversion
        /// i.e. wide/narrow string format
        #if defined(UNICODE) && defined(_UNICODE)
        mText = StrConverter::StringToWString(text);
        #else
        mText = text;
        #endif

        /// manipulate the menu item's struct data ...
        mInfo.fType = MF_STRING;
        mInfo.dwTypeData = (LPTSTR) mText.c_str();
        mInfo.cch = mText.length();
        /// ensure the changes reflect in the menu's view
        update();
    }

    std::string getText() const {
        #if defined(UNICODE) && defined(_UNICODE)
        return StrConverter::WStringToString(mText);
        #else
        return mText;
        #endif
    }

    /// @brief method to toggle the boolean state of a menu item
    ///        whether the item is to be a separator or NOT
    /// @param flag ~ boolean flag indicating this state
    void separator(bool flag) {
        linebreak = flag;
        if (linebreak) {
            mInfo.fMask |= MIIM_TYPE;
            mInfo.fType |= MFT_SEPARATOR;
        } else {
            mInfo.fType &= ~MFT_SEPARATOR;
        }
        update();
    }

    /// @brief method to toggle whether the item is enabled/disabled
    void setEnable(bool flag) {
        active = flag;
        if (active) {
            mInfo.fMask |= MIIM_STATE;
            mInfo.fState |= MFS_ENABLED;
            mInfo.fState &= ~MFS_DISABLED;
        } else {
            mInfo.fMask |= MIIM_STATE;
            mInfo.fState |= MFS_DISABLED;
            mInfo.fState &= ~MFS_ENABLED;
        }
        /// ensure the changes are reflected in the menu's view
        update();
    }

    /** @defgroup states Item State */
    /** @brief methods to update the enable/disable state of an item */
    /** @{ */
    void setDisable(bool flag) { setEnable(!flag); }
    /// @brief shorthand method for `setEnable(true)`
    void enable() { setEnable(true); }
    /// @brief shorthand method for `setDisable(true)`
    void disable() { setDisable(true); }
    /** @} */

    /// @brief method to check the enable-state of an item
    bool isEnable() { return active; }
    /// @brief method to check the disable-state of an item
    bool isDisable() { return !active; }

    /// @brief interface method to toggle the check-uncheck state of a menu item
    /// @param flag ~ boolean flag to adjust the check-state on/off
    virtual void setCheckState(bool flag) {
        checkstate = flag;
        mInfo.fMask |= MIIM_STATE;
        if (checkstate) {
            mInfo.fState |= MFS_CHECKED;
            mInfo.fState &= ~MFS_UNCHECKED;
        } else {
            mInfo.fState &= ~MFS_CHECKED;
            mInfo.fState |= MFS_UNCHECKED;
        }
        update();
    }

    /// @brief shorthand method for `setCheckState(true)`
    void check() { setCheckState(true); }
    /// @brief shorthand method for `setCheckState(false)`
    void uncheck() { setCheckState(false); }

    /// @brief method to inspect the check-state of a menu item
    bool getCheckState() { return checkstate; }

    /// @brief     method to set the checkbox image for an item in check-state
    /// @param[in] path ~ The file path of the image to load
    /// @param[in] w ~ integer representative of the width of the image (default 15)
    /// @param[in] h ~ integer representative of the height of the image (default 15)
    /// @remark    if any of dimensions w or h are -1 then the image accepts the original size
    void setCheckStateImage(
        const std::string& path,
        int w = 15, int h = 15
    ) {

        /// @todo do some error checking & exception handling ...
        checkImagePath = path; // store the image path ...
        
        /// @todo implement exception handling ...
        /// check whether loaded successfull ...
        LoadImage(checkImagePath, hBitmapCheck, w, h);
        
        // check whether the bitmap handle is valid ...
        if (hBitmapCheck) {
            mInfo.fMask |= MIIM_CHECKMARKS;
            // set the image at check state ...
            mInfo.hbmpChecked = hBitmapCheck;
        } else {
            mInfo.fMask &= ~MIIM_CHECKMARKS;
        }
        update();
    }

    /// @brief     method to set the checkbox image for an item in uncheck-state
    /// @param[in] path ~ The file path of the image to load
    /// @param[in] w ~ integer representative of the width of the image (default 15)
    /// @param[in] h ~ integer representative of the height of the image (default 15)
    /// @remark    if any of dimensions w or h are -1 then the image accepts the original size    
    void setUncheckStateImage(
        const std::string& path,
        int w = 15, int h = 15
    ) {
        // store the image path ...
        // do some error checking & exception handling ...
        uncheckImagePath = path;

        // check whether loaded successfull ...
        // implement exception handling ...
        LoadImage(uncheckImagePath, hBitmapUncheck, w, h);
        // check whether the bitmap handle is valid ...
        if (hBitmapUncheck) {
            mInfo.fMask |= MIIM_CHECKMARKS;
            // set the image at check state ...
            mInfo.hbmpUnchecked = hBitmapUncheck;
        } else {
            mInfo.fMask &= ~MIIM_CHECKMARKS;
        }
        update();
    }

    /// @brief     method to set the checkbox image for an item regardless of check-state
    /// @param[in] path ~ The file path of the image to load
    /// @param[in] w ~ integer representative of the width of the image (default 15)
    /// @param[in] h ~ integer representative of the height of the image (default 15)
    /// @remark    if any of dimensions w or h are -1 then the image accepts the original size
    void setImage(
        const std::string& path,
        int w = 15, int h = 15
    ) {
        // invoke `setCheckStateImage`
        setCheckStateImage(path, w, h);
        // invoke `setUncheckStateImage`
        setUncheckStateImage(path, w, h);
        // with the same data ...
    }

    /// @brief  method to retrieve the image path for the item's 
    /// @todo   remove this when refactoring `xImage` & image-related controls
    std::string getImagePath() {
        return checkImagePath;
    }

    /// @brief  method to retrieve the check-state image path for the item's
    /// @todo   remove this when refactoring `xImage` & image-related controls
    std::string getCheckStateImagePath() {
        return checkImagePath;
    }

    /// @brief  method to retrieve the uncheck-state image path for the item's
    /// @todo   remove this when refactoring `xImage` & image-related controls
    std::string getUncheckStateImagePath() {
        return uncheckImagePath;
    }

    /// @brief     method to append/attach a submenu to this menu item
    /// @param[in] submenu ~ pointer of the submenu object to add/attach
    /// @details   implementation for this method is contained in `xMenu.h`
    void add(xMenu* submenu);

    /// @brief     method to remove/detatch a submenu from this menu item
    /// @param[in] submenu ~ pointer of the submenu object to remove/detatch
    /// @details   implementation for this method is contained in `xMenu.h`
    void remove(xMenu* submenu);

    // print out item data, requires submenu data, i.e. xMenu ...
    #ifndef NDEBUG
    /// @brief friend ostream operator overload to stream item data to console (DEBUG)
    friend std::ostream& operator << (std::ostream& os, const xMenuItem& item);
    #endif

    #ifndef NDEBUG
    /// @brief Helper method to stream separately the item's image-related data
    /// @todo  create a solid `xImage` class that handles all the image-related data,
    ///        for all types of widgets including menus ... <br/>
    ///        This is a short-term solution ...
    std::ostream& streamImageData(std::ostream& os, const xMenuItem& item) const {
        
        os << "images: " << "\n";

        if (item.hBitmapCheck) {
            os << "\t\t\t" << item.checkImagePath << "; ";
        } else { os << "\t\t\t" << "none"; }

        os << "\n";

        if (item.hBitmapUncheck) {
            os << "\t\t\t" << item.uncheckImagePath << "; ";
        } else { os << "\t\t\t" << "none"; }

        os << "\n";
        return os;
    }
    #endif

    // mInfo.fState |= MFS_HILITE
    // mInfo.fState |= MFS_UNHILITE
    // mInfo.fState |= MF_HILITE
    // mInfo.fState |= MF_UNHILITE
    // mInfo.fState |= MF_RIGHTJUSTIFY
    // mInfo.fState |= MF_LINKS
    // mInfo.fState |= MF_MENUBARBREAK
    // mInfo.fState |= MF_SYSMENU
    // mInfo.fState |= MF_MENUBARBREAK
};

/// implementation for `xItemManager::clear()`
void xItemManager::clear() {
    // destroy menu items held in temp ...
    for (size_t i = 0; i < items.size(); i++) {
        items[i]->destroy();
    }
    items.clear();
}

/// implementation for `xItemManager::clearTemp()`
void xItemManager::clearTemp() {
    // destroy menu items held in temp ...
    for (size_t i = 0; i < temp.size(); i++) {
        temp[i]->destroy();
    }
    temp.clear();
}

#ifndef NDEBUG
/// implementation for `xItemManager::EnumerateItems()`
void xItemManager::EnumerateItems() {
    std::map<int, xMenuItem*>::iterator i;
    for (i = items.begin(); i != items.end(); i++) {
        std::cout << "map index: " << i->first
        << " => " << *i->second << std::endl;
    }
}
#endif

#ifndef NDEBUG
/// implementation for `xItemManager::EnumerateTempItems()`
void xItemManager::EnumerateTempItems() {
    std::unordered_map<int, xMenuItem*>::iterator i;
    for (i = temp.begin(); i != temp.end(); i++) {
        std::cout << "map index: " << i->first
        << " => " << *i->second << std::endl;
    }
}
#endif

/// implementation for `xItemManager::triggerMenuItemClickEvent`
void xItemManager::triggerMenuItemClickEvent(int itemID, xWidget* pWidget) {
    // retrieve the item pointer from `xItemManager`
    xMenuItem* item = xItemManager::getItemByID(itemID);
    // check if item is not `nullptr`
    if (item) {
        // check if the item has `mOnMenuItemClick` function pointer
        if (item->mOnMenuItemClick) {
            // invoke the menu item's `mOnMenuItemClick` callback
            item->mOnMenuItemClick(item, pWidget);
        }
    }
}

/// implementation for `xMenuItem::createMenuItem`
xMenuItem* xItemManager::createMenuItem(
    const std::string& text = "",
    event_onMenuItemClick action = xMenuItem::defaultMenuItemClickEvent
) {
    
    // sum of used & temp items ...
    int size = xItemManager::items.size() + xItemManager::temp.size();

    // increment the size by 1 => mID cannot be 0
    // 0 indicated menu item selection cancelled
    xMenuItem* item = new xMenuItem(++size, text);

    // store the item in temp container ...
    xItemManager::storeTempItem(item);

    item->setOnMenuItemClick(action);

    // return pointer to client ...
    return item;
}

/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
void xItemManager::storeItem(xMenuItem* item) {
    if (!item) { return; }
    items[item->mID] = item;
    removeTempItem(item);
}

/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
void xItemManager::storeTempItem(xMenuItem* item) {
    if (!item) { return; }
    temp.insert(std::make_pair(item->mID, item));
}

/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
void xItemManager::removeItem(xMenuItem* item) {
    if (!item) { return; }
    std::map<int, xMenuItem*>::iterator i;
    i = items.find(item->mID);
    if (i != items.end()) {
        // #ifndef NDEBUG
        // std::cout << *item << std::endl;
        // #endif
        item->destroy();
        items.erase(item->mID);
    }
}

/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
void xItemManager::removeTempItem(xMenuItem* item) {
    if (!item) { return; }
    std::unordered_map<int, xMenuItem*>::iterator i;
    i = temp.find(item->mID);
    if (i != temp.end()) {
        temp.erase(item->mID);
    }
}

/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
bool xItemManager::checkItem(xMenuItem* item) {
    if (!item) { return false; }
    std::map<int, xMenuItem*>::iterator i;
    i = items.find(item->mID);
    if (i != items.end()) {
        return true;
    } else { return false; }
}

/// implementation for `xMenuManager::EnumerateTempMenus(...)` [DEBUG]
bool xItemManager::checkTempItem(xMenuItem* item) {
    if (!item) { return false; }
    std::unordered_map<int, xMenuItem*>::iterator i;
    i = temp.find(item->mID);
    if (i != temp.end()) {
        return true;
    } else { return false; }
}

#endif // end of xMENUITEM_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
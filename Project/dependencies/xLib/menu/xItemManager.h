/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xItemManager.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This class contains the implementation for `xItemManager`
  * 
  * @details 	xItemManager.h contains some forward declaration,
  *             & interfaces with menu item classes ...
  */

/// @brief begin of xITEMMANAGER_H implementation
#ifndef xITEMMANAGER_H
#define xITEMMANAGER_H

#pragma once

// forward declarations ...
class xMenuItem;
class xRadioItem;

/**
 * @class    xItemManager
 * @brief   `xItemManager` defines the class & public interface members
 *           for creating interacting with `xMenuItem` objects
 * @details  This class implements the singleton design pattern
 *           & interfaces with `xMenuItem` factory design pattern
 *           to ensure proper resource management
 *           of `xMenuItem` resources
 */
class xItemManager {

    /// This using statement is declared for simplifying
    /// the syntax for the below function signatures
    using event_onMenuItemClick = std::function<void(xMenuItem* pItem, xWidget* pWidget)>;

public:

    /// @brief public singleton design pattern getter
    static xItemManager& get() {
        if (nullptr == instance) {
            instance = new xItemManager;
        }
        return *instance;
    }

    /// @brief deleted singleton `xItemManager` class copy constructor
    xItemManager(const xItemManager&) = delete;
    /// @brief deleted singleton `xItemManager` class copy assignment operator
    xItemManager& operator = (const xItemManager&) = delete;

    /// @brief public interface class destructor method
    static void destruct() {
        // destroy all the resources
        // held by the containers ...
        clear();
        clearTemp();
        delete instance;
        instance = nullptr;
    }

private:
    
    /// @brief default `xItemManager` singleton class constructor
    xItemManager() = default;
    /// @brief default `xItemManager` singleton class destructor
    ~xItemManager() = default;
    /// @brief variable storing singleton class instance pointer
    static xItemManager* instance;

private:

    /// @brief   container storing all used `xMenu` objects manufatured by `xMenu` the factory pattern
    /// @details a map is used to ensure ordered indexing, corresonding to the menu item's ID
    static std::map<int, xMenuItem*> items;
    /// @brief   container storing all temporary `xMenu` objects manufatured by `xMenu` the factory pattern
    /// @details an unordered map is used to keep the container relatively small (not critical)
    static std::unordered_map<int, xMenuItem*> temp;

public:

    /// @brief public factory pattern interface method to create menu item objects
    /// @param text ~ string text data to display in the menu item
    /// @param action ~ optional menu-item-click event callback/action
    /// @return ~ pointer of the menu item created by the factory pattern
    static xMenuItem* createMenuItem(
        const std::string& text,
        event_onMenuItemClick action
    );

    /// @brief public factory pattern interface method to create radio item objects
    /// @param text ~ string text data to display in the radio item
    /// @return ~ pointer of the radio item created by the factory pattern
    static xRadioItem* createRadioItem(const std::string& text);

    /// @brief method to retrieve the size of the menu item container
    /// @return integer representative of the menu item count
    int size() { return items.size(); }

    /// @brief method to retrieve the size of the temp menu item container
    /// @return integer representative of the temp menu item count
    int tempSize() { return temp.size(); }

    // implementation after `xMenuItem`
    /// @todo make this protected
    /// @brief method to store a menu item object
    ///        in the item manager's container
    static void storeItem(xMenuItem* item);
    /// @brief method to store a temp menu item object
    ///        in the item manager's temp container
    static void storeTempItem(xMenuItem* item);
    /// @brief method to remove a menu item object
    ///        from the item manager's container
    static void removeItem(xMenuItem* item);
    /// @brief method to remove a temp menu item object
    ///        from the item manager's temp container
    static void removeTempItem(xMenuItem* item);
    /// @brief method to check whether a menu item object
    ///        is contained in the item manager's container
    static bool checkItem(xMenuItem* item);
    /// @brief method to check whether a temp menu item object
    ///        is contained in the item manager's temp container
    static bool checkTempItem(xMenuItem* item);

    /// @brief method to get an item from the item manager's container by item ID
    /// @param itemID ~ integer representative of the item's ID
    /// @return pointer of the menu item found, or `nullptr` if NOT
    static xMenuItem* getItemByID(int itemID) {
        return items[itemID];
    }

    /// @brief method to get a temp item from the item manager's temp container by item ID
    /// @param itemID ~ integer representative of the item's ID
    /// @return pointer of the menu item found, or `nullptr` if NOT
    static xMenuItem* getTempItemByID(int itemID) {
        return temp[itemID];
    }

    /// @brief method to redirect/dispatch menu-item click
    /// callbacks/actions to the relevant menu item object
    /// @param itemID ~ integer representative of the menu item's ID
    /// used to look-up/find the item, for redirecting to it's callback/action
    /// @param pWidget ~ pointer of the widget containing the menu, containing the item
    static void triggerMenuItemClickEvent(int itemID, xWidget* pWidget);

    #ifndef NDEBUG
    /// @brief method to enumarate all the menu item data
    ///        held by the item manager's container
    static void EnumerateItems();
    #endif

    #ifndef NDEBUG
    /// @brief method to enumarate all the temp menu item data
    ///        held by the item manager's temp container
    static void EnumerateTempItems();
    #endif

    #ifndef NDEBUG
    /// @brief method to Log `xItemManager` container's data (DEBUG)
    static void LogItemData() {
        std::cout << "Menu items in use: " << std::endl;
        EnumerateItems();

        std::cout << "Temporary items in use: " << std::endl;
        EnumerateTempItems();

        // The number of used borders ...
        LOG(("Used count: " + std::to_string(items.size())).c_str());
        // The nnumber of existing borders ...
        LOG(("Temp count: " + std::to_string(temp.size())).c_str());

        // The difference between existing borders & used borders ...
        LOG(("Count Difference: " + std::to_string(calculateDifference())).c_str());
    }
    #endif

    /// @brief method to calculate the difference in size
    /// for the used-items container & temp-items container
    static int calculateDifference() {
        return items.size() - temp.size();
    }

    /// @brief method to clear the container,
    /// gracefully free-ing all item resources
    static void clear();
    /// @brief method to clear the temp container,
    /// gracefully free-ing all temp item resources
    static void clearTemp();
};

// C++11 DOES NOT SUPPORT inline static declaration
xItemManager* xItemManager::instance = nullptr;
std::map<int, xMenuItem*> xItemManager::items;
std::unordered_map<int, xMenuItem*> xItemManager::temp;

#endif // end of xITEMMANAGER_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
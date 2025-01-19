/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xMenuManager.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This class contains the implementation for `xMenuManager`
  * 
  * @details 	xMenuManager.h contains some forward declaration,
  *             & interfaces with menu classes ...
  */

/// @brief begin of xMENUMANAGER_H implementation
#ifndef xMENUMANAGER_H
#define xMENUMANAGER_H

#pragma once

#include <set>

// forward declarations ...
class xMenu;
class xPopupMenu;
class xRadioMenu;

/**
 * @class    xMenuManager
 * @brief   `xMenuManager` defines the class & public interface members
 *           for interacting with the `xMenu` object containers (sets)
 * @details  This class implements the singleton design pattern
 *           & interfaces with `xMenu` factory design pattern
 *           to ensure proper resource management
 *           of `xMenu` resources
 */
class xMenuManager {

    /// This using statement is declared for simplifying
    /// the syntax for the below function signatures
    using event_onMenuChange = std::function<void(xMenuItem* pItem, xMenu* pMenu)>;

public:

    /// @brief public singleton design pattern getter
    static xMenuManager& get() {
        if (nullptr == instance) {
            instance = new xMenuManager;
        }
        return *instance;
    }

    /// @brief deleted singleton `xMenuManager` class copy constructor
    xMenuManager(const xMenuManager&) = delete;
    /// @brief deleted singleton `xMenuManager` class copy assignment operator
    xMenuManager& operator = (const xMenuManager&) = delete;

    /// @brief public interface class destructor method
    static void destruct() {
        // destroy all the resources
        // held by the containers ...
        delete instance;
        instance = nullptr;
    }

private:

    /// @brief default `xMenuManager` singleton class constructor
    xMenuManager() = default;
    /// @brief default `xMenuManager` singleton class destructor
    ~xMenuManager() = default;
    /// @brief variable storing singleton class instance pointer
    static xMenuManager* instance;

private:

    /// @brief container storing all used `xMenu` objects manufatured by `xMenu` the factory pattern
    static std::set<xMenu*> menus;
    /// @brief container storing all temporary `xMenu` objects manufatured by `xMenu` the factory pattern
    static std::set<xMenu*> temp;

public:

    /// @brief public factory pattern interface method to create menu objects
    /// @param action ~ optional menu-change event callback/action
    /// @return ~ pointer of the menu created by the factory pattern
    static xMenu* createMenu(event_onMenuChange action);

    /// @brief public factory pattern interface method to create popup menu objects
    /// @param action ~ optional menu-change event callback/action
    /// @return ~ pointer of the popup menu created by the factory pattern
    static xPopupMenu* createPopupMenu(event_onMenuChange action);

    /// @brief public factory pattern interface method to create radio menu objects
    /// @param action ~ optional menu-change event callback/action
    /// @return ~ pointer of the radio menu created by the factory pattern
    static xRadioMenu* createRadioMenu(event_onMenuChange action);

    /// @brief method to retrieve the size of the menu container
    /// @return integer representative of the menu count
    static int size() { return menus.size(); }

    /// @brief method to retrieve the size of the temp menu container
    /// @return integer representative of the temp menu count
    static int tempSize() { return temp.size(); }

    // implementation after xMenu
    /// @todo make this protected
    /// @brief method to store a menu object
    ///        in the menu manager's container
    static void storeMenu(xMenu* menu);
    /// @brief method to store a temp menu object
    ///        in the menu manager's temp container
    static void storeTempMenu(xMenu* menu);
    /// @brief method to remove a menu object
    ///        from the menu manager's container
    static void removeMenu(xMenu* menu);
    /// @brief method to remove a temp menu object
    ///        from the menu manager's temp container
    static void removeTempMenu(xMenu* menu);
    /// @brief method to check whether a menu object
    ///        is contained in the menu manager's container
    static bool checkMenu(xMenu* menu);
    /// @brief method to check whether a temp menu object
    ///        is contained in the menu manager's temp container
    static bool checkTempMenu(xMenu* menu);

    #ifndef NDEBUG
    /// @brief method to enumarate all the menu data held by the menu manager's container
    static void EnumerateMenus();
    #endif

    #ifndef NDEBUG
    /// @brief method to enumarate all the temp menu data held by the menu manager's temp container
    static void EnumerateTempMenus();
    #endif

    #ifndef NDEBUG
    /// @brief method to Log `xMenuManager` container's data (DEBUG)
    static void LogMenuData() {
        std::cout << "Menus in use: " << std::endl;
        EnumerateMenus();

        std::cout << "Temporary menus in use: " << std::endl;
        EnumerateTempMenus();

        // The number of used borders ...
        LOG(("Used count: " + std::to_string(menus.size())).c_str());
        // The nnumber of existing borders ...
        LOG(("Temp count: " + std::to_string(temp.size())).c_str());

        // The difference between existing borders & used borders ...
        LOG(("Count Difference: " + std::to_string(calculateDifference())).c_str());
    }
    #endif

    /// @brief method to calculate the difference in size
    /// for the used-menus container & temp-menus container
    static int calculateDifference() {
        return menus.size() - temp.size();
    }

    /// @brief method to clear the container,
    /// gracefully free-ing all menu resources
    static void clear();
    /// @brief method to clear the temp container,
    /// gracefully free-ing all temp menu resources
    static void clearTemp();
};

// C++11 DOES NOT SUPPORT inline static declaration
xMenuManager* xMenuManager::instance = nullptr;
std::set<xMenu*> xMenuManager::menus;
std::set<xMenu*> xMenuManager::temp;

#endif // end of xMENUMANAGER_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
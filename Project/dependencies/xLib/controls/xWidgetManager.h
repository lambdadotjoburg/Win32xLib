/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xWidgetManager.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  singleton `xWidgetManager` declaration & implementation header file
  * @details  xWidgetManager.h defineds the `xWidgetManager` singleton class to be used for
  *           `xWidget` management & window/widget class name registration,
  */
 
#pragma once

/// @brief begin of xWIDGETMANAGER_H implementation
#ifndef xWIDGETMANAGER_H
#define xWIDGETMANAGER_H

// forwar declaration ...
class xWidget;

#include <unordered_map>
#include <vector>

/**
 * @class   xWidgetManager
 * @brief   xWidgetManager singleton class for keeping track of all registered widgets
 * @details xWidgetManager is responsible for keeping track of all widgets registered
 *          in the program, throughout the program life-cycle
 *          & provides interface methods for performing the following routines:
 * 
 *              1) insert widgets into an `unordered_map` from `HWND` to `xWidget*`
 *              2) remove widgets (along with its children) when destroyed
 *              3) checks whether a widget/window class has already been registered
 *                 (since a windows class can only be registered once)
 */
class xWidgetManager {

    /*%*%*%*%*%*%*%*%*%*%*/
    /* Widget Management */
    /*%*%*%*%*%*%*%*%*%*%*/

    /// @note `xWidget` forward declaration => `xWidgetManager` only deals with `xWidgets` by pointers ...
    friend class xWidget;

public:

    /**
     * @brief   Enables client code to access unique `xWidgetManager` instance pointer
     * @return  reference to singleton `xWidgetManager` instance pointer
     * @details This class method returns an `xWidgetManager` instance pointer
     *          to the client code & constructs a new such instance if none exists
     */
    static xWidgetManager& get() {
        if (nullptr == instance) {
            instance = new xWidgetManager;
        };
        return *instance;
    }

    /// @brief delete copy constructor
    xWidgetManager(const xWidgetManager&) = delete;
    /// @brief delete copy assignment operator
    xWidgetManager& operator = (const xWidgetManager&) = delete;

    /// @brief destructor to free the `xWidgetManager` instance pointer
    static void destruct() {
        clear();
        delete instance;
        instance = nullptr;
    }

private:

    /// @brief default singleton class constructor
    xWidgetManager() = default;
    /// @brief default singleton class destructor
    ~xWidgetManager() = default;

    /// @brief `xWidgetManager` instance pointer
    static xWidgetManager* instance;
    
    /// @brief unordered map container to keep track of
    /// the registered components/widgets in the system
    static std::unordered_map<HWND, xWidget*> xWidgets;
    
    /// @brief container storing all the class names of the registered widgets
    static std::vector<std::string> xClassNames;

public:

    /// @brief method to retrieve the count of the number of `xWidget`s held by the app runtime
    int count() {
        /// size ... ?
        return xWidgets.size();
    }

    /// @brief method to remove widget from map
    /// @param[in] hWnd ~ the handle of the widget to be removed
    /// @return boolean flag ~ indication that the `xWidget*`
    ///         was successfully removed from the map
    bool remove(HWND hWnd) {
        // check if key hWnd exists before remove ...
        std::unordered_map<HWND, xWidget*>::iterator i;
        i = xWidgets.find(hWnd); // attempt to find
        if (i != xWidgets.end()) {
            // if found, erase ...
            xWidgets.erase(hWnd);
            return true; // erased successfully
        } // oterwize
        return false; // element not found ...
    }

    /**
     * @brief   method to retrieve all widget components
     * @details method to retrieve the unordered map of widgets held by the running application
     * @return  returns the entire map of registered components in use by the app
     */
    static std::unordered_map<HWND, xWidget*>& getWidgets() {
        return xWidgets;
    }

    /// @brief     method to retrieve `xWidget` from map
    /// @return    a pointer to an `xWidget` i.e. `xWidget*`
    /// @param[in] hWnd ~ hanlde of the `xWidget` to be retrieved
    /// @details   This method is mainly used in conjunction with the static `CALLBACK WndProc`
    static xWidget* getWidget(HWND hWnd) {
        return xWidgets[hWnd];
    }

    /// @brief method to store a `<HWND, xWidget*>` key-value pair in the `xWidgets` map
    /// @param hWnd ~ handle to the `xWidget*` to be stored => hWnd serves as key
    /// @param widget ~ pointer of the `xWidget` to be stored => widget serves as value
    static void storeWidget(HWND hWnd, xWidget* widget) {
        xWidgets[hWnd] = widget;
    }

    /// @brief     method for storing a class name in the 
    /// @param[in] classname ~ constant reference to a string representative of the class name to store
    static void setClassName(const std::string& classname) {
        // insert new class name into xClassNames vector
        xClassNames.push_back(classname);
    }

    /// @brief     method to check whether a class is already registered
    /// @param[in] classname ~ constant reference to a string representative of the class name to check
    static bool isRegistered(const std::string& classname) {
        // iterate the xClassNames vector container
        for (size_t i = 0; i < xClassNames.size(); i++) {
            if (xClassNames[i] == classname) {
                return true; // class already registered
            }
        }
        return false;
    }

    /// @brief     method to check whether the widget exists in the map
    /// @param[in] hWnd ~ handle of the widget to find
    static bool exists(HWND hWnd) {
        return xWidgets.find(hWnd) != xWidgets.end();
    }

    #ifndef NDEBUG
    /// @brief      method to enumerate all the registered class names ...
    /// @note       mainly for debug purposes, not really useful in general!
    /// @deprecated to be removed in release build
    static void EnumerateClassNames() {
        for (size_t i = 0; i < xClassNames.size(); i++) {
            std::cout << xClassNames[i] << std::endl;
        }
    }
    #endif

    /// @brief   method to get all widgets by `classname`
    /// @details This method filters the unordered_map to retrieve only
    ///          those widgets having a specified `classname`
    /// @return  `std::vector<xWidget*>` ~ a vector of pointers to `xWidgets` having specified `classname`
    std::vector<xWidget*> getWidgetsByClassName(const std::string& classname);

    // implementation after `xWidget`
    static void clear();
};

/// @brief initialize `xWidgetManager` static singelton instance pointer
xWidgetManager* xWidgetManager::instance = nullptr;

/// @brief static member declaration for `xWidgets`
std::unordered_map<HWND, xWidget*> xWidgetManager::xWidgets;

/// @brief static member declaration for `xClassNames`
std::vector<std::string> xWidgetManager::xClassNames;

#endif // end of xWIDGETMANAGER_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
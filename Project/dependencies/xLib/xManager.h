/**
  * @file    xManager.h
  * @author  &lambda;ambda
  * @date    \showdate "%Y-%m-%d"
  * @brief   sigleton `xManager` declaration & implementation header file
  * @details xManager.h defines the `xManager` singleton class instance
  *          to be used for `xWidget` management & window classname registration
  */

#pragma once

#ifndef xMANAGER_H
#define xMANAGER_H

#include <unordered_map>

/// `xManager` depends on `xWidget` because ...
#include "./xWidget.h"

/**
 * @brief   `xManager` singleton class for managing windows/`xWidget` registration
 * 
 * @details `xManager` is responsible for:
 * 
 *              - Keeping track of registered window classnames
 *                `<HWND, xWidget*>` unordered_map/container pairs
 *              
 *              - Keeping track of (add/remove/find) `xWidget`
 *                objects' `HWND` & `xWidget*`
 */

class xManager {
public:
    // singleton design pattern here ...

     /**
    * @brief   Enables client code to access unique `xManager` instance pointer
    * @return  reference to a singleton `xManager` instance pointer
    * @details This class method returns an `xManager` instance pointer
    *          to the client code & constructs a new such instance if none exists
    */
    static xManager& get() {
        // may be responsible for creating its own unique instance ...
        if (nullptr == instance) {
            instance = new xManager;
        }
        return *instance; // creating own unique instance
    }

    /// @brief deleted `xManager` copy constructor ~ adheres <a href="https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)" target="_blank">rule of three (C++)</a>
    xManager(const xManager&) = delete;
    /// @brief deleted `xManager` copy assignment operator ~ adheres <a href="https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)" target="_blank">rule of three (C++)</a>
    xManager& operator=(const xManager&) = delete;

    /// @brief method for destroying the singleton `xManager` instance pointer ~ 
    static void destruct() {
        delete instance;
        instance = nullptr;
    }

    // method for checking whether a given window class instance exists based on `HWND`
    // to be defined when encountered in the remainder of the code ...

    // method for checking if classname is registered ...
    bool isRegisered(const std::string& classname) {
        for (size_t i = 0; i < xClassNames.size(); i++) {
            if (xClassNames[i] == classname) {
                return true;
            }
        }
        return false;
    }

    // method for storing a classname in the xClassNames vector ...
    void setClassName(const std::string& classname) {
        xClassNames.push_back(classname);
    }

    // method for getting the classname, given an integer index ...
    std::string& getClassName(int i) {
        return xClassNames[i];
    }

private:
    /// @brief default `xManager` constructor
    xManager() = default; // no public constructor
    ~xManager() = default; // no public destructor

    /**
     * @brief `xManager` singleton instance pointer initialized
     *        to `new xManager()` using `xManager::get()`
     *        during program runtime to access class members
     */
    static xManager* instance;

    /**
     * @brief container storing registered window classnames
     */
    std::vector<std::string> xClassNames;

    /**
     * @brief container storing map from `HWND` to `xWidget*` pointers
     */
    std::unordered_map<HWND, xWidget*> xWidgets;
};

/// @brief `xManager` singleton instance pointer initialized to `nullptr` before app runtime
xManager* xManager::instance = nullptr;

#endif
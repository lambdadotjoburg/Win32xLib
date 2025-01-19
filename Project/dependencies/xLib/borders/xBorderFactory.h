/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xBorderFactory.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  singleton `xBorderFactory` declaration & implementation header file
  * @details  xBorderFactory.h defineds the `xBorderFactory` singleton class
  *           to be used for creating/manufacturing new border objects.
  */
 
#pragma once

/// @brief begin of xBORDERFACTORY_H implementation
#ifndef xBORDERFACTORY_H
#define xBORDERFACTORY_H

// include `xBorder` class ...

// xClassicBorder.h includes xBevelBorder.h & xEtchedBorder.h
#include "./xClassicBorder.h"
#include "./xModernBorder.h"

#include <vector>

/**
 * @class    xBorderFactory
 * @brief   `xBorderFactory` singleton class for manufacturing new `xBorder` objects
 * @details `xBorderFactory` is responsible for creating/manufacturing new `xBorder` objects
 *           used by `xWidgets` in the program, throughout the program life-cycle
 *           & provides interface methods for creating borders having different styles ...
 */
class xBorderFactory {

    /*%*%*%*%*%*%*%*%*%*%*/
    /* Border Management */
    /*%*%*%*%*%*%*%*%*%*%*/

public:

    /**
     * @brief   Enables client code to access unique `xBorderFactory` instance pointer
     * @return  reference to singleton `xBorderFactory` instance pointer
     * @details This class method returns an `xBorderFactory` instance pointer
     *          to the client code & constructs a new such instance if none exists
     */
    static xBorderFactory& get() {
        if (nullptr == instance) {
            instance = new xBorderFactory;
        };
        return *instance;
    }

    /// @brief delete copy constructor
    xBorderFactory(const xBorderFactory&) = delete;
    /// @brief delete copy assignment operator
    xBorderFactory& operator=(const xBorderFactory&) = delete;

    /// @brief destructor to free the `xBorderFactory` instance pointer
    static void destruct() {
        clear();
        clearTemp();
        delete instance;
        instance = nullptr;
    }

private:

    /// @brief default singleton class constructor
    xBorderFactory() = default;
    /// @brief default singleton class destructor
    ~xBorderFactory() = default;

    /// @brief `xBorderFactory` instance pointer
    static xBorderFactory* instance;

private:

    /// @brief container storing all the `xBorder*` pointers used during program runtime
    /// @todo  move the borders & related interface methods to `xBorderFactory`
    /// @todo  change the `vector` to a `set`, so no need for `checkBorder()` ...
    static std::vector<xBorder*> borders;

    /// @brief container storing all the `xBorder*` pointers that have been
    /// created/manufactured by the `xBorderFactory` singleton class
    /// regardless of whether these borders are in use/being used
    /// by an `xWidget` since this allows for returning a pointer
    /// for an xBorder to the client code, i.e. the client code
    /// does not have access to `xBorder`s delete destructor,
    /// thus, can only receive back `xBorder*`
    /// @todo change the `vector` to a `set`, so no need for `checkTempBorder()` ...
    static std::vector<xBorder*> temp;

public:

    /// @brief     method to check whether an `xBorder` is in use by any of the `xWidget` controls
    /// @param[in] border ~ pointer to the `xBorder` to find
    /// @return    boolean flag ~ whether or not the `xBorder` is in use or NOT
    /// @todo      change the `vector` to a `set`, so no need for `checkBorder()` ...
    static bool checkBorder(xBorder* border) {
        // iterate the borders vector container
        for (size_t i = 0; i < borders.size(); i++) {
            if (borders[i] == border) {
                return true; // border in use ...
            }
        }
        return false; // if border not found
    }

    /// @brief     method to check whether an `xBorder` is in stored within the temp container
    /// @param[in] border ~ pointer to the `xBorder` to find
    /// @return    boolean flag ~ whether or not the `xBorder` is in the temp container or NOT
    /// @todo      change the `vector` to a `set`,
    ///            so no need for `checkTempBorder()` ...
    ///            `std::set` will be more efficient! 
    static bool checkTempBorder(xBorder* border) {
        // iterate the borders vector container
        for (size_t i = 0; i < temp.size(); i++) {
            if (temp[i] == border) {
                return true; // border stored in temp ...
            }
        }
        return false; // if border not found
    }

    /// @brief     method to insert a new `xBorder` into the `borders` vector of *used* borders
    /// @param[in] border ~ pointer of the `xBorder` to insert
    /// @todo      change the `vector` to a `set`,
    ///            so no need for `checkBorder()` ...
    ///            `std::set` will be more efficient! 
    static void storeBorder(xBorder* border) {

        // if border does NOT exist ...
        if (!checkBorder(border)) {
            // ... then store the new border ...
            borders.push_back(border); // only if not exists ...
            // remove the border from `temp` ...
            removeTempBorder(border);
        } // do anyway ...

        // increment the border ref count ...
        border->count++; // increment ref count
    }

    /// @brief method to remove unused `xBorder`s from the `borders` vector
    /// @param border ~ the border to be removed, if not in use ...
    static void removeBorder(xBorder* border) {

        // check if border exists, i.e. not nullptr
        if (border) {

            // decrement ref count BEFORE erase from container ...
            border->count--;
            
            // delete the resources associated to the `xBorder`
            if (border->count == 0) {

                // remove border from `borders` container ...
                for (size_t i = 0; i < borders.size(); i++) {
                    // find the border & erase ...
                    if (borders[i] == border) {

                        // actually remove the border ...
                        borders.erase(borders.begin() + i);
                        break; // noting more to do => escape loop ...
                    }
                }

                delete border; // free memory resources ...
                border = nullptr; // set border pointer to nullptr
            }
        }
        // #ifndef NDEBUG
        // LogBorderData();
        // #endif
    }

    /// @brief method to remove an `xBorder` from the `temp` vector
    /// @param border ~ the border to be removed
    static void removeTempBorder(xBorder* border) {

        // check if border exists, i.e. not nullptr
        if (border) {
            
            // remove border from `temp` container ...
            for (size_t i = 0; i < temp.size(); i++) {
                // find the border & erase ...
                if (temp[i] == border) {

                    // actually remove the border from `temp` ...
                    temp.erase(temp.begin() + i);

                    break; // noting more to do => escape loop ...
                }
            }
            // don't `delete` the border, only remove it from the `temp` vector
            // delete border; // free memory resources ...
            // border = nullptr; // set border pointer to nullptr
        }
        // #ifndef NDEBUG
        // LogBorderData();
        // #endif
    }

    /// @brief      method to enumerate all the *active*/*used* `xBorder`s ...
    /// @note       mainly for debug purposes, not really useful in general!
    /// @deprecated to be removed in release build
    #ifndef NDEBUG
    static void EnumerateBorders() {
        for (size_t i = 0; i < borders.size(); i++) {
            // check if exists
            if (borders[i]) {
                // using `xBorder`s `friend` `ostream` `operator` `<<`
                std::cout << *borders[i] << std::endl; // dereference `borders[i]`
            }
        }
    }
    #endif

    /// @brief      method to enumerate all the `temp` `xBorder`s ...
    /// @note       mainly for debug purposes, not really useful in general!
    /// @deprecated to be removed in release build
    #ifndef NDEBUG
    static void EnumerateTempBorders() {
        for (size_t i = 0; i < temp.size(); i++) {
            // check if exists
            if (temp[i]) {
                // using `xBorder`s `friend` `ostream` `operator` `<<`
                std::cout << *temp[i] << std::endl; // dereference `temp[i]`
            }
        }
    }
    #endif

    /// @brief      method to log the data held by `xBorderFactory`
    /// @note       mainly for debug purposes, not really useful in general!
    /// @deprecated to be removed in release build
    #ifndef NDEBUG
    static void LogBorderData() {

        std::cout << "Borders in use: " << std::endl;
        EnumerateBorders();

        std::cout << "Temporary borders: " << std::endl;
        EnumerateTempBorders();

        // The number of used borders ...
        LOG(("Used count: " + std::to_string(borders.size())).c_str());
        // The nnumber of existing borders ...
        LOG(("Temp count: " + std::to_string(temp.size())).c_str());

        // The difference between existing borders & used borders ...
        LOG(("Count Difference: " + std::to_string(calculateDifference())).c_str());
    }
    #endif

    /// @remark
    /// Once a border is registered/created/manufactured it is added to temp container
    /// if the border is associated to an `xWidget`, then it may be stored
    /// in the borders container & it can be safely removed from
    /// the temp container ...

    /// @brief  method to calculate the difference
    ///         between the number of used borders
    ///         & the number of temporary borders
    /// @return integer representative of the used & temp border difference
    static int calculateDifference() {
        return borders.size() - temp.size();
    }

    /// @brief method to clear the borders from `xBorderFactory`
    static void clear(); // implementation after `xWidget`

    /// @brief method to clear the `temp` borders
    static void clearTemp(); // implementation after `xWidget`

    /// @brief  interface method to create a bevel border object using `RGB(...)` macro
    /// @return pointer to the new manufactured `xBorder` object
    /// @note   signature forces client to choose `ClassicStyle` => `RAISED`/`SUNKEN`
    static xBorder* createBevelBorder(
        BYTE R, BYTE G, BYTE B, int w, int t,
        xBorder::ClassicStyle style, int inset = 0
    ) {
        temp.push_back(new xBevelBorder(R, G, B, w, t, style, inset));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief  overload interface method to create a bevel border object using `COLORREF` hexadecimal value
    /// @return pointer to the new manufactured `xBorder` object
    /// @note   signature forces client to choose `ClassicStyle` => `RAISED`/`SUNKEN`
    static xBorder* createBevelBorder(
        COLORREF colorRef, int w, int t,
        xBorder::ClassicStyle style, int inset = 0
    ) {
        temp.push_back(new xBevelBorder(colorRef, w, t, style, inset));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief  interface method to create an etched border object using `RGB(...)` macro
    /// @return pointer to the new manufactured `xBorder` object
    /// @note   signature forces client to choose `ClassicStyle` => `RAISED`/`SUNKEN`
    static xBorder* createEtchedBorder(
        BYTE R, BYTE G, BYTE B, int w, int t,
        xBorder::ClassicStyle style, int inset = 0
    ) {
        
        temp.push_back(new xEtchedBorder(R, G, B, w, t, style, inset));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief  overload interface method to create an etched border object using `COLORREF` hexadecimal value
    /// @return pointer to the new manufactured `xBorder` object
    /// @note   signature forces client to choose `ClassicStyle` => `RAISED`/`SUNKEN`
    static xBorder* createEtchedBorder(
        COLORREF colorRef, int w, int t,
        xBorder::ClassicStyle style, int inset = 0
    ) {
        
        temp.push_back(new xEtchedBorder(colorRef, w, t, style, inset));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }
     
    /// @brief  interface method to create a line border object using `RGB(...)` macro
    /// @return pointer to the new manufactured `xBorder` object
    /// @note   signature forces client to choose `StrokeStyle` => `DOT`/`DASH`/`SOLID`, etc ...
    static xBorder* createLineBorder(
        BYTE R, BYTE G, BYTE B, int w, int r,
        xBorder::StrokeStyle style, int inset = 0
    ) {
        temp.push_back(new xLineBorder(R, G, B, w, r, style, inset));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief  overload interface method to create a line border object using `COLORREF` hexadecimal value
    /// @return pointer to the new manufactured `xBorder` object
    /// @note   signature forces client to choose `StrokeStyle` => `DOT`/`DASH`/`SOLID`, etc ...
    static xBorder* createLineBorder(
        COLORREF colorRef, int w, int r,
        xBorder::StrokeStyle style, int inset = 0
    ) {
        temp.push_back(new xLineBorder(colorRef, w, r, style, inset));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief method to create focus borders, which by default are dotted lines,
    ///        but is essentially equivalent to `xLineBorders` ...
    /// @param[in] focus ~ boolean flag whether or not the border is a focus border (default `true`)
    static xBorder* createFocusBorder(
        BYTE R, BYTE G, BYTE B, int w = 1, int r = 1,
        xBorder::StrokeStyle style = xBorder::StrokeStyle::DOT,
        int inset = 4, boolean focus = 1
    ) {
        temp.push_back(new xLineBorder(R, G, B, w, r, style, inset, focus));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief overload method to create focus borders, which by default are dotted lines,
    ///        but is essentially equivalent to `xLineBorders` ...
    /// @param[in] focus ~ boolean flag whether or not the border is a focus border (default `true`)
    static xBorder* createFocusBorder(
        COLORREF colorRef, int w = 1, int r = 1,
        xBorder::StrokeStyle style = xBorder::StrokeStyle::DOT,
        int inset = 4, boolean focus = 1
    ) {
        temp.push_back(new xLineBorder(colorRef, w, r, style, inset, focus));
        // return a new xBorder here ...
        return temp.at(temp.size()-1);
    }

    /// @brief interface method to create empty border objects
    /// @param[in] radius ~ border radius to apply to
    /// the empty border for rounded button corners
    /// @param[in] inset ~ number of pixels between
    /// the button's rectangle & the empty border
    /// @remark color, width, style & thickness are NOT applicable
    static xBorder* createEmptyBorder(int radius = 0, int inset = 0) {
        // only inset of readius of control can be adjusted
        temp.push_back(new xEmptyBorder(radius, inset));
        // return a new `xBorder` here ...
        return temp.at(temp.size()-1);
    }

    /// @todo define interface method to create title borders ...
    static xBorder* createTitleBoder() {
        // still figuring this out ...
        return nullptr; // just for demo
    }    
};

/// @brief initialize `xBorderFactory` static singelton instance pointer
xBorderFactory* xBorderFactory::instance = nullptr;

/// @brief static member declaration for `borders`
/// @todo  change the `vector` to a `set`, so no need for `checkBorder()` ...
std::vector<xBorder*> xBorderFactory::borders;

/// @brief static member declaration for `temp`
/// @todo  change the `vector` to a `set`, so no need for `checkBorder()` ...
std::vector<xBorder*> xBorderFactory::temp;

/// @todo
/// Define java-style border types:
/// 1) title border
/// 2) combo/composite borders (difficult?)

#endif // end of xBORDERFACTORY_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
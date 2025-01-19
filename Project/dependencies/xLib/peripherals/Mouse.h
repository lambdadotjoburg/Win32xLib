/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		Mouse.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		file contains implementation of `Mouse`
  * @details 	This file contains the implementation of the
  *             `Mouse` interface to work with cursors!
  */

/// @brief begin of MOUSE_H implementation
#ifndef MOUSE_H
#define MOUSE_H

#pragma once

// include tupe to handle mouse coordinates
#include <tuple>

/**
 * @class    Mouse
 * @brief    singleton pattern for Gdiplus Mouse/Cursor implementation
 * @details  Mouse defines interface methods for working with curors & monitors,
 *           specifically, to retrieve the cursor's coordinates/position
 */
class Mouse {

public:

    /// @brief method to retrieve the mouse perhipheral's coordinate/position
    /// @return
    /// returns `std::tuple<int, int>` the first of which
    /// is the x-coordinate & the second the y-coordinate
    /// of the mouse pointer on the screen
    /// @todo move this method to elsewhere => `xApp`/`xWidgetManager`
    static std::tuple<int, int> TupleFromMouseCoordinates(LPARAM lParam) {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        return std::make_tuple(x, y);
    }

    /// @brief   method to retrieve a c-style `POINT` struct, given an `LPARAM`
    /// @param   lParam ~ data from which to extract the `LOWORD` & `HIWORD`, i.e. the x & y coordinates
    /// @return `POINT` struct containing the extracted coordinate pair
    static POINT PointFromMouseCoordinates(LPARAM lParam) {
        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);
        return p;
    }

    /// @brief  method to retrieve the handle of the windows nearest to the specified point
    /// @param  point ~ the coordinate, nearest to which the handle for the window is to be retrieved
    /// @return handle of the window nearest to the specified point
    static HWND HandleRelativeToPoint(POINT point) {
        return WindowFromPoint(point);
    }

    /// @brief  method to retrieve the widget's pointer relative to the mouse cursor's position/coordinates
    /// @param  point ~ the coordinate, nearest to which the handle for the window is to be retrieved
    /// @return pointer of the `xWidget` nearest to the specified point
    static xWidget* WidgetRelativeToPoint(POINT point) {
        return xWidgetManager::get().getWidget(HandleRelativeToPoint(point));
    }

    /// @brief overload method of `HandleRelativeToPoint(POINT point)` accepting tuple as input
    static HWND HandleRelativeToPoint(std::tuple<int, int> point) {
        POINT p;
        p.x = static_cast<short>(std::get<0>(point));
        p.y = static_cast<short>(std::get<1>(point));
        return WindowFromPoint(p);
    }

    /// @brief overload method of `WidgetRelativeToPoint(POINT point)` accepting tuple as input
    static xWidget* WidgetRelativeToPoint(std::tuple<int, int> point) {
        POINT p;
        p.x = static_cast<short>(std::get<0>(point));
        p.y = static_cast<short>(std::get<1>(point));
        return xWidgetManager::get().getWidget(WindowFromPoint(p));
    }
};

#endif // end of MOUSE_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
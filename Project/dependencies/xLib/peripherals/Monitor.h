/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		Monitor.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		file contains implementation of `Monitor`
  * @details 	This file contains the implementation of the `Monitor`
  *             interface to work with monitors & screens
  */

/// @brief begin of MONITOR_H implementation
#ifndef MONITOR_H
#define MONITOR_H

#pragma once

/**
 * @class    Monitor
 * @brief    singleton pattern for Gdiplus Monitor implementation
 * @details  Monitor defines interface methods for working with
 *           monitors & screens, specifically, for dimensions!
 */
class Monitor {

public:

    /// @brief   method to get the screen width
    /// @return `int` representative of the screen width
    static int ScreenWidth() {
        return GetSystemMetrics(SM_CXSCREEN);
    }

    /// @brief   method to get the screen height
    /// @return `int` representative of the screen height
    static int ScreenHeight() {
        return GetSystemMetrics(SM_CYSCREEN);
    }

    /// @brief     method to get the monitor width
    /// @return   `int` representative of the monitor width
    /// @param[in] hWnd ~ handle of window on monitor for
    ///            which to get the monitor width
    static int MonitorWidth(HWND hWnd) {
        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = {};
        mi.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &mi);
        int width = mi.rcMonitor.right - mi.rcMonitor.left;
        return width;
    }

    /// @brief     method to get the monitor height
    /// @return   `int` representative of the monitor height
    /// @param[in] hWnd ~ handle of window on monitor for
    ///            which to get the monitor height
    static int MonitorHeight(HWND hWnd) {
        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = {};
        mi.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &mi);
        int height = mi.rcMonitor.bottom - mi.rcMonitor.top;
        return height;
    }

    /// @brief     method to get the monitor's horizontal position (most likely 0)
    /// @return    `int` representative of the monitor x-position
    /// @param[in] hWnd ~ handle of window on monitor for
    ///            which to get the monitor x-position
    static int MonitorX(HWND hWnd) {
        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = {};
        mi.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &mi);
        int x = mi.rcMonitor.left;
        return x;
    }

    /// @brief     method to get the monitor's vertical position (most likely 0)
    /// @return   `int` representative of the monitor y-position
    /// @param[in] hWnd ~ handle of window on monitor for
    ///            which to get the monitor y-position
    static int MonitorY(HWND hWnd) {
        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = {};
        mi.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &mi);
        int y = mi.rcMonitor.top;
        return y;
    }
};

#endif // end of MONITOR_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
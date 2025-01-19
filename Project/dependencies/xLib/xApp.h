/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xApp.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		single include header file
  *             "./dependencies/xLib/xApp.h"
  *             for entire xLib project
  * 
  * @details 	xApp.h defines the `xApp` singleton class
  *             to be used for app init & other important
  *             "global" app data during program life-cycle
  */

#pragma once

/// @brief begin of xAPP_H implementation
#ifndef xAPP_H
#define xAPP_H

// include windows accross all files from here ...
#include <windows.h>

// include uxtheme to allow for theme toggle between classic & modern ...
#include <uxtheme.h>

// include desktop window manager
// for tray icons, non-client area margins & alpha/transparency ...
#include <dwmapi.h>

// include "external" utils
#include "../utils/log/log.h"
#include "../utils/type/type.h"
#include "../utils/str/StrConverter.h"
#include "../utils/ex/SystemException.h"

// include macro for general MACRO's used in program script ...
// #include "../utils/macro/macro.h" // CMD.h includes macro.h

// include `CMD` for parsing `WinMain` cmd line args
#include "../utils/cmd/CMD.h"

// include `xFile` for `xIcon` & `xError`
#include "./utils/xFile.h"

// include for graphics library Gdiplus
#include "./global/xGDI.h"

// include for debugging
#include "./utils/xMsg.h"
// & error/exception handling ...
#include "./utils/xError.h"

// include Keyboard support ...
#include "./peripherals/Key.h"

// include icons
#include "./custom/xIcon.h"
// & fonts ...
#include "./custom/xFont.h"

// each widget & menu object can attach (some) events ...
#include "../events/iEvent.h"

// include `xWidgetManager` before `xApp`, since `xApp` initializes
// a singleton `xWidgetManager` instance to be used during runtime
#include "./controls/xWidgetManager.h"

// include before `xWidget`, since `xWidget` requires
// access to `xBorderFactory::get().storeBorder(...)`
// & `xBorderFactory::get().removeBorder(...)`
#include "./borders/xBorderFactory.h"

// include `xLib` menu system ...
#include "./menu/xMenuFactory.h"

/**
  * @class   xApp
  * @brief 	 singleton `xApp` header file implementation
  *          for application initialization & "global" data
  * @details implemets singleton design pattern for `xApp` <br/>
  *          `xApp` is responsible for initializing application state/data <br/>
  *          `xApp` is responsible for parsing `WinMain` command line args <br/>
  *          `xApp` is responsible for keeping track of "global" variables <br/>
  *          & other important app-related data & hosts the main message loop ... <br/>
  *  <br/>
  * `xApp` implements the singelton design pattern to ensure that there is always at most one <br/>
  * `xApp` instance throughout the duration of the program runtime, for managing RAII resources. <br/>
  *  <br/>
  * @see <a href="https://en.wikipedia.org/wiki/Singleton_pattern" target="_blank">singleton design pattern (C++)</a>
  * @see <a href="https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)" target="_blank">rule of three (C++)</a>
  * @see <a href="https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization" target="_blank">RAII (C++)</a>
  * 
  */
class xApp {

    /*%*%*%*%*%*%*%*%*%*%*%*/
    /* Application Runtime */
    /*%*%*%*%*%*%*%*%*%*%*%*/

public:

    /**
     * @brief   static method that retrieves the `xApp` singleton instance pointer
     * @return  reference to `xApp` singleton instance pointer
     * @details This class method returns an `xApp` instance pointer
     *          to the client code & privately constructs
     *          a new such instance if none exists
     */
    static xApp& get() {
        if (instance == nullptr) {
            // using private constructor
            instance = new xApp;
        }
        return *instance;
    }

    /// @brief delete copy constructor
    xApp(const xApp&)=delete;
    /// @brief delete copy assignment operator
    xApp& operator=(const xApp&)=delete;

    /// @brief method for destroying singleton `xApp` instance pointer
    static void destruct() {

        // cleanup ...
        
        // may have to re-order ...

        // `xWidgetManager`
        // destroy all resources held by `xWidgetManager`
        xWidgetManager::get().destruct();

        // `xGDI` shutdown ...
        // cleanup `xGDI` RAII resources ...
        // safe to destroy `xGDI` resources
        xGDI::get().destruct();
        
        // `xMsg` ...
        // safe to destroy xMsg resources ...
        xMsg::get().destruct();

        // `xSystemFont` ...
        // safe to destroy `xSystemFont` resources ...
        xSystemFont::get().destruct();

        // destroy all menu items ...
        xItemManager::get().destruct();

        // destroy all menu ...
        xMenuManager::get().destruct();

        // ... 

        // more code here, if any ...

        // ...

        // destroy msg translation unit ...
        // ensure the message map is destroyed ...
        #ifndef NDEBUG // only in debug mode ...
        xMsg::get().destruct();
        #endif // NDEBUG

        delete instance;
        instance = nullptr;

        LOG("All resources destroyed ...");
    }

    /// @brief hInstance received from `WinMain` args to be used by app runtime ...
    HINSTANCE hGlobalInstance = NULL;

    /**
      * @brief     method for processing used `WinMain` command line args
      * @param[in] hInstance ~ handle on window instance
      * @param[in] szCmdLine ~ zero-terminated string => command line args
      * @details  
      * Parses `szCmdLine` & storing `hInstance`
      * as `hGlobalInstance` within `xApp` singleton
      * <br/>
      * `init` is responsible for initializing the
      * singleton class instances, even though it's
      * NOT necessary to initialize here, but NOT
      * much overhead involved to do so ...
      */
    void init(HINSTANCE hInstance, LPSTR szCmdLine) {

        // store hInstance received from `WinMain`
        hGlobalInstance = hInstance; // store hInstance
        
        ParseCMD(szCmdLine);

        // initialize common controls library ...
        bool loaded = initCmnCtrl(); // default argument ~ ICC_STANDARD_CLASSES
        std::string message = "initCmnCtrl() => " + std::to_string(loaded);
        LOG(message.c_str()); // msg variable reserved for Win32 MSG queue struct

        // not necessary to initialize `xGDI` instance here ...
        // this `xGDI` instance will be used when converting .png to .ico ...
        // applications that load .ico don't need `xGDI` instance,
        // but we keep it here nonetheless, for cleanup by `xApp` later ...
        xGDI::get();

        // initialize `xWidgetManager` singleton instance
        xWidgetManager::get();

        // initialize msg translation unit ...
        #ifndef NDEBUG // only in debug mode ...
        // initialize `xMsg` singleton instance
        xMsg::get(); // `msgs` map is statically allocated ...
        #endif // NDEBUG
        
        // initialize `xFont` singleton instance
        xSystemFont::get().CollectSystemFonts();        
    }

    /**
     * @brief method for processing unused `WinMain` command line args
     * @param hInstance ~ handle on module instance
     * @param hPrevInstance ~ handle on previous instance
     * @param szCmdLine ~ zero-terminated string => command line args
     * @param nCmdShow ~ this parameter specifies how the app window is shown (show/hide)
     * @details suppress `UNUSED(hPrevInstance)` && `UNUSED(nCmdShow)`
     */
    void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) {

        // suppress unused argument compier warning ...
        UNUSED(hPrevInstance);
        UNUSED(nCmdShow);

        // show build mode to console for developer ...
        TPRINT(BUILDMODE);
        
        // pass the non-void params to the above init method
        // taking corresponding two parameters ...
        init(hInstance, szCmdLine);
    }

    /**
     * @brief     method to process the given command line args
     * @param[in] szCmdLine ~ the command line args received from `WinMain`
     */
    void ParseCMD(LPSTR szCmdLine) {
        // parse szCmd
        CMD::get().parse(szCmdLine);
        CMD::get().show();
        CMD::get().count();
        CMD::get().path();
        
        // usage example ...
        bool isFound = CMD::get().exists("-f"); // check if "-f" is a cmd line arg
        UNUSED(isFound); // suppress example `isFound`
    }

    /**
     * @brief     method to initialize common controls library
     * @param[in] flag ~ default value ~ `ICC_STANDARD_CLASSES`
     * @return    boolean value ~ indicating common controls loaded
     * @details   Common controls to be used in conjunction with
     *            an application manifest file to load vizual styles
     *            activated from WindowsXP++
     */
    bool initCmnCtrl(DWORD flag = ICC_STANDARD_CLASSES) {
        INITCOMMONCONTROLSEX icc { };
        icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icc.dwICC = flag; // ICC_STANDARD_CLASSES
        // initialize common controls
        return InitCommonControlsEx(&icc);
    }

    /**
      * @brief main application message queue
      *        to store Win32 messages received
      *        from `LRESULT CALLBACK WndProc(...)`
      */
    MSG msg; // message queue

    /**
      * @brief   messagepump() ~ main message queue handler
      * @details main message loop for the running application ...
      */
    int messagepump() {
        
        std::cout << "messagepump()" << std::endl;

        // init message queue
        msg = { };

        // main message loop ...
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg); // translate message
            DispatchMessage(&msg); // dispatch messages to correct handler 
        }

        // before program exit
        this->destruct();
        return EXIT_SUCCESS;
    }

private:

    /// @brief default singleton class constructor
    xApp() = default; // no public constructor
    /// @brief default singleton class destructor
    ~xApp() = default; // no public destructor

    /// @brief xApp static reference to singleton instance
    static xApp* instance;
};

/// @brief initialize xApp static singelton instance pointer
xApp* xApp::instance = nullptr;

// include `xTheme` class for allowing client to set classic/modern ...
// declared & defined after `xWidgetManager` since global theme setter
// must enumerate the xWidgets stored in `xWidgetManager::xWidgets` container
#include "./global/xTheme.h"

// include Mouse & Monitor support ...
// break up into two separate classes ...
// #include "./peripherals/xPeripheral.h"
#include "./peripherals/Mouse.h"
#include "./peripherals/Monitor.h"

// include `xImage` here (before `xImageButton` & others ...)
// since some `xControl` derived-types may interface with it
#include "./custom/xImage.h"

// includes after `xApp` declaration & definition
// include `xButton.h` before `xFrame.h`
// `xFrame` includes `xWidget` which includes `xWindow`
// `xButton` declaration & implementation to be "known" by `xWindow`
#include "./controls/xButton.h"
#include "./controls/xImageButton.h"
#include "./controls/xFrame.h"
// similar include order when declaring
// & implementing other `xWidget` derived types

/// @todo define types for:
/// `xLabel`, `xTextBox`, `xRadioButton`, `xCheckBox`, `xDropDown`, `xTreeView`, etc ...

// ...
#include "./xLabel.h"

// xTextBox requires CUSTOM implementation
// for drawing the borders & changing text
// & background color, as well as handling
// charet & other movement, like copy & paste, etc ...
// #include "./xTextBox.h"
#include "./xMultiLineTextBox.h" // includes `xTextBox` ...

// ComboBox ...
#include "./xDropDown.h"

// ListBox ...
// #include "./xListBox.h"
#include "./xMultiSelectListBox.h" // includes `xListBox`

// implement the following button types ...

// The below are NOT THE SAME AS
// checkbox menu items & radio items

// #include "./xCheckBox.h"
// #include "./xRadioButton.h"
// #include "./xButtonGroup.h"

// #include "./xTreeView.h"

// #include "./xXXX.h"
// ...

// widget derived types to be known
// before implementing each
// custom draw methods ...
#include "./global/xDraw.h"

// dialog boxes ...
#include "./xDialog.h"

#endif // end of xAPP_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
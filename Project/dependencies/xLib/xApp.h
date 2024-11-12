/**
  * @file    xApp.h
  * @author  &lambda;ambda
  * @date    \showdate "%Y-%m-%d"
  * @brief   single include header file "./src/xLib/xApp.h" for the entire project
  * @details xApp.h defines the `xApp` singleton class instance
  *          to be used for app init & other relevant/important
  *          "global" app data during runtime/life-cycle
  */

#pragma once

#ifndef xAPP_H
#define xAPP_H

// include log.h for logging build/debug info to `std::cerr`
#include "../utils/log/log.h"

// include `CMD.h` for parsing WinMain command line args
#include "../utils/cmd/CMD.h"

// include `macro.h` for `UNUSED(x)` MACRO
// to suppress "unused" warnings & `TPRINT(x)` MACRO
// for streaming `std::string` & `std::wstring` to the console
#include "../utils/macro/macro.h"

/**
  * `xApp` depends on `xManager` because `xApp`
  * is responsible for destroying data in `xManager`
  * when terminating the application/program
  * This may be refactoring in the future ...
  */
#include "./xManager.h"
// xManager.h includes xWidget.h
// xWidget.h includes commctrl.h

/**
  * @class   xApp
  * @brief   singleton xApp header file implementation
  *          for application initialization & "global" data
  * @details implement singleton design pattern for xApp
  *          xApp is responsible for initializing application state
  *          xApp is responsible for parsing WinMain command line args
  *          xApp is responsible for keeping track of "global" variables
  *          & other important app-related data ...
  */
class xApp {

public:
  /**
    * @brief   Enables client code to access unique `xApp` instance pointer
    * @return  reference to a singleton `xApp` instance pointer
    * @details This class method returns an `xApp` instance pointer
    *          to the client code & constructs a new such instance if none exists
    */
  static xApp& get() {
    // may be responsible for creating its own unique instance ...
    if (nullptr == instance) {
      instance = new xApp;
    }
    return *instance; // creating own unique instance
  }

  /// @brief deleted `xApp` copy constructor ~ adheres <a href="https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)" target="_blank">rule of three (C++)</a>
  xApp(const xApp&) = delete;
  /// @brief deleted `xApp` copy assignment operator ~ adheres <a href="https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)" target="_blank">rule of three (C++)</a>
  xApp& operator=(const xApp&) = delete;

  /// @brief method for destroying the singleton `xApp` instance pointer ~ 
  static void destruct() {
    delete instance;
    instance = nullptr;
  }

  /// @brief command line `CMD` instance to store data given via command line
  CMD cmd;

  /// @brief hInstance received from WinMain arguments to be used by app during runtime
  HINSTANCE hGlobalInstance;
  
  /**
    * @brief     method to process the given command line args
    *            display the data to the console, & check for specified flags
    * @param[in] szCmdLine ~ the command line arguments received from WinMain
    */
  void ParseCMD(LPSTR szCmdLine) {

    cmd.parse(szCmdLine);
    cmd.show();
    cmd.count();
    cmd.path();
    bool isFound = cmd.exists("-f");
    UNUSED(isFound);
  }
  
  /**
    * @brief     method for processing used WinMain command line args
    * @param[in] hInstance ~ handle on Window Instance
    * @param[in] szCmdLine ~ zero-terminated string => command line arguments
    * @details   Parses szCmdLine & storing hInstance as hGlobalInstance within xApp singleton
    */
  void init(HINSTANCE hInstance, LPSTR szCmdLine) {
    // store hInstance
    hGlobalInstance = hInstance;
    // process szCmdLine
    ParseCMD(szCmdLine);
    // initialize common controls library here ...
    bool loaded = initCmnCtrl(); // default param ~ ICC_STANDARD_CLASSES
    std::string message = "initCmnCtrl() => " + std::to_string(loaded);
    LOG(message.c_str());
  }

  /**
    * @brief     method for processing unused WinMain command line args
    * @param[in] hInstance ~ handle on Window Instance
    * @param[in] hPrevInstance ~ handle on Previous Window Instance
    * @param[in] szCmdLine ~ zero-terminated string => command line arguments
    * @param[in] nCmdShow ~ this parameter specifies how the application windows
    * @details   Suppresses UNUSED(hPrevInstance) & UNUSED(nCmdShow)
    */
  void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) {
    
    // suppress compiler warnings for unused params
    UNUSED(hPrevInstance);
    UNUSED(nCmdShow);

    TPRINT(BUILDMODE);

    // pass the non-void params to the
    // above init method taking
    // corresponding two parameters
    init(hInstance, szCmdLine);
  }

  /// @brief Method to initialize common controls library for the app
  /// @param flag default value ~ ICC_STANDARD_CLASSES
  /// @return boolean value ~ indicating common controls library loaded
  /// @details Common controls to be used in conjunction
  ///          an application manifest file to load visual styles
  ///          Visual Styles are activated from WinowsXP++
  bool initCmnCtrl(DWORD flags = ICC_STANDARD_CLASSES) {
    // declare the common controls structs
    INITCOMMONCONTROLSEX icc { };
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = flags; // ICC_STANDARD_CLASSSES
    // initialize common controls
    return InitCommonControlsEx(&icc);
  };

  /**
    * @brief main application message queue
    *        to store Win32 messages received
    *        from `LRESULT CALLBACK WndProc(...)`
    */
  MSG msg;

  /**
    * @brief   messagepump()
    * @details main message loop for running app ...
    */
  int messagepump() {

    std::cout << "messagepump()" << std::endl;

    // init message queue
    msg = { };

    // main message loop
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
      TranslateMessage(&msg); // translate messages
      DispatchMessage(&msg); // dispatch messages
    };

    // destroy `xManager` instance
    xManager::get().destruct();
    // destroy `xApp` instance
    this->destruct();
    return 0;
  }

private:

  /// @brief `default` `xApp` constructor
  xApp() = default; // no public constructor
  
  /// @brief `default` `xApp` destructor
  ~xApp() = default; // no public destructor

  /**
    * @brief `xApp` singleton instance pointer initialized
    *        to `new xApp()` using `xApp::get()`
    *        during program runtime to access class members
    */
  static xApp* instance;
};

/// @brief `xApp` singleton instance pointer initialized to `nullptr` before app runtime
xApp* xApp::instance = nullptr;

#endif

/**
 * TODO:
 * 
 * - Define:
 *   1) `xLabel`
 *   2) Define `xTextBox`
 *   3) Define `xTextArea`
 * 
 * - Activate visual-styles (Windows 95++)
 * 
 * - Define `xButton` derived types: (after visual styles)
 * 
 *   1) `xRadioButton`
 *   2) `xCheckBox`
 *   3) `DropDownMenu`
 *   4) `xStateButton`
 * 
 * - Define `xCheckList`/`xComboBox`/`xListBox`
 * 
 * - Define `xTreeView`/`xDateTimePicker`
 * 
 * etc ...
 * 
 */ 

/**
  * `xApp` includes `xFrame` & `xButton`
  * & other `x-controls` (still to be define)
  * at the END of this file, to ensure that
  * `xWindow.h` & `xControl.h` is included
  * AFTER `xApp.h` so that `xWindow.h` & `xControl.h`
  * have access to `xApp::get().hGlobalInstance`
  */
#include "./xFrame.h"
#include "./xButton.h"
// other x-controls to be included here ...
// #include "./xLabel"
// #include "./xTextBox"
// #include "./xTextArea"
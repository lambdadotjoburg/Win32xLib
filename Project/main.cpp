/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		 main.cpp
  * @brief 	  `WinMain` program entry point
  * @author 	 &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * 
  * @details 	This file contains the `WinMain` entry point of the program ...
  *           <br/>
  *           include "./src/xLib/xApp.h" to get started ...
  */

// single include file for `xLib` probject ...
#include "./dependencies/xLib/xApp.h"

/**
 * @name events
 * @{
 * Some Demo Client Event-Handlers ...
 * <br/>
 * Attaching/replacing some of these events
 * may remove the "default" behaviour, in which
 * case, client is to define ALL the logic for
 * custom event ... */
void ClickClientDemoHandler(xWidget* pWidget);
void MouseClickAnimationHandler(xWidget* pWidget);
void DoubleClickClientDemoHandler(xWidget* pWidget);
void FocusClientDemoHandler(xWidget* pWidget, FocusEvent event);
void MouseHoverClientDemoHandler(xWidget* pWidget, MouseEvent event);
void MouseDoubleClickClientDemoHandler(xWidget* pWidget, MouseKey key);
void CloseClientDemoHandler(xWidget* pWidget);
void MenuItemClickCloseEventClientDemoHandler(xMenuItem* pItem, xWidget* pWidget);
/** @} */

// Main program entry point ...
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) {

  // init `xApp` (DON'T REMOVE!) ...
  xApp::get().init(hInstance, hPrevInstance, szCmdLine, nCmdShow);

  // #ifndef NDEBUG // for developers ...
  // see all available message translations ...
  // xMsg::get().EnumerateMessages();
  // #endif

  /*%*%*%*%*%*%*%*%*/

  // 1. `xFrame` stuff ...
  
  xFrame parentFrame("Parent Frame", 1500, 950);
  // parentFrame.create();

  xFrame childFrame(&parentFrame, "Child Frame", 400, 300, 500, 600);
  // childFrame.show();

  xFrame siblingFrame(&parentFrame, "Sibling Frame", 400, 300, 1100, 600);
  // siblingFrame.show();

  /*%*%*%*%*%*%*%*%*/

  // 2. `xIcon` stuff ...

  parentFrame.setLargeIcon("./src/ico/90cpu.ico");
  parentFrame.setSmallIcon("./src/ico/win95.ico");

  childFrame.setLargeIcon("./src/ico/45cpu.ico");
  childFrame.setSmallIcon("./src/ico/setup.ico");

  siblingFrame.setLargeIcon("./src/ico/45cpu.ico");
  siblingFrame.setSmallIcon("./src/ico/setup.ico");

  // allow setting icons before/after create/show/add
  parentFrame.create();
  childFrame.show();
  siblingFrame.show();

  /*%*%*%*%*%*%*%*%*/

  // 3. `xButton` stuff ...

  /// @todo investigate button center problems ...
  xButton parentButton(&parentFrame, "OK", 60, 35, 150, 50);
  // parentButton.center(); // problems ...
  xButton childButton(&childFrame, "YES", 60, 35, 50, 50);
  xButton siblingButton(&siblingFrame, "NO", 60, 35, 50, 50);

  // attach a mouse listener to a button, more about this below ...
  parentFrame.setOnMouseHover(MouseHoverClientDemoHandler);

  /*%*%*%*%*%*%*%*%*/

  // 4. `xFont` stuff ...

  // Enumerating all System Fonts ...
  // xApp::get().EnumerateSystemFonts();
  // xSystemFont::get().EnumerateSystemFonts();

  // set font before/after create ...
  parentButton.setFont("Arial", 18, false, false, false, false);
  childButton.setFont("Calibri", 15, false, false, false, false);
  siblingButton.setFont("Cooper", 15, false, false, false, false);

  // adding `xComponent`s to an `xFrame`
  childFrame.add(&childButton); // childButton.create();
  parentFrame.add(&parentButton); // parentButton.create();
  siblingFrame.add(&siblingButton); // siblingButton.create();

  // default constructed button ...
  // NO default button attributes provided at construction,
  // i.e. no text font, no color, no borders, no fancy stuff ...
  xButton defaultButton;
  defaultButton.setText("Default");
  parentFrame.add(&defaultButton);
  defaultButton.setPosition(150,300);

  /*%*%*%*%*%*%*%*%*/

  // 5. Background color for `xWindow`s/`xFrame`s ...

  // using `RGB(...)` MACRO's ...
  // parentFrame.setBackgroundColor(123, 231, 132);
  // childFrame.setBackgroundColor(231, 132, 213);

  // using `COLORREF` ...
  // `WIN_2000_BG` & `WIN_95_BG` are `COLORREF` `#define`s in `xWidget` class ...
  childFrame.setBackgroundColor(WIN_95_BG);
  siblingFrame.setBackgroundColor(WIN_95_BG);
  parentFrame.setBackgroundColor(WIN_2000_BG);
  // client-to define own color constants, if so desired ...

  /*%*%*%*%*%*%*%*%*/

  // 6. "modality", `idle`/`inert` state & `fullscreen` mode ...

  // issues:
  // fullscree mode leaves a "gap" where
  // the task-bar would normally be,
  // for child frame minimized!

  // user preference / preserve state / persistent state ...
  // parentFrame.disable(); // persists until `.enable()` invoked ...

  // set modality for child window/frame
  childFrame.isModal(true); // must have a parent, else nothing ...
  siblingFrame.isModal(true); // multiple modal child frames ...

  // toggle fullscreen ...
  // parentFrame.FullScreenMode(true); // false => restore state ...

  /*%*%*%*%*%*%*%*%*/

  // 7. `xButton`s Background & Foreground color & text alignment ...

  parentButton.setTextColor(0, 0, 255);
  parentButton.setBackgroundColor(225, 0, 0);

  // set `childButton` styles
  childButton.setTextColor(255, 0, 255);
  childButton.setBackgroundColor(0, 255, 0);

  // set `siblingButton` styles
  siblingButton.setTextColor(255, 255, 0);
  siblingButton.setBackgroundColor(0,0,255);

  // global constants available for this ...
  parentButton.setTextAlignment(CENTER | VERTICAL | SINGLELINE); // default

  /*%*%*%*%*%*%*%*%*/

  // 8. `xBorder`s & `xBorderFactory` ...

  // `xBorderFactory` implements the factory design pattern
  //  to ensure proper resources management in the `xLib` system

  xBorder* parentButtonBorder = xBorderFactory::createLineBorder(
    0, 0, 0, // RGB
    3,       // line/stroke width
    8,       // radius
    xBorder::StrokeStyle::SOLID, // style
    3        // inset => distance from button edge
  );

  xBorder* childButtonBorder = xBorderFactory::createBevelBorder(
    0, 0, 0, // RGB
    2,       // width (edge) => shadow/3D effect
    0,       // radius => no effect on bevel border
    xBorder::ClassicStyle::RAISED, // style
    3        // inset => distance from button edge
  );

  xBorder* siblingButtonBorder = xBorderFactory::createEtchedBorder(
    0, 0, 0, // RGB
    1,       // width (edge) => shadow/3D effect
    3,       // thickness => distance between light & dark edges
    xBorder::ClassicStyle::SUNKEN, // style
    2        // inset => distance from button edge
  );

  /// @important:
  /// DO NOT ... <br>
  /// delete parentButtonBorder;  <br/>
  /// delete childButtonBorder;   <br/>
  /// delete siblingButtonBorder; <br/>
  /// <br/>
  /// factory pattern prohibits delete for `xBorder` objects, 
  /// It is illegal to `delete` `xBorder` & derived types
  /// because the destructor for these types are `protected`

  // Show all the used borders ...
  // xWidgetManager::get().EnumerateBorders();

  // Show all the unused/temp borders ...
  // xWidgetManager::get().EnumerateBorders();

  parentButton.setButtonBorder(parentButtonBorder);
  childButton.setButtonBorder(childButtonBorder);
  siblingButton.setButtonBorder(siblingButtonBorder);
  
  // Focus borders ...

  // Focus borders are equivalent to `xLineBorder`s with some pre-set params
  xBorder* sharedFocusBorder = xBorderFactory::createFocusBorder(
    100, 100, 100, // RGB
    1, // width (default 1)
    2, // radius (default 1)
    xBorder::StrokeStyle::DOT, // style (default `DOT`)
    8, // inset (default 4)
    1  // focus (default true)
  );

  // set focus border manually ...
  childButton.setFocusBorder(sharedFocusBorder);
  siblingButton.setFocusBorder(sharedFocusBorder);

  // set the hover color for a widget control explicitly ...
  siblingButton.setHoverColor(RGB(0,255,255)); // Cyan
  
  // Note*: No need to change the borders or background colors
  // using on-mouse-hover listener, since this can actually
  // cause unnecessary/unwanted flickering! <br/>
  // This wil happen whenever the mouse moves over
  // the widget => the color change will be triggered
  // It is ADVISED to use `.setHoverColor(...)` instead!

  // remove all unused borders from `xBorderFactory` ...
  xBorderFactory::get().clearTemp();

  // log the `xBorderFactory` details to the console ...
  xBorderFactory::get().LogBorderData();

  /*%*%*%*%*%*%*%*%*/

  // 9. `iEvent`s

  /// @note Attaching client-defined event handlers,
  /// may overwrite some default behaviour for the control
  /// Only use client-defined event handlers if you know
  /// what you're doing ...
  childButton.setOnClick(ClickClientDemoHandler);
  childButton.setOnDoubleClick(DoubleClickClientDemoHandler);
  childButton.setOnMouseDoubleClick(MouseDoubleClickClientDemoHandler);

  // for use in demonstration of below `setOnClick` anonymous callback function ...
  int variable = 10; // capture variable ...
  // demonstrateion of lambda/anonymous button's on-click callback function ...
  siblingButton.setOnClick(
    [&variable](xWidget* pWidget) -> void {
      // do something with capture variable ...
      std::cout << "variable: " << variable << std::endl;
      // do something with pWidget, like stream details ...
      std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;
    }
  );

  // childFrame.setDefaultCloseOperation(CloseEvent::DO_NOTHING);
  childFrame.setOnClose(CloseClientDemoHandler); // default `CLOSE`

  /*%*%*%*%*%*%*%*%*/

  // 10. `xImageButton`s ...

  // default width & height for the button are that of the image attached to it
  // if the image fails to load, then the dimensions default to the "usual"
  // otherwize, if client-code specifies dimensions, it is used instead

  // Note*: .jpg & .jpeg are NOT TRANSPARENT!
  // It is advised to use .bmp, .png or .ico for transparency ...

  xImageButton parentImageButton(
    &parentFrame, "./src/jpg/moss.jpg",
    "OK", parentFrame.getWidth()-350, 50, 150, 150
  );
  parentImageButton.setBackgroundColor(RGB(0, 255, 0));  
  xBorder* parentImageButtonBorder = xBorderFactory::createLineBorder(
    255, 0, 0, // RGB
    10,        // width (edge) => shadow/3D effect
    20,        // radius => no effect on bevel border
    xBorder::StrokeStyle::SOLID, // style
    10         // inset => distance from button edges
  );
  parentImageButton.setButtonBorder(parentImageButtonBorder);
  parentFrame.add(&parentImageButton);

  xImageButton childImageButton(
    &childFrame, "./src/png/gears_hollow.png",
    "OK", childFrame.getWidth()-150, 50, 100, 100
  );
  childImageButton.setBackgroundColor(RGB(0, 123, 213));
  xBorder* childImageButtonBorder = xBorderFactory::createBevelBorder(
    255, 0, 0, // RGB
    5,       // width (edge) => shadow/3D effect
    0,       // radius => no effect on bevel border
    xBorder::ClassicStyle::RAISED, // style
    8        // inset => distance from button edge
  );
  childImageButton.setButtonBorder(childImageButtonBorder);
  childFrame.add(&childImageButton);
  
  xImageButton siblingImageButton(
    &siblingFrame, "./src/bmp/maple.bmp",
    "OK", siblingFrame.getWidth()-150, 50, 100, 100
  );
  siblingImageButton.setBackgroundColor(RGB(0, 255, 0));
  xBorder* siblingImageButtonBorder = xBorderFactory::createEtchedBorder(
    255, 0, 0, // RGB
    1,       // width (edge) => shadow/3D effect
    2,       // radius => thickness for etched border
    xBorder::ClassicStyle::RAISED, // style
    1        // inset => distance from button edge
  );
  siblingImageButton.setButtonBorder(siblingImageButtonBorder);
  siblingFrame.add(&siblingImageButton);

  xImageButton anotherButton(
    &parentFrame, "./src/gif/hourglass.gif", "OKAY",
    parentImageButton.getHorizontalPosition(), 300,
    100, 100
  ); // reverse dimensions & coordinates
  anotherButton.setBackgroundColor(RGB(255, 255, 0));
  xBorder* anotherButtonBorder = xBorderFactory::createEmptyBorder(10, 10);
  anotherButton.setButtonBorder(anotherButtonBorder);
  parentFrame.add(&anotherButton);
  anotherButton.setOnClick(MouseClickAnimationHandler);

  // method to set the animation duration ...
  anotherButton.setDuration(10); // 10 seconds ...

  /*%*%*%*%*%*%*%*%*/

  // 11. menus

  // 11.0 various ways to create menus & menu items ...

  // 11.0.1 create menus & menu items via `xMenuFactory` class ...

  // xMenu* menu = xMenuFactory::createMenu(...); // optional arg
  // xPopupMenu* popup = xMenuFactory::createPopupMenu(...);
  // xRadioMenu *select = xMenuFactory::createRadioMenu(...);
  // xMenuItem *item = xMenuFactory::createMenuItem(...);
  // xRadioItem* choice = xMenuFactory::createRadioItem(...);

  /*%*%*%*%*%*%*%*%*/

  // 11.0.2 create menus & menu items
  // via `xMenuManager` & `xItemManager` ...

  // xMenu* menu = xMenuManager::createMenu(...); // optional arg
  // xPopupMenu* popup = xMenuManager::createPopupMenu(...);
  // xRadioMenu *select = xMenuManager::createRadioMenu(...);
  // xMenuItem *item = xItemManager::createMenuItem(...);
  // xRadioItem* choice = xItemManager::createRadioItem(...);

  /*%*%*%*%*%*%*%*%*/

  // 11.0.3 create menus & menu items
  // using public static `create(...)` interface method

  // xMenu* menu = xMenu::create(...); // optional arg
  // xPopupMenu* popup = xPopupMenu::create(...);
  // xRadioMenu *select = xRadioMenu::create(...);
  // xMenuItem *item = xMenuItem::create(...);
  // xRadioItem* choice = xRadioItem::create(...);

  // all of these interface with `xMenuManager` & `xItemManager`
  // to properly store & manager menu & menu item resources ...

  // Note*: menu & menu item classes have protected constructor & destructor

  /*%*%*%*%*%*%*%*%*/

  // 11.1 creating menus & menu items ...

  // create a menu to attach to the `xFrame` widget
  xMenu* topLevelMenu = xMenu::create(); // no arg specified

  // define a menu item to attach to the frame's menu bar ...
  xMenuItem* fileMenuItem = xMenuItem::create("File"); // string arg => item text
  topLevelMenu->add(fileMenuItem); // add the menu item to the menu bar

  // define more menu items here ...
  xMenuItem* aboutMenuItem = xMenuItem::create("About");
  topLevelMenu->add(aboutMenuItem); // add the menu item to the menu bar

  // capture variable for demonstration of 
  int response = -1;
  // do something when click "About" menu item,
  // like show a new dialog box/message box ...
  aboutMenuItem->setOnMenuItemClick(
    // about menu item callback ...
    [&response](xMenuItem* pItem, xWidget* pWidget) -> void {
      
      std::cout << pItem->getText() << std::endl;
      std::cout << pWidget->getText() << std::endl;
      
      std::string dialogTitle = "About Dialog Title ...";
      std::string dialogMessage = "This is an about dialog box ...";
      xDialog::DialogType dType = xDialog::DialogType::OKCANCEL; // no default ...
      xDialog::IconType iType = xDialog::IconType::CONFIRM; // default
      
      response = xDialog::xDialogBox(
        pWidget, dialogTitle, dialogMessage, dType, iType
      );

      // do something with response ...
      std::cout << "response: " << response << std::endl;

      if (response == 1) {
        std::cout << "confirm!" << std::endl;
      } else if (response == 2) {
        std::cout << "cancel!" << std::endl;
      }
    }
  );

  /// @important:
  /// DO NOT ...           <br/>
  /// delete topLevelMenu; <br/>
  /// delete fileMenuItem; <br/>
  /// <br/>
  /// factory pattern prohibits `delete` for any of the menu
  /// types & it's menu item types objects ... <br/>
  /// `delete`-ing any of these types is illegal/prohibited,
  /// because the destructor for these types are `protected`
  /// & is managed within the manager classes ...

  // create a popup/context menu to appear
  // whenever `fileMenuItem` is selected
  xPopupMenu* filePopupMenu = xPopupMenu::create();

  // add the `filePopupMenu` context/popup to the menu item, i.e. the frame's menu bar ...
  fileMenuItem->add((xMenu*)filePopupMenu); // add popup menu to file menu item

  // populate the `fileMenuItem` with more menu items ...
  xMenuItem* openItem = xMenuItem::create("Open ...");
  xMenuItem* saveItem = xMenuItem::create("Save");
  xMenuItem* saveAsItem = xMenuItem::create("Save As ...");
  // xMenuItem* exitItem = xMenuItem::create("Exit");
  xMenuItem* exitItem = xMenuItem::create("Exit", MenuItemClickCloseEventClientDemoHandler);

  /// @remark
  /// The only downside of "separator" API is that client-code <br/>
  /// to carefully when using "separator" as text data for the <br/>
  /// menu item, UNLESS `separator(false)` is set right after  <br/>
  /// creating the separator menu item object                  <br/>
  /// <br/>
  /// The upside is that this is more intuitive to use for     <br/>
  /// creating a menu item separator, using "separator"        <br/>
  /// <br/>
  /// This will display separator in the menu item's text      <br/>
  /// It is extremely unlikely that this will ever be a requirement!

  // someMenuItem->separator(false); // any item can be made a separator
  xMenuItem* separator = xMenuItem::create("separator"); // re-usable

  // add the menu items to the file popup menu ...
  filePopupMenu->add(openItem);   // append to popup menu
  filePopupMenu->add(separator);  // append to popup menu
  filePopupMenu->add(saveItem);   // append to popup menu
  filePopupMenu->add(saveAsItem); // append to popup menu
  filePopupMenu->add(separator);  // append to popup menu
  filePopupMenu->add(exitItem);   // append to popup menu

  childFrame.setMenu(topLevelMenu);
  // no effect, since menus & items are UNIQUE!
  // siblingFrame.setMenu(topLevelMenu);

  // create a submenu, which is itself an `xPopupMenu` object ...
  xPopupMenu* openSubMenu = xPopupMenu::create();

  // add the sub(popup)menu to the menu item ...
  openItem->add(openSubMenu);
  
  // create menu items to append to the sub(popup)menu ...
  xMenuItem* openFileSubMenuItem = xMenuItem::create("Open File ...");
  xMenuItem* openMultipleFilesSubMenuItem = xMenuItem::create("Open Files ...");
  xMenuItem* openFolderSubMenuItem = xMenuItem::create("Open Folder ...");
  
  // append the items to the sub(popup)menu ...
  openSubMenu->add(openFileSubMenuItem);
  openSubMenu->add(openMultipleFilesSubMenuItem);
  openSubMenu->add(openFolderSubMenuItem);

  std::cout << "openSubMenu->size()" << openSubMenu->size() << std::endl;

  // set menu for the frame after creating the menu ...
  parentFrame.setMenu(topLevelMenu);
  childFrame.setMenu(topLevelMenu);
  siblingFrame.setMenu(topLevelMenu);

  /*%*%*%*%*%*%*%*%*/

  // capture variable for demo open-file
  // menu item click (dailog box) callback/action ...
  std::string filePath; // store path of file open by user ...
  std::string fileContent; // store the contents of the file, for later use ...

  // This is an `ordered` map, thus,
  // to ensure that selected index
  // is correct, the map must be
  // alphabetically ordered correctly ...
  std::map<std::string, std::string> fileTypes {
    { "ALL/ANY",     "*.*"    }, // wildcard / all file types ...
    { "Audio/Video", "*.mp4"  },
    { "Excel",       "*.xlsx" }, // selected item index ...
    { "MS Word",     "*.docx" },
    { "text",        "*.txt"  },        
    { "Webfile",     "*.html" },
    { "xLambda",     "*.xl"   },
  }; // pass this map as capture variable to lambda function ...

  // This will be applied to all
  // open-file/save-file dialog operations,
  // unless explicit `fileTypes` map is provided ...
  xDialog::setDefaultExtensionsMap(fileTypes);

  // open-file menu item click event
  // (dialog box) demo callback/action ...
  openFileSubMenuItem->setOnMenuItemClick(
    // capture variables ...
    [&filePath, &fileContent /*, &fileTypes*/]
    (xMenuItem* pItem, xWidget* pWidget) -> void {

      std::cout << "open-file dialog box demo ..." << std::endl;

      // // do something with `pWidget`
      IMPLICIT(pWidget);
      // std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;

      // // do something with `pItem`
      IMPLICIT(pItem);
      // std::cout << "pItem->getText() => " << pItem->getText() << std::endl;

      // invoke `OpenFileDialogBox(...)`
      
      // std::string result =
      // xDialog::OpenFileDialogBox(
      //   pWidget, "", fileTypes, 3
      // );

      // index 3 => `{"Excel", "*.xlsx"}`
      std::string result = xDialog::OpenFileDialogBox(pWidget);
      
      // check whether the retrieved file path is NOT empty!
      if (!result.empty()) {
        // do something with result, like store to `filePath` ...
        filePath = result; // store the file path ...
        // print the file path ...
        std::cout << "filePath: " << filePath << std::endl;

        // read the file contents
        // & store in string variable `fileContent`
        // client-code to store in wide string format,
        // for `UNICODE` applications ...
        std::ifstream file(filePath);

        // check if file can be opened ...
        if (!file.is_open()) {
          // otherwize, inform used that
          // the file couldn't be opened,
          // or some similar operation!
          std::string message = "The file could not be opened";
          std::cerr << message << std::endl;
          xDialog::xDialogBox(
            pWidget, "File Error",
            message, // std::cerr << message;
            xDialog::DialogType::OK,
            xDialog::IconType::ERR
          );
          // escape the function ...
          return;
        };

        // otherwize, process the file's data ...
        // read line-by-line ...
        std::string line; // variable to hold a line in the file ...
        while(std::getline(file, line)) {
          fileContent += line;
        } // close after reading ...
        file.close(); // close the file ...
        // do something with `fileContent` ...
        std::cout << "fileContent: " << fileContent << std::endl;
        fileContent.clear();
      }
    }
  );
  
  // capture variable for storing paths of files open by user ...
  std::vector<std::string> filesPaths;
  // capture variable for storing the
  // contents of the files, for later use ...
  // NOT necessary, since `filesPaths`
  // can always be used later to read
  // the data from the files on request ...
  std::vector<std::string> filesContents;

  // menu item click (dailog box) callback/action ...
  openMultipleFilesSubMenuItem->setOnMenuItemClick(
    // capture variables ...
    [&filesPaths, &filesContents /*, &fileTypes*/]
    (xMenuItem* pItem, xWidget* pWidget) -> void {

      std::cout << "open-multiple-file dialog box demo ..." << std::endl;

      // // do something with `pWidget`
      IMPLICIT(pWidget);
      // std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;

      // // do something with `pItem`
      IMPLICIT(pItem);
      // std::cout << "pItem->getText() => " << pItem->getText() << std::endl;

      // invoke `OpenFileDialogBox(...)`
      
      // std::string result = // ...
      // xDialog::OpenFileDialogBox(pWidget, fileTypes);

      // std::vector<std::string> files = // ...
      // xDialog::OpenMultipleFilesDialogBox(
      //   pWidget, "", 4, fileTypes, 1
      // );

      std::vector<std::string> files = // ...
      xDialog::OpenMultipleFilesDialogBox(pWidget);

      // do something with files, like store to `filesPaths` ...
      filesPaths = files;

      // get/store the size of the file vector/container ...
      int s = filesPaths.size();
      // print the file paths count, i.e. the number of selected files ...
      std::cout << "filesPaths.size() => " << s << std::endl;

      // check whether the retrieved file path is NOT empty!
      // if (!filesPaths.empty()) { // alternative to `s > 0`
      if (s > 0) { // check count positive ...
        // iterate the file paths ...
        for (int i = 0; i < s; i++) {
          // & print to console ...
          std::cout << filesPaths[i] << std::endl;
        }
        // store the file content to `filesContents`
        filesContents.reserve(s);
        // This activity is left as an exercise for the reader ... :)
      }

      // clear the container/vector for another round ...
      filesPaths.clear();
    }
  );

  // capture variable for demo open-folder
  // menu item click (dailog box) callback/action ...
  std::string folderPath; // store the path of folder open by user ...
  // std::string folderContent; // store the open-file data in this string ...

  // open-folder menu item click event
  // (dialog box) demo callback/action ...
  openFolderSubMenuItem->setOnMenuItemClick(
    [&folderPath](xMenuItem* pItem, xWidget* pWidget) -> void {

      std::cout << "open-folder dialog box demo ..." << std::endl;

      // do something with `pItem`
      std::cout << "pItem->getText() => " << pItem->getText() << std::endl;
      // do something with `pWidget`
      std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;

      std::string folder = xDialog::OpenFolderDialogBox(
        pWidget, // pointer of the parent widget ...
        "C:\\Users\\Lambda\\Documents\\", // Optional
        "Folder Explorer Dialog Search Box ...",
        xDialog::BrowserAttributes::MODERN
          // xDialog::BrowserAttributes::DIRS
        // | xDialog::BrowserAttributes::READONLY
        // | xDialog::BrowserAttributes::MODERN
        // "Optional File Open Dialog Box Title ..."
      );

      // Check whether the selected folder path is not empty ...
      if (!folder.empty()) {

        // print the selected folder path to the console ...
        std::cout << "folder path: " << folder << std::endl;
        // do a recursive algorithm to iterate
        // the contents of the file or whaterver ...
      }      
    }
  );

  // disabling menu items ...
  // saveItem->disable();
  // saveItem->enable();
  // Don't disable `saveItem` for demo
  // purposes on save-click events ...

  // Enable `saveItem` for demonstration
  // purposes with save-file-dialog ...
  saveItem->enable(); // true ...

  // capture variable for save-file-menu-item-click-dialog demo ...
  std::string saveFileContentDemoData = "Some data by user, usually a (rich) textbox ...";
  
  // if `saveFilePath` is empty then dialog prompt,
  // does not suggest file path string to (over)write
  std::string saveFilePath; // store user-selected path ...

  // save-file menu item click event
  // (dialog box) demo callback/action ...
  saveItem->setOnMenuItemClick(
    [&saveFilePath, &saveFileContentDemoData]
    (xMenuItem* pItem, xWidget* pWidget) -> void {

      // do something with `pItem`
      std::cout << "pItem->getText() => " << pItem->getText() << std::endl;
      // do something with `pWidget`
      std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;

      // Quick-Save functionality,
      // i.e. save without save-file-dialog & no prompt ...
      // No need for save-file-dialog, upon `ctrl` + `s`

      // ...

    }
  );

  // capture variables for save-as-file-menu-item-click-dialog demo ...
  std::string saveAsFileContentDemoData = "Some data by user (usually a textbox) ...";
  
  // The below string path is used to store user-selected path
  // but can also be used to specify the file to overwrite
  // (if provided before-hand & the file path exists exists) ...
  std::string saveAsFilePath = "";
  // std::string saveAsFilePath = "C:\\Example\\of\\File\\that\\does\\NOT\\Exist\\";

  // capture variable for default open-prompt-directory/location ...
  // client-code to dynamically find `Users` directory
  // std::string saveAsFileDefaultDirectory = "C:\\Users\\Lambda\\Documents\\";
  std::string saveAsFileDefaultDirectory = "";

  // save-file menu item click event
  // (dialog box) demo callback/action ...
  saveAsItem->setOnMenuItemClick(
    [ &saveAsFilePath, &saveAsFileContentDemoData,
      &fileTypes, &saveAsFileDefaultDirectory ]
    (xMenuItem* pItem, xWidget* pWidget) -> void {

      std::cout << "save-as-file dialog box demo ..." << std::endl;

      // do something with `pItem`
      std::cout << "pItem->getText() => " << pItem->getText() << std::endl;
      // do something with `pWidget`
      std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;

      // std::string path = xDialog::SaveFileDialogBox(
      //   pWidget, saveAsFilePath,
      //   saveAsFileDefaultDirectory,
      //   true, fileTypes, 4
      // );

      std::string path = xDialog::SaveFileDialogBox(
        pWidget, saveAsFilePath, saveAsFileDefaultDirectory
      );

      // check if the returned path is NOT empty ...
      if (!path.empty()) {
        // overwrite `saveFilePath` string variable ...
        saveAsFilePath = path;
        // print out the save-file-path ...
        // ment
        std::cout << "saveAsFilePath: " << saveAsFilePath << std::endl;
        // do something with `saveFilePath`, like ...
        // write `saveAsFileContentDemoData` to `saveAsFilePath`
        // ...
        // ...
      }       
    }
  );

  // 11.2 `xPopupMenu` / context/sub-menus ...

  // create a context/popup menu object to associate to a frame widget
  xPopupMenu* contextMenu = xPopupMenu::create();

  // create a menu item to attach/add/append to the context/popup menu ...
  xMenuItem* contextMenuItemFirst = xMenuItem::create("First Item ...");
  xMenuItem* contextMenuItemSecond = xMenuItem::create("Second Item ...");
  xMenuItem* contextMenuItemThird = xMenuItem::create("Third Item ...");

  // add the context menu items to the popup/context menu ...
  contextMenu->add(contextMenuItemFirst);
  contextMenu->add(contextMenuItemSecond);
  contextMenu->add(contextMenuItemThird);

  // create a sub(context/popup)menu object to attach to a menu item ...
  xPopupMenu* subContextMenu = xPopupMenu::create();

  // create submenu items to attach to the sub(context/popup)menu strip
  xMenuItem* subContextMenuItemFirst = xMenuItem::create("First Subitem ...");
  xMenuItem* subContextMenuItemSecond = xMenuItem::create("Second Subitem ...");
  
  // add the sub(context/popup)menu items to the `subContextMenu`
  // Order of insertion can be specified as an arg for the add method
  // to indicate at which index to insert a menu item in the menu!
  subContextMenu->add(subContextMenuItemFirst, 1);  // higher index
  subContextMenu->add(subContextMenuItemSecond, 0); // lower index

  // More examples of inserting menu items at specified indices ...
  xMenuItem* subContextMenuItemThird = xMenuItem::create("Third Subitem ...");
  xMenuItem* subContextMenuItemFourth = xMenuItem::create("Fourth Subitem ...");
  subContextMenu->add(subContextMenuItemThird, 4);  // testing index insertion
  subContextMenu->add(subContextMenuItemFourth, 2); // testing index insertion

  // add the sub(context)menu to the `siblingContextMenuItemSecond`
  contextMenuItemSecond->add(subContextMenu);

  // add the context menu to the sibling frame/widget object ...
  siblingFrame.setContextMenu(contextMenu);

  /*%*%*%*%*%*%*%*%*/

  // 11.3 menu-item-click events ...

  // client-defined on-menu-item-click event callback/action
  // to demonstrate how to append a sibling menu item ...
  subContextMenuItemThird->setOnMenuItemClick(
    [&separator](xMenuItem* pItem, xWidget* pWidget) -> void {
      std::cout << "anonymous/lambda callback" << std::endl;
      // do something with `pWidget` ...
      std::cout << pWidget->getText() << std::endl;
      // do something with `pItem` ...
      std::cout << *pItem << std::endl;
      // append an item to the menu item's parent menu ...
      xMenu* parent = pItem->getParent();
      parent->add(xMenuItem::create("Sibling Subitem"));
      std::cout << "parent->size() => " << parent->size() << std::endl;
    }
  );

  // client-defined on-menu-item-click event callback/action
  // to demonstrate how to append append a sibling of the parent menu ...
  subContextMenuItemFourth->setOnMenuItemClick(
    [&separator](xMenuItem* pItem, xWidget* pWidget) -> void {      
      std::cout << "anonymous/lambda callback" << std::endl;
      // do something with `pItem` like ...
      std::cout << *pItem << std::endl;
      // pItem->parent->parent->... //  check if exists ...
      // do something with `pWidget` ...
      // append an item to the item's parent menu's parent menu ...
      xPopupMenu* popup = pWidget->getContextMenu();
      // append an existing "separator" menu item object each time ...
      popup->add(separator);
      // append a menu item with text != "separator"
      popup->add(xMenuItem::create("Dynamic Item ..."));
      // append a new menu item object each time ...
      popup->add(xMenuItem::create("separator"));
      // print popup menu data ...
      std::cout << *popup << std::endl;
    }
  );

  // checkboxes
  contextMenuItemFirst->setOnMenuItemClick(
    [&variable](xMenuItem* pItem, xWidget* pWidget) -> void {
      // do something with pWidget ...
      UNUSED(pWidget);
      // do something with pItem ...
      std::cout << "toggle menu item's check state demo ..." << std::endl;
      pItem->setCheckState(!pItem->getCheckState());
      std::cout << "variable: " << variable << std::endl;
    }
  );

  /*%*%*%*%*%*%*%*%*/

  // 11.4 menu item images/icons ...

  // Explicitly specify w & h for the image (default w & h 15)
  contextMenuItemSecond->setImage("./rsc/img/icon.ico", 25, 25);
  // w & h -1 => using the original image dimensions (no re-scaling)
  contextMenuItemThird->setImage("./src/png/setup.png", -1, -1);
  // set the check-state image
  contextMenuItemFirst->setCheckStateImage("./rsc/img/tickgreen.ico");
  // set the uncheck-state image
  contextMenuItemFirst->setUncheckStateImage("./rsc/img/crossred.ico", 15, 15);

  // getting the image directory path associated to a menu item ...
  std::cout << contextMenuItemSecond->getCheckStateImagePath() << std::endl;
  std::cout << contextMenuItemSecond->getUncheckStateImagePath() << std::endl;

  // menu-changed-listener demo to extract menu object's data ...
  contextMenu->setOnMenuChange(
    [&variable](xMenuItem* pItem, xMenu* menu) -> void {
      std::cout << "mOnMenuChange client demo handler" << std::endl;
      // do something with pItem => the item that triggered the menu change ...
      std::cout << "pItem->getText() => " << pItem->getText() << std::endl;
      // do something with menu => the menu that receives the menu change notfication ...
      std::cout << "menu->size() => " << menu->size() << std::endl;
      std::cout << "variable: " << variable << std::endl;
    }
  );

  // create a menu item to attach/add/append to the context/popup menu ...
  xMenuItem* contextMenuItemFourth = xMenuItem::create("Radio BG Ex 1 ...");

  // add the context menu items to the popup/context menu ...
  contextMenu->add(contextMenuItemFourth); // append new item to context menu ...

  // create a radio item menu object to attach to a menu item ...
  xRadioMenu* radioItemMenu = xRadioMenu::create();

  // create radio buttons to attach to the radio item (sub)menu 
  xRadioItem* radioItemFirst = xRadioItem::create("Radio Item 1");
  xRadioItem* radioItemSecond = xRadioItem::create("Radio Item 2");
  xRadioItem* radioItemThird = xRadioItem::create("Radio Item 3");
  xRadioItem* radioItemFourth = xRadioItem::create("Radio Item 4");
  
  // add the radio button items to the radio item menu
  radioItemMenu->add(radioItemFirst);
  radioItemMenu->add(radioItemSecond);
  radioItemMenu->add(radioItemThird);
  radioItemMenu->add(radioItemFourth);

  // append the radio item menu to the context menu item
  contextMenuItemFourth->add(radioItemMenu);

  // set the (default) check state for any of the radio item ...
  // AT MOST ONE item can be selected at a time, i.e.
  // if one item is selected after another, then the
  // previous item's select state is toggled off ...
  radioItemSecond->setCheckState(true); // this item will be unselected
  radioItemThird->setCheckState(true); // this item will be selected

  // on menu change demo to get selected radio item menu's active item ...
  radioItemMenu->setOnMenuChange(
     [](xMenuItem* pItem, xMenu* pMenu) -> void {
      // do something with pMenu, like get
      // the active/selected radio item ...
      IMPLICIT(pMenu);
      // do something with pItem ...
      std::cout << pItem->getText() << std::endl;
      // cast the menu to an appropriate type ...
      xRadioMenu* pRadioMenu = static_cast<xRadioMenu*>(pMenu);
      // get the selected item ID ...
      std::cout << pRadioMenu->getSelectedItemID() << std::endl;
      // get the selected item ...
      xMenuItem* pSelectedItem = pRadioMenu->getSelectedItem();      
      // if (pSelectedItem) { // safe, but not necessary to check ...
        std::cout << "pSelectedItem->getID() => " << pSelectedItem->getID() << std::endl;
      // }
      // xRadioItem* pSelectedItem = static_cast<xRadioMenu*>(pMenu)->getSelectedItem(); // one-liner
    }
  );

  // set the (default) check state for any of the radio buttons ...
  // only one item can be selected at a time, i.e. if
  // one item is selected after another, then the
  // previous item's select state is switched off ...
  radioItemSecond->setCheckState(true); // this item will be unselected
  radioItemThird->setCheckState(true); // this item will be selected

  /*%*%*%*%*%*%*%*%*/

  // 11.5 insert items from a vector into a menu ...

  // 11.5.1 Example of a radio item menu ...

  // create a (sub)menu object
  xMenu* nestedMenu = xMenu::create();

  std::vector<xMenuItem*> nestedMenuItems = {
    xMenuItem::create("Nested 1"),
    xMenuItem::create("Nested 2"),
    xMenuItem::create("Nested 3"),
    xMenuItem::create("Nested 4"),
  };

  // add the vector items to the submenu ...
  nestedMenu->add(nestedMenuItems);
  // add the submenu to the context menu ...
  contextMenuItemThird->add(nestedMenu);
  subContextMenuItemThird->add(nestedMenu); // won't be added, since menu in use ...

  /*%*%*%*%*%*%*%*%*/

  // 11.5.2 yet another radio item menu example ...

  // create a menu item to attach/add/append to the context/popup menu ...
  xMenuItem* contextMenuItemFifth = xMenuItem::create("Radio BG Ex 2 ...");

  // add the context menu items to the popup/context menu ...
  contextMenu->add(contextMenuItemFifth); // append new item to context menu ...

  // create a radio button menu object to attach to a menu item ...
  // while providing a callback menu-changed function as arg ...
  // xRadioMenu* altRadioMenu = xRadioMenu::create();
  xRadioMenu* altRadioMenu = xRadioMenu::create(
    [](xMenuItem* pItem, xMenu* pMenu) -> void {
      std::cout << "alt radio menu create() lambda callback demo ..." << std::endl;
      // do something with pItem ...
      std::cout << "pItem->getText() => " << pItem->getText() << std::endl;
      // do something with pMenu ...
      std::cout << "pMenu->size() => " << pMenu->size() << std::endl;
    }
  );

  // client-usability preferred add method for radio button menus
  std::vector<xRadioItem*> altRadioItems = {
    xRadioItem::create("Alt Radio Item 1"),
    xRadioItem::create("Alt Radio Item 2"),
    xRadioItem::create("Alt Radio Item 3"),
    xRadioItem::create("Alt Radio Item 4"),
  };

  // add the radio items to the radio item menu
  // optional (second) parameter to specify selected item ...
  altRadioMenu->add(altRadioItems, 2); // item at index 2 selected ...

  // easy example to demonstrate the use of a loop
  // for setting the check & uncheck-state images
  // for ALL the items in a menu's item list ...
  for (int i = 0; i < altRadioMenu->size(); i++) {
    // set the check-state image for all the items ...
    altRadioMenu->getItemByIndex(i)->setCheckStateImage("./rsc/img/circlegreen.ico");
    // set the uncheck-state image for all the items ...
    altRadioMenu->getItemByIndex(i)->setUncheckStateImage("./rsc/img/circlered.ico");
  }

  // add the radio button menu to the context menu item
  contextMenuItemFifth->add(altRadioMenu);

  /*%*%*%*%*%*%*%*%*/

  // 11.6.1 getting menus & menu items ...

  // To access menu items, it is advised to use any of:

  xMenuItem* item = subContextMenu->getItemByID(14);
  // check if the item actually exists ...
  if (item) { std:: cout << "*subContextMenu->getItemByID(13) => " << *item << std::endl; }  
  item = subContextMenu->getItemByIndex(1);
  if (!item) { std::cout << "subContextMenu->getItemByIndex(0) => " << *item << std::endl; }
  item = subContextMenu->getItemByText("First Subitem ...");
  if (!item) { std::cout << "subContextMenu->getItemByText(\"First Subitem ...\") => " << *item << std::endl; }
  
  // demonstration of retrieving/using a menu item that has not been found ...
  xMenuItem* notFound = subContextMenu->getItemByText("Item NOT FOUND!");

  // check if not `nullptr`
  if (notFound) { // if exists ...
    std::cout << "subContextMenu->getItemByText(\"First Subitem ...\") => " << "NOT Found!" << std::endl;  
  } else { // if not exists ...
    std::cout << "subContextMenu->getItemByText(\"First Subitem ...\") => " << notFound << std::endl;  
  }
  
  // Below demonstrates how to use subscript `[index]` operator
  // to access menu items, NOT preferred, since private destructor
  // & no copy assignment & copy constructor for `xMenuItem` class
  // & unnecessarily complicated to use!
  // Use pointer/arrow notation -> instead ...
  std::cout << "Using subscript/indexing operator [] to retrieve menu items ..." << std::endl;
  xMenuItem* itemBySubscr = (*contextMenu)[3]; // access by index
  // do something with retrieved menu item ...
  if (itemBySubscr) { std::cout << *itemBySubscr << std::endl; }
  
  /*%*%*%*%*%*%*%*%*/

  // 11.6.2 removing menus & menu items ...
  
  // The below demonstrates how to remove menus & menu items ...
  // commented out but kept for demonstration purposes ...

  // removing entire menus from a frame

  // childFrame.removeMenu();
  // siblingFrame.removeContextMenu();

  // removing a menu by pointer value ...
  // radioItemMenu->remove(radioItemFourth);

  // removes submenus associated to the menu item ...
  // contextMenu->remove(contextMenuItemFourth);

  // remove a menu item by index ...
  // radioItemMenu->removeByIndex(0);

  // remove a menu item by ID ...
  // if ID not found, item not removed (obviously!)
  // altRadioItemMenu->removeByIndex(5);

  // remove a menu item by string text ...
  // altRadioItemMenu->removeByText("Alt Radio Item 1");

  /*%*%*%*%*%*%*%*%*/

  // 12. `iKeyEventListener` interface ...

  // define a lambda/anonymous callback on-key-pressed
  // listener function & associate/attach to a frame
  childFrame.setOnKeyPress(
    [](xWidget* pWidget, Key key) {

      // do something with pWidget ...
      IMPLICIT(pWidget);
      
      // do something with key, like ...
      // get the code of the keyboard button ...
      std::cout << key.getCode() << std::endl;
      // get the symbol of the keyboard button ...
      // Note*: upon invoking symbol(...) the actual
      // key's value is stored in the Key object, otherwize NOT
      std::cout << Key::symbol(key.getCode()) << std::endl;
      // stream the key object's data to console (DEBUG) ...
      std::cout << key << std::endl;

      // comparing keys using enum constants ...
      if (key.getCode() == Key::ARROW::UP) {
        // if key comparison true, then do something ...
        std::cout << "Key::ARROW::UP" << std::endl;
      }

      // Note*: Other enums include `ARROW`, `EXTRA`, `NUMPAD`, `F`
      // another key comparison to enum constant!
      if (key.getCode() == Key::EXTRA::ENTER) {
        // do something for true comparison ...
        std::cout << "Key::EXTRA::ENTER" << std::endl;
      }
    }
  );

  // define a lambda/anonymous callback on-key-release
  // listener function & associate/attach to a frame
  childFrame.setOnKeyRelease(
    [](xWidget* pWidget, Key key) {
      // do something with pWidget ...
      IMPLICIT(pWidget);
      // do something with key, like stream to console (DEBUG) ...
      // std::cout << key << std::endl; // commented out for demo ...
      // get the code of the keyboard button ...
      std::cout << key.getCode() << std::endl;
      // get the symbol of the key that was pressed ...
      std::cout << key.getSymbol() << std::endl;
    }
  );

  // To Log the Menu & Menu Items data, do the following ...
  // xMenuManager::LogMenuData();

  /*%*%*%*%*%*%*%*%*/

  // 13. Themes ...

  // set the global theme before or after creating a widget
  // & also change for individual components/controls ...
  
  // set the global theme for the app ...
  // xTheme::SetGlobalTheme(xTheme::Theme::CLASSIC);

  // classic themes ...
  // parentFrame.SetWidgetTheme(xTheme::Theme::CLASSIC);
  // parentButton.SetWidgetTheme(xWidget::Theme::CLASSIC);

  // childFrame.SetWidgetTheme(xWidget::Theme::CLASSIC);
  // parentFrame.SetWidgetTheme(xWidget::Theme::MODERN);

  // xTheme::SetGlobalTheme(xTheme::Theme::CLASSIC);
  // siblingFrame.SetWidgetThemeRecursive(xTheme::Theme::CLASSIC);
  // parentFrame.SetWidgetThemeRecursive(xTheme::Theme::CLASSIC);

  // parentFrame.setMargins(50,50,50,50);
  // parentFrame.setMargins(50,50,50,50);

  // work in progress ...

  // opacity between 0-255
  parentFrame.setOpacity(200);
  childFrame.setOpacity(200);
  siblingFrame.setOpacity(200);

  /*%*%*%*%*%*%*%*%*/

  // 14. `xLabel`

  /// @todo default width & height to assume the text font & string "length"

  // Create a label component/control ...
  xLabel label(
    &parentFrame,
    "This is a label ... ",
    150, 500, 150, 50
  );

  // set a font for the text in the label control ...
  label.setFont("Arial", 20, true, true, true, true);

  // set the background & text color
  // for the `xLabel` object ...
  
  // default BG color => parent's BG color

  label.setBackgroundColor(0,200,255); // kept for demo purposes ...
  label.setTextColor(255,0,0); // RED

  // associate a border to the label ...
  // default border for label is empty ...
  label.setWidgetBorder(
    xBorderFactory::createLineBorder(
      255, 255, 0, // RGB (BLACK)
      1,       // line/stroke width
      5,       // radius
      xBorder::StrokeStyle::DOT, // style
      0        // inset => distance from button edge
    )
  );

  label.setOnMouseHover(
    [](xWidget* pWidget, MouseEvent event) -> void {
      // do something with pWidget ...
      std::cout << "pWidget->getHoverState() => " << pWidget->getHoverState() << std::endl;
      // print the color code for the object's hover state ...
      std::cout << "R: " << (int)GetRValue(pWidget->getHoverColor()) << std::endl;
      std::cout << "G: " << (int)GetGValue(pWidget->getHoverColor()) << std::endl;
      std::cout << "B: " << (int)GetBValue(pWidget->getHoverColor()) << std::endl;
      if (event == MouseEvent::ENTER) {
        std::cout << "label on-mouse-hover (enter) callback demo" << std::endl;
      } else if (event == MouseEvent::EXIT) {
        std::cout << "label on-mouse-hover (exit) callback demo" << std::endl;
      }
    }
  );

  label.setOnMouseClick(
    [](xWidget* pWidget, MouseKey key) -> void {
      IMPLICIT(pWidget);
      std::cout << "label on-mouse-click callback demo" << std::endl;
      if (key == MouseKey::LeftButton) {
        std::cout << "MouseKey::LeftButton" << std::endl;
      } else if (key == MouseKey::RightButton) {
        std::cout << "MouseKey::RightButton" << std::endl;
      }
    }
  );

  label.setOnClick(
    [](xWidget* pWidget) -> void {
      IMPLICIT(pWidget);
      std::cout << "label on-click callback demo" << std::endl;
    }
  );

  label.setOnDoubleClick(
    [](xWidget* pWidget) -> void {
      IMPLICIT(pWidget);
      std::cout << "label on-double-click callback demo" << std::endl;
    }
  );

  // static controls/labels don't receive focus ...
  // label.setOnFocus(
  //   [](xWidget* pWidget, FocusEvent event) -> void {
  //     IMPLICIT(pWidget);
  //     std::cout << "label on-focus-event callback demo" << std::endl;
  //     if (event == FocusEvent::GAINED) {
  //       std::cout << "FocusEvent::GAINED" << std::endl;
  //     } else if (event == FocusEvent::LOST) {
  //       std::cout << "FocusEvent::LOST" << std::endl;
  //     }
  //   }
  // );

  // ...

  // add the label to a frame ...
  parentFrame.add(&label);

  /*%*%*%*%*%*%*%*%*/

  // 15. `xTextBox`

  // create a textbox control ...
  xTextBox textbox(
    &parentFrame,
    "This is a textbox ... ",
    parentFrame.getWidth()-350, 500, 150, 50 // reverse order ...
  );

  // set the font for the text in the textbox control ...
  textbox.setFont("Arial", 18, false, false, false, false);

  /// @todo re-design the `xTextBox` class based on
  /// `WC_STATIC` to handle ALL the custom drawing

  // set the background & text color for the `xTextBox` object ...
  // currently customizing textboxes (other than font) is not supported ...
  // This will be implemented after xDropDown
  // & perhaps after xTreeView controls ...
  // textbox.setBackgroundColor(RGB(255,0,0));
  // textbox.setTextColor(RGB(0,255,255));

  // textbox events ...

  // associate text-change event callback/action to textbox widget ...
  textbox.setOnTextChange(
    [](xTextBox* pTextbox, std::string text) -> void {
      // do something with text ...
      std::cout << "text: " << text << std::endl;
      // do something with pTextbox
      std::cout << "pTextbox->getText() => " << pTextbox->getText() << std::endl;
      std::cout << "pTextbox->getDefaultText() => " << pTextbox->getDefaultText() << std::endl;
    }
  );

  textbox.setOnCharacterLimit(
    [](xTextBox* pTextbox, int limit) -> void {
      std::cout << "character limit reached!!!" << std::endl;
      // do something with pTextbox
      std::cout << pTextbox->getText() << std::endl;
      // do something with limit
      std::cout << "limit: " << limit << std::endl;
    }
  );

  textbox.setOnKeyPress(
    [](xWidget* pWidget, Key key) -> void {
      std::cout << "Key Press Listener ... " << std::endl;
      std::cout << pWidget->getText() << std::endl;
      std::cout << key << std::endl;
    }
  );

  // textbox.setOnFocus(
  //   [](xWidget* pWidget, FocusEvent event) -> void {

  //     // cast to appropriate type ...
  //     xTextBox* pTextBox = static_cast<xTextBox*>(pWidget);

  //     // get the text & default text for the textbox ...
  //     std::cout << "pTextBox->getText() => " << pTextBox->getText() << std::endl;
  //     std::cout << "pTextBox->getDefaultText() => " << pTextBox->getDefaultText() << std::endl;

  //     // do something with event, like compare `GAINED`/`LOST`

  //     // if (event == FocusEvent::GAINED) {
  //     //   std::cout << "textbox on-focus gained callback demo" << std::endl;
  //     // } else if (event == FocusEvent::LOST) {
  //     //   std::cout << "textbox on-focus lost callback demo" << std::endl;
  //     // }
  //   }
  // );

  // To prevent default text from overwriting 
  // empty textbox upon losing focus, do the following ...
  // textbox.setRestoreText(false); // defaul true ...

  // textbox.setOnMouseHover(
  //   [](xWidget* pWidget, MouseEvent event) -> void {
  //     IMPLICIT(pWidget);
  //     if (event == MouseEvent::ENTER) {
  //       std::cout << "textbox on-mouse-hover (enter) callback demo" << std::endl;
  //     } else if (event == MouseEvent::EXIT) {
  //       std::cout << "textbox on-mouse-hover (exit) callback demo" << std::endl;
  //     }
  //   }
  // );

  // textbox.setOnMouseClick(
  //   [](xWidget* pWidget, MouseKey key) -> void {
  //     IMPLICIT(pWidget);
  //     std::cout << "textbox on-mouse-click callback demo" << std::endl;
  //     if (key == MouseKey::LeftButton) {
  //       std::cout << "MouseKey::LeftButton" << std::endl;
  //     } else if (key == MouseKey::RightButton) {
  //       std::cout << "MouseKey::RightButton" << std::endl;
  //     }
  //   }
  // );

  // textbox.setOnClick(
  //   [](xWidget* pWidget) -> void {
  //     IMPLICIT(pWidget);
  //     std::cout << "textbox on-click callback demo" << std::endl;
  //   }
  // );

  // textbox.setOnDoubleClick(
  //   [](xWidget* pWidget) -> void {
  //     IMPLICIT(pWidget);
  //     std::cout << "textbox on-double-click callback demo" << std::endl;
  //   }
  // );

  // textbox.setOnKeyRelease(
  //   [](xWidget* pWidget, Key key) -> void {
  //     std::cout << "Key Release Listener ... " << std::endl;
  //     std::cout << pWidget->getText() << std::endl;
  //     std::cout << key << std::endl;
  //   }
  // );

  // `xTextBox` attributes ...

  textbox.setCharacterLimit(10);

  // set the textbox as a password field ...
  // textbox.setPasswordSymbol('@'); // before ...
  // textbox.setPasswordField(true); // default false

  // add the textbox to a frame ...
  parentFrame.add(&textbox);

  // set the textbox as a password field ...
  textbox.setPasswordSymbol('#'); // after ...
  textbox.setPasswordField(true); // default false
  // textbox.setPasswordSymbol(BULLET_CIRCLE_SMALL); // after ...

  // set the text case (`xTextBox::TextCase::UPPER`/`LOWER`/`NORMAL`)
  // textbox.setTextCase(xTextBox::TextCase::LOWER); // upper-case
  // textbox.setTextCase(xTextBox::TextCase::UPPER); // upper-case
  // textbox.setTextCase(xTextBox::TextCase::NORMAL); // upper-case

  // set the textbox as read-only ...
  // textbox.setReadOnly(true); // default false

  // set the textbox as numbers-only ...
  // textbox.setNumbersOnly(true); // default false

  // get the password symbol ...
  std::cout << "multiLineTextBox.getPasswordSymbol() => " << textbox.getPasswordSymbol() << std::endl;

  // NOT working as expected ...
  // textbox.setOnCharacterInput(
  //   [](xTextBox* pTextbox, Key key) -> void {
  //     std::cout << "character input!!!" << std::endl;
  //     // do something with pTextbox
  //     std::cout << pTextbox->getText() << std::endl;
  //     // do something with limit
  //     std::cout << "character: " << key << std::endl;
  //   }
  // );

  /*%*%*%*%*%*%*%*%*/

  // 16. `xDrowpDown`

  // Example 1 ...

  // create a dropdown menu / combobox
  xDropDown dropdown(
    &parentFrame,
    150, 700
  );

  // add the menu to the combobox ...
  parentFrame.add(&dropdown);

  // adjust the dimensions (height/width) of the combobox ...
  dropdown.setHeight(50);
  dropdown.setWidth(200);

  // associate a selection change event ...
  dropdown.setOnSelectionChange(
    [](xControl* pControl, int index) -> void {
      xDropDown* pDropdown = static_cast<xDropDown*>(pControl);
      // do something with pDropdown ...
      IMPLICIT(pDropdown);
      std::cout << "dropdown demo on-selection-change callback demo ..." << std::endl;
      std::cout << "selected item index: " << index << std::endl;
    }
  );

  // add items before or after create ...
  dropdown.add("Test item 1"); // index 0
  dropdown.add("Test item 2", 0); // index 0 // item 1 now index 1
  dropdown.add("Test item 3"); // index 2
  dropdown.add("Test item 4", 2); // index 2 // item 3 now index 3

  // index out of bounds, selects index 0
  // dropdown.select(10);

  // for testing purposes ...
  // dropdown.add(
  //   std::vector<std::string> {
  //     "Item after 1",
  //     "Item after 2",
  //     "Item after 3",
  //     "Item after 4",
  //   }, 3 // "Test item 3" now selected ...
  // );

  // select a dropdown/combobox menu item programatically ...
  dropdown.select(3);

  // set the font of the items' text in the combobox ...
  dropdown.setFont("Arial", 20, false, false, false, false);

  // inspect the number of items in the dropdown/combobox
  // std::cout << "dropdown.count() => " << dropdown.count() << std::endl;

  #ifndef NDEBUG
  // print the data to the console ...
  dropdown.EnumerateItems();
  #endif

  /*%*%*%*%*%*%*%*%*/

  // Example 2 ...

  // create a dropdown menu / combobox
  xDropDown altDropdown(
    &parentFrame, parentFrame.getWidth()-350, 700,
    std::vector<std::string> {
      "Alt Item 1", // index 0
      "Alt Item 2", // index 1
      "Alt Item 3", // index 2
      "Alt Item 4", // index 3
    }, 2 // selected item index
  );

  // adjust the font of the text for the items in the dropdown/combobox
  altDropdown.setFont("Calibri", 13, true, false, false, false);

  // associate an on-focus listener callback/action to the dropdown
  altDropdown.setOnFocus(
    [](xWidget* pWidget, FocusEvent event) -> void {
      IMPLICIT(pWidget);
      if (event == FocusEvent::GAINED) {        
        std::cout << "alt drop-down menu focus gained" << std::endl;
      } else if (event == FocusEvent::LOST) {
        std::cout << "alt drop-down menu focus lost" << std::endl;
      }
    }
  );

  // associate an on-selection-change listener callback/action to the dropdown
  altDropdown.setOnSelectionChange(
    [](xControl* pControl, int index) -> void {
      // first convert to appropriate type ...
      xDropDown* pDropdown = static_cast<xDropDown*>(pControl);
      // do something with `pDropdown` ...
      IMPLICIT(pDropdown);
      std::cout << "on-selection-change client demo callback ..." << std::endl;
      std::cout << "selected item index: " << index << std::endl;
    }
  );

  // append/attache/add the dropdown to a frame ...
  parentFrame.add(&altDropdown);

  // remove menu items by string value or by index ...
  // altDropdown.select("Alt Item 4"); // index 3
  // // altDropdown.remove(3); // "Alt item 4" => select "Alt Item 1"
  // altDropdown.remove("Alt Item 4"); // remove by string value ...

  // add items at specified indices / for testing purposes ...
  // altDropdown.add("Alt Item 5", 0);
  // altDropdown.add("Alt Item 6", altDropdown.count());

  // // NOTE*: changing/setting background color is not YET supported ..
  // altDropdown.setBackgroundColor(RGB(255, 255, 0));

  // 17. `xMultiLineTextBox`

  // re-use `xTextBox` class => inherit ...

  // default character limit for multi-line textbox is 1000

  xMultiLineTextBox multiLineTextBox(
    &parentFrame,
    "This is a paragraph ... ",
    500, 50, 400, 300
  );

  // add the multi-line textbox to the parent frame ...
  parentFrame.add(&multiLineTextBox);

  // associate a character-limit event callback ...
  multiLineTextBox.setOnCharacterLimit(
    [](xTextBox* pTextbox, int limit) -> void {
      std::cout << "character limit reached!!!" << std::endl;
      // do something with pTextbox
      std::cout << pTextbox->getText() << std::endl;
      // do something with limit
      std::cout << "limit: " << limit << std::endl;
    }
  );

  // associate a ckey-press event callback ...
  multiLineTextBox.setOnKeyPress(
    [](xWidget* pWidget, Key key) -> void {
      std::cout << key << std::endl;
      std::cout << pWidget->getText() << std::endl;
    }
  );

  /*%*%*%*%*%*%*%*%*/

  // 18. `xListBox` ... similar to dropdown menu ...
  
  // create a listbox control ...
  xListBox listbox(&parentFrame, 500, 500);

  // listbox.setWidth(85);
  // listbox.setHeight(30);

  // create a list of items to add to the listbox ...
  std::vector<std::string> listboxItems = {
    "listbox item 1",
    "listbox item 2",
    "listbox item 3",
    "listbox item 4",
  };

  // add the list of items to the control ...
  listbox.add(listboxItems);

  // add the control to a parnet widget ...
  parentFrame.add(&listbox);

  // adjust the font of a listbox control ...
  listbox.setFont("Calibri", 18, true, true, false, false);

  // adjust the dimensions (width/height) of a listbox
  listbox.setWidth(85);
  listbox.setHeight(30);

  // (capture) variable to keep track of number of listbox item clicks ...
  int clickCount = 0;

  // define a selection-change event listener to the listbox ...
  listbox.setOnSelectionChange(
    [&clickCount](xControl* pControl, int index) -> void {
      
      std::cout << "listbox on-selection-change event callback/action" << std::endl;

      // convert/cast `pControl` to an appropriate type ...
      xListBox* pListbox = static_cast<xListBox*>(pControl);

      // do something with `pListbox` ...
      std::cout << "pListbox->count() => " << pListbox->count() << std::endl;
      std::cout << "index: " << index << std::endl;

      clickCount++;
      std::cout << "clickCount: " << clickCount << std::endl;
    }
  );

  // set the listbox as unselectable ... 
  listbox.setUnselectable(true); // default false ...

  // 19. `xMultiSelectListBox`

  // create a multi-select listbox control ...
  xMultiSelectListBox multiselectlistbox(
    &parentFrame, // parent pointer
    770, 500,     // dimensions
    std::vector<std::string> {
      "multiselect item 1",
      "multiselect item 2",
      "multiselect item 3",
      "multiselect item 4",
    }, // items to insert into listbox ...
    std::vector<int> {1, 3} // select all, use -1 as first arg ...
  );

  // come back to this (soon) ... 
  // multiselectlistbox.setUnselectable(true); // default false ...

  // create a multi-select listbox control ...
  // xMultiSelectListBox multiselectlistbox(
  //   &parentFrame, // parent pointer
  //   770, 500,     // dimensions
  //   std::vector<std::string> {
  //     "multiselect item 1",
  //     "multiselect item 2",
  //     "multiselect item 3",
  //     "multiselect item 4",
  //   }, // items to insert into listbox ...
  //   std::vector<int> { -1 } // (leave out / or blank) => deselect all ... 
  // );

  // add the control to a parnet widget ...
  parentFrame.add(&multiselectlistbox);

  // adjust the font of a multiselectlistbox control ...
  multiselectlistbox.setFont(
    "Arial", 15, false, true, true, false
  );

  // adjust the dimensions (width/height) of a listbox
  multiselectlistbox.setWidth(100);
  multiselectlistbox.setHeight(200);

  // deselect ALL items ...
  // multiselectlistbox.deselect(-1);

  // capture variable for multi-select listbox selection-change callback action ...
  int countClick = 0;

  // define a selection-change event listener to the multiselectlistbox ...
  multiselectlistbox.setOnSelectionChange(
    [&countClick](xControl* pControl, int index) -> void {

      IMPLICIT(index);

      std::cout << "multi-select listbox on-selection-change event callback/action" << std::endl;

      // convert/cast `pControl` to an appropriate type ...
      xMultiSelectListBox* pMultiSelectListbox = static_cast<xMultiSelectListBox*>(pControl);

      countClick++;
      std::cout << "countClick => " << countClick << std::endl;

      pMultiSelectListbox->Enumerate();
      pMultiSelectListbox->EnumerateSelectedIndices();

      // std::vector<std::string> items = pMultiSelectListbox->getSelectedItems();
      std::cout << "pMultiSelectListbox->getSelectedCount() => " << pMultiSelectListbox->getSelectedCount() << std::endl;
      // std::cout << "pMultiSelectListbox->getSelectedIndex() => " << pMultiSelectListbox->getSelectedIndex() << " (NOT REALLY USEFUL IN THIS CONTEXT!) " << std::endl;
    }
  );

  /*%*%*%*%*%*%*%*%*/

  // 20. `xDialog` ...

  /// @see `iEvent`s
  /// The code for this is demonstrated
  /// in menu-item callback/actions click events ...

  /*%*%*%*%*%*%*%*%*/

  // 21. `xRichTextBox` ...

  // 22. `xImageBox` ...

  // 23. `xTreeView` ...

  // 24. `xStuff` ...

  // 25. other x-controls ...

  /*%*%*%*%*%*%*%*%*/

  // main message loop (DON'T REMOVE!) ...
  xApp::get().messagepump();
  return EXIT_SUCCESS;
}

// implementation of client demo event handlers ...

// definitions of client demo click event handler
void ClickClientDemoHandler(xWidget* pWidget) {
  // do something with pWidget ...
  pWidget->setText("click");
}

// definition of client demo double click event handler
void DoubleClickClientDemoHandler(xWidget* pWidget) {
  // do something with pWidget, like ...
  // change the background color of the widget/control
  pWidget->setBackgroundColor(RGB(0,255,255));
}

// definition of client demo mouse double click event handler
void MouseDoubleClickClientDemoHandler(xWidget* pWidget, MouseKey key) {
  // do something with pWidget ...
  IMPLICIT(pWidget);
  // do something with key ...
  // like check/compare
  // MouseKey::LeftButton
  IMPLICIT(key);
  // do something else ...
}

// definition of client demo focus event handler
void FocusClientDemoHandler(xWidget* pWidget, FocusEvent event) {
  // do something with pWidget ...
  IMPLICIT(pWidget);
  // check whether focus gained or lost ...
  if (event == FocusEvent::GAINED) {
    // do something ...
  } else if (event == FocusEvent::LOST) {
    // do something else ...
  }
}

// definition of client demo hover event handler
void MouseHoverClientDemoHandler(xWidget* pWidget, MouseEvent event) {
  // do something with pWidget
  IMPLICIT(pWidget);
  // do something with event,
  // like check/compare
  // event == MouseEvent::ENTER
  IMPLICIT(event);
  // do something else ...
}

// definition of client demo mouse click animation handler
void MouseClickAnimationHandler(xWidget* pWidget) {

  xImageButton* pImgBtn = static_cast<xImageButton*>(pWidget);
  // int duration = pImgBtn->getDuration();

  // if clicked while in animation, stop/pause animation ...
  if (pImgBtn->isAnimating()) {
    pImgBtn->stop();
    return;
  }

  // if no arguments => use stored animation duration ...
  // i.e. `setDuration(int duration)`
  pImgBtn->animate();
}

// definition of client demo close event hanlder
void CloseClientDemoHandler(xWidget* pWidget) {
  IMPLICIT(pWidget);
  // do stuff before actually invoking close() on the frame ...
  std::cout << "CloseClientDemoHandler" << std::endl;
}

// definition of client demo menu item click close event hanlder
void MenuItemClickCloseEventClientDemoHandler(xMenuItem* pItem, xWidget* pWidget) {

  // do something with pItem ...
  std::cout << "pItem->getText() => " << pItem->getText() << std::endl;
  // do something with pWidget ...
  std::cout << "pWidget->getText() => " << pWidget->getText() << std::endl;

  // close frame on exit ...
  static_cast<xFrame*>(pWidget)->close(); // cast to appropriate type ...
  
}

// end of main.cpp
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
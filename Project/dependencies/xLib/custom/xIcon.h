/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xIcon.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xIcon` class declaration & implemenation
  * @details  xIcon.h defines the `xIcon` class
  */

#pragma once

/// @brief begin xICON_H implementation
#ifndef xICON_H
#define xICON_H

/**
 * @class    xIcon
 * @brief   `xIcon` is used to create icons for widgets
 * @details `xIcon` contains two main component interface methods    <br/>
 *          `xIcon` can be directly created from `.ico` file types   <br/>
 *          `xIcon` can be indirectly created from `.png` file types <br/>
 * @todo     resolve any discrepancies between `xImage` & `xIcon` (if any)
 */
class xIcon {

    /// @brief provide access to `xWidget` class
    /// for using private `xIcon` constructors & other members
    /// <br/>
    /// `xWidget` to interface with `xIcon` class
    /// to ensure proper resource management
    friend class xWidget; // remove this soon ...
    friend class xWindow;

private:

    /// @brief   exception class for reporting `xIcon`-related exceptions
    /// @details This class is mainly used privately to report on
    ///          whether certain icon-related issues may have occurred
    ///          specifically, if the icon file path is not
    ///          a valid/acceptable file format or if the
    ///          icon cannot be associated to the `xWidget`
    ///          for Win32-specific reasons, also reported as log
    class xIconException : public xFile::xFileException {
    public: xIconException(const std::string& dscrptn)
        : xFileException(dscrptn) {
            // ...
        }
    };

private:

    /// @brief store handle of icon
    HICON hIcon = NULL;

    /// @brief store the icon path `string`/`wstring` depending on `UNICODE`/`ANSI`
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstring mIconPath;
    #else
    std::string mIconPath;
    #endif

    /// @brief store the icon path extension for "no reason" ...
    std::string iconExt;

    /// @brief     method to convert icon path from `string` to `wstring` if `UNICODE`
    /// @param[in] path ~ the path of the icon to set/commit (.ico/.png format ONLY)
    void setIconPath(const std::string& path) {
        
        try {
            // check if icon file extension is valid ~ .ico/.png
            if (!(xFile::valid(path, "ico") || xFile::valid(path, "png"))) {

                // if not valid, throw xIconException
                std::string message = "invalid icon extension => .ico/.png ONLY";
                throw xIconException(message);

            } else { // if valid extension

                #if defined(UNICODE) && defined(_UNICODE)
                // store the icon path in wide string format
                mIconPath = StrConverter::StringToWString(path);
                #else
                mIconPath = path; // store in narrow string format
                #endif

                // store the file extension "for no reason"
                iconExt = xFile::extension(path);
            }
            
        } catch(xIconException &ex) {
            
            LOG(ex.what());
            std::cerr << ex.what() << std::endl;
        }
    }

private:

    /// @brief delete default/parameterless constructor
    xIcon() = delete;

    // parametrized constructor
    xIcon(const std::string& path) {
        setIconPath(path);
    }

    /// @brief destructor for releasing icon resources
    ~xIcon() {
        // check if hIcon valid
        LOG("releasing xIcon resources ...");
        if (hIcon) {
            // release resources
            /// @note as per MSDN ~ do not use LR_SHARED for images
            /// that have non-standard sizes, that may change after
            /// loading, or that are loaded from a file.
            DestroyIcon(hIcon); // free hIcon
        }
    }

    /// @brief method for retrieving the handle of an icon
    HICON Handle() {
        return hIcon;
    }

public:

    /// @brief  method to retrieve the icon file path
    /// @return string representative of the icon file path 
    std::string getIconPath() {
        #if defined(UNICODE) && defined(_UNICODE)
        return StrConverter::WStringToString(mIconPath); // convert to wide string
        #else
        return mIconPath; // return as-is
        #endif
    }

private:

    // These are the defined icon sizes:
    // #define ICON_SMALL  0
    // #define ICON_BIG    1
    // #define ICON_SMALL2 2

    /// @brief default icon size
    short ICON_SIZE = ICON_BIG;

private:

    /// @brief method to set the icon of a window `xWidget` (`xButton` image or `xFrame` icon)
    /// @param hWnd ~ handle of the window that receives the icon
    /// @param size ~ size of the icon to set (default ICON_BIG)
    void setIcon(HWND hWnd, short size = ICON_BIG) {
        
        ICON_SIZE = size; // set size of the icon

        // treat file extensions differently ...
        if (iconExt == "ico") {
            // if ext is "ico" => easy handling ...
            setICO(hWnd);
        } else if (iconExt == "png") {
            // if ext is "png" => complicated handling ...
            setPNG(hWnd);
        }
    }

private:

    /// @brief method to set the icon for a widget if file type is .ico
    /// @param hWnd ~ the handle of the window/`xWidget` to receive the icon
    /// @details
    /// Handling this case is simple:     <br/>
    /// 1) just set the icon as per usual <br/>
    /// 2) SendMessage(...) to handle     <br/>
    /// @note icons are generally required to be small files,
    ///       usually resolution not exceeding `256x256` pixels
    void setICO(HWND hWnd) {

        try {
            
            /// @note
            /// DO NOT use LR_SHARED for images that have non-standard
            /// sizes, that may change after loading, or that are loaded
            /// from a file ~ as per MSDN
            hIcon = (HICON) LoadImage(
                0, mIconPath.c_str(), IMAGE_ICON, 0, 0,
                LR_DEFAULTSIZE | LR_LOADFROMFILE // LR_SHARED
            );

            // check that the handle was "loaded" correctly ...
            if (!hIcon) {
                int error = GetLastError();
                std::string message = "cannot set .ico file: " + std::to_string(error);
                throw xIconException(message);
            }

        } catch(xIconException &ex) {
            
            LOG(ex.what());
            std::cerr << ex.what() << std::endl;
        }

        // set the icon for the window ...
        SendMessage(hWnd, WM_SETICON, ICON_SIZE, (LPARAM) hIcon);
    }

    /// @brief method to set the icon for a widget if file type is .png
    /// @param hWnd ~ the handle of the window/`xWidget` to receive the icon
    /// @details
    /// Handling this case is more involved:         <br/>
    /// 1) create a bitmap image .bmp                <br/>
    /// 2) extract the image from the bitmap as .ico <br/>
    /// 3) set the icon of the widget using .ico     <br/>
    /// 4) SendMessage(...) to handle                <br/>
    /// @note icons are generally required to be small files,
    ///       usually resolution not exceeding 256x256 pixels <br/>
    ///       .png's that are to large may fail to convert to .ico
    void setPNG(HWND hWnd) {

        // initialize GDI (Graphics Display Interface)
        xGDI::get();
        
        // local variable to store wide string path
        // since Gdiplus::Bitmap constructor takes only wide string
        std::wstring path;
        #if defined(UNICODE) && defined(_UNICODE)
        path = mIconPath; // take as-is
        #else
        path = StrConverter::StringToWString(mIconPath);
        #endif

        // create bitmap image from .png file
        Gdiplus::Bitmap bmp(path.c_str());

        // handle to bitmap object
        HBITMAP hBmp;
        // get the bitmap handle from the bmp instance & store it in hBmp ...
        bmp.GetHBITMAP(0, &hBmp);
        
        // extract the bitmap as .ico image from the hBmp handle
        bmp.GetHICON(&hIcon); // hIcon class member

        try {
            
            // check if hIcon is not valid => throw exception
            if (!hIcon) {
                int error = GetLastError();
                std::string message = "cannot set .png file: " + std::to_string(error);
                throw xIconException(message);
            }

        } catch(xIconException &ex) {

            DeleteObject(hBmp); // free the hBmp bitmap handle
            LOG(ex.what()); // log the exception ...
            std::cerr << ex.what() << std::endl;
            return; // return early, i.e. don't `SendMessage(...)`
        }

        // otherwize ...
        SendMessage(hWnd, WM_SETICON, ICON_SIZE, (LPARAM) hIcon);
        DeleteObject(hBmp); // free resource ...
    }

}; // end of xIcon

#endif // end xICON_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

/**
 * @example xIcon
 * 
 * create an xFrame as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame parentFrame("Parent Frame", 1200, 800, 150, 100);
 * parentFrame.show(); // create & show
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * then provide a relative/full path to
 * a .ico or .png file for small & large icons
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * parentFrame.setSmallIcon("./src/win95.ico"); // replace with file path
 * parentFrame.setLargeIcon("./src/90cpu.png"); // replace with file path
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * create a child frame & set it's parent (if client's intention) ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame childFrame(&parentFrame, "Child Frame", 300, 200);
 * childFrame.show(); // create & show
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * set both small & large icons for the child frame as follows ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * childFrame.setIcon("./src/setup.png"); // replace with file path
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Retrieving pointer of the icon associated to a frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xIcon* liPtr = childFrame.getLargeIcon(); // retrieves a pointer to the childframe's large icon
 * xIcon* siPtr = childFrame.getLargeIcon(); // retrieves a pointer to the childframe's small icon
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * If client code wishes to access the icon's handle,
 * it can be achieved by extracting the data from its pointer ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * HICON hIcon = childFrame.getLargeIcon()->Handle(); // retrieves a large icon handle
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
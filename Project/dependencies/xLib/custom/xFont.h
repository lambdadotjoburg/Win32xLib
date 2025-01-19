/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xFont.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * @brief 		contains `xFont` & `xSystemFont` class declaration & implemenation
  * @details 	xFont.h defines the `xFont` & `xSystemFont` classes
  */

#pragma once

/// @note forward declaration of `xFont` class for use by `xSystemFont`
class xFont;

/// @brief begin of xSYSFONT_H implementation
#ifndef xSYSFONT_H
#define xSYSFONT_H

#include <set>

/**
 * @class    xSystemFont
 * @brief    For managing fonts accross `xApp`
 * @details `xSystemFont` manages `xApp` available `xFont`s & provides
 *           an interface for collecting OS fonts & setting
 *           a global font for the `xApp`
 */
class xSystemFont {

    // @brief make `xSystemFont` friend class of `xFont`
    // for interfacing with private members
    // friend class xSystemFont;

    // friend classes that can access
    // private/protected `xSystemFont` members
    // friend class xApp;
    // friend class xWidget;

private:

    /// @brief static Win32 API callback method to enumerate the system fonts
    /// @param lpelfex ~  long pointer to `ENUMLOGFONTSEX` struct
    /// @param lpntmex ~  unused   [see `macro.h`]
    /// @param FontType ~ unused   [see `macro.h`]
    /// @param lParam ~   implicit [see `macro.h`]
    /// @return 
    static int CALLBACK EnumFontFamExProc(
        ENUMLOGFONTEX* lpelfex, NEWTEXTMETRICEX* lpntmex,
        DWORD FontType, LPARAM lParam
    ) {

        // mark unused params
        UNUSED(lpntmex);
        UNUSED(lParam);
        IMPLICIT(FontType);

        // extract the font name from lpelfex
        // Take care of both `ANSI` & `UNICODE`
        #if defined(UNICODE) && defined(_UNICODE)
        std::wstringstream ss;
        ss << lpelfex->elfFullName; // extract font name
        #else
        std::stringstream ss;
        ss << lpelfex->elfFullName; // extract font name
        #endif

        // convert the string stream to string
        #if defined(UNICODE) && defined(_UNICODE)
        std::string str = StrConverter::WStringToString(ss.str());
        #else
        std::string str = ss.str();
        #endif

        // clean the extracted font name
        // & add it to set<std::string> gFontFamilies;
        // clean ~ remove font's pre-pended with '@' character
        if (!(str.find('@') < str.length())) {
            gFontFamilies.insert(str);
        }

        // important:
        // return 1 to ensure the procedure
        // does not HALT until ALL fonts visited
        return 1;
    };

    /// @brief using a set to store strings of all available fonts.
    /// The set is used because it removes any duplicate elements,
    /// & enumerating the fonts may actually give duplicate names,
    /// depending on how the fonts are enumerated.
    static std::set<std::string> gFontFamilies;

    /// @brief default constructor
    xSystemFont() = default;

public:

    /// @brief   method to populate set of available system fonts
    /// @details This method is invoked upon app startup by `xApp` manager
    ///          to ensure all the system fonts are loaded & ready for use
    void CollectSystemFonts() {
        LOGFONT lFont; // LOGFONT to fill in font details
        // c-style clear LOGFONT struct members
        memset(&lFont, 0, sizeof(LOGFONT));
        // as per MSDN, enumerate ALL fonts as follows ...
        lFont.lfFaceName[0] = '\0'; // note* use of '\0'
        lFont.lfCharSet = DEFAULT_CHARSET; // all character sets ...
        HDC hDC = GetDC(NULL); // get device context handle
        // invoke callback function ...
        EnumFontFamiliesEx(hDC, &lFont, (FONTENUMPROC) EnumFontFamExProc, (DWORD) NULL, 0);
        DeleteDC(hDC); // free device context handle
    }

    /// @brief method to destroy the resources
    ///        held by singleton class instance
    void destruct() {
        // free if instance exists ...
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }

    /// @brief delete copy constructor
    xSystemFont(const xSystemFont&) = delete;
    /// @brief delete copy assignment operator
    xSystemFont& operator=(const xSystemFont&) = delete;

    /// @brief method to retrieve the singleton class instance pointer
    static xSystemFont& get() {
        if (instance == nullptr) {
            instance = new xSystemFont;
        }
        return *instance;
    }

    #ifndef NDEBUG // only for debug mode (console output)
    /// @brief   method to enumerate all the available fonts
    /// @details This method is mainly used for debug purposes,
    ///          to enumerate all the available system fonts
    ///          installed on the platform
    void EnumerateSystemFonts() {
        // define set iterator
        std::set<std::string>::iterator i;
        // iterate the elements in the set
        for (i = gFontFamilies.begin(); i != gFontFamilies.end(); i++) {
            std::cout << *i << std::endl; // deref & console out
        }
    }
    #endif // NDEBUG

    /// @brief     method to check whether a font exists (mainly before using)
    /// @details   The `xFont` class interfaces with this method to check
    ///            whether a specifified font exists before create
    /// @param[in] font ~ the font to find in the set of system fonts
    bool FindFont(const std::string& font) {
        // "short-hand" for finding element in an STL container
        return (gFontFamilies.find(font) != gFontFamilies.end());
    }

private:

    /// @brief instance pointer to `xSystemFont` singleton
    static xSystemFont* instance;

}; // end of xSystemFont

/// @brief static declaration of `xSystemFont` members
xSystemFont* xSystemFont::instance = nullptr; // init nullptr
std::set<std::string> xSystemFont::gFontFamilies;

#endif // end of xSYSFONT_H

/// @brief begin of xFONT_H implementation
#ifndef xFONT_H
#define xFONT_H

/**
 * @class    xFont
 * @brief    For working with `xFont` objects
 * @details `xFont` provides an interface for working with `xFont` objects
 */
class xFont {

    /// @brief provide access to `xWidget` class
    /// for using private `xFont` constructors & other members
    /// <br/>
    /// `xWidget` to interface with `xFont` class to ensure
    /// proper resource management
    friend class xWidget;

    // friend-level access to `hFont` for `updateFont(...)` override
    friend class xDropDown;

    // friend-level access to `hFont` for `write(...)` override
    friend class xLabel;

    // friend-level access to `hFont` for `write(...)` override
    friend class xListBox;

    // // friend-level access to `hFont` for computing control's dimensions ...
    // friend class xLabel;
    // friend class xTextBox;

protected:

    /// @brief Win32 font handle
    HFONT hFont = (HFONT) NULL;
    /// @brief Win32 Logical Font object
    /// from which to construct other fonts
    /// using `CreateFontIndirect(...)`
    LOGFONT mLogFont;

    /// @brief variable to store previous font handle for managing resources
    HFONT hPrevFont; // This can be done in delete font method ...

    /// @brief variable to store the size/height of the font in pixels ...
    int mSize = 0;

    /// @brief (w)string variable to store the font name
    /// `UNICODE` & `ANSI/ASCII` compatible
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstring mFontName;
    #else
    std::string mFontName;
    #endif

    /// @brief     interface method for updating `LOGFONT` face name
    /// @param[in] font ~ the string value to use for setting the face name
    void setLogFontName(const std::string& font) {
        // handle unicode case
        #if defined(UNICODE) && defined(_UNICODE)
        mFontName = StrConverter::StringToWString(font);
        wcscpy(mLogFont.lfFaceName, mFontName.c_str()); // wcscpy => wide-character string copy
        #else // handle ANSI/ASCII case
        mFontName = font;
        strcpy(mLogFont.lfFaceName, mFontName.c_str()); // strcpy => string copy
        #endif
    }

    /// @brief     method for updating this class font name
    /// @details   checks whether a font exists before updating
    ///            otherwize a default font is selected
    /// @param[in] font ~ the name of the new selected font
    void setName(const std::string& font) {
        // check if font exists ...
        // only then set the font name & log font name
        setLogFontName(font);
    }

    /// @brief     method to adjust the font weight
    /// @param[in] weight ~ the weight value to be applied to the text
    void setWeight(int weight) {
        mLogFont.lfWeight = weight;
        update();
    }

    /// @brief     method to set the text font to bold
    /// @param[in] bold ~ boolean flag to indicated bold font
    void setBold(bool bold) {
        if (bold) {
            setWeight(FW_BOLD);
        } else {
            setWeight(FW_DONTCARE);
        }
    }

    /// @brief     method to set the text font to italic/slant
    /// @param[in] italic ~ boolean flag to indicated italix/slant font
    void setItalic(bool italic) {
        mLogFont.lfItalic = italic;
        update();
    }

    /// @brief method to update the size of the selected font
    /// @param size ~ the size of the font
    void setSize(int size) {
        mSize = size;
        // convert integer size in pixels to logical units ...
        HDC hDC = GetDC(NULL);
        long h = -MulDiv(size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
        mLogFont.lfHeight = h;
        DeleteDC(hDC);
        update();
    }

    /// @brief     method to adjust whether or not the text is to be underlined
    /// @param[in] underline ~ boolean flag to indicate whether to underline the text
    void setUnderline(bool underline) {
        mLogFont.lfUnderline = underline;
        update();
    }

    /// @brief     method to adjust whether or not to draw a strike-out line through the text
    /// @param[in] underline ~ boolean flag to indicate whether to strike a line through the text
    void setStrikeout(bool strikeout) {
        mLogFont.lfStrikeOut = strikeout;
        update();
    }

    /// @brief   private constructor
    /// @details
    /// `xFont` constructor to be accessible by `xWidget` <br/>
    /// `xFont` objects to be manufactured by `xWidget`   <br/>
    /// `xWidget` to manage `xFont` resources to avoid    <br/>
    /// improper use of `xFont` objects by client-code
    /// @param[in] font ~ `string` value representative of the font name to use (default "Tahoma")
    /// @param[in] size ~ `int` value representative of the font-size in pixels (default 10)
    /// @param[in] italic ~ `bool` flag whether (`true`) or not (`false`) to set the text as italic/slant (default `false`)
    /// @param[in] bold ~ `bool` flag whether (`true`) or not (`false`) to set the text as bold/`FW_BOLD` (default `false`)
    /// @param[in] underline ~ `bool` flag whether (`true`) or not (`false`) to underline the text (default `false`)
    /// @param[in] strikeout ~ `bool` flag whether (`true`) or not (`false`) to strike out the text (default `false`)
    xFont(
        const std::string& font = "Tahoma", // default font name
        int size = 10,
        bool italic = false,
        bool bold = false,
        bool underline = false,
        bool strikeout = false
    ) {
        // create font ...
        // set mLogFont properties, etc ...

        // clear the LOGFONT structure data
        memset(&mLogFont, 0, sizeof(LOGFONT));
        
        setName(font); // invokes update once

        mLogFont.lfHeight = size;
        mLogFont.lfItalic = italic;
        
        if (bold) {
            mLogFont.lfWeight = FW_BOLD;
        } else {
            mLogFont.lfWeight = FW_DONTCARE; // redundant?
        }

        mLogFont.lfUnderline = underline;
        mLogFont.lfStrikeOut = strikeout;

        update();
    }

    /// @brief method for updating the font, once any
    /// properties/attributes have been changed as-well-as
    /// cleaning up any resources after updating the font handle
    void update() {
        
        // temporarily store font in `hPrevFont` for cleanup ...
        hPrevFont = hFont;

        // create a new font from the mLogFont member & update font handle
        hFont = CreateFontIndirect(&mLogFont);

        // check if hPrevFont exists/NOT NULL
        if (hPrevFont) {
            // deselect the font from any device context before delete
            HDC hDC = GetDC(NULL);
            if (hDC) {
                // select a stock object
                SelectObject(hDC, GetStockObject(SYSTEM_FONT));
                ReleaseDC(NULL, hDC);
            }
            DeleteObject(hPrevFont);
        }
    }

    /// @brief   destructor
    /// @details It is essential to delete the
    /// resources held by the xFont class,
    /// specifically `hFont`
    ~xFont() {
        LOG("releasing xFont resources ...");
        // check if hFont valid
        if (hFont) {
            // free object
            DeleteObject(hFont); // free hFont
        };
    }

    #if defined(UNICODE) && defined(_UNICODE)
    static int calculateTextWidth(HWND hWnd, const std::wstring& text)
    #else
    static int calculateTextWidth(HWND hWnd, const std::string& text)
    #endif
    {

        HDC hDC = GetDC(hWnd);

        HFONT hOldFont = (HFONT) SelectObject(
            hDC, (HFONT) SendMessage(hWnd, WM_GETFONT, 0, 0)
        );

        SIZE size;
        GetTextExtentPoint32(hDC, text.c_str(), text.length(), &size);

        // restore the old font ...
        SelectObject(hDC, hOldFont);
        ReleaseDC(hWnd, hDC);
        
        return size.cx;
    }

    #if defined(UNICODE) && defined(_UNICODE)
    static int calculateTextHeight(HWND hWnd, const std::wstring& text)
    #else
    static int calculateTextHeight(HWND hWnd, const std::string& text)
    #endif
    {

        HDC hDC = GetDC(hWnd);

        HFONT hOldFont = (HFONT) SelectObject(
            hDC, (HFONT) SendMessage(hWnd, WM_GETFONT, 0, 0)
        );

        SIZE size;
        GetTextExtentPoint32(hDC, text.c_str(), text.length(), &size);

        // restore the old font ...
        SelectObject(hDC, hOldFont);
        ReleaseDC(hWnd, hDC);
        
        return size.cy;
    }

}; // end of xFont

/// @todo define a shared/global font mechanism

#endif // end of xFONT_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

/// @todo include demonstration/example graphics/images for each of the below ...

/**
 * @example xFont
 * 
 * create an `xFrame` as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xFrame frame("Demo Frame", 1200, 800);
 * frame.center();
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * create an `xButton` as per usual & attach/associate it to a frame ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xButton button(&frame, "OK", 50, 25, 100, 100); // provide params
 * frame.add(&button); // create & add to frame ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Set the font that you wish the `xWidget` to have ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * button.setFont("Arial", 10, true, true, true, true); // provide params
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * client-code can set font before or after `frame.add(...)` <br/>
 * 
 * To see a list of all available fonts, enumerate the available system fonts ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xSystemFonts::get().EnumerateSystemFonts(); // output in console/debug only!
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
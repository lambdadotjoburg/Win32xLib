/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	   Key.h
  * @author    &lambda;ambda
  * @date      \showdate "%Y-%m-%d"
  * @brief 	   `Key` declaration & implementation header file
  * @details   Key.h defineds the `Key` class to be used for
  *            interfacing with key-events
  */

/// @brief begin of KEY_H implementation
#ifndef KEY_H
#define KEY_H

#pragma once

// include unordered map to associate
// UINT keyboard button values
// to human-readable strings
#include <unordered_map>

/**
 * @class    Key
 * @brief   `Key` class interfacing with widgets & iKeyEventListener
 *           to allow client-code to translate & extract key-info
 */
class Key {

public:

    /// @brief public enum for `ARROW` keys
    enum ARROW {
        UP = VK_UP,
        DOWN = VK_DOWN,
        LEFT = VK_LEFT,
        RIGHT = VK_RIGHT,
    };

    /// @brief public enum for special/`EXTRA` keys
    enum EXTRA {
        TAB = VK_TAB,
        ALT = VK_MENU,
        CTRL = VK_CONTROL,
        CAPS = VK_CAPITAL,
        SHIFT = VK_SHIFT,
        SPACE = VK_SPACE,
        ENTER = VK_RETURN,
        ESC = VK_ESCAPE,
        BACK_SPACE = VK_BACK,
        NUM_LOCK = VK_NUMLOCK,
        PAUSE_BREAK = VK_PAUSE,
        SCROLL_LOCK = VK_SCROLL,
        PRINT_SCREEN = VK_PRINT,
        APPS = VK_APPS,
        WIN_LEFT = VK_LWIN,
        WIN_RIGHT = VK_RWIN,
    };

    /// @brief enum class for function `F` keys
    enum F {
        F1 = VK_F1,   F2 = VK_F2,   F3 = VK_F3,
        F4 = VK_F4,   F5 = VK_F5,   F6 = VK_F6,
        F7 = VK_F7,   F8 = VK_F8,   F9 = VK_F9,
        F10 = VK_F10, F11 = VK_F11, F12 = VK_F12,
        F13 = VK_F13, F14 = VK_F14, F15 = VK_F15,
        F16 = VK_F16, F17 = VK_F17, F18 = VK_F18,
        F19 = VK_F19, F20 = VK_F20, F21 = VK_F21,
        F22 = VK_F22, F23 = VK_F23, F24 = VK_F24,
    };

    /// @brief public enum constants for `NUMPAD` keys
    enum NUMPAD {
        NUMPAD0 = VK_NUMPAD0,
        NUMPAD1 = VK_NUMPAD1,
        NUMPAD2 = VK_NUMPAD2,
        NUMPAD3 = VK_NUMPAD3,
        NUMPAD4 = VK_NUMPAD4,
        NUMPAD5 = VK_NUMPAD5,
        NUMPAD6 = VK_NUMPAD6,
        NUMPAD7 = VK_NUMPAD7,
        NUMPAD8 = VK_NUMPAD8,
        NUMPAD9 = VK_NUMPAD9,

        NUMPAD_ADD = VK_ADD,
        NUMPAD_SUBTRACT = VK_SUBTRACT,
        NUMPAD_MULTIPLY = VK_MULTIPLY,
        NUMPAD_DIVIDE = VK_DIVIDE,        
        NUMPAD_DECIMAL = VK_DECIMAL,
        NUMPAD_ENTER = VK_SEPARATOR, // separator
    };

    /// @brief variable to store the virtual
    ///        key code of the keyboard button
    UINT _c; // code

    /// @brief variable to store the human-readable
    ///        string associated to the keyboard button
    std::string _s; // symbol

    // For the sake of keeping cleint-code simple,
    // this will not be implemented ... <br/>
    // variables for keeping track whether
    // shift-down state, alt-down state or
    // ctrl-down state upon key press ...
    // boolean _shift = false;
    // boolean _ctrl = false;
    // boolean _alt = false;

public:

    /// @todo
    /// Investigate whether to translate the key code at 
    /// Key object instantiation or only if requested
    /// by user/client-code for faster processing,
    /// since no translation done at this stage?

    /// @brief method for retrieving the key code `UINT` value
    UINT getCode() const {
        return _c;
    }

    /// @brief method for getting the human-readable
    ///        string associated with the key
    std::string getSymbol() {
        // if the string is empty, first find its translation ...
        // if (_s.empty()) { _s = symbol(this->_c); }
        return _s;
    }
    
    /// @brief public Key constructor
    /// @param c ~ `UINT` key code extracted from wParam's `WM_KEYDOWN`/`WM_KEYUP`
    Key(UINT c) : _c{c}, _s{symbol(c)} { /* ... */ }

    

    /// @brief   public overloaded constructor taking
    ///          as arg a const reference to a string
    /// @details
    /// useful if client code uses a character sequence
    /// representative of a virtual key, to extract the code from it ...
    Key(const std::string& s) : _c{code(s)}, _s{s} { /* ... */ }

    /// @brief     overload (wide string) method to extract
    ///            the virtual key code from a string/character
    /// @param[in] s ~ (wide) string representative of the key's character ...
    static UINT code(const std::wstring& s) {
        return code(StrConverter::WStringToString(s));
    }

    /// @brief     (narrow string) method to extract
    ///            the virtual key code from a string/character
    /// @param[in] s ~ (narrow) string representative of the ke
    static UINT code(const std::string& s) {

        // if s is empty, then no character to translate/extract ...
        if (s.empty()) {
            return 0; // return 0 => indicates no virtual key code
        }

        // first do a reverse lookup in the map ...
        std::unordered_map<UINT, std::string>::iterator i;
        for (i = special_keys.begin(); i != special_keys.end(); i++) {
            // if found by comparison ...
            if (i->second == s) {
                // then return the map item's key
                return i->first;
            }
        }
    
        // otherwize translate the key as per usual...
        SHORT result = VkKeyScan(s[0]);

        // if the key translation was successful ...
        if (result != -1) {
            // mask for _shift, _alt, _ctrl
            // then return the result masked for the `UINT` "code" value
            return result & 0xFF;
        }

        // otherwize return 0;
        return 0;
    }


    /// @brief     method to translate a virtual key code 
    ///            to human-readable string/character format
    /// @param[in] c ~ `UINT` virtual key code to translate
    /// @details   convert virtual key code to `char` or `wchar_t`
    ///            depending on `ASCII`/`UNICODE build, respectively!
    static std::string symbol(UINT c) {

        /// @todo 
        // HOW to handle F10 & print-screen keys ... ?
        // if (c == VK_F10)   { return "F10"; }
        // if (c == VK_PRINT) { return "PRINT_SCREEN"; }

        // first look for the key code in the map, since ...
        // it may be an `EXTRA`,`F` or `ARROW` key!
        std::unordered_map<UINT, std::string>::iterator i;
        i = special_keys.find(c); // if found, return value ...
        if (i != special_keys.end()) { return i->second; }

        // BYTE buffer ...
        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);

        UINT scanCode = MapVirtualKey(c, MAPVK_VK_TO_VSC);

        // handle unicode/ascii differently ...
        #if defined(UNICODE) && defined(_UNICODE)
        wchar_t buffer[5] = {0};
        int result = ToUnicode(c, scanCode, keyboardState, buffer, 4, 0);
        #else
        char buffer[5] = {0};
        int result = ToAscii(c, scanCode, keyboardState, (LPWORD)buffer, 4);
        #endif

        // if the key translation result positive ...
        if (result > 0) {
            // handle unicode/ascii differently ...
            #if defined(UNICODE) && defined(_UNICODE)
            return StrConverter::WStringToString(std::wstring(buffer, result));
            #else
            return std::string(1, buffer[0]);
            #endif
            // return the human-readable string/character value ...
        }
        
        // otherwize, key could not be determined ...
        return "UNDEFINED";
    }
    
    /// @brief friend ostream operator overload method to print Key instance data ...
    friend std::ostream& operator << (std::ostream& os, const Key& key) {
        os << "key code: " << key._c << " => " << "key symbol: " << key._s;
        return os;
    }
    // #endif

    /// @brief unordered map/container for storing the `UINT`-`string` translations!
    static std::unordered_map<UINT, std::string> special_keys;
};

/// initialization of the `UINT`-`string` translation map
std::unordered_map<UINT, std::string> Key::special_keys = {

    {VK_UP, "UP"},
    {VK_DOWN, "DOWN"},
    {VK_LEFT, "LEFT"},
    {VK_RIGHT, "RIGHT"},
    {VK_MENU, "ALT"},
    {VK_CONTROL, "CTRL"}, 
    {VK_CAPITAL, "CAPS"},
    {VK_SHIFT, "SHIFT"},
    {VK_BACK, "BACK_SPACE"},
    {VK_NUMLOCK, "NUM_LOCK"},
    {VK_PAUSE, "PAUSE_BREAK"},
    {VK_SCROLL, "SCROLL_LOCK"},
    {VK_APPS, "APPS"},
    {VK_LWIN, "WIN_LEFT"},
    {VK_RWIN, "WIN_RIGHT"},
    {VK_INSERT, "INSERT"},
    {VK_HOME, "HOME"},
    {VK_PRIOR, "PAGE_UP"},
    {VK_NEXT, "PAGE_DOWN"},
    {VK_END, "END"},
    {VK_DELETE, "DELETE"},

    {VK_ESCAPE, "ESC"},
    {VK_RETURN, "ENTER"},
    {VK_TAB, "TAB"},
    {VK_PRINT, "PRINT_SCREEN"}, // PRINT_SCREEN = VK_PRINT
    {VK_SPACE, "SPACE"},

    {VK_F1,  "F1" }, {VK_F2,  "F2" }, {VK_F3,  "F3" },
    {VK_F4,  "F4" }, {VK_F5,  "F5" }, {VK_F6,  "F6" },
    {VK_F7,  "F7" }, {VK_F8,  "F8" }, {VK_F9,  "F9" },
    {VK_F10, "F10"}, {VK_F11, "F11"}, {VK_F12, "F12"},
    {VK_F13, "F13"}, {VK_F14, "F14"}, {VK_F15, "F15"},
    {VK_F16, "F16"}, {VK_F17, "F17"}, {VK_F18, "F18"},
    {VK_F19, "F19"}, {VK_F20, "F20"}, {VK_F21, "F21"},
    {VK_F22, "F22"}, {VK_F23, "F23"}, {VK_F24, "F24"},

    {VK_SEPARATOR, "NUMPAD_ENTER"},
    {VK_DECIMAL, "NUMPAD_DECIMAL"},
    {VK_SUBTRACT, "NUMPAD_SUBTRACT"},
    {VK_MULTIPLY, "NUMPAD_MULTIPLY"},
    {VK_DIVIDE, "NUMPAD_DIVIDE"},
    {VK_ADD, "NUMPAD_ADD"},
    
    {VK_NUMPAD0, "NUMPAD0"}, {VK_NUMPAD1, "NUMPAD1"}, {VK_NUMPAD2, "NUMPAD2"},
    {VK_NUMPAD3, "NUMPAD3"}, {VK_NUMPAD4, "NUMPAD4"}, {VK_NUMPAD5, "NUMPAD5"},
    {VK_NUMPAD6, "NUMPAD6"}, {VK_NUMPAD7, "NUMPAD7"}, {VK_NUMPAD8, "NUMPAD8"},
    {VK_NUMPAD9, "NUMPAD9"},
};

#endif // end of KEY_H
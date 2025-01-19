/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xMsg.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This file contains the declaration
  *             & implementation for the `xMsg` singleton class
  * 
  * @details 	The class defined in this file
  *             is used to translate messages
  *             from macro hexadecimal constants
  *             to human-readable strings (debugging)
  */

#pragma once

// only in debug mode ...
#ifndef NDEBUG

/// @brief begin of xMSG_H implementation
#ifndef xMSG_H
#define xMSG_H

// include map for storing unique key-value pairs `<UINT, std::string>`
#include <map>

/**
 * @class    xMsg
 * @brief   `xMsg` is a singleton class that defines a message map
 * @details `xMsg` is implemented for debugging purposes to translate
 *          `msg`s from Win32 API `UINT` macro's to human readable
 *          `std::string` format
 *           <br/>
 *           The translated values are viewed as console output (debug)
 */
class xMsg {

public:

    /**
     * @brief   static method that retrieves the `xMsg` singleton instance pointer
     * @return  reference to `xMsg` singleton instance pointer
     * @details This class method returns an `xMsg` instance pointer
     *          to the client code & privately constructs
     *          a new such instance if none exists
     */
    static xMsg& get() {
        if (instance == nullptr) {
            // manufacture
            instance = new xMsg;
        }
        return *instance;
    }

    /// @brief delete copy constructor
    xMsg(const xMsg&) = delete;

    /// @brief delete copy assignment operator
    xMsg& operator=(const xMsg&) = delete;

    /// @brief method for cleanup RAII resources
    static void destruct() {
        delete instance;
        instance = nullptr;
    }

    // ...

    /// @brief      method to translate `UINT` `msg`s to `std::string`
    /// @param[in]  ID ~ `UINT` Win32 MACRO value of the message to translate
    /// @return    `std::string` human-readable message translation (debug)
    /// @details    This method looks/searches for the associated string value
    ///             by integer key in the statically allocated
    ///             `map<UINT, std::string>`
    ///             <br/>
    ///             If not found, errorTranslate returns empty "UNDEFINED"
    static std::string wmTranslate(UINT ID) {
        
        std::map<UINT, std::string>::iterator i;

        i = msgs.find(ID);
        if (i != msgs.end()) {
            return i->second;
        } // otherwize ...
        return "UNDEFINED"; // dummy return
    }

    /// @brief method to enumerate all stored messages in the map
    static void EnumerateMessages() {
        std::map<UINT, std::string>::iterator i;
        for (i = msgs.begin(); i != msgs.end(); i++) {
            std::cout << "ID: " << i->first << " => " << "MSG: " << i->second << std::endl;
        }
    }

private:

    /// @brief private default constructor
    xMsg() = default; // to create `xMsg` `instance` pointer

    /// @brief private default destructor
    ~xMsg() = default; // nothing to do ...

    /// @brief singleton instance pointer
    static xMsg* instance;

    /// @brief   map of `UINT` ID's to `msg` string translation
    /// @details This map stores all predefined macro's
    ///          & corresponding translations
    static std::map<UINT, std::string> msgs;
};

/// @brief initialize & declare static singleton instance pointer
xMsg* xMsg::instance = nullptr;

/// @brief MACRO for translating messages using
/// `TRANSLATE(msg)` syntax where `msg` of a `UINT` type
#define xTRANSLATE(m)\
std::string message = xMsg::wmTranslate(m);\
if (message != "UNDEFINED") {\
std::cerr\
<< "[" << "fn: " << __FUNCTION__ << "]"\
<< "[ID:" << m << " => " << "msg: " << message << "]"\
<< std::endl;\
};\

/// @brief   `msgs` map
/// @details  map from `UINT` to `std::string`
///           <br/>
///           This map contains all the message translations
///           that developer may need/want to "know"
///           <br/>
///           It is deemed unnecessary to store ALL message translations
///           in which case a "filter" mechanism can be defined, but
///           that is OVERKILL!
#define MAP(x) {x, #x}
std::map<UINT, std::string> xMsg::msgs = {
    MAP(WM_NULL),                   //   0
    MAP(WM_CREATE),                 //   1
    MAP(WM_DESTROY),                //   2
    MAP(WM_MOVE),                   //   3
    MAP(WM_SIZE),                   //   5
    MAP(WM_ACTIVATE),               //   6
    MAP(WM_SETFOCUS),               //   7
    MAP(WM_KILLFOCUS),              //   8
    MAP(WM_ENABLE),                 //  10
    MAP(WM_SETTEXT),                //  12
    MAP(WM_GETTEXT),                //  13
    MAP(WM_PAINT),                  //  15
    MAP(WM_CLOSE),                  //  16
    MAP(WM_QUIT),                   //  18
    MAP(WM_ERASEBKGND),             //  20
    MAP(WM_SYSCOLORCHANGE),         //  21
    MAP(WM_SHOWWINDOW),             //  24
    MAP(WM_WININICHANGE),           //  26
    MAP(WM_FONTCHANGE),             //  29
    MAP(WM_SETCURSOR),              //  32
    MAP(WM_GETMINMAXINFO),          //  36
    MAP(WM_ICONERASEBKGND),         //  38
    MAP(WM_DRAWITEM),               //  43
    MAP(WM_DELETEITEM),             //  45
    MAP(WM_SETFONT),                //  48 
    MAP(WM_GETFONT),                //  49
    MAP(WM_SETHOTKEY),              //  50
    MAP(WM_GETHOTKEY),              //  51
    MAP(WM_QUERYDRAGICON),          //  55
    MAP(WM_COMMNOTIFY),             //  68
    MAP(WM_WINDOWPOSCHANGING),      //  70
    MAP(WM_WINDOWPOSCHANGED),       //  71
    MAP(WM_POWER),                  //  72
    
    // for enabling drag/drop => include ... ?
    // MAP_MSG(WM_COPYGLOBALDATA),  //  73 

    MAP(WM_COPYDATA),               //  74
    MAP(WM_NOTIFY),                 //  78
    MAP(WM_HELP),                   //  83
    MAP(WM_USERCHANGED),            //  84
    MAP(WM_NOTIFYFORMAT),           //  85
    MAP(WM_CONTEXTMENU),            // 123
    MAP(WM_STYLECHANGING),          // 124
    MAP(WM_STYLECHANGED),           // 125
    MAP(WM_DISPLAYCHANGE),          // 126
    MAP(WM_GETICON),                // 127
    MAP(WM_SETICON),                // 128
    MAP(WM_NCCREATE),               // 129
    MAP(WM_NCDESTROY),              // 130
    MAP(WM_NCCALCSIZE),             // 131
    MAP(WM_NCHITTEST),              // 132
    MAP(WM_NCPAINT),                // 133
    MAP(WM_NCACTIVATE),             // 134
    MAP(WM_GETDLGCODE),             // 135
    MAP(WM_NCMOUSEMOVE),            // 160
    MAP(WM_NCLBUTTONDOWN),          // 161
    MAP(WM_NCLBUTTONUP),            // 162
    MAP(WM_NCLBUTTONDBLCLK),        // 163
    MAP(WM_NCRBUTTONDOWN),          // 164
    MAP(WM_NCRBUTTONUP),            // 165
    MAP(WM_NCRBUTTONDBLCLK),        // 166
    MAP(WM_NCMBUTTONDOWN),          // 167
    MAP(WM_NCMBUTTONUP),            // 168
    MAP(WM_NCMBUTTONDBLCLK),        // 169
    MAP(WM_NCXBUTTONDOWN),          // 171
    MAP(WM_NCXBUTTONUP),            // 172
    MAP(WM_NCXBUTTONDBLCLK),        // 173

    MAP(BM_GETCHECK),               // 240
    MAP(BM_SETCHECK),               // 241
    MAP(BM_GETSTATE),               // 242
    MAP(BM_SETSTATE),               // 243
    MAP(BM_SETSTYLE),               // 244
    MAP(BM_CLICK),                  // 245
    MAP(BM_GETIMAGE),               // 246
    MAP(BM_SETIMAGE),               // 247
    MAP(BM_SETDONTCLICK),           // 248

    // MAP(BM_CLICKED),             //
    // MAP(BM_LBUTTONDOWN),         //
    // MAP(BM_LBUTTONUP),           //

    MAP(WM_INPUT),                  // 255
    MAP(WM_KEYDOWN),                // 256
    MAP(WM_KEYUP),                  // 257
    MAP(WM_CHAR),                   // 258
    MAP(WM_DEADCHAR),               // 259
    MAP(WM_SYSKEYDOWN),             // 260
    MAP(WM_SYSKEYUP),               // 261
    MAP(WM_SYSCHAR),                // 262
    MAP(WM_SYSDEADCHAR),            // 263
    MAP(WM_UNICHAR),                // 265
    MAP(WM_INITDIALOG),             // 272
    MAP(WM_COMMAND),                // 273
    MAP(WM_SYSCOMMAND),             // 274
    MAP(WM_TIMER),                  // 275
    MAP(WM_HSCROLL),                // 276
    MAP(WM_VSCROLL),                // 277
    MAP(WM_INITMENU),               // 278
    MAP(WM_INITMENUPOPUP),          // 279

    MAP(WM_MENUSELECT),             // 287
    MAP(WM_MENUCHAR),               // 288
    MAP(WM_ENTERIDLE),              // 289
    MAP(WM_MENURBUTTONUP),          // 290
    MAP(WM_MENUDRAG),               // 291
    MAP(WM_MENUGETOBJECT),          // 292
    MAP(WM_UNINITMENUPOPUP),        // 293
    MAP(WM_MENUCOMMAND),            // 294
    MAP(WM_CHANGEUISTATE),          // 295
    MAP(WM_UPDATEUISTATE),          // 296
    MAP(WM_QUERYUISTATE),           // 297
    MAP(WM_CTLCOLORMSGBOX),         // 306
    MAP(WM_CTLCOLOREDIT),           // 307
    MAP(WM_CTLCOLORLISTBOX),        // 308
    MAP(WM_CTLCOLORBTN),            // 309 
    MAP(WM_CTLCOLORDLG),            // 310
    MAP(WM_CTLCOLORSCROLLBAR),      // 311
    MAP(WM_CTLCOLORSTATIC),         // 312
    MAP(WM_MOUSEMOVE),              // 512
    MAP(WM_LBUTTONDOWN),            // 513
    MAP(WM_LBUTTONUP),              // 514
    MAP(WM_LBUTTONDBLCLK),          // 515
    MAP(WM_RBUTTONDOWN),            // 516
    MAP(WM_RBUTTONUP),              // 517
    MAP(WM_RBUTTONDBLCLK),          // 518
    MAP(WM_MBUTTONDOWN),            // 519
    MAP(WM_MBUTTONUP),              // 520
    MAP(WM_MBUTTONDBLCLK),          // 521
    MAP(WM_MOUSEWHEEL),             // 522
    MAP(WM_XBUTTONDOWN),            // 523
    MAP(WM_XBUTTONUP),              // 524
    MAP(WM_XBUTTONDBLCLK),          // 525
    MAP(WM_MOUSEHWHEEL),            // 526
    MAP(WM_PARENTNOTIFY),           // 528
    MAP(WM_ENTERMENULOOP),          // 529
    MAP(WM_EXITMENULOOP),           // 530
    MAP(WM_NEXTMENU),               // 531
    MAP(WM_SIZING),                 // 532
    MAP(WM_CAPTURECHANGED),         // 533
    MAP(WM_MOVING),                 // 534
    MAP(WM_POWERBROADCAST),         // 536
    MAP(WM_DEVICECHANGE),           // 537

    // MDI deprecated (for the most part) ...

    MAP(WM_MDICREATE),              // 544
    MAP(WM_MDIDESTROY),             // 545
    MAP(WM_MDIACTIVATE),            // 546
    MAP(WM_MDIRESTORE),             // 547
    MAP(WM_MDINEXT),                // 548
    MAP(WM_MDIMAXIMIZE),            // 549
    MAP(WM_MDITILE),                // 550
    MAP(WM_MDICASCADE),             // 551
    MAP(WM_MDIICONARRANGE),         // 552
    MAP(WM_MDIGETACTIVE),           // 553
    MAP(WM_MDISETMENU),             // 560
    MAP(WM_ENTERSIZEMOVE),          // 561
    MAP(WM_EXITSIZEMOVE),           // 562
    MAP(WM_DROPFILES),              // 563
    MAP(WM_MDIREFRESHMENU),         // 564

    MAP(WM_IME_SETCONTEXT),         // 641
    MAP(WM_IME_NOTIFY),             // 642
    MAP(WM_IME_CONTROL),            // 643
    MAP(WM_IME_COMPOSITIONFULL),    // 644
    MAP(WM_IME_SELECT),             // 645
    MAP(WM_IME_CHAR),               // 646
    MAP(WM_IME_REQUEST),            // 648
    MAP(WM_IME_KEYDOWN),            // 656
    MAP(WM_IME_KEYUP),              // 657

    MAP(WM_NCMOUSEHOVER),           // 672
    MAP(WM_MOUSEHOVER),             // 673
    MAP(WM_NCMOUSELEAVE),           // 674
    MAP(WM_MOUSELEAVE),             // 675

    MAP(WM_CUT),                    // 768
    MAP(WM_COPY),                   // 769
    MAP(WM_PASTE),                  // 770
    MAP(WM_CLEAR),                  // 771
    MAP(WM_UNDO),                   // 772

    MAP(WM_RENDERFORMAT),           // 773
    MAP(WM_RENDERALLFORMATS),       // 774
    MAP(WM_DESTROYCLIPBOARD),       // 775
    MAP(WM_DRAWCLIPBOARD),          // 776
    MAP(WM_PAINTCLIPBOARD),         // 777
    MAP(WM_VSCROLLCLIPBOARD),       // 778
    MAP(WM_SIZECLIPBOARD),          // 779
    MAP(WM_ASKCBFORMATNAME),        // 780
    MAP(WM_CHANGECBCHAIN),          // 781
    MAP(WM_HSCROLLCLIPBOARD),       // 782
    MAP(WM_QUERYNEWPALETTE),        // 783
    MAP(WM_PALETTEISCHANGING),      // 784
    MAP(WM_PALETTECHANGED),         // 785

    MAP(WM_HOTKEY),                 // 786
    MAP(WM_PRINT),                  // 791

    MAP(WM_PRINTCLIENT),            // 792
    MAP(WM_APPCOMMAND),             // 793

    /// @todo decide how to store large UINT message values,
    ///       i.e. in an attempt to keep the `xMsg` map reasonably small

    // WM_USER
    // MAP(WM_USER),                // 1024

    // WM_APP
    // MAP(WM_APP),                 // 32768 // large value => perhaps store in separate map => still to decide

}; // close the map initializer list ...
#undef MAP

#endif // end of xMSG_H

#endif // NDEBUG
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
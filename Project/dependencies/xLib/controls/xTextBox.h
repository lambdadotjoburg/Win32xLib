/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xTextBox.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xTextBox` class declaration & implemenation
  * @details  xTextBox.h defines the `xTexBox` class
  */

#pragma once

/// @brief begin of xTEXTBOX_H implementation
#ifndef xTEXTBOX_H
#define xTEXTBOX_H

/**
 * @class    xTextBox
 * @brief   `xTextBox` provides the interface for creating text-box controls
 * @details `xTextBox` defines textbox-related methods for creating
 * & manipulating `xTextBox` objects derived from `xControl`
 * @implements iTextChangeEventListener
 */
class xTextBox : public xControl, public iTextChangeEventListener {

protected:

    /// @brief string storing the default text for textbox ...
    /// @details This text is set via constructor
    /// but can be updated by invoking `setDefaultText`
    /// implemented to be `ASCI`/`UNICODE` compatible
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstring mDefaultText = L""; // empty wide string
    #else
    std::string mDefaultText = ""; // empty narrow string
    #endif

protected:

    /// @remark alias for void(*)(...) function pointer signature
    /// @param[in] pWidget ~ pointer of textbox which receives the character-limit-reached callback trigger
    using event_onCharacterLimit = std::function<void(xTextBox* pTextbox, int limit)>;

    /// @brief variable storing the character-limit
    event_onCharacterLimit mOnCharacterLimit = nullptr;

    // NOT WORKING AS EXPECTED!
    // using event_onCharacterInput = std::function<void(xTextBox* pTextbox, Key key)>;
    // event_onCharacterInput mOnCharacterInput = nullptr;

public:

    /// @brief override interface method to update/change/set
    /// an on-text-change event listener to the textbox
    virtual void setOnTextChange(event_onTextChange callback) {
        mOnTextChange = std::move(callback);
    }

    /// @brief 
    /// @param callback 
    /// @note  This method is not an interface override,
    ///        i.e. unique to this implementation ...
    virtual void setOnCharacterLimit(event_onCharacterLimit callback) {
        mOnCharacterLimit = std::move(callback);
    }

    // NOT WORKING AS EXPECTED!
    // virtual void setOnCharacterInput(event_onCharacterInput callback) {
    //     mOnCharacterInput = std::move(callback);
    // }

    /// @brief   method for retrieving the default text for the textbox
    std::string getDefaultText() {
        // return mDfaultText;
        #if defined(UNICODE) && defined(_UNICODE)
        return StrConverter::WStringToString(mDefaultText);
        #else
        return mDefaultText;
        #endif
    }

    /// @brief     method for setting/updating the default text for the textbox
    /// @param[in] text ~ std::string reference of the default text
    /// @note      Designed to be `UNICODE` & `ANSI/ASCI` compatible
    void setDefaultText(const std::string& text) {
        #if defined(UNICODE) && defined(_UNICODE)
        // copy the `const std::string&` data from `text`
        // into a new `std::wstring` variable `wstr`
        std::wstring wstr(text.begin(), text.end());
        mDefaultText = wstr; // assign the copied data to `mText`
        #else
        // just copy the data directly into `mDefaultText`
        mDefaultText = text;
        #endif
        // DON'T update window text if the window exists ...
        // if (exists) {
        //     SetWindowText(mhWnd, mText.c_str());
        // }
    }

public:

    /// @brief public parameterless/default constructor
    xTextBox() : xControl() {
        init();
        // setWidgetBorder(xBorderFactory::createEmptyBorder());
    }

    /// @brief public parameterized constructor accepting
    ///        string text, dimensions & coordinates
    xTextBox(
            const std::string& text,
            int x, int y, int w, int h // reverse order
    ) : xControl(text, w, h, x, y) {
        init();
        // setWidgetBorder(xBorderFactory::createEmptyBorder());
        setDefaultText(text); // initially text & default text are the same
    }

    /// @brief public parameterized constructor accepting
    ///        string text, dimensions & coordinates
    ///        as-well-as a widget pointer for its parent
    xTextBox(
        xWidget* parent,
        const std::string& text,
        int x, int y, int w, int h
    ) : xControl(parent, text, w, h, x, y) {
        init();
        // setWidgetBorder(xBorderFactory::createEmptyBorder());
        setDefaultText(text); // initially text & default text are the same
    }

    /// @brief   override method to create textbox objects
    /// @return  boolean indication of whether the
    ///          component was created successfully
    /// @details This method serves the purpose of
    ///          performing additional work
    ///          before/after "create" ...
    bool virtual create() override {

        // first invoke base class ...
        bool result = xWidget::create();
        
        if (result) {
            // ensure updates are made
            // after create where necessary ...
            setPasswordField(pwField);            
            setPasswordSymbol(pwSymbol);
            setCharacterLimit(characterLimit);
            setReadOnly(readonlyField);
            setNumbersOnly(numericField);
            setTextCase(textCase);
        }

        // success/fail ...
        return result;
    }

protected:

    /// @brief method to initialize data in
    ///        the constructor & create phase
    void init() {
        // append windows styles ...
        appendWindowStyle(
              ES_AUTOHSCROLL
            | WS_CHILD   | ES_WANTRETURN
            | WS_TABSTOP | WS_CLIPSIBLINGS
        );
    }
    
public:

    // constants for password (ascii) characters
    #define BULLET_CIRCLE_SMALL 0x95
    #define BULLET_ASTERISK     0x2A // default
    // add more ...

    /// @brief public enum constants
    ///        to select whether the
    ///        textbox data are to be
    ///        capitalized or NOT
    enum TextCase {
        NORMAL, // CANNOT RECOVER STATE!
        UPPER = ES_UPPERCASE,
        LOWER = ES_LOWERCASE,
    };

protected:

    /// @brief   default text-case enum constant
    /// @details Options are `NORMAL`/`UPPER`/`LOWER`
    TextCase textCase = TextCase::NORMAL; // default NORMAL

    /// @brief protected helper method to transform
    ///        the text to upper/lower case
    /// @param textcase ~ selected case enum constant
    /// @note  If text is transformed, then the
    ///        original state can never be recovered
    void transformCase(TextCase textcase) {

        if (textcase == TextCase::UPPER) {

            #if defined(UNICODE) && defined(_UNICODE)
                for (wchar_t& c : mText) { c = std::towupper(c); }
            #else
                for (char& c : mText) { c = std::toupper(c); }
            #endif

        } else if (textcase == TextCase::LOWER) {

            #if defined(UNICODE) && defined(_UNICODE)
                for (wchar_t& c : mText) { c = std::towlower(c); }
            #else
                for (char& c : mText) { c = std::tolower(c); }
            #endif
        }
    }

    /// @brief   variable storing the character limit
    ///          that the textbox can hold at max capacity
    /// @details if "full" then triggers `EN_MAXTEXT`
    /// @see     `mOnCharacterLimit`
    int characterLimit = 256; // the max length ...

    /// @brief boolean variable indicating whether or not
    ///        the textbox is read-only mode,
    ///        i.e. prohibiting editing
    bool numericField = false;

    /// @brief varia
    bool readonlyField = false;
    
    /// @brief boolean variable indicating whether or not
    ///        the textbox is intended for password use,
    ///        obscuring the data in view if `true`
    bool pwField = false;

    /// @brief   variable holding the symbol to use
    ///          if the texbox is a password field
    /// @details The default password character is * (asterisk)
    #if defined(UNICODE) && defined(_UNICODE)
    wchar_t pwSymbol = (wchar_t) BULLET_ASTERISK ; // default password symbol ...
    #else
    char pwSymbol = (char) BULLET_ASTERISK; // default password symbol ...
    #endif

public:

    /// @brief combined method to set the textbox
    /// as a password field & optional param for 
    /// specifying the password character to use!
    // void isPasswordField(
    //     bool state,
    //     char s = '*'
    // ) {
    //     UNUSED(state);
    //     UNUSED(s);
    // }

    /// @brief
    /// public interface method to change/update/set
    /// the password character to use if password box
    /// @param[in] s ~ character to use for password view
    /// @note Designed for `UNICODE`/`ASCI` compatibility <br/>
    /// converts to wide character symbol in `UNICODE` build
    void setPasswordSymbol(char s) {

        #if defined(UNICODE) && defined(_UNICODE)
            wchar_t wchar_res;
            int res = mbtowc(&wchar_res, &s, 1);
            if (res != -1) {
                pwSymbol = wchar_res;
            } else {
                // throw ...
                pwSymbol = (wchar_t) BULLET_ASTERISK; // set default ...
            }
        #else
            pwSymbol = s;
        #endif

        // invoke `setPasswordField(...)`
        // to ensure update on control ...
        if (exists) {
            setPasswordField(pwField);
        }
        updateText();
    }

    /// @brief public interface method to
    /// set the textbox as a password box
    void setPasswordField(bool flag) {

        pwField = flag;

        if (pwField) {

            appendWindowStyle(ES_PASSWORD);

            if (!exists) { return; }

            if (pwSymbol != 0) {

                SendMessage(
                    mhWnd, EM_SETPASSWORDCHAR,
                    (WPARAM) pwSymbol, (LPARAM) NULL
                );
            }

        } else {

            removeWindowStyle(ES_PASSWORD);
        }
        updateText();
    }

    /// @brief public interface method to
    ///        set the textbox control as
    ///        field for accepting numbers only
    void setNumbersOnly(bool flag) {
        numericField = flag;
        if (numericField) {
            appendWindowStyle(ES_NUMBER);
        } else {
            removeWindowStyle(ES_NUMBER);
        }
        updateText();
    }

    /// @brief public interface method to
    ///        set the textbox control as
    ///        field that is read-only
    void setReadOnly(bool flag) {
        readonlyField = flag;
        if (readonlyField) {
            appendWindowStyle(ES_READONLY);
        } else {
            removeWindowStyle(ES_READONLY);
        }
        updateText();
    }

    /// @brief public interface method to
    /// update/change/set the text case
    /// @param[in] textcase ~ enum constant
    /// with options `NORMAL`/`UPPER`/`LOWER`
    void setTextCase(TextCase textcase) {

        removeWindowStyle(ES_UPPERCASE | ES_LOWERCASE);
        
        if (textcase == TextCase::UPPER) {

            appendWindowStyle(ES_UPPERCASE);
            // update the internal string data ...
            transformCase(TextCase::UPPER);

        } else if (textcase == TextCase::LOWER) {

            appendWindowStyle(ES_LOWERCASE);
            transformCase(TextCase::LOWER);
        }
        // update the control ...
        updateText();
    }

    /// @brief method to update/change/set
    /// the character limit of the textbox
    /// before triggering `mOnCharacterLimit`
    /// if such callback action is defined
    /// @param limit ~ integer representative
    /// of the limit for the textbox/field
    void setCharacterLimit(int limit) {

        // ensure positive ...
        if (limit < 0) {
            // throw ...
            return;
        }
        
        #if defined(UNICODE) && defined(_UNICODE)
        characterLimit = limit;
        #else
        characterLimit = limit;
        #endif

        if (exists) {
            SendMessage(
                mhWnd, EM_SETLIMITTEXT,
                (WPARAM) characterLimit,
                (LPARAM) NULL
            );
        }
        updateText();
    }

    /// @defgroup getters getters
    /** @{ */
    /// @brief  public interface getter
    /// methods to retrieve textbox data
    bool getNumbersOnly() { return numericField; }
    bool getReadOnly() { return readonlyField; }
    bool getPasswordField() { return pwField; }
    TextCase getTextCase() { return textCase; }
    /** @} */

    /// @brief method to retrieve the character limit
    /// @return integer representative of the character limit
    int getCharacterLimit() { return characterLimit; }

    /// @brief method to retrieve the password character
    /// used by the password box in the textbox view/display
    char getPasswordSymbol() {
        #if defined(UNICODE) && defined(_UNICODE)
            char char_res;
            int res = wctomb(&char_res, pwSymbol);
            if (res != -1) {
                return char_res;
            } else {
                // throw ...
                return (char) BULLET_ASTERISK; // default ... 
            }        
        #else
        return pwSymbol;
        #endif
    }

protected:

    /// @brief protected override method to ensure correct class type
    virtual LPCTSTR TypeName() const override { return WC_EDIT; }
    /// @brief protected override method to ensure correct class name
    virtual LPCTSTR ClassName() const override { return TEXT("xTextBox"); }

    /// @brief variable indicating whether the textbox control
    ///        should restore to default text upon focus lost if empty ...
    bool restore = true; // default true

    /// @brief helper method to manipulate
    /// the internal state of the textbox,
    /// specifically for restoring the text
    /// upon focus lost, if empty ...
    void restoreText() {

        // if restore is false, by client preferences,
        // don't do anything, avoid the below, just return ...

        // std::cout << "restore: " << restore << std::endl;
        if (!restore) { return; }

        // check whether the textbox is empty,

        // std::cout << "getText() => " << getText() << std::endl;
        // std::cout << "getDefaultText() => " << getDefaultText() << std::endl;
        // std::cout << "getText().empty() => " << getText().empty() << std::endl;

        if (getText().empty()) {
            // if so, restore the textbox text to default ..
            setText(getDefaultText()); // set the text to default ...
            SetWindowText(mhWnd, mText.c_str());
        }
    }

    /// @brief protected helper method to update
    ///        the text view in the control, i.e.
    ///        to reflect that of the internal state!
    void updateText() {
        SetWindowText(mhWnd, mText.c_str());
    }

    /// @brief helper method to update the text
    ///        of the win32 control upon `EN_CHANGE`
    ///        (text-change) notification received ...
    void pullText() {
        
        #if defined(UNICODE) && defined(_UNICODE)
        wchar_t buffer[characterLimit]; // 
        #else
        char buffer[characterLimit+1]; // extra character for '\0'
        #endif
                
        GetWindowText(mhWnd, buffer, sizeof(buffer));
        
        #if defined(UNICODE) && defined(_UNICODE)
        setText(StrConverter::WStringToString(std::wstring(buffer)));
        #else
        setText(std::string(buffer));
        #endif
    }
    
    virtual void setText(const std::string& text) override {
        #if defined(UNICODE) && defined(_UNICODE)
        // copy the `const std::string&` data from `text`
        // into a new `std::wstring` variable `wstr`
        std::wstring wstr(text.begin(), text.end());
        mText = wstr; // assign the copied data to `mText`
        #else
        // just copy the data directly into `mText`
        mText = text;
        #endif
        // DON'T update window ...
        // if (exists) {
        //     SetWindowText(mhWnd, mText.c_str());
        // }
    }

    // inspect further `xDropDown` ...
    // the character encoding here is interfering with dropdown control ...

public:

    /// @brief method to toggle whether or not to restore
    ///        default text for the textbox if empty & focus lost ...
    void setRestoreText(bool flag) {
        restore = flag;
    }

    /// @brief method to retrieve boolean state
    ///        for whether to restore text or NOT
    //         upon focus lost & text empty ...
    bool getRestoreText() {
        return restore;
    }

    /// @brief interface method override to associate
    ///        callback function pointer for click-events
    virtual void setOnClick(event_onClick callback) override {
        mOnClick = std::move(callback);
    }

    /// @brief interface method override to associate
    ///        callback function pointer for double-click-events
    virtual void setOnDoubleClick(event_onDoubleClick callback) override {
        mOnDoubleClick = std::move(callback);
    }

    /// @brief protected base class override method
    ///        for handling the drawing of the control
    virtual LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) override {
        IMPLICIT(msg);
        IMPLICIT(wParam);
        IMPLICIT(lParam);
        return 0;
    }

    /// @brief `xTextBox` derived class main message loop ...
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {

        switch(msg) {

            case WM_PAINT: {
                // std::cout << "xTextBox::WM_PAINT" << std::endl;
                return 0;
            }

            case WM_NCPAINT: {
                // std::cout << "xTextBox::WM_NCPAINT" << std::endl;
                return 0;
            }

            case WM_COMMAND: {

                switch(HIWORD(wParam)) {

                    case EN_UPDATE: {
                        // std::cout << "EN_UPDATE" << std::endl;
                        // do something ...
                        break;
                    }

                    case EN_CHANGE: {
                        // std::cout << "EN_CHANGE" << std::endl;
                        
                        // if (restore) {
                            pullText();
                        // }
                        
                        if (mOnTextChange) {
                            mOnTextChange(this, getText());
                        }
                        // do something ...
                        break;
                    }

                    case EN_MAXTEXT: {
                        // std::cout << "EN_MAXTEXT" << std::endl;
                        if (mOnCharacterLimit) {
                            mOnCharacterLimit(this, characterLimit);
                        }
                        break;
                    }
                }

                break;
            }

            case WM_NOTIFY: {
                // std::cout << "xTextBox::WM_NOTIFY" << std::endl;
                break;
            }

            case WM_SETFOCUS: {
                // std::cout << "xTextBox::WM_SETFOCUS" << std::endl;
                if (mOnFocus) {
                    mOnFocus(this, FocusEvent::GAINED);
                }
                break;
            }

            case WM_KILLFOCUS: {
                // std::cout << "xTextBox::WM_KILLFOCUS" << std::endl;
                
                // invoke restore on default text, if empty ...
                restoreText();

                if (mOnFocus) {
                    mOnFocus(this, FocusEvent::LOST);
                }

                break;
            }

            // using the iKeyEventListener interface may be a lot more flexible ...
            // case WM_CHAR: {
            //     // std::cout << "xTextBox::WM_CHAR" << std::endl;
            //     if (mOnCharacterInput) {
            //         // last inserted character in string ...
            //         Key key(static_cast<UINT>(wParam));
            //         mOnCharacterInput(this, key);
            //     }
            //     break;
            // }

            case WM_ERASEBKGND: {
                // std::cout << "xTextBox::WM_ERASEBKGND" << std::endl;
                return 1;
            }

            // case WM_CTLCOLOREDIT: {                
            //     // std::cout << "xTextBox::WM_CTLCOLOREDIT" << std::endl;
            //     break;
            // }

            // case WM_COMMAND: {
            //     switch(HIWORD(wParam)) {
            //         case EN_SETFOCUS: {
            //             this->setFocus(true);
            //             if (mOnFocus != nullptr) {
            //                 mOnFocus(this, FocusEvent::GAINED);
            //             }
            //             break;
            //         }
            //         case EN_KILLFOCUS: {
            //             this->setFocus(false);
            //             // this->mOnFocusLost();
            //             if (mOnFocus != nullptr) {
            //                 mOnFocus(this, FocusEvent::LOST);
            //             }
            //             break;
            //         }
            //     }
            //     break;
            // }
        }

        return DefWindowProc(mhWnd, msg, wParam, lParam);
    }
};

#endif // end of xTEXTBOX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
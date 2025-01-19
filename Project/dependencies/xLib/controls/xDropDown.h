/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xDropDown.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xDropDown` class declaration & implemenation
  * @details  xDropDown.h defines the `xDropDown` class
  */

#pragma once

/// @brief begin of xDROPDOWN_H implementation
#ifndef xDROPDOWN_H
#define xDROPDOWN_H

/**
 * @class       xDropDown
 * @brief      `xDropDown` provides the interface for creating dropdown/combobox controls
 * @details    `xDropDown` defines dropdown/combobox-related methods for creating
 *              & manipulating `xDropDown` objects derived from `xControl`
 * @implements  iSelectionChangeEventListener
 * @todo        investigate templatization or base class for `xDropDown` & `xListBox`
 */
class xDropDown : public xControl, public iSelectionChangeEventListener {

    /// @brief default height for textbox controls
    #define HEIGHT_DEFAULT_TEXTBOX 18
    /// @brief default width for textbox controls
    #define WIDTH_DEFAULT_TEXTBOX 150

    /// @brief variables storing the selected item index
    /// @note -1 indicates no item selected ...
    int selectedItemIndex = -1;

public:

    /// @brief public default/parameterless constructor
    xDropDown() : xControl() {
        appendWindowStyle(
              WS_CHILD        | WS_TABSTOP
            | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST
        );
        // setWidgetBorder(xBorderFactory::createEmptyBorder());
        // setBackgroundColor(parent->colorBG);
        m_h = HEIGHT_DEFAULT_TEXTBOX;
        m_w = WIDTH_DEFAULT_TEXTBOX;
    }

    /// @brief parametrized constructor
    /// @param items ~ vector container of items to display in the dropdown (default empty)
    /// @param index ~ integer representative of the selected item index
    /// @note  width & height are reserved as last params,
    ///        since default width/height are usually acceptable
    /// @todo  borrow method from `xListBox` to auto-resize the component
    xDropDown(
        int x, int y,
        const std::vector<std::string>& items = { }, // list of items ...
        int index = -1, // (default) index of the selected item ...
        int w = WIDTH_DEFAULT_TEXTBOX,
        int h = HEIGHT_DEFAULT_TEXTBOX
    ) : xControl(" ", w, h, x, y) {        
        appendWindowStyle(
              WS_CHILD        | WS_TABSTOP
            | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST
        );

        // setWidgetBorder(xBorderFactory::createEmptyBorder());
        // setBackgroundColor(parent->colorBG);

        #if defined(UNICODE) && defined(_UNICODE)
        itemsList = StrConverter::VectorToWideString(items);
        #else
        itemsList = items;
        #endif

        selectedItemIndex = index;
    }

    /// @brief parametrized constructor taking parent widget as first argument
    /// @param items ~ vector container of items to display in the dropdown (default empty)
    /// @param index ~ integer representative of the selected item index
    /// @note  width & height are reserved as last params,
    ///        since default width/height are usually acceptable
    /// @todo  borrow method from `xListBox` to auto-resize the component
    xDropDown(
        xWidget* parent,
        int x, int y,
        const std::vector<std::string>& items = { }, // list of items ...
        int index = -1, // (default) index of the selected item ...
        int w = WIDTH_DEFAULT_TEXTBOX,
        int h = HEIGHT_DEFAULT_TEXTBOX
    ) : xControl(parent, " ", w, h, x, y) {
        
        appendWindowStyle(
              WS_CHILD        | WS_TABSTOP
            | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST
        );

        // setWidgetBorder(xBorderFactory::createEmptyBorder());
        // setBackgroundColor(parent->colorBG);

        #if defined(UNICODE) && defined(_UNICODE)
        itemsList = StrConverter::VectorToWideString(items);
        #else
        itemsList = items;
        #endif

        selectedItemIndex = index;
    }

    /// @brief interface override method to attach
    ///        on-selection-change callback action
    ///        `WM_COMMAND` => `CBN_SELCHANGE`
    /// @param callback ~ function pointer of the action
    /// to invoke upon selection-change event detected
    virtual void setOnSelectionChange(event_onSelectionChange callback) {
        mOnSelectionChange = std::move(callback);
    }

protected:

    /// @brief container storing the items of the dropdown/combobox menu ...
    #if defined(UNICODE) && defined(_UNICODE)
    std::vector<std::wstring> itemsList; // vector of wide strings ...
    #else
    std::vector<std::string> itemsList; // vector of narrow strings ...
    #endif

    /// @brief protected method to retrieve the type name
    virtual LPCTSTR TypeName() const override { return WC_COMBOBOX; }
    /// @brief protected method to retrieve the class name
    virtual LPCTSTR ClassName() const override { return TEXT("xDropDown"); }

    /// @brief protected override method to update the font
    ///        since additional logic is to be performed
    virtual void updateFont() override {
        // ensure that the control exists ...
        if (exists) {
            // get the font hight/size ...
            SendMessage(mhWnd, WM_SETFONT, (WPARAM) pFont->hFont, TRUE);

            // adjust the height of the items
            // in the combobox according to the font ...
            SendMessage(mhWnd, CB_SETITEMHEIGHT, (WPARAM)0, (LPARAM)pFont->mSize);
            redraw();
        }
    }

    /// @brief method to programatically show the dropdown menu, i.e. expand
    void show() {
        if (exists) {
            SendMessage(mhWnd, CB_SHOWDROPDOWN, TRUE, (LPARAM)NULL);
        }
    }

    /// @brief method to programatically hide the dropdown menu, i.e. shrink
    void hide() {
        if (exists) {
            SendMessage(mhWnd, CB_SHOWDROPDOWN, FALSE, (LPARAM)NULL);
        }
    }

    /// @brief method to initialize the dropdown items
    /// by inserting the vector elements into the view
    void init() {

        for (size_t i = 0; i < itemsList.size(); i++) {
            
            SendMessage(
                mhWnd, CB_INSERTSTRING, (WPARAM) NULL,
                (LPARAM)itemsList[itemsList.size()-i-1].c_str()
            );
        }
    }

public:

    /// @brief method to programatically
    /// change the selected item by string value
    /// @param text string representative of the item to select
    void select(const std::string& text) {

        #if defined(UNICODE) && defined(_UNICODE)
        std::wstring temp = StrConverter::StringToWString(text);
        #else
        std::string temp = text;
        #endif
        
        for (size_t idx = 0; idx < itemsList.size(); idx++) {
            if (itemsList[idx] == temp) {
                select(idx);
                break;
            }
        }
    }

    /// @brief method to programatically
    /// change the selected item by integer index
    /// @param idx integer representative of the item's index
    void select(int idx) {

        try {

            int s = this->count();
        
            if (idx < 0 || idx > s) {
                
                selectedItemIndex = 0;
                // add exception classes for various purposes ...
                throw RuntimeException("Selected item index out of range ...");

            } else {
                
                selectedItemIndex = idx;
            }

            // check if the list contains elements ...
            if (count() > 0) {
                SendMessage(mhWnd, CB_SETCURSEL, (WPARAM) selectedItemIndex, (LPARAM)NULL);
            }

        } catch(RuntimeException &ex) {

            LOG(ex.what());
            std::cerr << ex.what() << std::endl;
        }

        // invoke on-selection-change callback trigger action ...
        if (mOnSelectionChange) {
            mOnSelectionChange(this, selectedItemIndex);
        }
    }

    /// @brief method to retrieve the index of the selected item
    int getSelectedIndex() {
        return selectedItemIndex;
    }

    /// @brief method to retrieve the selected item string text
    std::string getSelectedItem() {        
        #if defined(UNICODE) && defined(_UNICODE)
        return StrConverter::WStringToString(itemsList[selectedItemIndex]);
        #else
        return itemsList[selectedItemIndex];
        #endif
    }

public:

    /// @brief override base class create method
    ///        to ensure that the items in the menu show ..
    virtual bool create() override {
        // when creating the xDropDown component
        // first create via base class
        bool success = xWidget::create();

        if (success) {
            // then add the items from inside the `itemsList`
            // to ensure that items that were inserted via
            // constructor args reflect/show in the menu view ...        
            init();

            // check if selected item index was provided during construction ...
            if (selectedItemIndex < 0) {
                // if not, i.e. negative, then use first item in list ...
                selectedItemIndex = 0;
            }
            // set selected item based on index ...
            select(selectedItemIndex);
        }

        // return based on whether
        // the item was created successfully ...
        return success;
    }

    /// @brief interface override method to associate a click event callback/action
    /// @param callback ~ function pointer of the action to invoke upon click event detected
    virtual void setOnClick(event_onClick callback) override {
        mOnClick = std::move(callback);
    }

    /// @brief interface override method to associate a double click event callback/action
    /// @param callback ~ function pointer of the action to invoke upon double click event detected
    virtual void setOnDoubleClick(event_onDoubleClick callback) override {
        mOnDoubleClick = std::move(callback);
    }

public:

    /// @brief method to combine width/height adjustment into a single call
    /// @param w ~ integer width to apply for the control
    /// @param h ~ integer height to apply for the control
    void setDimensions(int w, int h) {
        setWidth(w);
        setHeight(h);
    }

    /// @brief method to update/change/set the height
    ///        of the dropdown/combobox control
    /// @param[in] h ~ integer representative of height
    /// @note  logic slightly different than listbox
    void setHeight(int h) override {

        // if (h == m_h) { return; }

        // update `xWidget` height
        m_h = h;
        
        // if not exists, escape ...
        if (!exists) { return; }

        // propagate the changes to the control ...
        SendMessage(mhWnd, CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)m_h);
        redraw();
    }

    /// @brief method to update/change/set the width
    ///        of the dropdown/combobox control
    /// @param[in] w ~ integer representative of width
    /// @note logic slightly different than listbox
    void setWidth(int w) override {

        // if (w == m_w) { return; }
        
        // update `xWidget` width
        m_w = w;

        // if not exists, escape ...
        if (!exists) { return; }

        RECT rect;
        GetWindowRect(mhWnd, &rect);
        SetWindowPos(
            mhWnd, NULL,
            rect.left, rect.top, w, m_h,
            SWP_NOZORDER | SWP_NOMOVE
        );
        SendMessage(mhWnd, CB_SETDROPPEDWIDTH, (WPARAM) m_w, (LPARAM) 0);
        redraw();
    }

    /// @brief  method to retrieve the number of items in the dropdown/combobox
    /// @return integer representative of the number of items in the dropdown menu
    int count() {
        return itemsList.size();
    }

    /// @brief method to check whether an item is contained in the list
    /// @param item ~ string representative of the item to find
    /// @return boolean flag representative of whether the item is found
    bool checkItem(const std::string& item) {

        #if defined(UNICODE) && defined(_UNICODE)
        std::wstring temp = StrConverter::StringToWString(item);
        std::vector<std::wstring>::iterator i;
        #else
        std::string temp = item;
        std::vector<std::string>::iterator i;
        #endif

        i = std::find(itemsList.begin(), itemsList.end(), temp);

        if (i != itemsList.end()) {
            return true;
        }

        return false;
    }

    /// @brief method to add/insert new items into the dropdown menu
    /// @param item ~ string representative of the new item to be inserted
    /// @param index ~ position/index at which to insert the item <br/>
    ///        If none specified, then default position is at the end!
    void add(const std::string& item, int index = -1) {

        #if defined(UNICODE) && defined(_UNICODE)
        std::wstring temp = StrConverter::StringToWString(item);
        #else
        std::string temp = item;
        #endif

        // get the size of the menu items ...
        int s = this->count();
        // check whether the specified index is sensible & update if not ...
        int i = (index < 0 || index > s ? s : index);

        // std::cout << "index: " << i << std::endl;

        #if defined(UNICODE) && defined(_UNICODE)
        if (
            !checkItem(
                StrConverter::WStringToString(temp)
            )
        ) {
            // insert the item at the specified index 
            itemsList.insert(itemsList.begin() + i, temp);
        }
        #else
        if (!checkItem(temp)) {
            // insert the item at the specified index 
            itemsList.insert(itemsList.begin() + i, temp);
        }
        #endif

        // also ensure the item is not contained in the view ...

        // insert into the control's view ...
        SendMessage(mhWnd, CB_INSERTSTRING, (WPARAM) i, (LPARAM) temp.c_str());

        // if (index == selectedItemIndex) {
        //     select(selectedItemIndex);
        // }
        select(selectedItemIndex);
        // redraw();
    }

    /// @brief method to insert/add new items into the dropdown/combox control
    /// @param items ~ vector container of the items to insert
    /// @param index ~ integer representative of the selected item index
    void add(const std::vector<std::string>& items, int index = - 1) {

        // ... 

        // add the items to the combobox/dropdown's vector container ...
        for (size_t i = 0; i < items.size(); i++) {
            add(items[i]);
        }
        select(index);
    }

    /// @brief     method to remove an item from the combobox/dropdown
    /// @param[in] item ~ string representative of the item to remove
    void remove(const std::string& item) {

        #if defined(UNICODE) && defined(_UNICODE)
        std::wstring temp = StrConverter::StringToWString(item);
        #else
        std::string temp = item;
        #endif

        for (size_t i = 0; i < itemsList.size(); i++) {
            if (temp == itemsList[i]) {
                remove(i); // remove by index found ...
                break;
            }
        }
    }

    /// @brief     method to remove an item from the combobox/dropdown
    /// @param[in] index ~ integer representative of
    ///            the position for the item to removed
    void remove(int index) {

        if (index < 0 || index > count()) {
            // throw exception, index out of range ...
            return;
        };
        
        // remove the item from the dropdown view
        LRESULT result = SendMessage(mhWnd, CB_DELETESTRING, (WPARAM) index, (LPARAM)0);

        // check whether the item was remove from the combobox ...
        if (result != CB_ERR) {
            // remove the item from the vector container
            itemsList.erase(itemsList.begin() + index);
        }

        // ensure that if the removed item was selected,
        // then select the first item in the list ...
        if (index == selectedItemIndex) {
            select(0);
        }
    }

    /// @brief method to retrieve an item by index
    /// @param index ~ integer representative of the item's index to retrieve
    /// @return string representative of the item in the dropdown found
    std::string getItemByIndex(int index) {

        #if defined(UNICODE) && defined(_UNICODE)
        return StrConverter::WStringToString(itemsList[index]);
        #else
        return itemsList[index];
        #endif
    }

    #ifndef NDEBUG
    /// @brief method to enumerate all the items in the dropdown control
    void EnumerateItems() {

        for (size_t i = 0; i < itemsList.size(); i++) {
            std::cout << "index: " << i << " => " << getItemByIndex(i) << std::endl;
        }
    }
    #endif

    /// @todo  override custom drawing to be implemented
    ///        when background color & text color
    ///        is to be dynamically changed/adjusted
    virtual LRESULT CustomDraw(UINT msg, WPARAM wParam, LPARAM lParam) override {
        
        IMPLICIT(msg);
        IMPLICIT(wParam);
        IMPLICIT(lParam);

        return 0;
    }

    /// @brief `xDropDown` derived class main message loop ...
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {

        switch (msg) {

            // DON'T REMOVE!
            case WM_PAINT: {
                return 0;
            }

            // DON'T REMOVE!
            case WM_NCPAINT: {
                return 0;
            }

            // handle events ...
            case WM_COMMAND: {
                
                switch(HIWORD(wParam)) {

                    case CBN_SELCHANGE: {
                        // std::cout << "CBN_SELCHANGE" << std::endl;

                        LRESULT selectedIndex = SendMessage(mhWnd, CB_GETCURSEL, 0, 0);

                        if (selectedIndex != CB_ERR) {
                            // selectedItemIndex = selectedIndex;
                            select(selectedIndex);
                        }

                        break;
                    }

                    case CBN_KILLFOCUS: {
                        // std::cout << "CBN_KILLFOCUS" << std::endl;
                        if (mOnFocus) {
                            mOnFocus(this, FocusEvent::LOST);
                        }
                        break;
                    }

                    case CBN_SETFOCUS: {
                        // std::cout << "CBN_SETFOCUS" << std::endl;
                        if (mOnFocus) {
                            mOnFocus(this, FocusEvent::GAINED);
                        }
                        break;
                    }
                }

                break;
            }
        }
        
        return DefWindowProc(mhWnd, msg, wParam, lParam);
    }

    // ...
};

#endif // end of xTEXTBOX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
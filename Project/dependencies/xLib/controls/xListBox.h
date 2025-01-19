/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xListBox.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xListBox` class declaration & implemenation
  * @details  xListBox.h defines the `xListBox` class
  */

#pragma once

/// @brief begin of xLISTBOX_H implementation
#ifndef xLISTBOX_H
#define xLISTBOX_H

/**
 * @class      xListBox
 * @brief     `xListBox` provides the interface for creating listbox controls
 * @details   `xListBox` defines listbox-related methods for creating
 *             & manipulating `xListBox` objects derived from `xControl`
 * @implements iSelectionChangeEventListener
 * @todo       investigate templatization or base class for `xDropDown` & `xListBox`
 */
class xListBox : public xControl, public iSelectionChangeEventListener {

protected:

    /// @todo incorporate functionality for auto-resizing
    // xFont::calculateTextHeight(mhWnd, item)
    // xFont::calculateTextWidth(mhWnd, item);

    /// @brief default heigth for listbox controls
    #define HEIGHT_DEFAULT_LISTBOX 18
    /// @brief default width for listbox controls
    #define WIDTH_DEFAULT_LISTBOX  150

    /// @brief variables storing the selected item index
    /// @note -1 indicates no item selected ...
    int selectedItemIndex = -1;

public:

    /// @brief public default/parameterless constructor
    xListBox() : xControl() {
        appendWindowStyle(
              WS_CHILD        | WS_TABSTOP
            | WS_CLIPSIBLINGS | LBS_NOTIFY
            | WS_VSCROLL
        );
        // setWidgetBorder(xBorderFactory::createEmptyBorder());
        // setBackgroundColor(parent->colorBG);
        m_h = HEIGHT_DEFAULT_LISTBOX;
        m_w = WIDTH_DEFAULT_LISTBOX;
    }

    /// @brief parametrized constructor
    /// @param items ~ vector container of items to display in the listbox (default empty)
    /// @param index ~ integer representative of the selected item index
    /// @note  width & height are reserved as last params,
    ///        since this class's interface provides auto-resizing
    xListBox(
        int x, int y,
        const std::vector<std::string>& items = { }, // list of items ...
        int index = -1, // (default) index of the selected item ...
        int w = WIDTH_DEFAULT_LISTBOX,
        int h = HEIGHT_DEFAULT_LISTBOX
    ) : xControl(" ", w, h, x, y) {

        appendWindowStyle(
              WS_CHILD        | WS_TABSTOP
            | WS_CLIPSIBLINGS | LBS_NOTIFY
            | WS_VSCROLL
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
    /// @param items ~ vector container of items to display in the listbox (default empty)
    /// @param index ~ integer representative of the selected item index
    /// @note  width & height are reserved as last params,
    ///        since this class's interface provides auto-resizing
    xListBox(
        xWidget* parent,
        int x, int y,
        const std::vector<std::string>& items = { }, // list of items ...
        int index = -1, // (default) index of the selected item ...
        int w = WIDTH_DEFAULT_LISTBOX,
        int h = HEIGHT_DEFAULT_LISTBOX
    ) : xControl(parent, " ", w, h, x, y) {
        
        appendWindowStyle(
              WS_CHILD        | WS_TABSTOP
            | WS_CLIPSIBLINGS | LBS_NOTIFY
            | WS_VSCROLL
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
    ///        `WM_COMMAND` => `LBN_SELCHANGE`
    /// @param callback ~ function pointer of the action
    /// to invoke upon selection-change event detected
    virtual void setOnSelectionChange(
        event_onSelectionChange callback
    ) override {
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
    virtual LPCTSTR TypeName() const override { return WC_LISTBOX; }
    /// @brief protected method to retrieve the class name
    virtual LPCTSTR ClassName() const override { return TEXT("xListBox"); }

    /// @brief protected override method to update the font
    ///        since additional logic is to be performed
    virtual void updateFont() override {
        // ensure that the control exists ...
        if (exists) {
            // get the font hight/size ...
            SendMessage(mhWnd, WM_SETFONT, (WPARAM) pFont->hFont, TRUE);

            // adjust the height of the items
            // in the combobox according to the font ...
            SendMessage(mhWnd, LB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)pFont->mSize);
            redraw();
        }
    }

    // not used by listbox, but similar in dropdown
    // void show() {
    //     if (exists) {
    //         SendMessage(mhWnd, LB_SHOWSELITEMS, TRUE, (LPARAM)NULL);
    //     }
    // }

    // not used by listbox, but similar in dropdown
    // void hide() {
    //     if (exists) {
    //         SendMessage(mhWnd, CB_SHOWDROPDOWN, FALSE, (LPARAM)NULL);
    //     }
    // }

    /// @brief method to initialize the listbox items
    /// by inserting the vector elements into the view
    void init() {

        for (size_t i = 0; i < itemsList.size(); i++) {
            
            SendMessage(
                mhWnd, LB_INSERTSTRING, (WPARAM) NULL,
                (LPARAM)itemsList[itemsList.size()-i-1].c_str()
            );
        }
    }

protected:

    // this will recurse ...
    // void redraw() override {
    //     resize();
    //     xWidget::redraw();
    // }

    bool unselectable = false; // default false

public:

    /// @brief method to toggle the listbox "unselectable" attribute
    void setUnselectable(bool flag) {
        unselectable = flag;
    }

    /// @brief  method for deselecting/unselecting a listbox item
    /// @param  index ~ integer representative of the item's index
    /// @note   only applicable to item which is (currently) selected
    /// @remark marked virtual since `xMultiSelectListBox` derived override
    virtual void deselect(int index) {

        // cannot deselect an item which is not selected ...
        if (selectedItemIndex != index) { return; }
        
        SendMessage(mhWnd, LB_SETCURSEL, (LPARAM) -1, (WPARAM) FALSE);

        // update the internal state of the control ...
        selectedItemIndex = -1;

        // invoke on-selection-change callback trigger action ...
        if (mOnSelectionChange) {
            mOnSelectionChange(this, selectedItemIndex); // -1
        }
    }

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

            // store the count ...
            int s = this->count();

            // cannot select an item that is not in the list ... 
            if (idx < -1 || idx > s) {
                
                // selectedItemIndex = 0;
                // add exception classes for various purposes ...
                throw RuntimeException("Selected item index out of range ...");

            } else {
                
                selectedItemIndex = idx;
            }

            // check if the list contains elements ...
            if (count() > 0) {

                // -1 still acceptable,
                // but only for internal state
                if (selectedItemIndex == -1) {
                    // return early, don't throw ...
                    return;
                }

                SendMessage(
                    mhWnd, LB_SETCURSEL,
                    (WPARAM) selectedItemIndex,
                    (LPARAM)NULL
                );
            }

        } catch(RuntimeException &ex) {

            LOG(ex.what());
            std::cerr << ex.what() << std::endl;
            return;
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
                // selectedItemIndex = 0;
            }
            // set selected item based on index ...
            select(selectedItemIndex);

            // resize();
            setWidth(m_w);
            setHeight(m_h);
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

    /// @brief method to update/change/set
    /// the height of the listbox control
    /// @param[in] h ~ integer representative of height
    /// @note  logic slightly different than dropdown/combobox
    void setHeight(int h) override {

        // padd the height ...
        h += GetSystemMetrics(SM_CYHSCROLL) + 10;

        // update `xWidget` height
        m_h = h;

        // if not exists, escape ...
        if (!exists) { return; }

        RECT rect;
        GetWindowRect(mhWnd, &rect);

        SetWindowPos(
            mhWnd, NULL,
            rect.left, rect.top, m_w, m_h,
            SWP_NOZORDER | SWP_NOMOVE
        );

        redraw();
    }

    /// @brief method to update/change/set
    /// the width of the listbox control
    /// @param[in] w ~ integer representative of width
    /// @note logic slightly different than dropdown/combobox
    void setWidth(int w) override {

        // padd the width ...
        w += GetSystemMetrics(SM_CXVSCROLL) + 10;

        // update `xWidget` width
        m_w = w;

        // if not exists, escape ...
        if (!exists) { return; }

        RECT rect;
        GetWindowRect(mhWnd, &rect);

        SetWindowPos(
            mhWnd, NULL,
            rect.left, rect.top, m_w, m_h,
            SWP_NOZORDER | SWP_NOMOVE
        );

        redraw();
    }

    /// @brief  method to retrieve the number of items in the listbox
    /// @return integer representative of the number of items in the listbox menu
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

    /// @brief method to add/insert new items into the listbox menu
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
        SendMessage(mhWnd, LB_INSERTSTRING, (WPARAM) i, (LPARAM) temp.c_str());

        if (index == selectedItemIndex) {
            select(selectedItemIndex);
        }
    }

    /// @brief method to insert/add new items into the listbox control
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

    /// @brief     method to remove an item from the listbox
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

    /// @brief     method to remove an item from the listbox
    /// @param[in] index ~ integer representative of
    ///            the position for the item to removed
    void remove(int index) {

        if (index < 0 || index > count()) {
            // throw exception, index out of range ...
            return;
        };
        
        // remove the item from the dropdown view
        LRESULT result = SendMessage(mhWnd, LB_DELETESTRING, (WPARAM) index, (LPARAM)0);

        // check whether the item was remove from the combobox ...
        if (result != LB_ERR) {
            // remove the item from the vector container
            itemsList.erase(itemsList.begin() + index);
        }

        // ensure that if the removed item was selected,
        // then select the first item in the list ...
        if (index == selectedItemIndex) {
            select(0); // triggers redraw ...
        }
    }

    /// @brief method to retrieve an item by index
    /// @param index ~ integer representative of the item's index to retrieve
    /// @return string representative of the item in the listbox found
    std::string getItemByIndex(int index) {

        #if defined(UNICODE) && defined(_UNICODE)
        return StrConverter::WStringToString(itemsList[index]);
        #else
        return itemsList[index];
        #endif
    }

    #if defined(UNICODE) && defined(_UNICODE)
    int getIndex(const std::wstring& item)
    #else
    int getIndex(const std::string& item)
    #endif
    {
        for (size_t i = 0; i < itemsList.size(); i++) {
            if (itemsList[i] == item) {
                return i;
            }
        }
        return -1;
    }

    #ifndef NDEBUG
    /// @brief method to enumerate all the items in the listbox control
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

    /// @brief `xListBox` derived class main message loop ...
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

                    case LBN_SELCHANGE: {
                        // std::cout << "CBN_SELCHANGE" << std::endl;

                        LRESULT selectedIndex = SendMessage(mhWnd, LB_GETCURSEL, 0, 0);

                        // std::cout << "selectedIndex: " << selectedIndex << std::endl;

                        if (selectedIndex != LB_ERR) {

                            // if selected item is distinct from current ...
                            if (selectedIndex != selectedItemIndex) {

                                select(selectedIndex);

                            } else {

                                if (unselectable) {
                                    deselect(selectedItemIndex);
                                }
                            }
                        }

                        break;
                    }

                    // case LBN_SELCANCEL: {
                    //     // ...
                    //     break;
                    // }

                    case LBN_KILLFOCUS: {
                        // std::cout << "CBN_KILLFOCUS" << std::endl;
                        if (mOnFocus) {
                            mOnFocus(this, FocusEvent::LOST);
                        }
                        break;
                    }

                    case LBN_SETFOCUS: {
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

#endif // end of xLISTBOX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
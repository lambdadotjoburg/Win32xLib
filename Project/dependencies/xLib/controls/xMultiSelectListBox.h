/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xMultiSelectListBox.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xMultiSelectListBox` class declaration & implemenation
  * @details  xMultiSelectListBox.h defines the `xMultiSelectListBox` class
  */

#pragma once

/// @brief begin of xMULTISELECTLISTBOX_H implementation
#ifndef xMULTISELECTLISTBOX_H
#define xMULTISELECTLISTBOX_H

/// @todo
/// correct/fix/implement unselectable
/// feature for multi-select listbox class

#include "./xListBox.h"

/**
 * @class      xMultiSelectListBox
 * @brief     `xMultiSelectListBox` provides the interface for creating multi-select listbox controls
 * @details   `xMultiSelectListBox` defines listbox-related methods for creating
 *             & manipulating `xMultiSelectListBox` objects derived from `xListBox`
 */
class xMultiSelectListBox : public xListBox {

public:

    /// @brief public default/parameterless constructor
    xMultiSelectListBox() : xListBox() {
        appendWindowStyle(LBS_MULTIPLESEL);
        unselectable = true;
    }

    /// @brief parametrized constructor
    /// @param indices ~ vector of indices
    /// indicating default-selected items
    xMultiSelectListBox(
        int x, int y,
        const std::vector<std::string>& items = { }, // list of items ...
        const std::vector<int>& indices = { }, // (default empty)
        int w = WIDTH_DEFAULT_LISTBOX,
        int h = HEIGHT_DEFAULT_LISTBOX
    ) : xListBox(x, y, items, -1, w, h) {
        appendWindowStyle(LBS_MULTIPLESEL);
        unselectable = true;
        selectedIndices = indices;
    }

    /// @brief parametrized constructor taking parent widget as first argument
    /// @param items ~ vector container of items to display in the listbox (default empty)
    /// @param indices ~ vector of indices indicating default-selected items
    xMultiSelectListBox(
        xWidget* parent,
        int x, int y,
        const std::vector<std::string>& items = { }, // list of items ...
        const std::vector<int>& indices = { }, // (default empty)
        int w = WIDTH_DEFAULT_LISTBOX,
        int h = HEIGHT_DEFAULT_LISTBOX
    ) : xListBox(parent, x, y, items, -1, w, h) {
        appendWindowStyle(LBS_MULTIPLESEL);
        unselectable = true;
        selectedIndices = indices;
    }

protected:
    
    /// @brief variable to keep track of number of selected items ...
    int selectedIndicesCount = 0;

    /// @brief container to store the indices of the selected items ...
    std::vector<int> selectedIndices = { /* initially empty */ };

    /// @brief internal (protected) helper method
    /// to update the container of selected items ...
    void select(int* indices, int count) {

        selectedIndicesCount = count;

        std::vector<int> indexList;
        indexList.reserve(count);

        for (int i = 0; i < count; i++) {
            indexList.push_back(indices[i]);
        }

        selectedIndices = indexList;
        // #ifndef NDEBUG // for debugging
        // EnumerateSelectedIndices();
        // #endif
    }

public:

    /// @brief method to check whether an item is in a "select" state
    /// @param index ~ integer representative of the item's index to check
    /// @return boolean flag (true/false) whether an item is selected
    bool isSelected(int index) {

        std::vector<int>::iterator i;
        
        i = std::find(
            selectedIndices.begin(),
            selectedIndices.end(),
            index
        );

        if (i != selectedIndices.end()) {
            return true;
        }

        return false;
    }

    /// @brief method to check whether an item is in a "select" state
    /// @param item ~ string representative of the item to check
    /// @return boolean flag (true/false) whether an item is selected
    bool isSelected(const std::string& item)
    {
        #if defined(UNICODE) && defined(_UNICODE)
        int index = getIndex(StrConverter::StringToWString(item));
        #else
        int index = getIndex(item);
        #endif

        return isSelected(index);
    }

    /// @brief  method to retrieve the number of selected items
    /// @return integer representative of the number of selected items
    int getSelectedCount() {
        return selectedIndicesCount;
    }

    /// @brief public override create method to initialize
    /// & create the multi-select listbox component/widget
    /// @return boolean flag whether or NOT
    /// the item was created successfully
    bool create() override {
        bool success = xListBox::create();

        select(selectedIndices);

        return success;
    }

    /// @brief method to retrieve the list/vector of selected indices
    /// @return vector of integers, representing the selected indices
    std::vector<int> getSelectedIndices() {
        return selectedIndices;
    }

    /// @brief method to retrieve the list/vector of selected items
    /// @return vector of strings, representing the selected items
    std::vector<std::string> getSelectedItems() {
        
        std::vector<std::string> filter;

        for (size_t i = 0; i < selectedIndices.size(); i++) {
            filter.push_back(getItemByIndex(selectedIndices[i]));
        }

        return filter;
    }

    /// @brief method to select items in the list
    /// based on an input vector of integer indices
    /// @note NOT the most efficient mechanism,
    /// since there's a bit of passing around of params
    void select(const std::vector<int>& indices) {
        // iterate the given vector of indices ...  
        for (size_t i = 0; i < indices.size(); i++) {
            select(indices[i]);
        }
    }

    /// @brief method to select items in the list
    /// based on an input vector of string items
    /// @param items ~ list of item string values
    #if defined(UNICODE) && defined(_UNICODE)
    void select(const std::vector<std::wstring>& items)
    #else
    void select(const std::vector<std::string>& items)
    #endif
    {
        // check whether the item is found in the listbox ...
       for (size_t i = 0; i < items.size(); i++) {
            select(getIndex(items[i]));
       }
    }

    /// @brief method to toggle the "select" state
    /// of an item in the multi-select listbox
    /// @param index ~ integere representative
    /// of the item's position/index in the list
    void select(int index) {
        
        if (index < -1 || index > count()) { return; }

        if (index == -1) {
            // deselect all ...
            SendMessage(mhWnd, LB_SETCURSEL, (LPARAM) -1, (WPARAM) FALSE);
            return;
        }

        // if index valid, send message to select item ...
        SendMessage(mhWnd, LB_SETSEL, (LPARAM) TRUE, (WPARAM) index);
    }

    /// @brief method to deselect a multi-select
    /// listbox item, based on its integer index
    void deselect(int index) override {

        // std::cout << "xMultiSelectListBox::deselect(...)" << std::endl;
        // std::cout << "index: " << index << std::endl;
        
        if (index < -1 || index > count()) {
            return;
        }

        if (index == -1) {

            // std::cout << "deselect ALL ... " << std::endl;
            // if (unselectable) {
            //     // invoke select(index);
            //     // this will trigger `mOnSelectionChange` ...
            //     return;
            // }
            
            // update the internal state of the control ...
            selectedItemIndex = -1;

            // deselect all ...
            SendMessage(mhWnd, LB_SETSEL, (LPARAM) FALSE, (WPARAM) -1);

            return; // should still reveive `mOnSelectionChange` ...

        } // otherwize ...

        // if (unselectable) {
        //     // invoke select(index);
        //     // this will trigger `mOnSelectionChange` ...
        //     select(index);
        //     return;
        // }

        // invoke on-selection-change callback trigger action ...
        if (mOnSelectionChange) {
            mOnSelectionChange(this, selectedItemIndex); // -1
        }
    }

    /// @brief method to deselect a list/group
    /// of multi-select listbox item all at once
    /// based on an integer index vector
    void deselect(const std::vector<int>& items){
        for (size_t i = 0; i < items.size(); i++) {
            deselect(items[i]);
        }
    }

    /// @brief method to deselect a list/group
    /// of multi-select listbox item all at once
    /// based on a string items vector
    void deselect(const std::vector<std::string>& items){
        for (size_t i = 0; i < items.size(); i++) {
            deselect(items[i]);
        }
    }

    /// @brief method to deselect an item
    /// in the multi-select list box based
    /// on the item's string value ...
    void deselect(const std::string& text){

        #if defined(UNICODE) && defined(_UNICODE)
        int index = getIndex(StrConverter::StringToWString(text));
        #else
        int index = getIndex(text);
        #endif

        deselect(index);
    }

    #ifndef NDEBUG
    /// @brief method to enumerate the selected indices
    /// in the multi-select listbox control
    void EnumerateSelectedIndices() {

        if (selectedIndicesCount == 0) {
            std::cout << "[ ]" << std::endl;
            return; // escape ...
        }

        std::stringstream ss;
        size_t s = selectedIndices.size();
        // std::cout << "s: " << s << std::endl;

        ss << "["; // open bracket ...
        // enumerate till second last item ...
        for (size_t i = 0; i < s-1; i++) {
            // don't close until last item ...
            ss << selectedIndices[i] << ", ";
            // don't append comman for last item ...
        }
        ss << selectedIndices[s-1]; // last item ...
        ss << "]"; // close bracket

        // stream the data to the console ...
        std::cout << ss.str() << std::endl;
    }
    #endif

    #ifndef NDEBUG
    /// @brief method to enumerate the selected items
    /// in the multi-select listbox control
    void Enumerate() {

        if (selectedIndicesCount == 0) {
            std::cout << "no selected items ... " << std::endl;
            return; // escape ...
        }

        std::stringstream ss;
        size_t s = selectedIndices.size();
        // std::cout << "s: " << s << std::endl;

        // enumerate till second last item ...
        for (size_t i = 0; i < s; i++) {
            // don't close until last item ...
            ss
            << "index: " << selectedIndices[i]
            << " => "
            << "item: " << getItemByIndex(selectedIndices[i]) << std::endl;
            // don't append comman for last item ...
        }

        // stream the data to the console ...
        std::cout << ss.str();
    }
    #endif

public:

    /// @brief `xMultiSelectListBox` derived class main message loop ...
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {

        switch (msg) {

            // handle events ...
            case WM_COMMAND: {
                
                switch(HIWORD(wParam)) {

                    case LBN_SELCHANGE: {

                        int count = SendMessage(mhWnd, LB_GETSELCOUNT, 0, 0);

                        // unselectable ...
                        
                        // int c = this->count(); // count of current container ...

                        // // i.e. count can only go up, i.e. increase only ...
                        // if (unselectable && count < c) {
                        //     // just select all the items again ...
                        //     select(selectedIndices);
                        //     break; // don't allow further processing
                        // }

                        int* indexList = new int[count];

                        LRESULT result = SendMessage(mhWnd, LB_GETSELITEMS, (WPARAM) count, (LPARAM) indexList);

                        if (result != LB_ERR) {

                            select(indexList, count);
                        }

                        delete[] indexList;

                        break;
                    }

                }

                break;
            }
        }
        
        return xListBox::HandleMessage(msg, wParam, lParam);
    }

};

#endif // end of xLISTBOX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
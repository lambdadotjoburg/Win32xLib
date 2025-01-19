/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xMultiLineTextBox.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xMultiLineTextBox` class declaration & implemenation
  * @details  xMultiLineTextBox.h defines the `xMultiLineTextBox` class
  */

#pragma once

/// @brief begin of xMULTILINETEXTBOX_H implementation
#ifndef xMULTILINETEXTBOX_H
#define xMULTILINETEXTBOX_H

/// include parent class type declaration ...
#include "./xTextBox.h"

/**
 * @class      xMultiLineTextBox
 * @brief     `xMultiLineTextBox` provides the interface for creating multi-linte textbox controls
 * @details   `xMultiLineTextBox` defines multi-line texbox-related methods for creating
 *             & manipulating `xMultiLineTextBox` objects derived from `xControl`
 */
class xMultiLineTextBox : public xTextBox {

protected:

    /// @brief method to initialize the multiline textbox control
    /// specifically, for adjusting multiline parameters & character limit!
    void init() {
        appendWindowStyle(
            ES_MULTILINE | WS_VSCROLL // | ES_AUTOSCROLL
        );
        setCharacterLimit(1000);
    }

public:

    /// @brief public default/parameterless constructor
    xMultiLineTextBox() {
        init();
    }

    /// @brief public parametrized constructor
    xMultiLineTextBox(
        const std::string& text,
        int x, int y, int w, int h
    ) : xTextBox(text, x, y, w, h) {
        init();
    }

    /// @brief public parametrized constructor
    /// accepting widget parent as first arg
    xMultiLineTextBox(
        xWidget* parent,
        const std::string& text,
        int x, int y, int w, int h 
    ) : xTextBox(parent, text, x, y, w, h) {
        init();
    }

    // ...
};

#endif // end of xMULTILINETEXTBOX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xLabel.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xLabel` class declaration & implemenation
  * @details  xLabel.h defines the `xLabel` class
  */

#pragma once

/// @brief begin of xLABEL_H implementation
#ifndef xLABEL_H
#define xLABEL_H

/**
 * @class    xLabel
 * @brief   `xLabel` provides the interface for creating label controls
 * @details `xLabel` defines label-related methods for creating
 * & manipulating `xLabel` objects derived from `xControl`
 */
class xLabel : public xControl {

public:

    /// @brief public parameterless/default constructor
    /// @note  The control is given a default white background
    xLabel() : xControl() {
        // ...
    }

    /// @brief public parameterized constructor accepting
    ///        string text, dimensions & coordinates
    /// @note  default background color is the same
    ///        as the parent control (if any)
    xLabel(
            const std::string& text,
            int x, int y, int w, int h // note reverse order ...
        ) : xControl(text, w, h, x, y) {
        // ...
    }

    /// @brief public parameterized constructor accepting
    ///        string text, dimensions & coordinates
    ///        as-well-as a widget pointer for its parent
    /// @note  default background color is the same as the parent control
    xLabel(
        xWidget* parent,
        const std::string& text,
        int x, int y, int w, int h // note reverse order ...
        ) : xControl(parent, text, w, h, x, y) {
        // ...
    }

    void init() {

        appendWindowStyle(WS_CHILD | SS_NOTIFY | WS_TABSTOP | WS_CLIPSIBLINGS);

        if (!pWidgetBorder) {
            setWidgetBorder(xBorderFactory::createEmptyBorder());
        }
        
        if (!colorBG) {
            // check if parent exists ...
            if (parent) {
                setBackgroundColor(parent->colorBG);
            }
        }
        
        if (!hoverBG) {
            setHoverColor(parent->colorBG);
        }
    }

    virtual bool create() override {

        init();
        
        bool result = xWidget::create();

        HDC hDC = GetDC(mhWnd);
        setDrawContext(hDC);

        RECT rect;
        GetClientRect(mhWnd, &rect);
        setDrawRect(rect);

        return result;
    }

protected:

    /// @brief protected override method to ensure correct class type
    virtual LPCTSTR TypeName() const override { return WC_STATIC; }
    /// @brief protected override method to ensure correct class name
    virtual LPCTSTR ClassName() const override { return TEXT("xLabel"); }

public:

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

        // std::cout << "xLabel::CustomDraw" << std::endl;
        // std::cout << "R: " << (int)GetRValue(getBackgroundColor()) << std::endl;
        // std::cout << "G: " << (int)GetGValue(getBackgroundColor()) << std::endl;
        // std::cout << "B: " << (int)GetBValue(getBackgroundColor()) << std::endl;
        // std::cout << "getDrawContext() => " << getDrawContext() << std::endl;

        PAINTSTRUCT ps;
        BeginPaint(mhWnd, &ps);

        draw(this);

        EndPaint(mhWnd, &ps);

        return 0;
    }

    /// @brief `xLabel` derived class main message loop ...
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {

        switch(msg) {

            case WM_NCPAINT: {
                return 0;
            }

            case WM_PAINT: {
                // invokes draw rouine ...
                return CustomDraw(msg, wParam, lParam);
            }

            // case WM_ERASEBKGND: {
            //     // HDC hdc = getDrawContext();
            //     // RECT rect = getDrawRect();
            //     // HBRUSH hBrush = CreateSolidBrush(getBackgroundColor());
            //     // FillRect(hdc, &rect, hBrush);
            //     // DeleteObject(hBrush);
            //     // InvalidateRect(mhWnd, NULL, FALSE);
            //     return 1;
            // }

            // static controls don't receive focus ...
            // case WM_SETFOCUS: {
            //     // std::cout << "xLabel::WM_SETFOCUS" << std::endl;
            //     this->setFocus(true);
            //     if (mOnFocus != nullptr) {
            //         mOnFocus(this, FocusEvent::GAINED);
            //     }
            //     break;
            // }

            // case WM_KILLFOCUS: {
            //     // std::cout << "xLabel::WM_KILLFOCUS" << std::endl;
            //     this->setFocus(false);
            //     // this->mOnFocusLost();
            //     if (mOnFocus != nullptr) {
            //         mOnFocus(this, FocusEvent::LOST);
            //     }
            //     break;
            // }

            // case WM_CTLCOLORSTATIC: {
            //     HBRUSH hBrush = CreateSolidBrush(getBackgroundColor());
            //     SetBkMode((HDC)wParam, TRANSPARENT);
	        //     SetTextColor((HDC)wParam, getTextColor());
            //     return (LRESULT) hBrush; // cache the handle as a member ...
            // }

            case WM_COMMAND: {

                switch(HIWORD(wParam)) {

                    // For static controls/labels, it may be the case
                    // that receiving focus, would be equivalent
                    // to detecting click/double click event ...

                    case STN_CLICKED: {
                        
                        if (mOnClick != nullptr) {
                            mOnClick(this);
                        }

                        // if (mOnFocus != nullptr) {
                        //     mOnFocus(this, FocusEvent::GAINED);
                        // }

                        break;
                    }

                    case STN_DBLCLK: {
                        
                        if (mOnDoubleClick != nullptr) {
                            mOnDoubleClick(this);
                        }

                        // HOW TO REMOVE FOCUS ONCE CLICK DIFFERENT CONTROL DETECTED!
                        // if (mOnFocus != nullptr) {
                        //     mOnFocus(this, FocusEvent::LOST);
                        // }

                        break;
                    }

                    // // MAY CAUSE UNDEFINED BEHAVIOUR if it works AT ALL!
                    // case WM_SETFOCUS: {
                    //     if (mOnFocus != nullptr) {
                    //         mOnFocus(this, FocusEvent::GAINED);
                    //     }
                    //     break;
                    // }
                    // case WM_KILLFOCUS: {
                    //     if (mOnFocus != nullptr) {
                    //         mOnFocus(this, FocusEvent::LOST);
                    //     }
                    //     break;
                    // }
                    // break;
                }
                
                break;
            }

            // ...
        }

        return DefWindowProc(mhWnd, msg, wParam, lParam);
    }

    /// @defgroup draw-routines Draw Routines
    /** @{ */
    /// @brief various draw routines for xLabel derive class
    ///        to custom draw the background & text color
    ///        as-well-as the border associated to the control
    void draw(xWidget* pWidget) override;
    void camouflage(xWidget* widget) override;
    void write(xWidget* widget) override;
    /** @} */
};

/// @brief   implementation of `xLabel::camouflage`
/// @details to hide the window rect corners that may "spill" over
void xLabel::camouflage(xWidget* widget) {
    
    // create a solid brush
    // from parent's background color
    // createBrush(this->Parent()->getBackgroundColor());

    HBRUSH hBrush = CreateSolidBrush(
        widget->Parent()->getBackgroundColor()
    );
    // paint the rectangular region to clear
    // any "spill" of rectangle corners
    FillRect(
        widget->getDrawContext(),
        &widget->getDrawRect(),
        hBrush
    );
    // DON'T free handle of brush resource here ...
    DeleteObject(hBrush); // This is done by destructor ...
}

/// @brief   implementation of `xLabel::write`
///          for writing text in the control
/// @details This method is to be invoked last
///          within the `xWidget`s `draw(...)`
///          method, to avoid the case that
///          the `camouflage(...)` method 
///          to potentially obscure the text
void xLabel::write(xWidget* widget) {
    // set the text color based on
    // the control's text color

    HDC hDC = widget->getDrawContext();

    SetTextColor(
        hDC,
        widget->getTextColor()
    );
    // set the background mode to TRANSPARENT
    SetBkMode(hDC, TRANSPARENT);
    // write the text in the control

    HFONT hFont = pFont->hFont;
    HGDIOBJ hOldFont = SelectObject(hDC, hFont);

    DrawText(
        hDC,
        #if defined(UNICODE) && defined(_UNICODE)
        StrConverter::StringToWString(widget->getText()).c_str(),
        #else
        widget->getText().c_str(),
        #endif
        -1,
        &widget->getDrawRect(),
        widget->getTextAlignment()
    );

    SelectObject(hDC, hOldFont);
}

/// @brief   implementation of `xLabel::draw(...)` routine
/// @details Encapsulates the various draw stages of the control ...
void xLabel::draw(xWidget* widget) {

    std::cout << (widget->getHoverState()) << std::endl;

    // invoke camouflage method ...
    widget->camouflage(widget);

    xBorder* pBorder = widget->getWidgetBorder();

    if (pBorder) {
        // invoke the border-drawing routine ...
        std::cout << "pBorder: " << *pBorder << std::endl;
        pBorder->draw(this);
    }

    xBorder* pFocusBorder = widget->getFocusBorder();
        if (pFocusBorder) {
        // invoke the focus-border drawing routine ...
        pFocusBorder->draw(this);
    }

    // only write the button's text once
    // the area fill has been applied
    // to avoid painting over the text
    widget->write(widget);
}

#endif // end of xTEXTBOX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
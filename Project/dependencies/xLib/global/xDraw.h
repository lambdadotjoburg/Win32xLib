/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xDraw.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  `xMDraw` contains the draw routines for `xWidget`s & their associated `xBorder`s
  * @details  This file is included after the declaration of all the required types during the compilation of `xLib`
  */

#pragma once

/// @brief begin of xDRAW_H implementation
#ifndef xDRAW_H
#define xDRAW_H

/// @todo change all parameters to reflect the appopriate type, <br/>
/// i.e. apply appropriate naming conventions throughout te project <br/>
/// <br/>
/// For example:  <br/>
/// 1) `xWidget*` types to be named `pWidget` <br/>
/// 2) `xBorder*` types to be named `pBorder` <br/>
/// 3) `xMenu*`   types to be named `pMenu`   <br/>
/// 4)  etc ...   <br/>

/// @todo
/// Double-check all the logic to ensure the Handles
/// & device contexts & all other Win32 RAW resources
/// are managed/freed correctly! <br/>
/// Investigate whether cod-redundancies can/should be reduced!

#include "./xClassicBorder.h"
#include "./xModernBorder.h"

/// @brief implementation of draw routine for the bevel border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xBevelBorder::draw(xWidget* pWidget) {
    
    if (!pWidget) {
        return;
    }

    xBorder* pBorder = pWidget->getWidgetBorder();
    
    if (!pBorder) {
        return;
    }

    DWORD type = pBorder->getBorderStyle();

    if (type == xBorder::ClassicStyle::RAISED) {
        drawRaisedBevelBorder(pWidget);
    } else if (type == xBorder::ClassicStyle::SUNKEN) {
        drawSunkenBevelBorder(pWidget);
    }
}

/// @brief implementation of draw routine for `RAISED` bevel border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xBevelBorder::drawRaisedBevelBorder(xWidget* pWidget) {

    bool hover = pWidget->getHoverState();

    HDC hDC = pWidget->getDrawContext();
    if (!hDC) {
        std::cout << "!hDC" << std::endl;
        return;
    }
    
    // paint the rectangular button region ...
    // create a solid brush using widget's background color
    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );
    // fill the rectangle with the client's background color
    FillRect(hDC, &pWidget->getDrawRect(), hBrush);

    // draw the buttons borders ...

    // select new pen based on `hWhitePen`
    // for drawing the left & top edges 
    // & store the old pen from the device context
    // to be restored for the Gdiplus resources ...
    HPEN hOldPen = (HPEN) SelectObject(hDC, this->hWhitePen);

    int i = this->getBorderInset();
    int w = pWidget->getWidth();
    int h = pWidget->getHeight();

    // Draw Top Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, w-i, 0+i);

    // Draw Left Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, 0+i, h-i);

    // select new pen based on `hColorpen`

    SelectObject(hDC, this->hColorPen);

    // Draw Bottom Edge ...
    MoveToEx(hDC, 0+i, h-i, NULL);
    LineTo(hDC, w-i, h-i);

    // Draw Right Edge ...
    MoveToEx(hDC, w-i, 0+i, NULL);
    LineTo(hDC, w-i, h-i);

    // restore the previous pen for the device context
    SelectObject(hDC, hOldPen);

    // free handle of brush resource
    DeleteObject(hBrush);

    // DON'T delete `hWhitePen` => shared static `HPEN` object
    // DON'T delete `hColorPen` => resource freed by `xBorder`s destructor
}

/// @brief implementation of draw routine for `SUNKEN` bevel border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xBevelBorder::drawSunkenBevelBorder(xWidget* pWidget) {

    bool hover = pWidget->getHoverState();

    HDC hDC = pWidget->getDrawContext();
    if (!hDC) {
        std::cout << "!hDC" << std::endl;
        return;
    }
    
    // paint the rectangular button region ...
    // create a solid brush using widget's background color
    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );
    // fill the rectangle with the client's background color
    FillRect(hDC, &pWidget->getDrawRect(), hBrush);

    // draw the buttons borders ...

    // select new pen based on `hWhitePen`
    // for drawing the left & top edges 
    // & store the old pen from the device context
    // to be restored for the Gdiplus resources ...
    HPEN hOldPen = (HPEN) SelectObject(hDC, this->hColorPen);

    int i = this->getBorderInset();
    int w = pWidget->getWidth();
    int h = pWidget->getHeight();

    // Draw Top Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, w-i, 0+i);

    // Draw Left Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, 0+i, h-i);

    // select new pen based on `hColorpen`

    SelectObject(hDC, this->hWhitePen);

    /// @note
    /// subtracting 1 from bottom & right
    /// If the border is drawn on the bottom or right
    /// of the control's context rectangle,'
    /// the line will not be visible ...

    // Draw Bottom Edge ...
    MoveToEx(hDC, 0+i, h-i, NULL);
    LineTo(hDC, w-i, h-i);

    // Draw Right Edge ...
    MoveToEx(hDC, w-i, 0+i, NULL);
    LineTo(hDC, w-i, h-i);

    // restore the previous pen for the device context
    SelectObject(hDC, hOldPen);

    // free handle of brush resource
    DeleteObject(hBrush);
    // DeleteObject(hOldPen);

    // DON'T delete `hWhitePen` => shared static `HPEN` object
    // DON'T delete `hColorPen` => resource freed by `xBorder`s destructor
}

/// @brief implementation of draw routine for the etched border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xEtchedBorder::draw(xWidget* pWidget) {

    if (!pWidget) {
        return;
    }

    xBorder* pBorder = pWidget->getWidgetBorder();
    
    if (!pBorder) {
        return;
    }

    DWORD type = pBorder->getBorderStyle();
    
    if (type == xBorder::ClassicStyle::RAISED) {
        drawRaisedEtchedBorder(pWidget);
    } else if (type == xBorder::ClassicStyle::SUNKEN) {
        drawSunkenEtchedBorder(pWidget);
    }
}

/// @brief implementation of draw routine for `RAISED` etched border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xEtchedBorder::drawRaisedEtchedBorder(xWidget* pWidget) {

    bool hover = pWidget->getHoverState();

    HDC hDC = pWidget->getDrawContext();
    if (!hDC) {
        std::cout << "!hDC" << std::endl;
        return;
    }
    
    // paint the rectangular button region ...
    // create a solid brush using widget's background color
    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );
    // fill the rectangle with the client's background color
    FillRect(hDC, &pWidget->getDrawRect(), hBrush);

    // draw the buttons borders ...
    // draw outer/darker edge

    // select new pen based on `hWhitePen`
    // for drawing the left & top edges 
    // & store the old pen from the device context
    // to be restored for the Gdiplus resources ...
    HPEN hOldPen = (HPEN) SelectObject(hDC, this->hWhitePen);

    int i = this->getBorderInset();
    int w = pWidget->getWidth();
    int h = pWidget->getHeight();

    // Draw Top Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, w-1-i, 0+i);

    // Draw Left Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, 0+i, h-1-i);

    // select new pen based on `hColorpen`
    SelectObject(hDC, this->hColorPen);

    // Draw Bottom Edge ...
    MoveToEx(hDC, 0+i, h-1-i, NULL);
    LineTo(hDC, w-1-i, h-1-i);

    // Draw Right Edge ...
    MoveToEx(hDC, w-1-i, 0+i, NULL);
    LineTo(hDC, w-1-i, h-1-i);

    // draw inner/lighter edge

    int t = this->radius;

    // Draw Top Edge ...
    MoveToEx(hDC, 0+t+i, 0+t+i, NULL);
    LineTo(hDC, w-1-t-i, 0+t+i);

    // Draw Left Edge ...
    MoveToEx(hDC, 0+t+i, 0+t+i, NULL);
    LineTo(hDC, 0+t+i, h-1-t-i);

    SelectObject(hDC, this->hWhitePen);

    // Draw Bottom Edge ...
    MoveToEx(hDC, 0+t+i, h-1-t-i, NULL);
    LineTo(hDC, w-1-t-i, h-1-t-i);

    // Draw Right Edge ...
    MoveToEx(hDC, w-1-t-i, 0+t+i, NULL);
    LineTo(hDC, w-1-t-i, h-1-t-i);

    // restore the previous pen for the device context
    SelectObject(hDC, hOldPen);

    // free handle of brush resource
    DeleteObject(hBrush);
    // DeleteObject(hOldPen);

    // DON'T delete `hWhitePen` => shared static `HPEN` object
    // DON'T delete `hColorPen` => resource freed by `xBorder`s destructor    
}

/// @brief implementation of draw routine for `SUNKEN` etched border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xEtchedBorder::drawSunkenEtchedBorder(xWidget* pWidget) {
    
    bool hover = pWidget->getHoverState();

    HDC hDC = pWidget->getDrawContext();
    if (!hDC) {
        std::cout << "!hDC" << std::endl;
        return;
    }
    
    // paint the rectangular button region ...
    // create a solid brush using widget's background color
    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );
    // fill the rectangle with the client's background color
    FillRect(hDC, &pWidget->getDrawRect(), hBrush);

    // draw the buttons borders ...
    // draw outer/darker edge

    // select new pen based on `hWhitePen`
    // for drawing the left & top edges 
    // & store the old pen from the device context
    // to be restored for the Gdiplus resources ...
    HPEN hOldPen = (HPEN) SelectObject(hDC, this->hColorPen);

    int i = this->getBorderInset();
    int w = pWidget->getWidth();
    int h = pWidget->getHeight();

    // Draw Top Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, w-1-i, 0+i);

    // Draw Left Edge ...
    MoveToEx(hDC, 0+i, 0+i, NULL);
    LineTo(hDC, 0+i, h-1-i);

    // select new pen based on `hColorpen`
    SelectObject(hDC, this->hWhitePen);

    // Draw Bottom Edge ...
    MoveToEx(hDC, 0+i, h-1-i, NULL);
    LineTo(hDC, w-1-i, h-1-i);

    // Draw Right Edge ...
    MoveToEx(hDC, w-1-i, 0+i, NULL);
    LineTo(hDC, w-1-i, h-1-i);

    // draw inner/lighter edge

    /// repurpose radius for etched border thickness,
    /// i.e. separation between light & dark border
    int t = this->radius;

    // Draw Top Edge ...
    MoveToEx(hDC, 0+t+i, 0+t+i, NULL);
    LineTo(hDC, w-1-t-i, 0+t+i);

    // Draw Left Edge ...
    MoveToEx(hDC, 0+t+i, 0+t+i, NULL);
    LineTo(hDC, 0+t+i, h-1-t-i);

    SelectObject(hDC, this->hColorPen);

    // Draw Bottom Edge ...
    MoveToEx(hDC, 0+t+i, h-1-t-i, NULL);
    LineTo(hDC, w-1-t-i, h-1-t-i);

    // Draw Right Edge ...
    MoveToEx(hDC, w-1-t-i, 0+t+i, NULL);
    LineTo(hDC, w-1-t-i, h-1-t-i);

    // restore the previous pen for the device context
    // SelectObject(widget->getDrawContext(), hOldPen);
    SelectObject(hDC, hOldPen);

    // free handle of brush resource
    DeleteObject(hBrush);
    // DeleteObject(hOldPen);

    // DON'T delete `hWhitePen` => shared static `HPEN` object
    // DON'T delete `hColorPen` => resource freed by `xBorder`s destructor
}

/// @brief implementation of draw routine for the line border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xLineBorder::draw(xWidget* pWidget) {

    if (!pWidget) {
        return;
    }

    xLineBorder* pBorder = static_cast<xLineBorder*>(pWidget->getWidgetBorder());
    
    if (!pBorder) {
        return;
    }

    if (pBorder->isFocusBorder) {
        drawFocusBorder(pWidget);
    } else {
        drawBorderOutline(pWidget);
    }
}

/// @brief implementation of draw routine for a focus border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xLineBorder::drawFocusBorder(xWidget* pWidget) {
    // std::cout << "draw focus border" << std::endl;

    bool hover = pWidget->getHoverState();
    
    HDC hDC = pWidget->getDrawContext();
    
    // paint the rectangular button region ...
    // create a solid brush using widget's background color
    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );
    
    // select new pen & store original Gdiplus resource
    HPEN hOldPen = (HPEN) SelectObject(hDC, this->hColorPen);

    // select new brush & store original Gdiplus resource
    HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);
    
    int inset = this->getBorderInset();
    RECT rc = pWidget->getDrawRect();

    RoundRect(
        hDC,
        rc.left   + inset,
        rc.top    + inset,
        rc.right  - inset-1,
        rc.bottom - inset-1,
        this->getBorderRadius(),
        this->getBorderRadius()
    );

    // restore Gdiplus original/old HPEN instance
    SelectObject(hDC, hOldPen);

    // restore Gdiplus original/old HBRUSH instance
    SelectObject(hDC, hOldBrush);

    // safely delete old instances ...
    // DeleteObject(hPen);
    DeleteObject(hBrush);
    // DeleteObject(hOldPen); // don't delete ...
    // DeleteObject(hOldBrush);
}

/// @brief implementation of draw routine for a line border outline associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xLineBorder::drawBorderOutline(xWidget* pWidget) {

    bool hover = pWidget->getHoverState();

    HDC hDC = pWidget->getDrawContext();
    
    // paint the rectangular button region ...
    // create a solid brush using widget's background color
    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );

    if (!hBrush) { return; }

    // The color pen already exists
    // as an `xBorder` member ...
    // HPEN hPen = CreatePen(
    //   this->getBorderStyle(),
    //   this->getBorderWidth(),
    //   this->getBorderColor()
    // );
    // SetDCBrushColor(
    //     widget->getDrawContext(),
    //     widget->getBackgroundColor()
    // );
    // // select a different pen into the device context
    // SetDCPenColor(
    //     widget->getDrawContext(),
    //     this->getBorderColor()
    // );

    // select new brush & store original Gdiplus resource
    HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);

    // select new pen & store original Gdiplus resource
    HPEN hOldPen = (HPEN) SelectObject(hDC, this->hColorPen);
    

    int inset = this->getBorderInset();
    RECT rc = pWidget->getDrawRect();

    RoundRect(
        hDC,
        rc.left   + inset,
        rc.top    + inset,
        rc.right  - inset,
        rc.bottom - inset,
        this->getBorderRadius(),
        this->getBorderRadius()
    );

    // restore Gdiplus original/old HPEN instance
    SelectObject(hDC, hOldPen);

    // restore Gdiplus original/old HBRUSH instance
    SelectObject(hDC, hOldBrush);

    // safely delete old instances ...
    // DeleteObject(hPen);
    DeleteObject(hBrush);
    // DeleteObject(hOldPen);
    // DeleteObject(hOldBrush);
}

/// @brief implementation of draw routine for the empty border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xEmptyBorder::draw(xWidget* pWidget) {

    if (!pWidget) {
        return;
    }

    bool hover = pWidget->getHoverState();

    HRGN clipRegion = CreateRoundRectRgn(
        0,0,
        pWidget->getWidth(),
        pWidget->getHeight(),
        this->radius,
        this->radius
    );

    if (!clipRegion) { return; }

    HDC hdc = pWidget->getDrawContext();
    int saveDC = SaveDC(hdc); // save current DC state ...

    HBRUSH hBrush = CreateSolidBrush(
        hover ? pWidget->getHoverColor() : pWidget->getBackgroundColor()
    );

    if (hBrush) {

        /*HBRUSH hOldBrush = (HBRUSH) */ SelectObject(hdc, hBrush);

        FillRgn(hdc, clipRegion, hBrush);
        DeleteObject(hBrush);
    }

    SelectClipRgn(pWidget->getDrawContext(), clipRegion);
    RestoreDC(hdc, saveDC);

    DeleteObject(clipRegion);
}

/// @brief implementation of draw routine for a title border associated to a widget
/// @param pWidget ~ pointer of the widget to draw
void xTitleBorder::draw(xWidget* pWidget) {
    IMPLICIT(pWidget);
}

/// @brief draw routine for the `xImage` object associated to an `xWidget` control
/// @param pWidget ~ pointer of the `xWidget` for which to draw the `xImage`
/// @todo  define a mechanism to animate gif images! <br/>
///        May have to override `xImageButton::CustomDraw(...)` to achieve this ...
/// @todo  define a mechanism to shrink the image slightly
///        when clicking on an image button for visual effect <br/>
///        May have to override `xImageButton::CustomDraw(...)` to achieve this ...
void xImage::drawImage(xWidget* pWidget) {

    // xButton* pButton = static_cast<xButton*>(pWidget);

    // std::cout << "pButton width: " << pButton->getWidth() << std::endl;
    // std::cout << "pButton height: " << pButton->getHeight() << std::endl;

    Gdiplus::Graphics graphics(pWidget->getDrawContext());

    // account for the borders width & inset, etc ...
    xBorder* pBorder = pWidget->getWidgetBorder();

    // default 0 if no border, i.e. if `pBorder == nullptr` ...
    int w = 0;
    int i = 0;
    int r = 0;
    // depending on the type of border,
    // may have to subtract 2(w+t)
    // t ~ r is the thickness (radius)
    // for etched borders
    int t = 0;

    // if the widget has a border then subtract
    // these params from the image dimensions
    // to fit inside the borders ...
    if (pBorder) {
        
        // update the params
        i = pBorder->getBorderInset();
        w = pBorder->getBorderWidth();
        r = pBorder->getBorderRadius();
    }

    RECT rc = pWidget->getDrawRect();

    int left   = rc.left;
    int top    = rc.top;
    int right  = rc.right;
    int bottom = rc.bottom;

    // std::cout << "left: " << left << std::endl;
    // std::cout << "top: " << top << std::endl;
    // std::cout << "right: " << right << std::endl;
    // std::cout << "bottom: " << bottom << std::endl;

    // int left = pWidget->getHorizontalPosition();
    // int top = pWidget->getVerticalPosition();
    // int right = left + pWidget->getWidth();
    // int bottom = top + pWidget->getHeight();

    std::string borderType = pBorder->getBorderType();
    // std::cout << "borderType = " << borderType << std::endl;
    
    if (borderType == "xEtchedBorder") {
        // do etched-border-related work ... 
        // if etched, use the radius of the thickeness
        // & add the width of the inner border
        t = pBorder->getBorderRadius();
        r = 0; // reset r to 0 since no actual border radius for etched borders ...
    } else if (borderType == "xBevelBorder") {
        // i = 0;
        right  += 1;
        bottom += 1;
    }

    left   +=  (i+w+t);
    top    +=  (i+w+t);
    right  += -(i+w+t);
    bottom += -(i+w+t);

    // std::cout << "left: " << left << std::endl;
    // std::cout << "top: " << top << std::endl;
    // std::cout << "right: " << right << std::endl;
    // std::cout << "bottom: " << bottom << std::endl;

    bool clickState = pWidget->getClickState();
    int shrink = this->shrink;

    if (clickState) {
        left   += shrink;
        right  -= shrink;
        top    += shrink;
        bottom -= shrink;
    }
    
    // left = left+(i+w+t);
    // top = top+(i+w+t);

    // right = right-(i+w+t);
    // bottom = bottom-(i+w+t);

    int width  = (right - left);
    int height = (bottom - top);

    // std::cout << "width: " << width << std::endl;
    // std::cout << "height: " << height << std::endl;

    // To clip the image according to
    // the round button border corner edges
    // we need to first create a region
    HRGN hRgn = CreateRoundRectRgn(
        left,
        top,
        right,
        bottom,
        r, r
    );

    if (!hRgn) { return; }
    // (i+w+t)

    // create a region to clip the draw rectangle
    Gdiplus::Region gdiPlusRegion(hRgn);

    // clip the draw rectangle according to the region
    graphics.SetClip(&gdiPlusRegion);

    // width += i+w+t;
    // height += i+w+t;

    // create the Gdiplus rectangle (clipped)
    Gdiplus::Rect gdiRect(left, top, width, height);

    // draw the image in the clipped rectangle
    graphics.DrawImage(this->img, gdiRect);

    // reset the clip region
    graphics.ResetClip();

    // free the region handle
    DeleteObject(hRgn);
}

/// @brief helper method for toggling `RAISED`/`SUNKEN` state of default classic-style borders
/// @param pWidget ~ pointer of the widget to draw
void xClassicBorder::toggleClassicBorderStyle(xWidget* pWidget) {
    
    xBorder* pBorder = pWidget->getWidgetBorder();

    // toggle the initial style of the border ...
    if (pBorder->getBorderStyle() == xBorder::ClassicStyle::SUNKEN) {
        pBorder->setBorderStyle(xBorder::ClassicStyle::RAISED);
    } else if (pBorder->getBorderStyle() == xBorder::ClassicStyle::RAISED) {
        pBorder->setBorderStyle(xBorder::ClassicStyle::SUNKEN);
    }
}

/// @brief implementation of default mouse click handler
/// @param pWidget ~ pointer to any widget, usually (but not necessarily) the widget to which the event is attached
/// @param key ~ mouse key for client-code use to verify whether the correct mouse key was pressed & handle/distinguish cases
/// @note  Attaching client-defined events will lose all default implementation associated to this event
void xButton::defaultOnMouseClickHandler(xWidget* pWidget, MouseKey key) {

    // This method takes the key param to enable client-code to check
    // whether or not the correct/desired key was pressed & handle
    // the case in which an RightButton constant was detected instead ...
    if (key != MouseKey::LeftButton) {
        return;
    }

    if (!pWidget) {
        return;
    }

    // toggle click state ...
    pWidget->setClickState(true);

    // associate a default border to a button ...
    xBorder* pBorder = pWidget->getWidgetBorder();

    if (!pBorder) {
        return;
    }

    xClassicBorder* pClassicBorder = static_cast<xClassicBorder*>(pBorder);
    pClassicBorder->toggleClassicBorderStyle(pWidget);
}

/// @brief implementation of default mouse release handler
/// @param pWidget ~ pointer to any widget, usually (but not necessarily) the widget to which the event is attached
/// @param key ~ mouse key for client-code use to verify whether the correct mouse key was pressed & handle/distinguish cases
/// @note  Attaching client-defined events will lose all default implementation associated to this event
void xButton::defaultOnMouseReleaseHandler(xWidget* pWidget, MouseKey key) {

    // This method takes the key param to enable client-code to check
    // whether or not the correct/desired key was pressed & handle
    // the case in which an RightButton constant was detected instead ...
    if (key != MouseKey::LeftButton) {
        return;
    }

    if (!pWidget) {
        return;
    }

    // toggle click state ...
    pWidget->setClickState(false);

    // associate a default border to a button ...
    xBorder* pBorder = pWidget->getWidgetBorder();

    if (!pBorder) {
        return;
    }
    
    xClassicBorder* pClassicBorder = static_cast<xClassicBorder*>(pBorder);
    pClassicBorder->toggleClassicBorderStyle(pWidget);
}

/// @brief implementation of default focus handler
/// @param pWidget ~ pointer to any widget, usually (but not necessarily) the widget to which the event is attached
/// @param event ~ focus event for client-code use to verify whether the correct event (`GAINED`/`LOST`) was passed & handle/distinguish cases
/// @note  No default implementation provided
void xButton::defaultOnFocusHandler(xWidget* pWidget, FocusEvent event) {

    if (!pWidget) {
        return;
    }
    
    if (event == FocusEvent::GAINED) {
        // do something ...
    } else if (event == FocusEvent::LOST) {
        // do something else ...
    }
}

/// @brief   method to demonstrate default on-mouse-hover effect => do nothing
/// @param   pWidget ~ pointer to a widget object to manipulate
/// @param   event ~ MouseEvent enum constant => `ENTER`/`EXIT`
/// @details This method does nothing! Instead, the default hover background color
/// is defined in the xButton's `HandleMessage(...)` `WM_MOUSEMOVE` & `WM_MOUSELEAVE`
/// switch-cases to allow client code more flexibility, for doing additional work with
/// the mOnMouseHover function/event handler, i.e. not interfering with coloring/drawing!
void xButton::defaultOnMouseHoverHandler(xWidget* pWidget, MouseEvent event) {

    // only show this in debug mode ...
    #ifndef NDEBUG

        IMPLICIT(pWidget);
    
        if (event == MouseEvent::ENTER) {// check for mouse-enter
            // do something ...
        } else if (event == MouseEvent::EXIT) { // check for mouse-leave/exit
            // do something else ...
        }
        
    #endif
}

#endif // end of xDRAW_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
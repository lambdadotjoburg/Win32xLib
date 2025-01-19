/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xModernBorder.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  `xModernBorder` declaration & implementation header file
  * @details   xModernBorder.h defines the `xModernBorder` & its derived classes
  */

#pragma once

/// @brief begin of xMODERNBORDER_H implementation
#ifndef xMODERNBORDER_H
#define xMODERNBORDER_H

#include "./xBorder.h"

/**
 * @class    xModernBorder
 * @brief   `xModernBorder` is a derived-type of `xBorder`
 * @details `xModernBorder`s resemble that of "modern" windows (post-vista/NT)
 */
class xModernBorder : public xBorder {

    /*%*%*%*%*%*%*%*%*%*/
    /* Modern Borders */
    /*%*%*%*%*%*%*%*%*%*/

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

protected:

    /// @brief   protected constructor to instantiate a new `xModernBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xModernBorder(
            BYTE R, BYTE G, BYTE B, int w, int r,
            xBorder::StrokeStyle style, int i = 0
        )
        : xBorder(R, G, B, w, r, style, i) {
        // ...
    }

    /// @brief   overloaded protected constructor to instantiate a new `xModernBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xModernBorder(
            COLORREF colorRef, int w, int r,
            xBorder::StrokeStyle style, int i = 0
        ) : xBorder(colorRef, w, r, style, i) {
        // ...
    }

    /// @brief     override method to construct a color pen using client-provided style
    /// @param[in] b ~ blend-ratio to apply on the pen (not used)
    virtual void createColorPen(float b) override {

        setColorBlend(b);
        
        // keep a copy of the old pen to delete ...
        HPEN hOldPen = hColorPen;

        LOGBRUSH brushStyle;
        brushStyle.lbStyle = BS_SOLID;
        brushStyle.lbColor = this->getBorderColor(); // use client-provided color
        brushStyle.lbHatch = 0;

        /// create pen object for use with classic-style borders
        hColorPen = ExtCreatePen(
            PS_GEOMETRIC | this->getBorderStyle(),  // use client-provided style
            this->getBorderWidth(),                 // use client-provided border width
            &brushStyle,                            // ref to `LOGBRUSH`
            0, NULL
        );

        // delete the old pen ...
        if (hOldPen) {
            DeleteObject(hOldPen); // free resources ...
        }
    }

    /// @brief     override method to construct a white pen using client-provided style
    /// @param[in] b ~ blend-ratio to apply on the pen
    virtual void createWhitePen(float b) override {

        setColorBlend(b);

        // keep a copy of the old pen to delete ...
        HPEN hOldPen = hWhitePen;

        LOGBRUSH brushStyle;
        brushStyle.lbStyle = BS_SOLID;
        brushStyle.lbColor = RGB(255, 255, 255); // white => 3D border effect
        brushStyle.lbHatch = 0;

        /// create pen object for use with classic-style borders
        hWhitePen = ExtCreatePen(
            PS_GEOMETRIC | this->getBorderStyle(), // classic pens always use `PS_SOLID`
            this->getBorderWidth(),                // use client-provided border width
            &brushStyle,                           // ref to `LOGBRUSH`
            0, NULL
        );

        // delete the old pen ...
        if (hOldPen) {
            DeleteObject(hOldPen); // free resources ...
        }
    }

private:

    /// @brief     private method to set the border's style
    /// @param[in] s ~ style to apply to the border
    void setBorderStyle(DWORD s) override {
        style = s;
    }

public:

    /// @brief     public interface method to set the border's style
    /// @param[in] s ~ style to apply to the border
    void setBorderStyle(StrokeStyle style) {
        setBorderStyle((DWORD) style);
    }

public:

    /// @brief  method to retrieve the border's style
    /// @return `std::string` representative of the border's style
    virtual std::string getBorderType() = 0;

    /// @brief pure virtual method to enforce drawing routine in derived classes
    /// @param widget ~ pointer of the widget for which to draw this border
    virtual void draw(xWidget* widget) override = 0;
};

class xLineBorder : public xModernBorder {

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

    boolean isFocusBorder = false;

protected:

    /// @brief   protected constructor to instantiate a new `xLineBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xLineBorder(
            BYTE R, BYTE G, BYTE B, int w, int r,
            xBorder::StrokeStyle style,
            int inset = 0, boolean focus = 0
        ) : xModernBorder(R, G, B, w, r, style, inset) {
        // ...
        isFocusBorder = focus;
        createColorPen(0.0);
        createWhitePen(0.0);
    }

    /// @brief   overloaded protected constructor to instantiate a new `xBevelBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xLineBorder(
            COLORREF colorRef, int w, int r,
            xBorder::StrokeStyle style,
            int inset = 0, boolean focus = 0
        ) : xModernBorder(colorRef, w, r, style, inset) {
        // ...
        isFocusBorder = focus;
        createColorPen(0.0);
        createWhitePen(0.0);
    }

public:

    /// @brief  override method to retrieve the type of border
    /// @return `std::string` representative of this object type
    virtual std::string getBorderType() {
        return type(*this);
    }

    /// @brief     override method to draw `xLineBorder` objects
    /// @param[in] pWidget ~ pointer of the widget for which to draw the border
    void draw(xWidget* widget) override;

    /// @brief     method to draw the line border's outline
    /// @param[in] pWidget ~ pointer of the widget for which to draw border
    void drawBorderOutline(xWidget* widget);

    /// @brief     method to draw a focus border's
    /// @param[in] pWidget ~ pointer of the widget for which to draw focus border
    void drawFocusBorder(xWidget* widget);
};

/// @todo implement xTitleBorder class
class xTitleBorder : public xModernBorder {

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

public:

    /// @brief   protected constructor to instantiate a new `xTitleBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xTitleBorder(
            BYTE R, BYTE G, BYTE B, int w, int r,
            xBorder::StrokeStyle style, int inset = 0
        ) : xModernBorder(R, G, B, w, r, style, inset) {
        // ...
    }

    /// @brief   overloaded protected constructor to instantiate a new `xTitleBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xTitleBorder(
            COLORREF colorRef, int w, int r,
            xBorder::StrokeStyle style, int inset = 0
        ) : xModernBorder(colorRef, w, r, style, inset) {
        // ...
    }

public:

    /// @brief  override method to retrieve the type of border
    /// @return `std::string` representative of this object type
    virtual std::string getBorderType() {
        return type(*this);
    }

    /// @brief     override method to draw the title border
    /// @param[in] pWidget ~ pointer of the widget for which to draw the title border
    void draw(xWidget* widget) override;
};

/// @todo implement xEmptyBorder class
class xEmptyBorder : public xModernBorder {

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

public:

    /// @brief   overloaded protected constructor to instantiate a new `xTitleBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xEmptyBorder(int radius = 0, int inset = 0)
        : xModernBorder(
            RGB(255, 255, 255), 0, radius,
            xBorder::StrokeStyle::SOLID, inset
        ) {
        // ...
    }

public:

    /// @brief  override method to retrieve the type of border
    /// @return `std::string` representative of this object type
    virtual std::string getBorderType() {
        return type(*this);
    }

    /// @brief     override method to draw the title border
    /// @param[in] pWidget ~ pointer of the widget for which to draw the title border
    void draw(xWidget* widget) override;
};

#endif // end of xBEVELBORDER_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	   xClassicBorder.h
  * @author    &lambda;ambda
  * @date      \showdate "%Y-%m-%d"
  * @brief 	  `xClassicBorder` declaration & implementation header file
  * @details   xClassicBorder.h defines the `xClassicBorder` & its derived classes
  */

#pragma once

/// @brief begin of xCLASSICBORDER_H implementation
#ifndef xCLASSICBORDER_H
#define xCLASSICBORDER_H

#include "./xBorder.h"

/**
 * @class    xClassicBorder
 * @brief   `xClassicBorder` is a derived-type of `xBorder`
 * @details `xClassicBorder`s resemble that of earlier windows (pre-vista/NT)
 */
class xClassicBorder : public xBorder {

    /*%*%*%*%*%*%*%*%*%*/
    /* Classic Borders */
    /*%*%*%*%*%*%*%*%*%*/

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

protected:

    /// @brief   protected constructor to instantiate a new `xClassicBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xClassicBorder(
            BYTE R, BYTE G, BYTE B, int w, int t,
            xBorder::ClassicStyle s, int i = 0
        ) : xBorder(R, G, B, w, t, s, i) {
        // ...
    }

    /// @brief   overloaded protected constructor to instantiate a new `xClassicBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    xClassicBorder(
            COLORREF colorRef, int w, int t,
            xBorder::ClassicStyle style, int i = 0
        ) : xBorder(colorRef, w, t, style, i) {
        // ...
    }

    /// @brief     override method to construct a solid color pen
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
            PS_GEOMETRIC | PS_SOLID,        // classic pens always use `PS_SOLID`
            this->getBorderWidth(),         // use client-provided border width
            &brushStyle,                    // ref to `LOGBRUSH`
            0, NULL
        );

        // delete the old pen ...
        if (hOldPen) {
            DeleteObject(hOldPen); // free resources ...
        }
    }

    /// @brief     override method to construct a solid white pen
    /// @param[in] b ~ blend-ratio to apply on the pen
    virtual void createWhitePen(float b) override {

        setColorBlend(b);

        // extract R, G, B values from `COLORREF`s `color`
        BYTE red = GetRValue(color);
        BYTE green = GetGValue(color);
        BYTE blue = GetBValue(color);

        // 255, 255, 255 => white
        // if blend = 0 for all X, then X + 255 - X  = 255 => 255
        // if blend = 1 for all X, then X + (255-X)*(1-1) => X

        BYTE blendR = static_cast<BYTE>(red+(255-red)*(1-blend));
        BYTE blendG = static_cast<BYTE>(red+(255-green)*(1-blend));
        BYTE blendB = static_cast<BYTE>(red+(255-blue)*(1-blend));

        // keep a copy of the old pen to delete ...
        HPEN hOldPen = hWhitePen;

        LOGBRUSH brushStyle;
        brushStyle.lbStyle = BS_SOLID;

        // brushStyle.lbColor = RGB(255, 255, 255); // use client-provided color
        brushStyle.lbColor = RGB(blendR, blendG, blendB);

        brushStyle.lbHatch = 0;

        /// create pen object for use with classic-style borders
        hWhitePen = ExtCreatePen(
            PS_GEOMETRIC | PS_SOLID,        // classic pens always use `PS_SOLID`
            this->getBorderWidth(),         // use client-provided border width
            &brushStyle,                    // ref to `LOGBRUSH`
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
    void setBorderStyle(ClassicStyle s) {
        setBorderStyle((DWORD) s);
    }

public:

    /// @brief  method to retrieve the border's style
    /// @return `std::string` representative of the border's style
    virtual std::string getBorderType() = 0;

    /// @brief pure virtual method to enforce drawing routine in derived classes
    /// @param widget ~ pointer of the widget for which to draw this border
    void draw(xWidget* widget) override = 0;

    /// @brief method to toggle the border to be `RAISED`/`SUNKEN`
    ///        which is only applicable to default classic borders
    /// @param pWidget ~ pointer of the widget for which to toggle the border style
    void toggleClassicBorderStyle(xWidget* pWidget);
};

/**
 * @class    xBevelBorder
 * @brief   `xBevelBorder` is a derived-type of `xClassicBorder`
 * @details `xBevelBorder`s resemble that of pop-up/3D-style windows (pre-vista/NT)
 */
class xBevelBorder : public xClassicBorder {

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

protected:
    
    /// @brief   protected constructor to instantiate a new `xBevelBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    /// @note    `t` ("thickness") for bevel border is meaningless
    xBevelBorder(
            BYTE R, BYTE G, BYTE B, int w, int t,
            xBorder::ClassicStyle style, int inset = 0
        ) : xClassicBorder(R, G, B, w, t, style, inset) {
        // ...
        createColorPen(0.4);
        createWhitePen(0.4);
    }

    /// @brief   overloaded protected constructor to instantiate a new `xBevelBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    /// @note    `t` ("thickness") for bevel border is meaningless
    xBevelBorder(
            COLORREF colorRef, int w, int t,
            xBorder::ClassicStyle style, int inset = 0
        ) : xClassicBorder(colorRef, w, t, style, inset) {
        // ...
        createColorPen(0.4);
        createWhitePen(0.4);
    }

public:

    /// @brief  override method to retrieve the type of border
    /// @return `std::string` representative of this object type
    virtual std::string getBorderType() {
        return type(*this);
    }

    /// @brief     override method to draw `xBevelBorder` objects
    /// @param[in] pWidget ~ pointer of the widget for which to draw the border
    void draw(xWidget* pWidget) override;

    /// @brief     method to draw a `RAISED` bevel border
    /// @param[in] pWidget ~ pointer of the widget for which to draw the `RAISED` bevel border
    void drawRaisedBevelBorder(xWidget* pWidget);

    /// @brief     method to draw a `SUNKEN` bevel border
    /// @param[in] pWidget ~ pointer of the widget for which to draw the `SUNKEN` bevel border
    void drawSunkenBevelBorder(xWidget* pWidget);
};

/**
 * @class    xEtchedBorder
 * @brief   `xEtchedBorder` is a derived-type of `xClassicBorder`
 * @details `xEtchedBorder`s resemble that of pop-up/3D-style windows (pre-vista/NT)
 */
class xEtchedBorder : public xClassicBorder {

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

protected:

    /// @brief   protected constructor to instantiate a new `xEtchedBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    /// @note    `t` ("thickness") is the distance between the light & color/dark edges
    xEtchedBorder(
            BYTE R, BYTE G, BYTE B, int w, int t,
            xBorder::ClassicStyle style, int inset = 0
        ) : xClassicBorder(R, G, B, w, t, style, inset) {
        // ...
        createColorPen(0.3);
        createWhitePen(0.3);
    }

    /// @brief   overloaded protected constructor to instantiate a new `xEtchedBorder` object via `xBorderFactory`
    /// @details parameters are defined within the `xBorderFactory` interface methods
    /// @note    `t` ("thickness") is the distance between the light & color/dark edges
    xEtchedBorder(
            COLORREF colorRef, int w, int t,
            xBorder::ClassicStyle style, int inset = 0
        ) : xClassicBorder(colorRef, w, t, style, inset) {
        // ...
        createColorPen(0.3);
        createWhitePen(0.3);
    }

    /// @brief  override method to retrieve the type of border
    /// @return `std::string` representative of this object type
    virtual std::string getBorderType() {
        return type(*this);
    }
    
    /// @brief     override method to draw `xEtchedBorder` objects
    /// @param[in] pWidget ~ pointer of the widget for which to draw the border
    void draw(xWidget* widget) override;

    /// @brief     method to draw a `RAISED` etched border
    /// @param[in] pWidget ~ pointer of the widget for which to draw the `RAISED` etched border
    void drawRaisedEtchedBorder(xWidget* pWidget);
    
    /// @brief     method to draw a `SUNKEN` etched border
    /// @param[in] pWidget ~ pointer of the widget for which to draw the `SUNKEN` etched border
    void drawSunkenEtchedBorder(xWidget* pWidget);
};

#endif // end of xCLASSICBORDER_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
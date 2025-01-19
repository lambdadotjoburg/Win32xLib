/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	   xBorder.h
  * @author    &lambda;ambda
  * @date      \showdate "%Y-%m-%d"
  * @brief 	  `xBorder` declaration & implementation header file
  * @details   xBorder.h defineds the `xBorder` class to be used for
  *            creating new `xBorder` objects & interfaces with `xWidgetManager`
  */

#pragma once

/// @brief begin of xBORDER_H implementation
#ifndef xBORDER_H
#define xBORDER_H

// forward declaration
class xWidget;

/**
 * @class    xBorder
 * @brief   `xBorder` interfaces with `xBorderFactory`
 *           to allow client-code for creating new `xBorder` objects
 * @details `xBorder` does NOT provide public interface for creating
 *          `xBorder` objects but does provide public interface for
 *           altering/manipulating `xBorder`s object's properties/state
 *           once registered/created/manufactured by `xBorderFactory`
 * @todo     define mechanism for border shadow
 */
class xBorder {

    
    /*%*%*%*%*%*%*%*%*%*%*/
    /* Border Base Class */
    /*%*%*%*%*%*%*%*%*%*%*/

    /// @details
    // `xWidgetManager` declared as a `friend` class to allow access
    /// to `xBorder`s constructors & destructor `~xBorder()`
    friend class xBorderFactory;

private:

    /// @brief ref count => to keep track of this
    /// `xBorder` instance usage in `xBorderFactory` ...
    int count = 0;

public:

    /// @brief  method to return the count of an `xBorder` object
    /// @return integer representative of the `xBorder`s count
    int getCount() {
        return count;
    }

public:

    /// @brief enum constants for classic-style borders
    enum ClassicStyle {
        RAISED,
        SUNKEN,
    };
    
    /// @brief enum constants for modern-style borders
    enum StrokeStyle {
        NONE = PS_NULL,
        SOLID = PS_SOLID,
        DOT = PS_DOT,
        DASH = PS_DASH,
        DASHDOT = PS_DASHDOT,
        DASHDOTDOT = PS_DASHDOTDOT,
        INSIDE = PS_INSIDEFRAME,               
    };

protected:

    /// @brief color value of the border
    COLORREF color;
    /// @brief border's stroke style
    DWORD style;
    /// @brief stroke width of the border
    int width;
    /// @brief distance from the widget's edges (rectangle)
    int inset;
    /// @brief border radius, not applicable for classic-style bevel borders
    /// @details for classic-style etched borders, radius plays the role of thickness
    int radius;

public:

    /// @brief     method to set/update/change the widget's
    ///            border color using `RGB(...)` MACRO
    /// @param[in] R ~ Red
    /// @param[in] G ~ Green
    /// @param[in] B ~ Blue
    void setBorderColor(BYTE R, BYTE G, BYTE B) {
        color = RGB(R,G,B);
    }

    /// @brief     Overload method for `setBorderColor(...)`
    ///            accepting hexadecimal `COLORREF`
    ///            like `0xF18EA32` or similar
    /// @param[in] colorRef ~ color to be used for the border
    void setBorderColor(COLORREF colorRef) {
        color = colorRef;
    }

    /// @brief   method to retrieve the border's color
    /// @return `COLORREF` hexadecimal value representative of the border color
    COLORREF getBorderColor() {
        return color;
    }

    /// @brief   method to set the border style
    /// @details This method is pure virtual to allow for derived-class overriding
    /// since classic-style borders & modern/line-style borders enum constants may overlap
    virtual void setBorderStyle(DWORD s) = 0;
    
    /// @brief  method to retrieve the border style
    /// @return `DWORD` representative of the border's style
    DWORD getBorderStyle() {
        return style;
    }

    /// @brief     method to set/update/change the border radius
    /// @details   smaller integer values correspond to sharper corders
    /// @param[in] r ~ radius to use for the widget's corner edges
    /// @note      border radius is ignored/disregarded for
    ///            classic-style bevel borders, however,
    ///            radius plays the role of `thickness`
    ///            for classic-style etched borders!
    void setBorderRadius(int r) {
        radius = r;
    }

    /// @brief  method to retrieve the border's radius
    /// @return integer representative of the border's radius
    int getBorderRadius() {
        return radius;
    }

    /// @brief     method to update/change/set the width of the border
    /// @param[in] w ~ the width to use for the border
    void setBorderWidth(int w) {
        width = w;
    }

    /// @brief  method to retrieve the border's width
    /// @return integer representative of the border's width
    int getBorderWidth() {
        return width;
    }

    /// @brief     method to update/change/set the border's inset,
    ///            i.e. the distance between widget's rect & border
    /// @param[in] i ~ integer inset to be used when drawing the border
    void setBorderInset(int i) {
        inset = i;
    }

    /// @brief  method to retrieve the borders inset
    /// @return integer representative of the borders inset
    int getBorderInset() {
        return inset;
    }

public:

    /// @brief  virtual abstract method to retrieve the type of this objects
    /// @return `std::string` representative of the type of border
    virtual std::string getBorderType() = 0;

protected:

    /// @brief color pen to be used when drawing the border
    HPEN hColorPen = nullptr;
    /// @brief   white pen to be used when drawing the border
    /// @details The white pen is only applicable to classic-style borders
    HPEN hWhitePen = nullptr;

    /// @brief     parametrized constructor taking `RGB(...)` macro colors
    /// @param[in] R ~ stroke color `R` value
    /// @param[in] G ~ stroke color `G` value
    /// @param[in] B ~ stroke color `B` value
    /// @param[in] w ~ border `int` width
    /// @param[in] r ~ border `int` radius ~ line borders [etched borders ~ thickness, bevel borders ~ NOT applicable]
    /// @param[in] s ~ `DWORD` like `xBorder::StrokeStyle::solid`
    /// @param[in] i ~ distance/margin between widget border & widget rectangle
    xBorder(BYTE R, BYTE G, BYTE B, int w, int r, DWORD s, int i = 0) {
        // ...
        color = RGB(R,G,B);
        width = w;
        radius = r;
        style = s;
        inset = i;
    }

    /// @brief     parametrized constructor taking `COLORREF` hexadecimal value like `0xF18EA32` or similar
    /// @param[in] w ~ border `int` width
    /// @param[in] r ~ border `int` radius ~ line borders [etched borders ~ thickness, bevel borders ~ NOT applicable]
    /// @param[in] s ~ `DWORD` like `xBorder::StrokeStyle::solid`
    /// @param[in] i ~ distance/margin between widget border & widget rectangle
    xBorder(COLORREF colorRef, int w, int r, DWORD s, int i = 0) {
        // ...
        color = colorRef;
        width = w;
        radius = r;
        style = s;
        inset = i;
    }

    /// @brief     pure virtual abstract method to construct a color pen
    /// @param[in] b ~ the blend ratio to apply when constructing the pen
    /// @details   require to use a solid pen or a stylized pen, depending on the type
    virtual void createColorPen(float b) = 0;
    /// @brief     pure virtual abstract method to construct a white pen
    /// @param[in] b ~ the blend ratio to apply when constructing the pen
    /// @details   require to use a solid pen or a stylized pen, depending on the type
    virtual void createWhitePen(float b) = 0;

    protected:

    /// @brief variable storing the blend ratio
    /// to blend the light & dark/color edges
    /// of a classic-style border
    float blend = 0.4; // default

public:

    /// @brief method to retrieve the blend-ration associated to the color pen object
    float getColorBlend() {
        return blend;
    }

    /// @brief method to adjust/update/change/set the blend-ratio to be used when constructing the pen
    /// @param degree ~ the ratio to be used for the color-blending property
    void setColorBlend(float degree) {
    
        // if client-code provides absurd negative value,
        // then default to 0.0 blend-ratio instead
        if (degree < 0.0) {
            blend = 0.0;
            return;
        }
        
        // if client-code provides absurd positive value,
        // then default to 1.0 blend-ratio instead
        if (degree > 1.0) {
            blend = 1.0;
            return;
        }

        // assign/commit the blend ratio
        blend = degree;
    }

public:

    /// @brief   pure virtual abstract method to allow derived-type drawing
    /// @details This method makes most sense to be kept in `xBorder`
    /// since it allows access to all the `xBorder` styles & `types`
    /// via `this` pointer as well as to the widget-specific styles
    /// & params via the `xWidget*` pointer `widget`
    /// @remark
    /// Each of the `xBorder` derived types are reponsible
    /// for drawing their own border outlines ...
    /// @note
    /// Make use of forward declaration for `xWidget`
    virtual void draw(xWidget* widget) = 0;

public:

    #ifndef NDEBUG
    /// @brief   method to print `xBorder` data to console => mainly for debugging purposes!
    /// @param   os ~ reference to `std::ostream` object
    /// @param   border ~ const reference to `xBorder` object whose data is to be streamed to console
    /// @return `std::ostream&` output stream object reference
    friend std::ostream& operator << (std::ostream& os, const xBorder& border) {
        
        os
        << "type: " << type(border)
        << ", "
        << "count: " << border.count
        << ", "
        << "color: (R,G,B) = "
        << "("
        << (int) GetRValue(border.color)
        << ","
        << (int) GetGValue(border.color)
        << ","
        << (int) GetBValue(border.color)
        << ")"
        << ", "
        << "style: " << border.style
        << ", "
        << "width: " << border.width
        << ", "
        << "radius: " << border.radius
        << ", "
        << "inset: " << border.inset;
        
        return os;
    }
    #endif

protected:

  /// @brief    class destructor
  /// @details `~xBorder()` is declared `protected` for use by
  ///          `xBorderFactory`s internal mechanisms only!
  /// @note
  /// Client-code NOT to `delete` `xBorder` pointers <br/>
  /// The appropriate usage is to leave `xBorder` pointers "dangling"
  /// since `xBorderFactory` is designed to take care of the `xBorder` object's resources
  /// The `xBorderFactory` class is responsible for keeping track of any "shared" border objects
  /**
    * @example
    * The following usage of `xBorder` pointers is prohibited:
    * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    * xButton button(...);             // create button control
    * xBorder border = new xBorder();  // create border object
    * button.setWidgetBorder(border);  // assign/associate border to button
    * delete border;                   // Illegal! // NOT to be deleted => private
    * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
    virtual ~xBorder() {
        
        LOG("releasing xBorder resources ...");
        
        // free hColorPen & hWhitePen resources
        
        if (hColorPen) {
            DeleteObject(hColorPen);
        }

        if (hWhitePen) {
            DeleteObject(hWhitePen);
        }
    }
};

#endif // end of xBORDER_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
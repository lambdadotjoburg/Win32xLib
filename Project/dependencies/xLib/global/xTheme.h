/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xTheme.cpp
  * @brief 		File containing xTheme class declaration
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details  This file contains the declaration & implementation for the `xTheme` class
  */

#pragma once

#ifndef xTHEME_H
#define xTHEME_H

/// @todo a FUCKTON of things to improve for `xTheme`

// forward declaration for xWidget class ...
class xWidget;

/**
 * @class    xTheme
 * @brief   `xTheme` is a minimalistic static interface class for selecting themes
 * @details `xTheme` contains a single static interface method & an enum of the theme options
 */
class xTheme {

    public:

  /*%*%*%*%*%*/
  /* Themes  */
  /*%*%*%*%*%*/

  enum Theme {
    DEFAULT,   // default/Explorer
    CLASSIC,   // windows95/2000 => pre-commcntrl's
  };

  /// @brief variable to indicate that a global theme is selected
  static bool globalThemeSelected; // false

  /// @brief apply a global theme to all widgets ...
  static xTheme::Theme globalTheme; // UNDEFINED

  /// @brief     method to set the theme of an `xWidget` control
  /// @param[in] widget ~ the widget for which to apply the theme
  /// @param[in] t ~ the theme to be applied to the widget
  static void SetWidgetTheme(xWidget* widget, xTheme::Theme t);

  /// @brief     method to recursively set the theme of an
  ///           `xWidget` control & it's children components
  /// @param[in] widget ~ the root widget for which to apply the theme
  /// @param[in] t ~ the theme to be applied to the widget-hierarchy
  static void SetWidgetThemeRecursive(xWidget* widget, xTheme::Theme t);

  /// @brief     method to apply a global them to all `xWidget`s in the app
  /// @param[in] t ~ the theme to be applied to the widget
  /// @details   apply the selected Theme to ALL the `xWidget`
  ///            objects in the `xWidgetManager::xWidgets` container
  static void SetGlobalTheme(xTheme::Theme t);

};

// initialize `globalThemeSelected` to `false`
bool xTheme::globalThemeSelected = false;

// initialize `globalTheme` value to `MODERN`
xTheme::Theme xTheme::globalTheme = xTheme::Theme::DEFAULT;

#endif // end of xTHEME_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/

// set the global theme before or after creating a widget
// & also change for individual components/controls ...
// xTheme::SetGlobalTheme(xTheme::Theme::CLASSIC);

/**
 * @example Theme
 * 
 * The following examples demonstrate how set a theme (`CLASSIC`/`DEFAULT`) for:
 * 
 *    1) a specific/single/individual `xWidget` component/control
 *    2) a global theme applied to all widgets *before* OR *after* creating the `xWidget` object instance
 *    3) a theme applied recursively to a widget & all of it's child components ...
 * 
 * Apply a global theme BEFORE creating widget objects ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xTheme::SetGlobalTheme(xTheme::Theme::CLASSIC); // `CLASSIC` or `DEFAULT`
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Create some frames & buttons (controls) as per usual ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * // frames ...
 * xFrame parentFrame("Parent Frame", 1200, 800, 150, 100);
 * xFrame childFrame("Child Frame", 500, 500, 400, 300);
 * xFrame siblingFrame("Sibling Frame", 500, 500, 400, 300);
 * 
 * // buttons ...
 * xButton frameButton(&parentFrame, "OK");
 * xButton childButton(&childFrame, "OK");
 * xButton siblingButton(&siblingFrame, "OK");
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Apply a different theme to any of the controls ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * childFrame.SetWidgetTeme(xTheme::Theme::DEFAULT); // different than `CLASSIC` above ...
 * parentButton.SetWidgetTeme(xTheme::Theme::DEFAULT); // different than `CLASSIC` above ...
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Apply a theme recursively to a widget & it's children
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * siblingFrame.SetWidgetTemeRecursive(xTheme::Theme::DEFAULT); // This affects both `siblingFrame` & `siblingButton`
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Apply a global theme AFTER creating widget objects ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xTheme::SetGlobalTheme(xTheme::Theme::CLASSIC); // `CLASSIC` or `DEFAULT`
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @see `ClienDemoClass`
 */
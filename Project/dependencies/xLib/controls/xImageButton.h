/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xImageButton.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xImageButton` class declaration & implemenation
  * @details  xImageButton.h defines the `xImageButton` class
  */

#pragma once

/// @brief begin of xIMAGEBUTTON_H implementation
#ifndef xIMAGEBUTTON_H
#define xIMAGEBUTTON_H

// include xButton since xImageButton is a derived type
#include "./xButton.h"

/**
 * @class    xImageButton
 * @brief   `xImageButton` provides the interface for creating image-button controls
 * @details `xImageButton` defines image-button interface methods
 *           for creating & manipulating `ImagexButton` objects
 * @todo     Merge this class with `xButton` to reduce code-redundancy
 *           by providing overload constructor for `xButton`
 *           taking image path as arg <br/>
 *           Clean up implementation & reduce code in `LoadImage(...)`
 *           i.e. properly refactor to easily re-use `xImage`
 */
class xImageButton : public xButton {

    /// @brief variable storing pointer to `xImage` object
    ///        for drawing the image in the button
    xImage* pImage = nullptr;

public:

    /// @brief paramatereless/default constructor
    xImageButton()
        : xButton() {
        // ...
        this->setButtonBorder(xBorderFactory::createEmptyBorder(0));
        initDefaultHandlers();
    }

    /// @brief overload parametrized construtor
    /// @param[in] x ~ x-coordinate / position
    /// @param[in] y ~ y-coordinate / position
    /// @param[in] w ~ button width  (default -1)
    /// @param[in] h ~ button height (default -1)
    /// @note      params for dimensions & coordinates are reversed ...
    xImageButton(
        const std::string& path, const std::string& text,
        int x, int y, int w = -1, int h = -1
    ) : xButton(parent, text, w, h, x, y) {
        // pass the image data to `LoadImage(...)`
        LoadImage(path);
        this->setButtonBorder(xBorderFactory::createEmptyBorder(0));
    }

    /// @brief     overload parametrized construtor
    /// @param[in] x ~ x-coordinate / position
    /// @param[in] y ~ y-coordinate / position
    /// @param[in] w ~ button width  (default -1)
    /// @param[in] h ~ button height (default -1)
    /// @note      params for dimensions & coordinates are reversed ...
    xImageButton(
        xWidget* parent, const std::string& path, const std::string& text,
        int x, int y, int w = -1, int h = -1
    ) : xButton(parent, text, w, h, x, y) {
        // pass the image data to `LoadImage(...)`
        LoadImage(path);
        this->setButtonBorder(xBorderFactory::createEmptyBorder(0));
    }

    /// @brief derived class override method for destroying `xImageButton` resources
    /// specifically, for freeing the data related to the image button's `xImage`
    virtual void destroy() override {
        // std::cout << "xImageButton::destroy()" << std::endl;
        if (this->pImage) {
            delete this->pImage;
            this->pImage = nullptr;
        }
        // invoke base class destroy ...
        xWidget::destroy();
    }

    /// @brief derived type destructor
    ~xImageButton() {
        this->destroy();
    }

public:

    /// @brief   override create method to create the `xImageButton` control
    /// @details When creating an `xImageButton` the values provided in the
    ///          constructor's parameter list is taken into consideration:
    ///         
    ///             1) button's dimensions [`w`, `bh`]
    ///             2) whether the image was successfully loaded ...
    /// 
    /// @remark
    /// then the image dimensions are used for the button
    /// to fit the image in the button's borders
    virtual bool create() {

        // if any of button's dimensions not provided ...
        if (m_w == -1 || m_h == -1) {

            // check whether image was loaded ...
            if (pImage->img != nullptr) {

                m_w = pImage->width();
                m_h = pImage->height();

            } else {

                // if image was not loaded,
                // use default button size ...
                m_w = DEFAULT_BUTTON_WIDTH;
                m_h = DEFAULT_BUTTON_HEIGHT;
            }

        } // otherwize, button's dimensions have been provided ...

        // create the button ...
        bool res = xWidget::create();
        return res; // whether button created successfully ...
    }

public:

    /// @brief  method to retrieve the width of the image associated to the button
    /// @return integer representative of the width of the button's image
    int getImageWidth() {
        return pImage->width();
    }

    /// @brief  method to retrieve the height of the image associated to the button
    /// @return integer representative of the height of the button's image
    int getImageHeight() {
        return pImage->height();
    }

    /// @brief  method to retrieve the string path of the image file associated to the button
    /// @return copy of a `std::string` representative of the image file path of the button's image
    std::string getImagePath() {
       return pImage->getImagePath();
    }

protected:

    /// @brief   helper method to load an image from path to be used by the image button
    /// @details interfaces with `xImage` class to ensure proper resource managemenet
    /// @param   path ~ the path of the image to load
    /// @return  boolean flag representative of whether the image was loaded successfully
    bool LoadImage(const std::string& path) {

        // hold onto the previous image ...
        xImage* pPrevImage = pImage;

        // implement xGif derived class of xImage class ...
        if (xFile::extension(path) == "gif") {
            pImage = new xGif(path); // derived class of `xImage`
        } else if (
            xFile::extension(path) == "png"
            || xFile::extension(path) == "jpg"
            || xFile::extension(path) == "jpep"
            || xFile::extension(path) == "bmp"
            || xFile::extension(path) == "ico"
        ) {
            // update the image to new image ...
            pImage = new xImage(path);
        } else {
            return false;
        }

        // delete the previous image if it exists ...
        if (pPrevImage) {
            delete pPrevImage;
            pPrevImage = nullptr;
        }

        return (pImage->img != nullptr);
    }

    /// @brief method to set/update/change the image for the image button
    void setImage(const std::string& path) {
        
        // load the image & draw anyway ...
        LoadImage(path);
        
        // draw this control to reflect updates ...
        draw(this);
    }

protected:

    /// @brief override draw routine for the `xImageButton` class
    /// @param pWidget ~ pointer of the widget control to draw to the screen
    void draw(xWidget* pWidget) override;

public:

    /// @todo do some safety checks to prevent crash for non-gif images ...
    /// @todo log animation data (duration, etc ...)
    void animate(int d = -1) {
        
        if (d == -1) {
            d = this->duration;
        }

        xGif* pGif = static_cast<xGif*>(pImage);
        pGif->StartAnimation(mhWnd, d);
    }

    /// @brief variable storing the duration relating to .gif images
    ///        This variable is NOT used for non-gif images
    int duration = 5; // seconds

    /// @todo do some safety checks to prevent crash for non-gif images ...
    void setDuration(int d) {
        duration = d; // won't apply to non-gif obviously ...
    }

    /// @todo do some safety checks to prevent crash for non-gif images ...
    int getDuration() {
        return duration;
    }

    /// @todo do some safety checks to prevent crash for non-gif images ...
    void stop() {
        xGif* pGif = static_cast<xGif*>(pImage);
        pGif->StopAnimation();
    }

    /// @todo do some safety checks to prevent crash for non-gif images ...
    bool isAnimating() {
        xGif* pGif = static_cast<xGif*>(pImage);
        return pGif->isAnimating();
    }

protected:

    /// @brief variable to toggle whether the text appears
    ///        in front or behind the associated image
    bool textBehind = false;

    /// @brief method to toggle whether the text
    ///        appears behind or in front of the
    ///        button's image
    void setTextBehind(bool flag) {
        textBehind = flag;
    }
};

/// @brief     override implementation of `xImageButton::draw(...)` routine
/// @details   Encapsulates the various draw stages of the control ...
/// @param[in] pWidget ~ pointer of the widget to draw
void xImageButton::draw(xWidget* pWidget) {
 
    /// draw the button as per usual
    /// for preserving the borders, etc ...
    xButton::draw(this);

    // draw the image if not `nullptr`
    if (pImage->img != nullptr) {
        // if the image exists ...
        pImage->drawImage(this);
    }

    // draw the text again if `textBehind == false` ...
    // note: duplicate writing of text,
    // but simplest technique to ensure that
    // the text is not obscured by the image
    if (!textBehind) {
        pWidget->write(pWidget);
    }
}

#endif // end of xIMAGEBUTTON_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
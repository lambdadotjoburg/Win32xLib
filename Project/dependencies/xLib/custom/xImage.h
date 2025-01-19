/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xImage.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xImage` class declaration & implemenation
  * @details  xImage.h defines the `xImage` class
  */

#pragma once

/// @brief begin of xIMAGE_H implementation
#ifndef xIMAGE_H
#define xIMAGE_H

#include <chrono> // for steady clock
#include <thread> // for executing animation on separate thread
#include <atomic> // to access variables safely accross active threads
#include <condition_variable> // for mutex/mutual exclusive thread locking

using namespace std::chrono;

/**
 * @class    xImage
 * @brief   `xImage` provides the interface for creating images for various controls
 * @details `xImage` defines image-related interface methods for creating & manipulating `xImage` objects
 * @todo             define mechanism for fade-in animation & transition effects!
 * @todo             Refactor `xImage`, `xButtonImage`, `xMenuItem`
 *                   to all make reliable use of the common mechanisms
 *                   in this `xImage` implementation!
 */
class xImage {

/// @todo define friend ostream operator << for printing image data ...
/// @todo refactor this class so that menu items can use it ...

protected:

    /// @brief friend class to allow for private & protected access
    /// @todo  declrate other control classes here that required friend-level access
    friend class xImageButton;

    /// @brief provide friend-level access to associate an image to a menu item
    friend class xMenuItem;

    // Come back to this when re-factoring ...
    // allow friend-level access for menu items to use images/bitmaps
    // friend class xMenuItem;

    /// @brief class to report image-related problems, 
    ///        such as failing to load from file, etc ...
    class xImageException : public RuntimeException {
    public: xImageException(const std::string& dscrptn)
        : RuntimeException(dscrptn) {
            // ...
        }
    };

    /// @brief pointer to the Win32 `Gdiplus::Image` element
    Gdiplus::Image* img = nullptr;

    /// @brief variable to store the image's file path ...
    /// @note  Designed to be UNICODE/ANSI-friendly
    #if defined(UNICODE) && defined(_UNICODE)
    std::wstring mImagePath;
    #else
    std::string mImagePath;
    #endif

    /// @brief  variable to keep track of the image's file extension ...
    std::string imageExt;

protected:

    /// @brief private deleted parameterless/default constructor
    xImage() = delete;

    /// @brief parametrized `xImage` constructor
    /// @param path ~ file path of the image to use
    xImage(const std::string& path) {
        LoadImage(path);
    }

    /// @brief helper method to set the image file path
    /// @todo  cleanu up this method's code ...
    void setImagePath(const std::string& path) {

        try {
            // check if icon file extension is valid
            if (
                !(
                       xFile::valid(path, "ico")
                    || xFile::valid(path, "png")
                    || xFile::valid(path, "bmp")
                    || xFile::valid(path, "gif")
                    || xFile::valid(path, "jpg")
                    || xFile::valid(path, "jpeg")
                )
            ) {
                // if not valid, throw xIconException
                std::string message = "invalid image extension  => .bmp/.jpg/.jpeg/.gif/.ico/.png ONLY";
                throw xImageException(message);
            }
            
        } catch(xImageException &ex) {
            
            LOG(ex.what());
            std::cerr << ex.what() << std::endl;
        }

        // store the image path in any case ...
        #if defined(UNICODE) && defined(_UNICODE)
        // store the icon path in wide string format
        mImagePath = StrConverter::StringToWString(path);
        #else
        mImagePath = path; // store in narrow string format
        #endif

        // store the file extension "for no reason"
        imageExt = xFile::extension(path);
    }

    /// @brief  method to load an image from a file into the `Gdiplus::Image` object
    /// @param  path ~ file path of the image to load
    /// @return boolean flag representative of whether or not the image was loaded success
    /// @note
    /// if the image failed to load, then it is immediately deleted but the path
    /// of the image is persisted for exception-handling/reporting purposes
    bool LoadImage(const std::string& path) {

        // initialize Gdiplus instance ...
        // not necessary, initialized by `xApp()`
        xGDI::get();

        // store the path of the image ...
        setImagePath(path);

        // hold on the the previous image data ...
        Gdiplus::Image* pPrevImg = img;

        try {
            
            #if defined(UNICODE) && defined(_UNICODE)
            img = new Gdiplus::Image(mImagePath.c_str());
            #else
            img = new Gdiplus::Image(StrConverter::StringToWString(mImagePath).c_str());
            #endif

            // ensure that the image was loaded successfully ...
            if (img->GetLastStatus() != Gdiplus::Ok) {
                // otherwize, throw exception, including indication of the file path ...
                throw xImageException("failed to load image: " + getImagePath());
            }

        } catch(xImageException &ex) {
            
            LOG(ex.what()); // log the exception data ...
            std::cerr << ex.what() << std::endl;

            // if `Gdiplus::Image` failed to load, delete the data ...
            delete img;
            img = nullptr;
        }

        // delete the previous image in any way (if it exists)
        if (pPrevImg) {
            delete pPrevImg;
            pPrevImg = nullptr;
        }

        /// return `true` if `img` is not `nullptr`, otherwize return `false`
        return (img ? true : false);
    }

public:

    /// @brief  method to retrieve the string path of the image
    /// @return copy of `std::string` representative of the image file path
    std::string getImagePath() {
        #if defined(UNICODE) && defined(_UNICODE)
        // convert to narrow string
        return StrConverter::WStringToString(mImagePath);
        #else
        return mImagePath; // return as-is
        #endif
    }

public:

    /// @todo use the `convertToBitmap` method to implement bitmap pictures for menu items ...
    /// @brief static/shared method to convert a `Gdiplus::Image*` to `Gdiplus::Bitmap*`
    /// @param pImage ~ pointer of the xImage object to convert to Gdiplus::Bitmap*
    /// @details
    /// mainly applicable for menu items, for setting the checkbox bitmap image
    /// <br/>
    /// menu item images MUST be in .bmp format but `xLib`
    /// to provide client-capacity for choosing image any
    /// image format [.png, .ico, .jpg, .jpeg, .gif] <br/>
    /// Of course, .gif will not be an animation!
    /// @remark  important* client code to manage return pointer
    /// @return  pointer to a `Gdiplus::Bitmap` object
    ///          client's responsibility to free image resources
    static Gdiplus::Bitmap* convertToBitmap(Gdiplus::Image* pImage) {
        /// ensure a valid Gdiplus::Image* is provided ...
        if (!pImage || pImage->GetLastStatus() != Gdiplus::Ok) {
            return nullptr;
        }
        // pass the image dimensions & color-constant to Bitmap constructor
        Gdiplus::Bitmap* pBM= new Gdiplus::Bitmap(
            pImage->GetWidth(),
            pImage->GetHeight(),
            PixelFormat32bppARGB
        );        
        // check whether the image conversion was Ok
        if (!pBM || pBM->GetLastStatus() != Gdiplus::Ok) {
            delete pBM; // free the Gdiplus::Bitmap resources
            return nullptr;
        }
        // return a pointer to the bitmap image
        return pBM; // client code to ensure this pointer is stored, to avoid dangling reference ...
    }

protected:

    /// @brief   method to retrieve the pointer of the `Gdiplus::Image` object
    /// @details fuck-around-&-find-out!
    Gdiplus::Image* getImage() {
        if (img) {
            return img; // if not nullptr
        }
        return nullptr; // if not set
    }

    /// @brief  method to retrieve the width of the `Gdiplus::Image`
    /// @return integer representative of the image width
    /// @note 
    /// if the image failed to load, then it is automatically deleted
    /// & the return result is -1 indicating that the image was not loaded
    int width() {
        if (img) {
            return img->GetWidth(); // if not `nullptr`
        }
        return -1; // if not set => indicates no `Gdiplus::Image` object set
    }

    /// @brief  method to retrieve the height of the `Gdiplus::Image`
    /// @return integer representative of the image height
    /// @note 
    /// if the image failed to load, then it is automatically deleted
    /// & the return result is -1 indicating that the image was not loaded
    int height() {
        if (img) {
            return img->GetHeight(); // if not `nullptr`
        }
        return -1; // if not set => indicates no `Gdiplus::Imag`e object set
    }
    
    /// @brief   destructor
    /// @details ensures proper cleanup after use by other `xWidget` controls
    ~xImage() {
        LOG("releasing xImage resources ...");
        if (img) {
            /// free the Gdiplus::Image resources ...
            delete img;
            img = nullptr;
        }
    }

protected:

    /// @todo
    /// define "shrink factor", i.e. as a *percentage*
    /// of the image/button width/height/ratio <br/>
    /// For NOW using the pixel-based shrink-factor is OK

    /// @brief number of pixels by which
    ///        to shrink the image when clicked
    int shrink = 1; // default 1 pixel

public:

    /// @brief method to update/change/set the
    /// "shrink factor" for an image when clicked
    /// @param s ~ integer representative of
    /// the number of pixels to shrink the image
    /// @todo change the name of the menthod & related-variables ...
    void setShrink(int s) {
        shrink = s;
    }

    /// @brief method to retrieve the "shrink factor"
    int getShrink() {
        return shrink;
    }

protected:

    /// @brief helper method to draw the image associated to the `xImageButton` object
    /// @details
    /// depending on the type of border used for the button,
    /// some parameters are taken into account, such as the border's:
    ///
    ///     1) inset
    ///     2) width
    ///     3) radius (thickness in the context of etched borders)
    /// 
    /// @remark This method uses `Gdiplus::Graphics`
    ///         to clip the region of the image
    ///         so as to fit inside the button's border
    void drawImage(xWidget* pWidget);

};

/**
 * @class   xGif
 * @brief   derived from `xImage` to handle .gif image formats
 * @details This class derives from `xImage` to implement
 *          some additional functionality related to animation
 */
class xGif : public xImage {

    /// @brief friend class to allow for private & protected access
    friend class xImageButton;

protected:

    /// @brief     constructor
    /// @param[in] path ~ file path of the .gif image to load
    xGif(const std::string& path) : xImage(path) {
        LoadImage(path);
        initGifInfo();
    }

protected:

    /// @brief variable storing the .gif image frame count data
    UINT frameCount = 0;

    /// @brief vector containing the .gif image frame delays data
    std::vector<UINT> frameDelays;

    /// @brief variable to keep track of
    /// the current frame in use by the animation
    UINT currentFrame = 0;

    /// @brief atomic boolean variable to be shared
    /// accross threads when animation in running
    std::atomic<bool> inAnimation;

    /// @brief conditianal variable ~ for thread-safety & locking
    std::condition_variable cv;

    /// @brief mutex ~ for thread-safety & locking
    std::mutex cvMutext;

    /// @brief variable to stored the .gif image `GUID` data
    GUID dimensionGuid;

    /// @brief method to initialize the .gif image data ...
    void initGifInfo() {
        inAnimation = false;        
        // check if the image was loaded ...
        if (!img) {
            return;
        }
        dimensionGuid = Gdiplus::FrameDimensionTime;
        frameCount = img->GetFrameCount(&dimensionGuid);
        // Get frame delays & store in std::vector 
        UINT size = img->GetPropertyItemSize(PropertyTagFrameDelay);
        if (size > 0) {            
            Gdiplus::PropertyItem* propItem = (Gdiplus::PropertyItem*)malloc(size);            
            if (
                img->GetPropertyItem(PropertyTagFrameDelay, size, propItem) == Gdiplus::Ok
            ) {              
                UINT frameCount = size/sizeof(UINT);
                frameDelays.assign(
                    (UINT*) propItem->value,
                    (UINT*) propItem->value + frameCount
                );
            }
            free(propItem);
        }
    }

    /// @brief     method to initialize the .gif image animation
    /// @param[in] hWnd ~ Handle of the window containing the .gif image object
    /// @param[in] duration ~ amount of time (in second) for the animation
    void StartAnimation(HWND hWnd, UINT duration) {
        // check whether image gif can be animated in the first place ...
        if (frameDelays.size() > 1) {
            // cannot animate gif that is already in animation ...
            if  (inAnimation) {
                return;
            }
            inAnimation = true; // toggle animation state ...
            std::thread(AnimationThread, this, hWnd, duration).detach();
        }
    }

    /// @brief method to stop the .gif image animation
    void StopAnimation(/*HWND hWnd*/) {
        // IMPLICIT(hWnd);
        inAnimation = false;
        cv.notify_all();
    }

    /// @brief     Core animation function ~ repsonsible for animating the .gif image
    /// @param[in] hWnd ~ Handle of the window for which to animate the .gif image
    /// @param[in] duration ~ amount of time (in seconds) to animate the .gif
    void AnimationThread(HWND hWnd, UINT duration) {
        steady_clock::time_point startTime = std::chrono::steady_clock::now();
        // auto startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(startTime.time_since_epoch()).count();
        while (inAnimation) {
            // auto currentTime = std::chrono::steady_clock::now();
            steady_clock::time_point currentTime = std::chrono::steady_clock::now();            
            // auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
            updateFrame(hWnd);
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelays[currentFrame] * 10));
            long long int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            // check if duration elapsed ...
            if (elapsedTime >= duration) {
                StopAnimation(/*hWnd*/);
                break;
            }
        }
        inAnimation = false;
    }

    /// @brief helper method to update the current frame of the .gif animation
    /// @param hWnd ~ Handle of the window for which to update the .gif frame
    void updateFrame(HWND hWnd) {
        img->SelectActiveFrame(&dimensionGuid, currentFrame);
        // currentFrame = (currentFrame + 1) % frameDelays.size(); // incorrect!
        currentFrame = (currentFrame + 1) % frameCount; // correct!
        InvalidateRect(hWnd, NULL, TRUE);
    }

    /// @brief method to check whether a .gif is in animation or not
    bool isAnimating() {
        return inAnimation;
    }
};

#endif // end of xIMAGE_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
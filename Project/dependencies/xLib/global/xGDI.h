/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xGDI.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		for initializing Gdiplus elements such as `xFont`s & png to bitmap `xIcon`s
  * 
  * @details 	GDI ~ Graphics Display Interface
  *             <br/>
  *             This class is intended to be used for loading icons
  *             & other Gdiplus elements from non-standard types
  * 
  * @see <a href="https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-gdi-start" target="_blank">Gdiplus</a>
  * 
  */

/// @brief begin of xGDI_H implementation
#ifndef xGDI_H
#define xGDI_H

// include gdiplus
#include <commctrl.h>
#include <gdiplus.h>

/**
 * @class    xGDI
 * @brief    singleton pattern for Gdiplus RAII resource management
 * @details `xGDI` implements the RAII singleton design pattern <br/>
 *           for managing Gdiplus elements throughout the duration <br/>
 *           of the applications lifecycle as managed by `xApp` <br/>
 *           <br/>
 *           `xGDI` is initialized in `xApp` `init(...)` to include <br/>
 *           vizual styles & easy-of-use for graphic-related data, <br/>
 *           like loading images, bitmap, .png & .ico files, etc ...
 */
class xGDI {

    /**
     * @class    xGDIException
     * @brief   `xGDIException` is a private subclass of `xGDI`
     * @details `xGDIException` is used in the `xGDI` constructor
     *           to ensure proper exception handling of the xGDI RAII
     *           class initialization
     */
    class xGDIException : public SystemException {
    public:
        xGDIException(const std::string& dscrptn)
            : SystemException(dscrptn) {};
        ~xGDIException() {};
    };

public:

    /// @brief  static method that retrieves the `xGDI` singleton instance pointer
    /// @return reference to `xGDI` singleton instance pointer 
    static xGDI& get() {
        if (instance == nullptr) {
            instance = new xGDI();
        }
        return *instance;
    }

    /// @brief delete copy constructor
    xGDI(const xGDI&) = delete;
    /// @brief delete copy assignment operator
    xGDI& operator=(const xGDI&)=delete;

    /// @brief method for destructing `xGDI` singleton instance
    static void destruct() {
        delete instance; // destructor takes care of clean-up
        instance = nullptr;
    }

private:

    /**
     * @brief   Gdiplus token
     * @details The xGDI constructor initializes the Gdiplus common controls resources!
     * @see
     * <a href="https://learn.microsoft.com/en-us/windows/win32/api/gdiplusinit/nf-gdiplusinit-gdiplusstartup" target="_blank">
     *      GdiplusStartup
     * </a>
     */
    ULONG_PTR token = 0;

    /// @brief xGDI contructor => responsible for initializing `Gdiplus`
    xGDI() {
        Gdiplus::GdiplusStartupInput startupInput;
        try {
            Gdiplus::Status status = Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
            if (status != Gdiplus::Ok) {
                int error = GetLastError();
                std::string msg = "xGDI: GdiplusStartup (initialization) failed" + std::to_string(error);
                throw xGDIException(msg);
            }
        } catch(xGDIException &ex) {
            LOG(ex.what());
            std::cerr << ex.what() << std::endl;
        }
    }

    /// @brief   `xGDI` destructor
    /// @details `~xGDI` to free RAII resources held by class instance
    ~xGDI() {
        LOG("destroy xGDI RAII resources ...");
        if (token) {
            Gdiplus::GdiplusShutdown(token);
        }
    }

    /// @brief `xGDI` singleton instance pointer
    /// initialized by `xGDI::get()` for program use
    static xGDI* instance;
};

/// @brief initialize xGDI singleton instance pointer
xGDI* xGDI::instance = nullptr;

#endif // end of xGDI_H
 /*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
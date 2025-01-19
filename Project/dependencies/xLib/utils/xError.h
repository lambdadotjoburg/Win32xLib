/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xError.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		This file contains the declaration
  *             & implementation for the `xError` singleton class
  * 
  * @details 	The class defined in this file is used to translate
  *             Error messages from macro hexadecimal constants
  *             to human-readable strings for exception reporting
  *             by searching through error codes stored in .txt file format ...
  */

#pragma once

/// @brief begin of xERROR_H implementation
#ifndef xERROR_H
#define xERROR_H

/**
 * @class    xError
 * @brief   `xError` is a singleton class that defines a Error message translator
 * @details `xError` is implemented for debugging purposes to translate
 *           error messages from Win32 API `UINT` macro's to human readable
 *          `std::string` format
 *           <br/>
 *           The translated values are viewed as console output (debug)
 */
class xError {

public:

    /**
     * @brief   static method that retrieves the `xError` singleton instance pointer
     * @return  reference to `xError` singleton instance pointer
     * @details This class method returns an `xError` instance pointer
     *          to the client code & privately constructs
     *          a new such instance if none exists
     */
    static xError& get() {
        if (instance == nullptr) {
            // manufacture
            instance = new xError;
        }
        return *instance;
    }

    /// @brief delete copy constructor
    xError(const xError&) = delete;

    /// @brief delete copy assignment operator
    xError& operator=(const xError&) = delete;

    /// @brief method for cleanup RAII resources
    static void destruct() {
        delete instance;
        instance = nullptr;
    }

    /// @brief method to determine from which file (1/4) to search for error code
    /// @param ID ~ macro `UINT` error code to determine the bounds of the error code within the files
    /// @return path of the file from which to retrieve the error code
    /// @todo clean up this code a bit ...
    static std::string selectFile(UINT ID) {

        std::string file = "NONE"; // code page to search

        if (ID < 1000) {
            // reassign file string
            file = "./dependencies/xLib/codes/0000_00999_error.txt";
        } else if (1000 <= ID && ID < 2000) {
            // reassign file string
            file = "./dependencies/xLib/codes/1000_01999_error.txt";
        } else if (2000 <= ID && ID < 6000) {
            // reassign file string
            file = "./dependencies/xLib/codes/2000_05999_error.txt";
        } else if (6000 <= ID && ID < 12000) {
            // reassign file string
            file = "./dependencies/xLib/codes/6000_11999_error.txt";
        }

        return file; // return the string file path to open
    }

    /// @brief method to translate/search error code to human readable string format retrieved from error code page
    /// @param[in] ID ~ macro `UINT` error code to be translated/searched in error code page
    static std::string errorTranslate(UINT ID) {

        std::string file = xError::selectFile(ID);

        // check if "valid" string path returned ...
        if (file != "NONE") {

            // translate the given ID to human-readable format ...
            return xFile::readLineWithSubstring(file, std::to_string(ID));
        }

        return "INVALID"; // dummy text if file == "NONE"
    }

private:

    /// @brief private default constructor
    xError() = default; // to create `xError` `instance` pointer

    /// @brief private default destructor
    ~xError() = default; // nothing to do ...

    /// @brief singleton instance pointer
    static xError* instance;
};

/// @brief initialize & declare static singleton instance pointer
xError* xError::instance = nullptr;

/// @brief MACRO for translating messages using
/// `xERROR(code)` syntax where `code` of a `UINT` type
#define xERROR(code)\
std::string msg = xError::errorTranslate(code);\
if (msg != "UNDEFINED") {\
std::cerr\
<< "[" << "fn: " << __FUNCTION__ << "]"\
<< "[ID:" << code << " => " << "msg: " << msg << "]"\
<< std::endl;\
};\

#endif // end of xERROR_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
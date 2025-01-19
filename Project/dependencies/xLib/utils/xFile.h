/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  xFile.h
  * @author   &lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @brief 	  contains `xFile` class declaration & implemenation
  * @details  xFile.h defines the `xFile` class
  */

#pragma once

/// @brief begin xFILE_H implementation
#ifndef xFILE_H
#define xFILE_H

// include filestream
#include <fstream>

/**
 * @class    xFile
 * @brief   `xFile` is used to check if a file exists
 *           & extract the extension of a file
 * @details
 * `xFile` is a rudimentary implementation of a file manager class  <br/>
 * i.e. `xFile` is not a "production-quality" file management class <br/>
 * & should be used with caution, but provides a minimal interface  <br/>
 * for working & handling `xIcon`-related file data                 <br/>
 * <br/>
 * C++11 does not interface with `std::filesystem` (C++17) <br/>
 * which is the recommended STL facility containing all    <br/>
 * relevant utilities for working with files/directories   <br/>
 */
class xFile {

public:

    /// @class xFileException
    /// @brief xFileException is useful for reporting file-related exceptions
    class xFileException : public SystemException {
    public:
        xFileException(const std::string& dscrptn)
            : SystemException(dscrptn) {
            // ...
        };
    };

public:

    /// @brief     method to validate file extensions
    /// @details   method to validate whether or not a given `ext` corresponds to that of a given `path`
    /// @param[in] path ~ the file path from which to extract & compare `ext`
    /// @param[in] ext ~ the extension to compare to that of `path`
    static bool valid(const std::string& path, const std::string& ext) {
        // check whether the given `path` has a valid extension `ext` ...
        // by comparing the extension "extracted" from `path` to `ext`
        if (extension(path) == ext) {
            return true;
        } // else
        return false;
    }

    /// @brief   method to "extract" the extension from a given `path`
    /// @param   path ~ the file `path` from which to extract the extension
    /// @return  string value representative of the extracted extension
    /// @details 
    /// if no matches are found using `find_last_of`
    /// then `std::string::npos` is returned
    /// `std::string::npos` is defined as
    /// `static const size_t npos = -1;`
    static std::string extension(const std::string& path) {
        // extract the file extension from `path` ...
        // i.e. find last occurence of character . in `path`
        
        // get the substring of `path`
        // starting at index of found character .

        int index;

        try {

            index = path.find_last_of('.');

            if (index == -1) {
                std::string message = "no file extension found!";
                throw xFileException(message);
            }

        } catch (xFileException &ex) {

            std::cerr << ex.what() << std::endl;

            return ""; // return empty string if no `ext` found
        }        
        
        // extract the substring starting at index + 1
        std::string ext = path.substr(index + 1);
        return ext;
    }

    // ...

    // ... 

    /// @brief     module to read a line from a file starting with a given substring
    /// @param[in] path ~ file to open for retrieving line starting with specified substring
    /// @param[in] substring ~ substring to compare with each line in the file
    static std::string readLineWithSubstring(const std::string& path, const std::string& substring) noexcept {

        // use path arg to open the specified file ...
        std::ifstream file(path);

        try {
            
            // if failed to open file
            if (!file.is_open()) {
                
                std::string message = "Could not open file: " + path;
                throw xFileException(message);

            } else {

                // line to store contents
                // & use to compare for substring
                std::string line;

                while (std::getline(file, line)) {
                    // compare substring ...
                    if (line.compare(0, substring.size(), substring) == 0) {
                        
                        file.close(); // close file

                        return line; // found line starting with given substring
                    }
                }
            }

        } catch (xFileException &ex) {

            // could not open file ...

            std::cerr << ex.what() << std::endl;

            LOG(ex.what());

            return "ERROR"; // indication that the file could not be opened ...
        }

        // otherwize, if line not found ...
        file.close(); // close the file anyway ...
        return "UNDEFINED"; // indication that value is not known/found ...
    }

    // ...

    // ... 

}; // end of xFile

#endif // end xFILE_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
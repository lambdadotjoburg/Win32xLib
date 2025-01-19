/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		SystemException.h
  * @brief 		Declares SystemException class
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @details 	
  * 
  * 	This file declares SystemException class
  */

#pragma once

/// @brief begin of SYSEX_H declaration
#ifndef SYSEX_H
#define SYSEX_H

#include "../type/type.h"

#include <exception>

/**
 * @class   SystemException
 * @brief   class for throwing `SystemException` during program errors
 * @details This class is implemented to aid in exception-handling
 *          for the `xWidget` windows wrapper class implementation
 */
class SystemException : public std::exception {
private:

    /// @details
    ///           `std::string _dscrptn` ~
    ///           string containing additional information
    ///           in the form of a message to be written
    ///           to the console upon invoking `what()`
    ///           after "throwing" ...
    ///
    /// @example SystemException
    /// 
    ///     try {
    ///         // do something ...
    ///         throw SystemException("This is a message");
    ///     } catch (SystemException &ex) {
    ///         std::cerr << ex.what() << std::endl;
    ///     }
    ///
    /// @see SystemException
    std::string _dscrptn;

public:

    /// @brief parameterized constructor
    SystemException(const std::string& dscrptn = "");

    /// @brief throws-clause for exception class
    const char* what() const noexcept override;

    /// @brief   destructor
    /// @details virtual default destructor
    virtual ~SystemException() = default;
};

/// @class  RuntimeException
/// @brief `RuntimeException` inherits from `SystemException`
/// @todo     move `RuntimeException` to separate class
///           when defining `xRuntime` class
class RuntimeException : public SystemException {
public:
    /// @brief parameterized constructor
    RuntimeException(const std::string& dscrptn);
    /// @brief   destructor
    /// @details virtual default destructor
    virtual ~RuntimeException() = default;
};

#endif // end of SYSEX_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
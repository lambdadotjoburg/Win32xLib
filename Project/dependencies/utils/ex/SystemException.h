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

#ifndef SYSEX_H
#define SYSEX_H

#include <exception>
#include <string>

#include "../type/type.h"

/**
 * @class   SystemException
 * @brief   class for throwing System Exceptions during program runtime
 * @details This class is implemented to aid in Exception handling
 *          for the `xWidget` windows wrapper class implementation
 */
class SystemException : public std::exception {
private:    
    std::string _dscrptn;

public:

    SystemException();
    SystemException(const std::string& dscrptn);
    const char* what() const throw() override;
    virtual ~SystemException();
};

/// @class  RuntimeException
/// @brief `RuntimeException` inherits from `SystemException`
class RuntimeException : public SystemException {
public:
    RuntimeException(const std::string& dscrptn);
    ~RuntimeException();
};

#endif
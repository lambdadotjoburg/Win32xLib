/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		CMD.h
  * @brief 		declaration for `CMD` class
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @details 	This file contains the declaration of the `CMD` class
  */

#pragma once

/// @brief begin of CMD_H declaration
#ifndef CMD_H
#define CMD_H

#include "../macro/macro.h"

#include <libloaderapi.h> // Max_PATH
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

/**
 * @class       CMD
 * @brief       Implementation/definition of parsing utilities
 * @details     This class contains the definition/implementation
 *              of CMD utility function interfaces
 */
class CMD {

public:

  /// @brief to retrieve static instance handle
  /// @note  to retrieve a handle on the instance,
  ///        use the following syntax:
  ///        `CMD::get()`
  static CMD& get();

  /// @brief    delete copy constructor
  /// @details  `CMD` contains a private delete copy constructor,
  ///           to ensure that all RAII resources are managed
  ///           correctly by the the singleton design implementation
  CMD(const CMD&) = delete;

  /// @brief    delete copy assignment operator
  /// @details  `CMD` contains a private delete copy assignment operator,
  ///           to ensure that all RAII resources are managed
  ///           correctly by the the singleton design implementation
  CMD& operator = (const CMD&) = delete;

  /// @brief method to manage resources
  static void destruct();

  /// @brief mimic cmd line params "extraction" accepting `main`-style args
  const std::vector<std::string>& parse(char *argv[], int *argc);
  /// @brief overload parse method accepting `WinMain`-style args
  const std::vector<std::string>& parse(char *szCmdLine);

  /// @brief checks for presence/existence of flag in cmd line args
  bool exists(const std::string& flag);

  /// @brief shows cmd line args info
  void show();

  /// @brief gets the executable path
  std::string path();

  /// @brief gets the args count ~ `argc`
  int count();

private:

  /// @brief private default `CMD()` constructor
  CMD() = default;
  /// @brief private default `~CMD()` destructor
  ~CMD() = default;

  /// @brief   argc ~ keeps track of number of args
  /// @details `_count` is used to keep track of
  ///          the number of command line args
  int _count;

  // store command line args
  /// @brief argv ~ stores a vector/list of parsed arguments
  std::vector<std::string> _args;
  
  /// @brief   stores the executable .exe file name
  /// @details store invoking executable file name
  ///          separate from other command line args
  ///          `TCHAR` expands as either `CHAR` (ANSII) or `WCHAR` (UNICODE)
  TCHAR _szFileName[MAX_PATH];

  /// @brief "extract" first argument from argv[0]
  void ParseFileName(char *argv_0);

  /// @brief static singleton instance member
  static CMD* instance;
};

#endif // end of CMD_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
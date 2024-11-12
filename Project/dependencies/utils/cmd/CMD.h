/**
  * @file 		CMD.h
  * @brief 		declaration for CMD class
  * @author 	&lambda;ambda
  * @date     \showdate "%Y-%m-%d"
  * @details 	This file contains the declaration of the CMD class
  */

#pragma once

#ifndef CMD_H
#define CMD_H

#include "../macro/macro.h"

#include <libloaderapi.h> // Max_PATH
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

/**
 * @class       CMD
 * @brief       Implementation/definition of parsing utilities
 * @details     This class contains the definition/implementation of CMD utility functions
 */
class CMD {

private:

  // keep track of the number
  // of command line args
  /// @brief argc ~ keeps track of number of args
  int _count;
  
  // store command line args
  /// @brief argv ~ stores a vector/list of parsed arguments
  std::vector<std::string> _args;

  // store file name / invoking executable
  // separate from other command line args
  /// @brief stores the executable .exe file name
  // TCHAR expands as either CHAR (ANSII) or WCHAR (UNICODE)
  TCHAR _szFileName[MAX_PATH]; // define a macro that prints either ...

  // method to parse file name / executable
  void ParseFileName(char *argv_0);

public:

  // default (parameterless) constructor
  CMD();
  // parametrized constructor accepting C-style string
  CMD(LPSTR szCmdLine);
  // parametrized constructor accepting main-entry style argv & argc
  CMD(char *argv[], int *argc);

  const std::vector<std::string>& parse(char *argv[], int *argc);
  const std::vector<std::string>& parse(char *szCmdLine);

  bool exists(const std::string& flag);

  void show();

  std::string path();
  int count();

};

#endif
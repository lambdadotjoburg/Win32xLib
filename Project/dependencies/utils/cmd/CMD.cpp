/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		CMD.cpp
  * @date       \showdate "%Y-%m-%d"
  * @author 	&lambda;ambda
  * 
  * @brief 		This file contains the class
  *             definition for `CMD`
  * @details 	
  * 
  * 	This file is dedicated to the definition/implementation of
  *     the `CMD` class which is used to manage
  *     command line parsing utility/functionality for
  *     WinMain & main program entry point
  */

/// @brief begin of CMD_CPP definition
#ifndef CMD_CPP
#define CMD_CPP

#include "./CMD.h"

/// @details `CMD` singleton instance pointer initialized to `nullptr` before app runtime
CMD* CMD::instance = nullptr;

/// @details method to return reference to singleton `CMD` instance pointer
/// @return  reference to the singleton CMD& instance
CMD& CMD::get() {
    if (instance == nullptr) {
        instance = new CMD();
    }
    return *instance;
}

/// @param[in]  argv_0 ~ `char *argv_0`
/// @details    method to "extract" file
///             name/executable path .exe
///             \f(0^{\text{th}}\f) arg from `argv[0]`
void CMD::ParseFileName(char *argv_0) {
    int i = 0;
    // store pointer
    // to first char in argv_0
    char *t = argv_0;
    while (*t) { // check *t != '\0'
        _szFileName[i++] = *(t++);
        // assign *t character to
        // szFileName char array
        // at index i
    }
}

/// @param[in]  argv ~ char *argv[] ~ received by main
/// @param[in]  argc ~ int *argc ~ the number of args received in main
/// @details    method that accepts main-like arguments for parsing cmd line args
/// @return		vector of parsed cmd line args, parsed/tokenized by (white-space) delimiter &nbsp; "&nbsp;"
const std::vector<std::string>& CMD::parse(char *argv[], int *argc) {
    
    // store count
    CMD::_count = *argc - 1;
    // one fewer argument, since file name stored

    // pass argv[0] pointer to ParseFileName
    ParseFileName(argv[0]);

    // insert/store arguments into vector
    for (int i = 1; i < *argc; i++) {
        _args.push_back(argv[i]);
    }

    // return the args vector
    return _args;
}

/// @param[in]  szCmdLine ~ char *szCmdLine ~ received by WinMain
/// @details    overload parse method for accepting C-style string
/// @return		vector of parsed cmd line args, parsed/tokenized by (white-space) delimiter " "
const std::vector<std::string>& CMD::parse(char *szCmdLine) {

    // szCmdLine does not give a pointer in the same way as argv[0] to the executable filename,
    // however, this may sometimes be a requirement, depending on the project requirements,
    // so we use the above GetModuleFileName to populate szFileName[]
    GetModuleFileName(NULL, _szFileName, MAX_PATH);

    // Parse szCmdLine string into tokens & push to std::vector<std::string>
    char* token;
    char* saveptr = szCmdLine;
    int i = 0; // use i to determine argument count

    // split string at empty space " " => delimiter
    while ((token=strtok_r(saveptr, " ", &saveptr))) {
        _args.push_back(token);
        i++;
    }

    // no argc parameter,
    // but can use i to determine count
    // store count
    // i incremented to size() of vector
    CMD::_count = i;

    // return the args vector
    return _args;
}

/// @param[in] flag ~ constant reference to a string flag to be matched
/// @details   method to check whether a given flag exists in the command line args list/vector
/// @return	   true or false, depending on whether the flag was matched in args vector
bool CMD::exists(const std::string& flag) {
    // iterate over the args vector
    for (size_t i = 0; i < CMD::_args.size(); i++) {
        // check if vector entry is flag
        if (_args[i] == flag) {
            return true;
        }
    }
    return false;
}

/// @details method for printing out the details of the command line args,
///          including the argument count, the exectuble file path & the actual args
void CMD::show() {
    // print file name

    std::cout << "Executable File Name: " << std::endl;
    // std::cout << _szFileName << std::endl;
    TPRINT(_szFileName);

    // print argument count
    std::cout << "Argument Count: " << CMD::_count << std::endl;

    // print arguments stored in vector
    std::cout << "Command Line Arguments: " << std::endl;
    for (size_t i = 0; i < CMD::_args.size(); i++) {
        std::cout << CMD::_args[i] << " ";
    }
    std::cout << std::endl;
}

/// @details method returning the executable path as a string
/// @return  copy of a string representative of executable file name
std::string CMD::path() {
    std::stringstream ss;
    ss << _szFileName;
    return ss.str();
}

/// @details method for returning the number of command line args
/// @return  copy of an `int` representative of args count
int CMD::count() {
    return CMD::_count;
}

/// @details method to manage RAII singleton class resources
///          This method is responsible for free-ing
///          the singleton instance pointer
void CMD::destruct() {
    delete instance;
    instance = nullptr;
}

#endif // end of CMD_CPP
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
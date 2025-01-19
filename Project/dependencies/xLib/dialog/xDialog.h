/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 		xDialog.h
  * @author 	&lambda;ambda
  * @date       \showdate "%Y-%m-%d"
  * 
  * @brief 		file contains implementation of `xDialog` interface methods
  * 
  * @details 	xDialog.h defines interfaces methods for various dialog-box types ...
  */

/// @brief begin of xDIALOG_H implementation
#ifndef xDIALOG_H
#define xDIALOG_H

// not the same as commctrl.h
// #include <commctrl.h> // in xGDI
#include <commdlg.h>
#include <shlobj.h>
// #include <shobjidl.h>

/**
  * @class   xDialog
  * @brief 	 class containing simplified dialog-box methods ...
  *          
  * @details This class containing static interface
  *          methods for creating dialog-box messages ...
  * 
  * @remark C++17 supports `<filesystem>` but C++11 does NOT
  *         `xLib` is built on C++11, thus open/save file
  *         dialog boxes do not rely on `<filesystem>`
  * 
  */
class xDialog {

    /// @remark for open-file dialogs, `xLib`
    /// dynamically appends file types based
    /// on a std::map<std::string, std::string> input ...
    
    /// @remarks
    /// An accepted file extensions
    /// const C-string literal would
    /// look something like ...
    /// ofn.lpstrFilter =
    ///     "All Files\0*.*\0"
    ///     "text Files\0*.txt\0"
    ///     "Text Files\0*.TXT\0"
    ///     "word/document Files\0*.docx\0"
    ///     "excel/document Files\0*.xlsx\0";

    /// @note '\0' end-of-string null-terminating
    /// character is appended after every file ext type ...

    /// @todo
    /// allow client-code to select any of the following
    /// tokens `/` or `\\` for file path separator/delimiter ...
    /// linux directory format => c/this/is/a/file/path/file.txt
    /// escaped backslash format => C:\\this\\is\\a\\file\\path\\file.txt

    /// @remark
    /// Use of .ini files to store the last saved location, i.e
    /// suggest `saveFilePath` based on user's last/preferred
    /// save-location ...

    /// @remark
    /// logic for `OpenFileDialogBox` & `OpenMultipleFilesDialogBox`
    /// is nearly identical, with slight difference in `OPENFILENAME`
    /// struct `DWORD` flags & extracting the paths/filenames
    /// from `lpstrFile`. We cannot, HOWEVER, unify/combine
    /// these methods, since the former
    /// 
    ///     returns `std::string`
    /// 
    ///         while the latter
    /// 
    ///     returns `std::vector<std::string>`

public:

    /// @brief using declaration for simplifying the ext-to-type map ...
    using extensionsMap = std::map<std::string, std::string>;

private:

    /// @brief variable to hold default-extensions map ...
    static extensionsMap defaultExtMap; // declaration ...

public:

    /// @brief common public enum constants
    /// for client to choose dialog box type
    enum DialogType {
        OK = MB_OK,
        YESNO = MB_YESNO,
        OKCANCEL = MB_OKCANCEL,
        YESNOCANECEL = MB_YESNOCANCEL,
        RETRYCANCEL = MB_RETRYCANCEL,
        ABORTRETRYIGNORE = MB_ABORTRETRYIGNORE,
    };

    /// @brief common public enum constants
    /// for client to dialog box icon type
    enum IconType {
        ERR = MB_ICONERROR,
        WARNING = MB_ICONWARNING,
        QUESTION = MB_ICONQUESTION,
        CONFIRM = MB_ICONINFORMATION,
        EXCLAMATION = MB_ICONEXCLAMATION,
    };

    /// @brief  static/global method to create message box/dialog boxes ...
    /// @param  parent ~ xWidget* pointer to the parent/owner of the dialog box
    /// @return integer representative of the dialog box return value/response!
    static int xDialogBox(
        xWidget* parent,
        const std::string& title,
        const std::string& text,
        DialogType t, // no default, select from enum constants!
        IconType i = IconType::CONFIRM, // default CONFIRM
        DWORD flags = (DWORD) NULL // additional/extra flags ...
    ) {

        // UNICODE/ASCI conversion for textbox C-style string ...
        #if defined(UNICODE) && defined(_UNICODE)
        std::wstring strTitle = StrConverter::StringToWString(title);
        std::wstring strText = StrConverter::StringToWString(text);
        #else
        std::string strTitle = title;
        std::string strText = text;
        #endif

        return MessageBox(
            (parent ? parent->Handle() : NULL),
            strText.c_str(), strTitle.c_str(),
            t | i | flags
        );
    }

    /// @brief   short-hand error message dialog box ...
    /// @details static/global method to create an error message dialog box ...
    /// @param   owner ~ xWidget* pointer to the owner of the dialog box
    static void xErrorDialogBox(
        xWidget* owner,
        const std::string& title,
        const std::string& text,
        DWORD err = (DWORD) NULL // optional err code ...
    ) {
        // declare non-const string message ...
        std::string message = text;

        if (err != 0) {
            message += "\n"; // insert line break for readability ...
            // translate error message to human-readable
            // string message & append to message string ...
            message += xError::get().errorTranslate(err);
        }

        // log data ...
        LOG(message.c_str());
        std::cerr << message << std::endl;

        // notice reverse order for title & text ...
        xDialogBox(
            owner, title, message,
            xDialog::DialogType::OK,
            xDialog::IconType::ERR
        );
    }

private:

    /// @brief private helper method to initialize
    ///        the drop-down file type & ext type map
    static void initOpenFileNameStructExtTypeMap(
        OPENFILENAME& ofn,
        const extensionsMap& fTypes,
        int index
    ) {
        // select the first filer item to show ...
        ofn.nFilterIndex = index;

        // declare string for populating'
        // file type names & file type extensions ...
        std::string filterStr; // concatenate all key-value pairs ...

        // loop over the map & concatenate
        // the key & value pairs into the filter string
        // including null terminating character
        // '\0' where appropriate ...
        for (const auto& pair : fTypes) {
            // append map's string key ...
            filterStr += pair.first;  filterStr.push_back('\0');
            // append map's string value ...
            filterStr += pair.second; filterStr.push_back('\0');
        }

        // end-off with a final terminating character ...
        // double null terminating character ...
        filterStr += "\0";
        filterStr += "\0";

        // convert the final/concatenated string ...
        #if defined(UNICODE) && defined(_UNICODE)
            ofn.lpstrFilter = (
                StrConverter::StringToWString(filterStr)
            ).c_str();
        #else
            ofn.lpstrFilter = filterStr.c_str();
        #endif        
    }

    /// @brief method to initialize `OPENFILENAME` struct data ...
    static void initOpenFileNameStructPathDataDialogBoxType(
        OPENFILENAME& ofn,
        HWND ownerHandle,
        DWORD flags,
        int count,
        TCHAR* szFile, // decays to pointer ...
        // #if defined(UNICODE) && defined(_UNICODE)
        // wchar_t* szFile, // decays to pointer ...
        // #else
        // char* szFile, // decays to pointer ...
        // #endif
        const std::string& defaultDir = "",
        const std::string& savePath = ""
    ) {
        // clear the struct data ...
        // ZeroMemory(&ofn, sizeof(OPENFILENAME));

        // intialize the struct data ...
        // ofn.lStructSize = sizeof(OPENFILENAME);

        // provide the parnet widget's handle ...
        ofn.hwndOwner = ownerHandle;

        // ensure that the file exists ...
        ofn.Flags = flags;

        if (flags & OFN_ALLOWMULTISELECT) {
            std::cout << "OFN_ALLOWMULTISELECT is set." << std::endl;
        }

        // allocate a string for storing the file ...
        // char szFile[MAX_PATH*count] = { 0 };

        std::cout << "sizeof(szFile): " << sizeof(szFile) << std::endl;

        // set szFile struct member using above file path size ...
        ofn.lpstrFile = szFile;

        // set max file path struct member ...
        // ofn.nMaxFile = sizeof(szFile);
        ofn.nMaxFile = MAX_PATH * count;

        std::cout << "sizeof(szFile) => " << sizeof(szFile) << std::endl;

        if (!defaultDir.empty()) {
            #if defined(UNICODE) && defined(_UNICODE)
                ofn.lpstrInitialDir = (
                    StrConverter::StringToWString(defaultDir)
                ).c_str();
            #else
                ofn.lpstrInitialDir = defaultDir.c_str();
            #endif
        }

        // copy the data from the path to the struct ...
        // only applicable to `OpenSaveFileDialog(...)`
        if (!savePath.empty()) {

            // narrow-string version ...
            // strncpy(szFile, savePath.c_str(), sizeof(szFile)-1);

            #if defined(UNICODE) && defined(_UNICODE)
                wcsncpy(
                    szFile,
                    (StrConverter::StringToWString(savePath)).c_str(),
                    sizeof(szFile)-1
                );
            #else
                strncpy(szFile, savePath.c_str(), sizeof(szFile)-1);
            #endif

            // keep for unicode/widse version
            szFile[MAX_PATH*count - 1] = '\0';
            // std::cout << "File Path(s): " << szFile << std::endl;
        }
    }

    // ...

public:
    
    /// @brief  static method to show & use an open-file dialog box
    /// @param  owner ~ xWidget* pointer to the owner of the dialog box
    /// @return string representative of the dialog box file path
    static std::string OpenFileDialogBox(
        xWidget* owner,
        const std::string& defaultDir = "",
        const extensionsMap& fTypes = defaultExtMap,
        int index = 1 //, // selected item index, default first ...
        // int count = 1 // indicates single file selection ...
    ) {
        
        // data for open-file C-style struct
        OPENFILENAME ofn;

        // clear the struct data ...
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        // intialize the struct data ...
        ofn.lStructSize = sizeof(OPENFILENAME);

        initOpenFileNameStructExtTypeMap(ofn, fTypes, index);

        // provide client-code preferences for this ...
        DWORD flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // open-file dialog count ALWAYS 1 ...
        int count = 1;

        // #if defined(UNICODE) && defined(_UNICODE)
        // wchar_t szFile[MAX_PATH * count] = { 0 };
        // #else
        TCHAR szFile[MAX_PATH * count] = { 0 };
        // #endif

        // open-file dialog does not require save-path ...
        initOpenFileNameStructPathDataDialogBoxType(
            ofn, owner->Handle(), flags,
            count, szFile, defaultDir, ""
        );

        std::cout << "ofn.nMaxFile => " << ofn.nMaxFile << std::endl;

        // check whether open file returns true, i.e. success ...
        if (GetOpenFileName(&ofn) == TRUE) {
            // construct a c++ string from
            // the struct's `lpstrFile` member
            // populated by `GetOpenFileName` function ...
            
            #if defined(UNICODE) && defined(_UNICODE)
                return StrConverter::WStringToString(
                    std::wstring(ofn.lpstrFile)
                );
            #else
                return std::string(ofn.lpstrFile);
            #endif

        } else {

            // UINT err = GetLastError();
            DWORD err = CommDlgExtendedError();

            if (err != 0) {

                // inform user that the something went wrong ...
                std::string message = "Could NOT open file ... ";
                message += "\n";

                // pass in `openFilePath` for this method ...
                // message += openFilePath;

                xErrorDialogBox(
                    nullptr,
                    "File Open Error",
                    message, err
                );
            }
        }

        // if file could NOT be opened, return empty string ...
        return ""; // empty string => no file path selected ...
    }

    /// @brief  static method to show & use an open-multiple-file dialog box
    /// @param  owner ~ xWidget* pointer to the owner of the dialog box
    /// @return vector container of strings representative of the dialog box file paths
    /// @remark
    /// MAX_PATH ~ 260 => 260 * 4 = 1040
    /// 256*4 = 1024 => approximately 4 (very long) file names ...    
    /// accept a count for scaling `char szFIle[count]` array ...
    /// count => can be adjusted, but plenty of space,
    /// NOT restricted to 4 (in most cases) depending
    /// on the length of file path ...
    static std::vector<std::string> OpenMultipleFilesDialogBox(
        xWidget* owner,
        const std::string& defaultDir = "",
        int count = 4, // count => to scale the allocated char array ...
        const extensionsMap& fTypes = defaultExtMap, // forced to provide default file types ...
        int index = 1 // selected item index, default first ...
    ) {        
        // data for open-file C-style struct
        OPENFILENAME ofn;

        // clear the struct data ...
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        // intialize the struct data ...
        ofn.lStructSize = sizeof(OPENFILENAME);

        initOpenFileNameStructExtTypeMap(ofn, fTypes, index);

        // ensure that the file exists & the path exists ...
        // provide client-code preferences for this ...
        DWORD flags =
          OFN_PATHMUSTEXIST
        | OFN_FILEMUSTEXIST
        | OFN_ALLOWMULTISELECT
        | OFN_EXPLORER;

        // #if defined(UNICODE) && defined(_UNICODE)
        // wchar_t szFile[MAX_PATH * count] = { 0 };
        // #else
        // char szFile[MAX_PATH * count] = { 0 };
        // #endif

        TCHAR szFile[MAX_PATH * count] = { 0 };

        // open-multiple-files dialog does not require save-path ...
        initOpenFileNameStructPathDataDialogBoxType(
            ofn, owner->Handle(), flags,
            count, szFile, defaultDir, ""
        );

        // std::cout << "ofn.nMaxFile => " << ofn.nMaxFile << std::endl;

        // declare vector container of items to return to client ...
        #if defined(UNICODE) && defined(_UNICODE)
        std::vector<std::wstring> filePaths;
        #else
        std::vector<std::string> filePaths;
        #endif

        // check whether open file returns true, i.e. success ...
        if (GetOpenFileName(&ofn) == TRUE) {
            
            #if defined(UNICODE) && defined(_UNICODE)
            wchar_t* p = ofn.lpstrFile;
            // create a wstring from this, called directory ...
            std::wstring directory(p);
            #else
            // declare a wchar_t pointer to
            // the chosen file directory ...
            char* p = ofn.lpstrFile;
            // create a string from this, called directory ...
            std::string directory(p);
            #endif

            // advance the pointer past the directory,
            // since what follows are the selected files ...
            p += directory.size() + 1;

            // if only one file selected ...
            if (*p  == '\0') { // MUST check if *p == '\0'
                filePaths.push_back(directory);
            } else {
                // continue iterating while `*p`
                // is not null/EOL character '\0'
                while (*p != '\0') { // MUST check if *p == '\0'
                    // insert the file path into the list/container ...
                    filePaths.push_back(directory + TEXT("\\") + p);
                    // get the length of p & advance
                    #if defined(UNICODE) && defined(_UNICODE)
                    p += wcslen(p) + 1;
                    #else
                    p += strlen(p) + 1;
                    #endif
                }
            }
            // // return the resulting strings to client ...
            // return filePaths;
        } else {

            // UINT err = GetLastError();
            DWORD err = CommDlgExtendedError();

            if (err != 0) {

                // inform user that the something went wrong ...
                // no wide-string conversion necessar, since
                // `xErrorDialogBox` accepts `std::string` ...
                std::string message;
                message += "Could NOT open files";
                message += "\n";

                // pass in `openFilePath` for this method ...
                // message += openFilePath;

                xErrorDialogBox(
                    nullptr,
                    "File Open Error",
                    message, err
                );
            }
        }

        // if fail, return empty ...
        // return std::vector<std::string> {};
        // return filePaths;

        #if defined(UNICODE) && defined(_UNICODE)
        // first convert to narrow string vector ...
        return StrConverter::VectorToNarrowString(filePaths);
        #else
        return filePaths; // return narrow string vector as-is ...
        #endif
    }

    /// @brief  static method to show & use a save-file dialog box
    /// @param  owner ~ xWidget* pointer to the owner of the dialog box
    /// @return string representative of the dialog box save-file path
    /// @note   client-code most-likely to specify `savePath` first ...
    static std::string SaveFileDialogBox(
        xWidget* owner,
        const std::string& savePath = "",
        const std::string& defaultDir = "",
        bool prompt = true, // default `true` ...
        const extensionsMap& fTypes = defaultExtMap,
        int index = 1 // first selected map item ...
    ) {

        // declare open-file-name struct ...
        OPENFILENAME ofn;

        // clear the struct data ...
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        // intialize the struct data ...
        ofn.lStructSize = sizeof(OPENFILENAME);

        initOpenFileNameStructExtTypeMap(ofn, fTypes, index);

        // save-file dialog `DWORD` flags
        DWORD flags = OFN_PATHMUSTEXIST;
        // check prompt-when-overwrite ...
        if (prompt) { flags |= OFN_OVERWRITEPROMPT; }
        
        // save-file dialog count ALWAYS 1
        int count = 1;

        // #if defined(UNICODE) && defined(_UNICODE)
        // wchar_t szFile[MAX_PATH * count] = { 0 };
        // #else
        // char szFile[MAX_PATH * count] = { 0 };
        // #endif

        TCHAR szFile[MAX_PATH * count] = { 0 };

        // std::cout << "ofn.nMaxFile => " << ofn.nMaxFile << std::endl;

        initOpenFileNameStructPathDataDialogBoxType(
            ofn, owner->Handle(), flags,
            count, szFile, defaultDir, savePath
        );

        if (GetSaveFileName(&ofn) == TRUE) {

            #if defined(UNICODE) && defined(_UNICODE)
                return StrConverter::WStringToString(
                    std::wstring(ofn.lpstrFile)
                );
            #else
                return std::string(ofn.lpstrFile);
            #endif

        } else {

            /// @todo include `CommDlgExtendedError` codes in `xError` handler
            // UINT err = GetLastError();
            DWORD err = CommDlgExtendedError();

            if (err != 0) {

                // inform user that the something went wrong ...
                std::string message;
                message += "The file could not be saved";
                message += "\n";
                message += savePath;

                xErrorDialogBox(
                    nullptr,
                    "File Save Error",
                    message, err
                );
            }
        }

        return "";
    }

    /// @brief enum defining constants
    /// for open-folder-dialog-box attributes ...    
    enum BrowserAttributes {
        DIRS = BIF_RETURNONLYFSDIRS,
        SEARCH = BIF_EDITBOX | BIF_VALIDATE,
        MODERN = BIF_USENEWUI | BIF_VALIDATE,
        READONLY = BIF_NONEWFOLDERBUTTON,
    }; // BIF_USENEWUI (BIF_NEWDIALOGSTYLE | BIF_EDITBOX)

    /// @brief  static method to show & use an open-folder dialog box
    /// @param  owner ~ xWidget* pointer to the owner of the dialog box
    /// @return string representative of the dialog box open-folder path
    static std::string OpenFolderDialogBox(
        xWidget* owner,
        const std::string& defaultDir = "",
        const std::string& defaultTitle = "Select Folder ... ",
        DWORD flags = BrowserAttributes::MODERN
    ) {
        // declare variable to hold `BROWSEINFO` struct ...
        BROWSEINFO bi;

        // zero/clea memory allocated to struct ...
        ZeroMemory(&bi, sizeof(BROWSEINFO));

        // assign the handle to the struct data, if any ...
        bi.hwndOwner = owner ? owner->Handle() : NULL;

        // provide the string title to the struct ...
        #if defined(UNICODE) && defined(_UNICODE)
            bi.lpszTitle = (
                StrConverter::StringToWString(defaultTitle)
            ).c_str();
        #else
            bi.lpszTitle = defaultTitle.c_str();
        #endif

        // specify the flags associated to the `BROWSERINFO` struct ...        
        bi.ulFlags = (DWORD) flags; // cast to appropriate DWORD type ..

        // check if flags contains `BIF_VALIDATE` flag ...
        if (flags & BIF_VALIDATE) {
            // only then provide the callback procedure function pointer ...
            // LOG("...");
            bi.lpfn = BrowseFoldersCallbackProc;
        }

        // Check whether default opebn-directory is provide ...
        if (!defaultDir.empty()) {
            
            // if so, then initialize the root directory ...
            PIDLIST_ABSOLUTE pidlRoot;
 
            // parse the root directory string path & grab result ...
            HRESULT res = SHParseDisplayName(
                (StrConverter::StringToWString(defaultDir)).c_str(),
                nullptr, &pidlRoot, 0, nullptr
            );

            // check if result successfull ...
            if (SUCCEEDED(res)) {
                // only then assign the root
                // directory struct data member ...
                bi.pidlRoot = pidlRoot;
            }
        }

        // hold onto pidl for checking success & free resources ...
        PIDLIST_ABSOLUTE pidl = SHBrowseForFolder(&bi);

        // check if pidl not nullptr
        if (pidl != nullptr) {

            // declare a buffer for holding the folder path ...
            #if defined(UNICODE) && defined(_UNICODE)
            wchar_t path[MAX_PATH]; // MAX_PATH ~ 260
            #else
            char path[MAX_PATH]; // MAX_PATH ~ 260
            #endif

            // invoke get-path shell function ...
            if (SHGetPathFromIDList(pidl, path)) {
                // convert TCHAR to std::string or std::wstring
                
                #if defined(UNICODE) && defined(_UNICODE)
                    return StrConverter::WStringToString(
                        std::wstring(path)
                    );
                #else
                    return std::string(path);
                #endif
            }

            // free the resources held by co-task ...
            CoTaskMemFree(pidl);
            // link to ole32.dll
            // undefined reference to `__imp_CoTaskMemFree
        }
        
        // If path not found, return empty string ...
        return ""; // retrun empty string ""
    }

private:

    /// @brief callback procdure for browse-file `BIF_VALIDATE` attribute ...
    static INT CALLBACK BrowseFoldersCallbackProc(
        HWND hWnd, UINT msg, LPARAM lParam, LPARAM lpData
    ) {

        IMPLICIT(hWnd);
        IMPLICIT(lpData);

        if (msg == BFFM_VALIDATEFAILED) {

            // extract the invalid file path from `lpData` ...
            LPCTSTR invalidPath = reinterpret_cast<LPCTSTR>(lParam);

            // inform user that the something went wrong ...
            std::string message = "Could NOT open Folder";
            message += "\n"; // insert new line for readability ...

            #if defined(UNICODE) && defined(_UNICODE)
            std::string path = StrConverter::WStringToString(invalidPath);
            #else
            std::string path = invalidPath;
            #endif

            // pass in `openFilePath` for this method ...
            // message += openFilePath;
            message += path;

            // show an error dialog box to user ...
            xErrorDialogBox(nullptr, "Folder Error", message, msg);

            // LOG the data ...

            // return non-zero to keep the dialog open ...
            return 1;
        }
        return 0;
    }

public:

    /// @note
    /// std::cout << "filterStr: " << filterStr << std::endl; <br/>
    /// The above will NOT show/output the end-of-string characters '\0'    
    #ifndef NDEBUG
    static void DebugPrintCharactersByteByByte(
        const std::string& filterStr
    ) {
        // Debug: Print the byte-by-byte contents
        // of filterStr to see null characters
        for (size_t i = 0; i < filterStr.size(); ++i) {
            std::cout
            << "Index: " << i << " => "
            << "char: "  <<       (unsigned char) filterStr[i] << std::endl
            << "byte: "  << (int) (unsigned char) filterStr[i] << std::endl;
        }
    }
    #endif

    static void setDefaultExtensionsMap(
        const extensionsMap& extMap
    ) {
        defaultExtMap = extMap;
    }

    static extensionsMap getDefaultExtensionsMap() {
        return defaultExtMap;
    }
};

// default extensions map ...
xDialog::extensionsMap xDialog::defaultExtMap = {
    {"ALL/ANY", "*.*"},
    {"text", "*.txt"}
};

#endif // end of xDIALOG_H
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
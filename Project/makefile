# https://www.youtube.com/watch?v=DtGrdB8wQ_8

.RECIPEPREFIX=>

BINARY=main
ROOT=.
BUILDDIR=build
BIN=bin
# default is debug => bin/debug
# release mode => bin/release
OUT=$(BIN)/debug

# The only variables that would ever really need to be changed
# for project specific requirements is the below INCDIRS variable
# in case the makefile user would like to add/remove folders
# that contain project-dependent .h header files

# Automate INCDIRS, so that makefile can discover the .h header files recursively
INCDIRS=$(sort $(dir $(call rwildcard $(ROOT)/*/)))
# INCDIRS=./src/inc/ #./Alt/Sub/ # list additional include .h header file directories here ...

# The following DIRS variable stores default directories to be create with `make init`
# src will be used to store all the client-code resource files => any folders created here is the developers work ...
# dependencies contains the actual xLib project source code files & git clones ...
# rsc contains thr project .rc files & manifest file created by the xLibrary ...
# rsc/img contains file logos for file-type association in the windows registry & executable info ...
# bin will be used to store the compiled executable program in either debug or release subfolder/subdirectory
# doc will be used to store the doxygen generated documentation files for the project
# doc/cstmz will be used to store customized doxygen generated documentation
# extern will be used to store any shared object .so & dynamically linked library .dll files external to the project
DIRS=src bin bin/debug bin/release extern extern/lib extern/dll
DOCS=doc doc/LOG doc/cstmz/html
DOXYFILE=DoxyFile
# add any additional directories to be created for project-specific folder hierarchy requirements

PROJ_NUM = 0.0.0 # prompt user for this during `make init`/`make setup`
COMPANY_NAME=&lambda;ambda

# compilation flag options:
# https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html#org3aa59c3
EXT=cpp
CXX=g++ # don't change CXX (in debug mode)
STD=-std=c++11

# define compiler optimization level
# toggle between -O0 -O1 -O2 -O3 & some others
OPT=-O0
DEPFLAGS=-MP -MD

# DEPFLAGS Details:

# -MP:
# This option instructs CPP to add a phony target for each dependency other
# than the main fle, causing each to depend on nothing.
# These dummy rules work around errors make gives
# if you remove header fles without updating
# the ‘Makefile’ to match

# -MD:
# The driver determines fle based on whether an ‘-o’ option is given.
# If it is, the driver uses its argument but with a suffixx of ‘.d’,
# otherwise it takes the name of the input file,
# removes any directory components and sufx,
# and applies a ‘.d’ suffix

WARNFLAGS=-Wall -Wextra
DEBUGFLAGS=$(WARNFLAGS) -DDEBUG -g

FLAGS=-C $(STD) $(foreach D, $(INCDIRS), -I$(D)) $(DEPFLAGS) $(OPT) # append to this (in debug mode)

# Stack Overflow post for recursive wildcard function:
# https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile
# https://stackoverflow.com/questions/3774568/makefile-issue-smart-way-to-scan-directory-tree-for-c-files
# https://bLOG.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
# https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
# define a recursive wildcard function to compile files in subdirectories in a recursive manner!
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

FILES:=$(sort $(call rwildcard,$(ROOT)/,*.$(EXT)))

# define the directory that stores dynamically linked library files ...
# The extern directory is searched recursively, so any fildes in subfolders are also included ...
# Folders containing .dll files MUST be stored in the extern folder
DLL_EXT=dll # on unix, change this to .so or .a
EXTERN_DIR=$(ROOT)/extern/lib
DLL:=$(basename $(sort $(notdir $(call rwildcard,$(EXTERN_DIR)/,*.$(DLL_EXT)))))
# This is done so that we can append -l to the library name, like -llibname to include the .dll files during linking

# https://stackoverflow.com/questions/7826448/linking-libraries-with-gcc-order-of-arguments
SUB=$(dir $(sort $(call rwildcard,$(EXTERN_DIR),*)))
# append the Folder/Directory to search for .dll's using -LdllFolderName
# LINKER_FLAGS = -L$(ROOT)
# LINKER_FLAGS=-L$(EXTERN_DIR)
# Example LINKER_FLAGS:
# Suppose sub_file.cpp depends on alt_file.cpp, then ...
# -L./extern/sub -lsub_file.cpp  -L./extern/alt -lalt_file.cpp
# Example usage:
# g++ main.cpp -o main.exe -L./extern/sub -lsub_file.cpp  -L./extern/alt -lalt_file.cpp
LINKER_FLAGS = # empty
LINKER_FLAGS += $(foreach s, $(SUB), -L$(s))
# append the -llibname to FLAGS during linking
LINKER_FLAGS += $(foreach dll, $(DLL), -l$(dll)) # append to this (in debug mode)

# CAUTION*: the order of linking is important in gcc/g++
# For certain applications, the above LINKER_FLAGS may need to be hard-coded
# specifically if there is dependencies, for example:
# if a.cpp depends on b.cpp & b.cpp depends on d.cpp
# then compiling g++ a.cpp -L./dir/to/libs -ldependencies
# MUST be done in the correct order:
# g++ a.cpp -L./dir/to/b -lb -L./dir/to/d -ld
# suppose ./dir/to/b = ./dir/to/d = ./ then
# g++ a.cpp -L. -lb -ld is the only correct order!
# So LINKER_FLAGS may need to be hard-coded
# depending on the user's specific needs!

# use pattern substitution to identify .o object & .d files for linker & dependencies
OBJECTS=$(patsubst %.$(EXT),$(ROOT)/$(BUILDDIR)/%.o,$(FILES))
DEPFILES=$(patsubst %.$(EXT),$(ROOT)/$(BUILDDIR)/%.d,$(FILES))

# `debug` appears before `all` because `debug` is the "default" make recipe/command
# define the program entry point
EXE=$(ROOT)/$(OUT)/$(BINARY).exe

# https://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds
debug : FLAGS += $(DEBUGFLAGS)
debug : all ## recipe for building the binaries in debug mode (program executable .exe resides in bin/debug)

# in release mode, build the executable main.exe in bin/release
release : OUT = bin/release
release : all ## compile the application in release mode (program executable .exe resides in bin/release)
>$(COPY)

# NOTE*:
# a better way, is to create an installer for the application
# so that the program's .dll & other file/folder structure
# remains in-tact, while the PC environment PATH variable is
# set instead, so that the .dll's have a permanent location
# & the environment PATH variable is ALWAYS set to point to
# the appropriate directory containing the relevant .dll's

# define a recipe for compiling the Project to a statical library
STATIC_FLAGS= # initially empty
static : STATIC_FLAGS=-static -static-libgcc -static-libstdc++
static : release ## recipe for compiling the project into a static re-distributable

# copy the .dll files over to the appropriate directory, alongside main.exe
COPY_FILES:=$(sort $(call rwildcard,$(ROOT)/,*.$(DLL_EXT)))
COPY=$(foreach file, $(COPY_FILES), $(shell cp -nf $(file) $(OUT)))

.PHONY : copy
copy:
>$(COPY)

# builds the binary executable for the program
all : $(BINARY) ## traditional recipe for building/compiling the project!
# >$(COPY)
# no need to copy .dll files to folder alongside .exe executable,
# since the PATH variable is updated to include directory
# to .dll files before runtime/startup/execution of main.exe
# However, for release mode, it is still preferred to copy the .dll files
# & make use of program installer to set environment variables in path

UNICODE_FLAGS= # initially empty
COMCTL_FLAG = -lcomctl32 # this will be included when running `make` or `make win`
GDI_FLAGS = -lGdiplus -lgdi32
THEME_FLAGS = -luxtheme # theme stuff ...
DWM_FLAGS = -ldwmapi # desktop window manager flags # tray icons, non-client area margins # alpha/transparency
DLG_FLAGS = -lcomdlg32 # for opening dialog boxes, specifically open file dialog, etc ...
COTASK_FLAGS = -lole32 # for `CoTaskMemFree` & othe CoTask-routines ...
# SHELL_FLAGS = -lshell32 # for opening folder-dialog boxes ...

# make
# builds the binary executable for the program
$(BINARY) : $(OBJECTS)
>@mkdir -p $(OUT)
>"$(MAKE)" rsc
>$(CXX) -o $(OUT)/$@ $^ $(COMCTL_FLAG) $(Win32_FLAGS) $(LINKER_FLAGS) $(RSCOBJECTS) $(GDI_FLAGS) $(DWM_FLAGS) $(THEME_FLAGS) $(DLG_FLAGS) $(SHELL_FLAGS) $(COTASK_FLAGS)
# >$(CXX) -o $(OUT)/$@ $^ $(COMCTL_FLAG) $(Win32_FLAGS) $(LINKER_FLAGS)

# Note*: linking order matters, i.e.
# LINKER_FLAGS may need to be hard-coded
# depending on makefile user's specific needs!!!
# link .dll's when compiling executable
# Win32_FLAGS are empty, unless compiling
# with make app which sets this Win32_FLAGS
# variable as -mwindows

# builds the object files for each .cpp/.c
# file in the project folder hierarchy
$(ROOT)/$(BUILDDIR)/%.o : %.$(EXT)
>@mkdir -p $(dir $@)
>$(CXX) $(FLAGS) $(UNICODE_FLAGS) -c -o $@ $<

DIST_FOLDER=$(ROOT)/dist

# resources stuff (.rc files & .res files)

RCEXT:=rc
RSCFILES:=$(call rwildcard,$(ROOT)/,*.$(RCEXT))
RSCOBJECTS:=$(patsubst %.$(RCEXT),$(ROOT)/$(BUILDDIR)/%.res,$(RSCFILES))

.PHONY : rsc
# an icon must be at most 96x96 pixels
# recipe for building .res files from .rc files into ./build directory
# preserving the project hierarchy => to be used when linking to produce .exe
# .rc files cannot be empty when compiling with windres [windows resources]
# windres is a gcc/g++ tool to compile .rc files for .res files
rsc : $(RSCOBJECTS)  ## recipe for compiling .res files from .rc files into ./build directory using windres
$(ROOT)/$(BUILDDIR)/%.res : %.rc
>@mkdir -p $(dir $@)
>windres -i $< -O coff -o $@ # GNU windres only does ANSI encoding
# https://stackoverflow.com/questions/8497838/windres-syntax-error

clean :	## recipe for removing all object .o & dependency .d files as well as resource .res files
>rm -rf $(OBJECTS) $(DEPFILES) $(DIST_FOLDER) $(RSCOBJECTS) # $(OUT) # $(BUILDDIR)

user:
>@echo $(USERPROFILE) # \AppData\Local

DLL_FILES:=$(sort $(call rwildcard,$(ROOT)/,*.$(DLL_EXT)))
ABS_PATH=$(dir $(foreach file, $(DLL_FILES), $(abspath $(file))))

# & we must use the recursive technique to add each
# of the paths to .dll files to the PATH variable
# at runtime, i.e. simulating runtime linking,
# while the user can keep the .dll's in a fixed
# directory/folder, i.e. no copying over the .dll files
# alongside the executable ...

# https://stackoverflow.com/questions/61311317/change-path-in-makefile
DELIMITER=:
export PATH := $(foreach P,$(ABS_PATH),$(P)$(DELIMITER))$(PATH) # append P followed by colon:

env:
>$(info $(PATH))

# test arguments (flags) to pass to the executable via command line
# can replace by any program specific arguments => to be replaced when building a program installer ...
TEST_CMD_LINE_ARGS := -these -are -test -args

.PHONY : run
run : ## recipe for running the program executable /.main.exe => equivalent to running ./main.exe from the command line/terminal with dummy command line arguments
>[ -f $(EXE) ] && $(EXE) $(TEST_CMD_LINE_ARGS) || echo "If the program executable $(EXE) does NOT exist => run make init to create a stub & then run make to build the executable & try again";

define PROGRAM
/**
 * @file 	main.cpp
 * @date 	\showdate "%Y-%m-%d"
 * @author 	$(COMPANY_NAME)
 *
 * @brief 	This file contains the `main` entry point of the program
 *
 * @details
 *          This file contains the `main` entry point of the program
 * 			& serves as a stub code block for quick-setup using
 * 			a makefile => to produce this code block, run `make init`
 * 			from the command line, which will NOT overwrite
 * 			an existing main.cpp file
 */

#include <iostream>

/**
 * @class       This is a Demo class
 * @brief       This is a Demo class to demonstrate the usage of doxygen & it's use with the makefile
 * @details     This Demo class is a dummy class that does not serve any purpose in the program
 * 			    other than to demonstrate the usage of doxygen-style comments in the makefile
 */

class Demo {
public:
	/**
	 * @brief   default constructor
	 * @details The default Demo class constructor for demonstrating doxygen & makefile
	 */
	Demo() {

	}
};

/// @brief      This is the `main` entry point of the program
/// @param[in]  argc The number of arguments passed in by the command line
/// @param[in]  argv The argument list passed in by the command line
/// @return     integer return value indicating program exit status

int main(int argc, char *argv[]) {
	std::cout << "C++ Program stub generated from makefile" << std::endl;
	std::cin.get();
	return 0;
}

endef
export PROGRAM

STUB_SRC = $(ROOT)/examples/stubmain.txt
STUB_TRG = $(ROOT)/main.cpp

generate_stub :
>[ -f $(ENTRY) ] && echo "If the $(ENTRY) file already exists, it will NOT be overwritten!" || cp $(STUB_SRC) $(STUB_TRG);

# Equivalent to ./main.cpp
ENTRY=$(ROOT)/$(BINARY).$(EXT)

# recipe for initializeing new project from makefile
.PHONY : init
# init : # run setup on `> make init` from the client/make user's perspective!
init : setup generate_stub ## recipe for initializing a new project => create all relevant project directories & library files as needed & generating a program stub `WinMain` entry point in a ./main.cpp file
>@mkdir -p $(DIRS);

# Win32 program stub
define APP
/**
  * @file 		main.cpp
  * @date     \showdate "%Y-%m-%d"
  * @author 	$(COMPANY_NAME)
  *
  * @brief 		This file contains the `WinMain` entry point of the Win32 application
  * 
  * @details
  * 
  * 	This file contains the `main` entry point of the program
  * 	& serves as a stub code block for quick-setup using
  * 	a makefile => to produce this code block, run `make init`
  * 	from the command line, which will NOT overwrite
  * 	an existing main.cpp file
  * 
  * <p>
  *	Compiling the program in debug mode using makefile <br>
  *	`> make win`
  * </p>
  *
  * <h3>Win32 Resources:</h3>
  * 
  * <a href="http://www.winprog.org/tutorial/start.html" target="_blank">The Forget's Win32 Program Tutorials</a> <br>
  * <a href="https://cplusplus.com/forum/articles/16820/#google_vignette" target="_blank">Making Win32 API Being Unicode Friendly - LPWTFISALLTHIS</a> <br>
  * <a href="https://www.transmissionzero.co.uk/computing/win32-apps-with-mingw/" target="_blank">Transmission Zero - Building Win32 GUI Applications with MinGW</a> <br>
  * <a href="https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-" target="_blank">Managing Application State - An Object Oriented Approach for coding Win32</a> <br>
  */

/// single include header file ...
#include "./dependencies/xLib/xApp.h"

/// @brief      `WinMain` program entry point
/// @details    This is the `WinMain` entry point program stub for a Win32 App
/// @return     integer return value indicating program `EXIT_STATUS`

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) {

	// initialize application ...
    xApp::get().init(hInstance, hPrevInstance, szCmdLine, nCmdShow);

    // Code ...

    // Create an `xFrame` object ...
    xFrame parentFrame("Parent Frame", 100, 100, 900, 800);
    parentFrame.show();

    // Create a child `xFrame` object ...
    xFrame childFrame(&parentFrame, "Child Frame", 100, 100, 600, 500);
    childFrame.show();

    // Create a button `xButton` object ...
    xButton button("click", 100, 100, 45, 25);
    parentFrame.append(&button);

    xApp::get().messagepump();
	return EXIT_SUCCESS;
}

endef
export APP

# recipe for initializeing new project from makefile
# I want `setup` to be dependent on `init `
.PHONY : console
console : init ## recipe for creating all relevant project directories as needed by the makefile & creating a program stub ./main.cpp entry point
console : ## a recipe for generating a traditional `main` entry point stub in a ./main.cpp file
>@mkdir -p $(DIRS)
>[ -f $(ENTRY) ] && echo "If the $(ENTRY) file already exists, it will NOT be overwritten!" || echo "$$PROGRAM" >> $(ENTRY);

# UNICODE_FLAGS= -D UNICODE -D _UNICODE -municode

# win : FLAGS += $(DEBUGFLAGS)

win : UNICODE_FLAGS += -DUNICODE -D_UNICODE # -municode
win : debug ## recipe for building the Win32 executable .exe binaries in debug mode (Windows GUI & debug console)

# in release mode, build the executable main.exe in bin/release
app : Win32_FLAGS = -mwindows
app : release ## recipe for building the Win32 executable .exe binaries in release (GUI) mode (suppressing the debug console) by passing -mwindows flag as arg to g++)

# recipe for initializeing new project from makefile
.PHONY : Win32
Win32 : ## recipe for creating all relevant project directories as needed by the `makefile` & creating a program stub `./main.cpp` entry point for Win32 apps
>@mkdir -p $(DOCS)
>[ -f $(ENTRY) ] && echo "If the $(ENTRY) file already exists, it will NOT be overwritten!" || echo "$$APP" >> $(ENTRY);

# recipe for listing all files files in project folder hierarchy
.PHONY : files
files : ## recipe for listing ALL relative file paths, w.r.t to $(ROOT) directory
>@echo FILES = [${FILES}]

# define variables for all header files, having different extensions, .h .hpp .tpp
H_EXT = h
HPP_EXT = hpp
TPP_EXT = tpp
HEADERS=$(sort $(call rwildcard,$(ROOT)/,*.$(H_EXT)))
HEADERS+=$(sort $(call rwildcard,$(ROOT)/,*.$(HPP_EXT)))
HEADERS+=$(sort $(call rwildcard,$(ROOT)/,*.$(TPP_EXT)))

# recipe for listing all header files in project folder hierarchy
.PHONY : headers
headers : ## recipe for showing all .h .hpp & .tpp header files
>@echo HEADERS = [${HEADERS}]



# clean :	## recipe for removing all object .o & dependency .d files as well as resource .res files
# >rm -rf $(OBJECTS) $(DEPFILES) $(DIST_FOLDER) $(RSCOBJECTS) # $(OUT) # $(BUILDDIR)

# define a recipe that runs `clean` removes build folder & the compiled executable
.PHONY : remove
remove : clean ## recipe for removing build directory, object .o files, dependency .d files & program executable .exe
>rm -rf $(BIN) ## $(ROOT)/doc $(DOXYFILE) # optionally, remove $(BUILDDIR)

# define a recipe that shows all the directories in the project structure, NOT including the file names
# this is equivalent to showing all the include directories, which are ALL the project folders & sub-folders & sub-sub-folders, etc ... (recursively)
.PHONY : dirs
dirs : ## recipe for listing all directories in project folder hierarchy
>@echo INCDIRS = [${INCDIRS}]

# define a variable using rwildcard that gets all the file names in the project
NAMES=$(sort $(notdir $(call rwildcard $(ROOT)/*/)))

# define a recipe that shows the names of ALL the files in the Project Folder hierarchy, NOT including the Directory Paths
# This recipe makes use of a variable that works similar as before using the rwildcard function defined above,
# but only targets files using the `notdir` makefile directive, in place of the `dir` directive
.PHONY : names
names : ## recipe for showing ALL the names (ONLY) of the files (NOT the full path)
>@echo NAMES = [${NAMES}]

# define a whitespace token
empty:=
space:=$(empty) $(empty)

#LOG data folder
LOG_DIR:=$(ROOT)/doc/LOG
# LOG info
SURNAME?=EMPTY
NAME=?EMPTY
EMAIL?=EMPTY
LOG_MSG?=EMPTY
LOG_DATE:=$(shell date '+%Y-%m-%d %H:%M:%S')
LOG_LINK:=$(subst $(space),-,$(subst :,-,$(LOG_DATE))) # replace : & whitespace by -
LOG_FILE:=$(subst $(space),,$(LOG_LINK)-LOG.txt) # replace any whitespace in the file name

# Target to get user input
.PHONY : LOG
LOG : ## recipe for generating log files for the project for changes made by developer for keeping track of project history
> @read -p "Enter Surname: " sur ;\
> read -p "Enter Name: " name; \
> read -p "Enter email: " email; \
> read -p "LOG Message: " LOG; \
> "$(MAKE)" writeLOGfile SURNAME="$$sur" NAME="$$name" EMAIL="$$email" LOG_MSG="$$LOG";\

define LOG_ENTRY
@showdate "%Y-%m-%d %H:%M:%S" $(LOG_DATE)
<a href="../LOG/$(LOG_FILE)" target="_blank">
    $(LOG_FILE)
</a>
<br>
endef
export LOG_ENTRY

# a DoxyFile configuration file is a free-form `ASCII` text file with a structure that is similar to that of a `makefile`
# running doxygen -g generates a default DoxyFile/Configuration file => This makefile will use the default name DoxyFile, NOT a user-preferred name
# we do NOT want to clutter the project folder with an html & latex folder in the ROOT directory, thus, we will store the output files for doxygen in the doc folder
writelogfile :
>echo "$$LOG_ENTRY" >> $(LOG_DIR)/changelog.md ;\
>echo "Date: " $(LOG_DATE) >> $(LOG_DIR)/$(LOG_FILE) ;\
>echo "Name: " $(NAME) >> $(LOG_DIR)/$(LOG_FILE) ;\
>echo "Surname: " $(SURNAME) >> $(LOG_DIR)/$(LOG_FILE) ;\
>echo "Email: " $(EMAIL) >> $(LOG_DIR)/$(LOG_FILE) ;\
>echo "LOG Message: " $(LOG_MSG) >> $(LOG_DIR)/$(LOG_FILE) ;\

# define a recipe for initializing git commands
.PHONY : git
git : ## recipe for quit .git setup
>git init
>$(info The status of the repository and the volume of per-file changes:)
>@git status
>@git diff --stat

# define a recipe for zipping the project folder for distributing the project!
# Create a .zip folder containing the redistributable source code
# DIST_FOLDER=$(ROOT)/dist
.PHONY : distribute
distribute : remove ## recipe for distributing the project source code => create a .zip folder containing project source code files
>mkdir -p $(DIST_FOLDER)
>tar.exe -a -cf $(DIST_FOLDER)/dist.zip *

.PHONY : delete
# define a recipe for removing all the project folder, except the Top-Level ROOT folder
delete : # clean ## CAREFULL: recipe for removing ALL folders & files excluding the original makefile
> rm -rf -- ./*/
> rm -rf $(ENTRY) $(ROOT)/DoxyFile # doc src bin

# NOTE*:
# running make clean removes the following:
# $(OBJECTS) $(DEPFILES) $(DIST_FOLDER)

# https://gist.github.com/prwhite/8168133
# define a recipe for showing the makefile directives/recipes/target commands
# Add the following 'help' target to your Makefile
# Add help text after each target name starting with '\#\#'
help : ## Show makefile help.
>@fgrep -h " ##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

# https://stackoverflow.com/questions/38256738/does-gnu-make-have-a-way-to-open-a-file-using-the-default-program
README_PATH:=$(ROOT)/doc/README.md
LISENCE_PATH:=$(ROOT)/doc/legal/LISENCE.md
CSTMZ_PATH:=$(ROOT)/doc/cstmz
HTML_PATH:=$(CSTMZ_PATH)/html
WEBSITE_URL:=https://www.lambda.joburg

BROWSER_PATH:=C:/Program Files/Google/Chrome/Application/chrome.exe

# provided by programmer using `make init`
PROJECT_NAME ?= Project Name
AUTH_NAME ?= Author Name
PROJECT_DESCR ?= Project Description
REPO_LINK ?= REPO Link

# define a recipe for doxygen, i.e. to auto-generate documentation for the project
.PHONY : doxygen
doxygen : setup
doxygen : generatelatexheader ## recipe for (auto)-generating documentation for project (produces updates documentation for project) run `doxygen man` to see the doxygen manual
>@echo "doxygen => (auto)-generating documentation for project ..."
>doxygen
>powershell.exe -c $(ROOT)/doc/html/index.html
# > Open index.html file in preferred browser => continued ...

PATH_TO_LATEX_MAKEFILE = $(ROOT)/doc/latex/
PATH_TO_LATEX_OUTPUT = $(ROOT)/doc/latex/refman.pdf

# Removing the Watermark "Generated by Doxygen" from the $\LaTeX$ .pdf output
# https://www.baeldung.com/linux/find-replace-text-in-file
# https://stackoverflow.com/questions/16211207/remove-generated-by-doxygen-and-timestamp-in-pdf
# Produce the default `header.tex` file ...

# produce latex header file to replace watermark "$generatedby Doxygen" by "$generatedby $(COMPANY_NAME)"
generatelatexheader :
> @mkdir -p $(ROOT)/doc/latex
> doxygen -w latex $(ROOT)/doc/latex/header.tex $(ROOT)/doc/latex/footer.tex $(ROOT)/doc/latex/doxygen.sty
> sed -i 's/Doxygen/Lambda/' $(ROOT)/doc/latex/header.tex
> sed -i 's/$$doxygenversion/ /' $(ROOT)/doc/latex/header.tex

# rule for producing latex output for project
# this rule should be dependent on `doxygen`
latex : ## a recipe for generating LaTeX documentation of project => MUST run `make doxygen` before `make latex` since LaTeX output is dependent on HTML output
> powershell.exe make -C $(PATH_TO_LATEX_MAKEFILE);\
> powershell.exe -C $(PATH_TO_LATEX_OUTPUT);\

# DOXYFILE=DoxyFile
# MORE doxygen stuff ...
# To be appended to the end of DoxyFile
define DOXYGEN_CONFIGURATION_SETTINGS

#---------------------------------------------------------------------------
# INDEX ALL PAGES
#---------------------------------------------------------------------------

EXTRACT_ALL             = YES
SOURCE_BROWSER          = YES

#---------------------------------------------------------------------------
# User-Defined Customization Tags
#---------------------------------------------------------------------------
INPUT                  += $(ROOT)/main.cpp
INPUT                  += $(ROOT)/src
INPUT                  += $(ROOT)/dependencies # remove when done building xLib
OUTPUT_DIRECTORY        = $(ROOT)/doc
RECURSIVE               = YES
FULL_PATH_NAMES         = YES
STRIP_FROM_PATH         = ..//..
PROJECT_NAME            = $(PROJECT_NAME)
PROJECT_NUMBER          = $(PROJ_NUM)
GENERATE_TREEVIEW       = YES

#---------------------------------------------------------------------------
# Customization for Main/Index/Home Page
#---------------------------------------------------------------------------
USE_MDFILE_AS_MAINPAGE  = $(README_PATH)
INPUT                  += $(README_PATH)

#---------------------------------------------------------------------------
# Additional .md file/pages to include
#---------------------------------------------------------------------------

INPUT                  += $(LISENCE_PATH)
INPUT                  += $(ROOT)/doc/legal/releasenotes.md
INPUT                  += $(ROOT)/doc/LOG/changeLOG.md

#---------------------------------------------------------------------------
# Technologies, Objective, Specs, Automata .md files
#---------------------------------------------------------------------------

INPUT                  += $(HTML_PATH)/objective.md
INPUT                  += $(HTML_PATH)/technologies.md
INPUT                  += $(HTML_PATH)/specs.md
INPUT                  += $(HTML_PATH)/automata.md
INPUT                  += $(HTML_PATH)/architecture.md

#---------------------------------------------------------------------------
# Additional Material/Getting Started .md files
#---------------------------------------------------------------------------

INPUT                  += $(HTML_PATH)/codeblocks.md
INPUT                  += $(HTML_PATH)/latex.md
INPUT                  += $(HTML_PATH)/table.md
INPUT                  += $(HTML_PATH)/tutorials.md

#---------------------------------------------------------------------------
# Dark Theme
#---------------------------------------------------------------------------

HTML_COLORSTYLE         = TOGGLE

#---------------------------------------------------------------------------
# MathJax Mathematics Rich Display Rendering
#---------------------------------------------------------------------------

USE_MATHJAX             = YES

#---------------------------------------------------------------------------
# html Header Customizations
#---------------------------------------------------------------------------

HTML_HEADER             = $(HTML_PATH)/header.html

#---------------------------------------------------------------------------
# html Footer Customizations
#---------------------------------------------------------------------------

HTML_FOOTER             = $(HTML_PATH)/footer.html

# Additionally, we will set the following tags.
# The first will suppress most/all of the output to the console,
# making doxygen less verbose and the second is specifically enabled
# so that we can still get important feedback about whether or not
# some of the source code has been documented or not - pretty much self explanatory!

#---------------------------------------------------------------------------
# Set Doxygen Verbosity
#---------------------------------------------------------------------------

# I would like to have some code undocumentated in certain cases => can toggle this on/off near release mode ...

QUIET                   = YES
WARN_IF_UNDOCUMENTED    = NO

#---------------------------------------------------------------------------
# UML Class Diagrams
#---------------------------------------------------------------------------

DOT_PATH                = C:\\Program Files\\Graphviz\\bin
HAVE_DOT                = YES
# UML_LOOK              = YES
CALL_GRAPH              = YES
CALLER_GRAPH            = YES

# While we're at it, let us also include the amsmath $\LaTeX$ package
# to allow us to type $\lambda ambda$ in the `header.tex` file for the signature,
# to customize the .pdf file output even more ...

GENERATE_LATEX          = YES
USE_PDFLATEX            = YES

#---------------------------------------------------------------------------
# Additional Packages for LaTeX Rendering
#---------------------------------------------------------------------------

EXTRA_PACKAGES          = longtable
EXTRA_PACKAGES         += amsmath
# EXTRA_PACKAGES       += unicode-math # to allow use of \omicron
EXTRA_PACKAGES         += mathtools

#---------------------------------------------------------------------------
# Custom LaTeX Header File
#---------------------------------------------------------------------------

LATEX_HEADER           = $(ROOT)/doc/latex/header.tex

#---------------------------------------------------------------------------
# Preprocessor MACRO Directives/File Guards
#---------------------------------------------------------------------------

ENABLE_PREPROCESSING    = YES
MACRO_EXPANSION         = YES
EXPAND_ONLY_PREDEF      = NO
SKIP_FUNCTION_MACROS    = NO

#---------------------------------------------------------------------------
# C-Style MACRO's Compilation Operations/Settings (Preprocessor PREDEFINE's)
#---------------------------------------------------------------------------

PREDEFINED              = __GNUC__
# PREDEFINED           += ALT

endef
export DOXYGEN_CONFIGURATION_SETTINGS # necessary!!!

# Target to get user input
.PHONY : setup
# recipe for quick project setup => makefile invokes `setup` on `make init`
setup : 
> @mkdir -p $(DIRS)
> @read -p "Enter Project Name: " proj; \
> read -p "Enter Project Number: " num; \
> read -p "Enter Author Name: " auth; \
> read -p "Enter Project Description: " descr; \
> read -p "Enter REPO Link: " link; \
> "$(MAKE)" initdoxyfile PROJECT_NAME="$$proj" PROJ_NUM="$$num" AUTH_NAME="$$auth" PROJECT_DESCR="$$descr" REPO_LINK="$$link";\
> "$(MAKE)" manifest PROJECT_NAME="$$proj" PROJ_NUM="$$num" AUTH_NAME="$$auth" PROJECT_DESCR="$$descr" REPO_LINK="$$link";\

# a DoxyFile configuration file is a free-form `ASCII` text file with a structure that is similar to that of a `makefile`
# running doxygen -g generates a default DoxyFile/Configuration file => This makefile will use the default name DoxyFile, NOT a user-preferred name
# we do NOT want to clutter the project folder with an html & latex folder in the ROOT directory, thus, we will store the output files for doxygen in the doc folder
# recipe for generating doxygen file => initialize DoxyFile configuration file
initdoxyfile :
>mkdir -p $(ROOT)/doc/html;\
>mkdir -p $(ROOT)/doc/cstmz/html/;\
>mkdir -p $(ROOT)/doc/cstmz/js/;\
>mkdir -p $(ROOT)/doc/legal;\
>mkdir -p $(ROOT)/doc/LOG;\
>[ ! -f $(DOXYFILE) ] && ((doxygen -g $(DOXYFILE)) && (echo "$$DOXYGEN_CONFIGURATION_SETTINGS" >> $(DOXYFILE))) || echo "If the $(DOXYFILE) file already exists, it will NOT be overwritten!";\
>[ ! -f $(README_PATH) ] && echo "$$README_HTML" >> $(README_PATH) || echo "If the $(README_PATH) file already exists, it will NOT be overwritten!";\
>[ ! -f $(ROOT)/doc/cstmz/js/toggle.js ] && echo "$$TOGGLE" >> $(ROOT)/doc/cstmz/js/toggle.js || echo "If the $(ROOT)/doc/cstmz/js/toggle.js file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/header.html ] && echo "$$HTML_HEADER" >> $(HTML_PATH)/header.html || echo "If the $(HTML_PATH)/header.html file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/footer.html ] && echo "$$HTML_FOOTER" >> $(HTML_PATH)/footer.html || echo "If the $(HTML_PATH)/footer.html file already exists, it will NOT be overwritten!";\
>"$(MAKE)" generatemarkdownfiles;\

# invoke recipe for each page to be created in html directory
# a recipe for generating the markdown files to be used with doxygen
# no need to invoke => invoke `make doxygen` for generating project documentation for release
generatemarkdownfiles :
>[ ! -f $(LISENCE_PATH) ] && (echo "$$LISENCE_HTML" >> $(LISENCE_PATH)) || echo "If the $(LISENCE_PATH) file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/objective.md ] && (echo "$$OBJECTIVE_HTML" >> $(HTML_PATH)/objective.md) || echo "If the $(HTML_PATH)/objective.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/technologies.md ] && (echo "$$TECH_HTML" >> $(HTML_PATH)/technologies.md) || echo "If the $(HTML_PATH)/technologies.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/specs.md ] && (echo "$$SPECS_HTML" >> $(HTML_PATH)/specs.md) || echo "If the $(HTML_PATH)/specs.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/automata.md ] && (echo "$$AUTOMATA_HTML" >> $(HTML_PATH)/automata.md) || echo "If the $(HTML_PATH)/automata.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/architecture.md ] && (echo "$$ARCHITECTURE_HTML" >> $(HTML_PATH)/architecture.md) || echo "If the $(HTML_PATH)/architecture.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(ROOT)/doc/legal/releasenotes.md ] && (echo "$$RELEASE_NOTES_HTML" >> $(ROOT)/doc/legal/releasenotes.md) || echo "If the $(ROOT)/doc/legal/releasenotes.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(ROOT)/doc/LOG/changeLOG.md ] && (echo "$$CHANGELOG_HTML" >> $(ROOT)/doc/LOG/changeLOG.md) || echo "If the $(ROOT)/doc/LOG/changeLOG.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/codeblocks.md ] && (echo "$$CODEBLOCKS_HTML" >> $(HTML_PATH)/codeblocks.md) || echo "If the $(HTML_PATH)/codeblocks.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/latex.md ] && (echo "$$LATEX_HTML" >> $(HTML_PATH)/latex.md) || echo "If the $(HTML_PATH)/latex.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/table.md ] && (echo "$$TABLE_HTML" >> $(HTML_PATH)/table.md) || echo "If the $(HTML_PATH)/table.md file already exists, it will NOT be overwritten!";\
>[ ! -f $(HTML_PATH)/tutorials.md ] && (echo "$$TUTORIAL_HTML" >> $(HTML_PATH)/tutorials.md) || echo "If the $(HTML_PATH)/tutorials.md file already exists, it will NOT be overwritten!";\

# define html for readme.md file
define README_HTML
# Main Page {#mainpage}

<b>@showdate "%A %d-%m-%Y %H:%M"</b>
<!-- this date is generated when doxygen compiles the documenation -->

<hr/>

<h2><b>$(PROJECT_NAME)</b></h2>
<h3><b>Project Description</b></h3>
<p>
	$(PROJECT_DESCR)
</p>

<p style="text-indent:30px;">
    Created by
    <a href="$(WEBSITE_URL)" target="_blank">
        $(COMPANY_NAME)
    </a>
</p>
<!--$(AUTH_NAME)-->

A copy of the project can be ascertained from
<a href="$(REPO_LINK)" target="_blank">
    REPO Link
</a>

\htmlonly
<br/>
    <script src="..\cstmz\js\toggle.js"></script>
        <div id="container">
        <img
            id="microchip"
            src="$(WEBSITE_URL)/assets/images/index/stock/vector_graphics/artificial_intelligence.svg"
            style="transform: rotate(45deg); display: block; margin-left: 50px;" width="100" height="100"
        />
    </div>
<br/>
\endhtmlonly

#### Here is a list of features:

- @subpage objective "Objective"
- @subpage Technologies "Technologies"
- @subpage Specs "Specs"
- @subpage Tutorials "Tutorials"

<br/>

@ref Lisence "Lisence"

endef
export README_HTML

# this toggle function will invert the color of the .svg files used
# that may appear dark on a dark background to appearing as inverted
# with respect to the bacgkround colors
define TOGGLE
// var toggleButton = document.getElementsByTagName('dark-mode-toggle');
// console.LOG(typeof(toggleButton));
// console.LOG(DarkModeToggle.title);
// console.LOG(DarkModeToggle.userPreference);
// console.LOG(typeof(DarkModeToggle.userPreference));
window.addEventListener('click', invert)
function invert()
{
    if (DarkModeToggle.userPreference) {
        // console.LOG("Dark Mode");
        document.getElementById("microchip").style.filter="invert(100%)";;
		document.getElementById("makelogo").style.filter="invert(100%)";;
    } else if (!DarkModeToggle.userPreference) {
        // console.LOG("Light Mode");
        document.getElementById("microchip").style.filter="invert(0%)";;
		document.getElementById("makelogo").style.filter="invert(0%)";;
    }
}
window.addEventListener("DOMContentLoaded", invert);
endef
export TOGGLE

define HTML_HEADER
<!-- HTML header for doxygen 1.9.7-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "https://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="$$langISO">
<head>
    <meta http-equiv="Content-Type" content="text/xhtml;charset=UTF-8"/>
    <meta http-equiv="X-UA-Compatible" content="IE=11"/>
    <meta name="generator" content="Doxygen $$doxygenversion"/>
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <!--BEGIN PROJECT_NAME--><title>$$projectname: $$title</title><!--END PROJECT_NAME-->
    <!--BEGIN !PROJECT_NAME--><title>$$title</title><!--END !PROJECT_NAME-->
    <link href="$$relpath^tabs.css" rel="stylesheet" type="text/css"/>

    <!-- the sole purpose of generating this html header file is to insert the additional favicon link below-->
    <!-- set the favicon for the browser tab -->
    <link rel="icon" href="$(WEBSITE_URL)/assets/images/index/logo/lambda_logo.svg" type="image/x-icon" />

    <!--BEGIN DISABLE_INDEX-->
      <!--BEGIN FULL_SIDEBAR-->
    <script type="text/javascript">var page_layout=1;</script>
      <!--END FULL_SIDEBAR-->
    <!--END DISABLE_INDEX-->
    <script type="text/javascript" src="$$relpath^jquery.js"></script>
    <script type="text/javascript" src="$$relpath^dynsections.js"></script>
    $$treeview
    $$search
    $$mathjax
    $$darkmode
    <link href="$$relpath^$$stylesheet" rel="stylesheet" type="text/css" />
    $$extrastylesheet
</head>

<body>
<!--BEGIN DISABLE_INDEX-->
  <!--BEGIN FULL_SIDEBAR-->
<div id="side-nav" class="ui-resizable side-nav-resizable"><!-- do not remove this div, it is closed by doxygen! -->
  <!--END FULL_SIDEBAR-->
<!--END DISABLE_INDEX-->

<div id="top"><!-- do not remove this div, it is closed by doxygen! -->

<!--BEGIN TITLEAREA-->
<div id="titlearea">
<table cellspacing="0" cellpadding="0">
 <tbody>
 <tr id="projectrow">
  <!--BEGIN PROJECT_LOGO-->
  <td id="projectlogo"><img alt="Logo" src="$$relpath^$$projectlogo"/></td>
  <!--END PROJECT_LOGO-->
  <!--BEGIN PROJECT_NAME-->
  <td id="projectalign">
   <div id="projectname">$$projectname<!--BEGIN PROJECT_NUMBER--><span id="projectnumber">&#160;$$projectnumber</span><!--END PROJECT_NUMBER-->
   </div>
   <!--BEGIN PROJECT_BRIEF--><div id="projectbrief">$$projectbrief</div><!--END PROJECT_BRIEF-->
  </td>
  <!--END PROJECT_NAME-->
  <!--BEGIN !PROJECT_NAME-->
   <!--BEGIN PROJECT_BRIEF-->
    <td>
    <div id="projectbrief">$$projectbrief</div>
    </td>
   <!--END PROJECT_BRIEF-->
  <!--END !PROJECT_NAME-->
  <!--BEGIN DISABLE_INDEX-->
   <!--BEGIN SEARCHENGINE-->
     <!--BEGIN !FULL_SIDEBAR-->
    <td>$$searchbox</td>
     <!--END !FULL_SIDEBAR-->
   <!--END SEARCHENGINE-->
  <!--END DISABLE_INDEX-->
 </tr>
  <!--BEGIN SEARCHENGINE-->
   <!--BEGIN FULL_SIDEBAR-->
   <tr><td colspan="2">$$searchbox</td></tr>
   <!--END FULL_SIDEBAR-->
  <!--END SEARCHENGINE-->
 </tbody>
</table>
</div>
<!--END TITLEAREA-->
<!-- end header part -->
endef
export HTML_HEADER

# Personalizing the Doxygen `html` Footer
# define html for lisence.md page
# The Doxygen "watermark" is contained in the `html` footer.
# So if you want to remove it for ALL auto generated `html` pages,
# then you would have to replace the `html` footer with your own personalized version.
# Thankfully, it's not to difficult - just change the details of the images, hyperlinks & wording ...
define HTML_FOOTER
        <!-- start footer part -->
        <!-- BEGIN GENERATE_TREEVIEW -->
        <div id="nav-path" class="navpath">
        <!-- id is needed for treeview function! -->
          <ul>
            $$navpath
            <li class="footer">
                &lambda;ambda
                <a href="$(WEBSITE_URL)">
                    <img
                        class="footer"
                        src="$(WEBSITE_URL)/assets/images/index/logo/lambda_logo.svg"
                        width="85" height="25" alt="doxygen"
                    />
                    <!-- src="$$relpath^doxygen.png" -->
                </a>
                <!-- $$doxygenversion -->
            </li>
          </ul>
        </div>
    </body>
</html>
endef
export HTML_FOOTER

# define code for lisence.md page
define LISENCE_HTML
@page Lisence Lisence
# MIT Lisence

<span style="color: blue;"><b>Copyright</b></span>
<a href="https://en.wikipedia.org/wiki/Copyright_symbol">&copy;</a>

<b>@showdate "%A %d-%m-%Y %H:%M:%S"</b>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

<b>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.</b>

@ref changeLOG "Change LOG" <br/>
@ref releasenotes "Release Notes"

<i><a href="$(WEBSITE_URL)">$(COMPANY_NAME)</a></i>

endef
export LISENCE_HTML

# define the technologies used in your project ...
define TECH_HTML
@page Technologies Technologies

This page contains a list of technologies used to develop the project:

<br/>

* Win32API
<a href="https://www.microsoft.com/en-gb/software-download/windows10" target="_blank">
    <img src="https://img-prod-cms-rt-microsoft-com.akamaized.net/cms/api/am/imageFileData/RE1Mu3b?ver=5c31"
     width="200px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* Windows Subsystem for Linux
<a href="https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux" target="_blank">
    <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/64px-Tux.svg.png"
     width="80px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* C++11
<a href="https://en.wikipedia.org/wiki/C%2B%2B14" target="_blank">
    <img src="https://cdn-icons-png.flaticon.com/512/6132/6132222.png"
         width="100px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* Doxygen
<a href="https://www.doxygen.nl/" target="_blank">
    <img src="https://www.doxygen.nl/images/doxygen.png"
     width="200px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* gcc/g++ GNU Compiler
<a href="https://gcc.gnu.org/" target="_blank">
    <img src="https://gcc.gnu.org/img/gccegg-65.png"
     width="100px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* MinGW32/64
<a href="https://www.mingw-w64.org/" target="_blank">
    <img src="https://www.mingw-w64.org/header.svg#gh-light-mode-only"
     width="250px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* git
<a href="https://git-scm.com/" target="_blank">
    <img src="https://git-scm.com/images/logo@2x.png"
     width="150px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* xeus-cling
<a href="https://github.com/jupyter-xeus/xeus-cling" target="_blank">
    <img src="https://github.com/jupyter-xeus/xeus-cling/raw/main/docs/source/xeus-cling.svg"
     width="200px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
    />
</a>

* GNU make (installed alongside g++/gcc compiler)
<a href="https://www.gnu.org/software/make/manual/make.html" target="_blank">
\htmlonly
    <script src="..\cstmz\js\toggle.js"></script>
    <div id="container">
		<img
			id="makelogo"
			src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/39/Official_gnu.svg/563px-Official_gnu.svg.png?20080303012513"
			width="150px;" style="display: block; margin-left: 50px; padding-top: 20px; padding-bottom: 40px;"
		/>
    </div>
\endhtmlonly
</a>

endef
export TECH_HTML

# define future/purpose ...
define OBJECTIVE_HTML
@page objective Objective

The project is an initiative to automate
<a href="https://en.wikipedia.org/wiki/Computing" target="_blank">
    programming & computing
</a> & a step towards simplifying
<a href="https://en.wikipedia.org/wiki/Software_documentation">
    software documentation
</a> &
<a href="https://en.wikipedia.org/wiki/Compiler">
	the software compilation
</a>
processes

* @subpage architecture "Software Architecture"
* @subpage automata "Programming Automata"
endef
export OBJECTIVE_HTML

define SPECS_HTML
@page Specs Specs

This page accentuates the minimum hardware specifications and software requirements:

<h3><a href="https://en.wikipedia.org/wiki/Computer_hardware"> Hardware </a></h3>

<h3><a href="https://en.wikipedia.org/wiki/Software"> Software </a></h3>

See @ref Technologies "Technologies"
endef
export SPECS_HTML

# define tutorials.md page
define TUTORIAL_HTML
@page Tutorials Tutorials

Here we list some common doxygen markup syntax tutorials:

- @subpage codeblocks "Code Blocks"
- @subpage latex "LaTeX Page"
- @subpage table "Table Page"

endef
export TUTORIAL_HTML

# define latex.md page
define LATEX_HTML
@page latex LaTeX Page

The powerseries representation \f( \exp{} = \sum_{n=0}^{\infty} \frac{x^n}{n!}\ \f) is an inline \f( \LaTeX \f) expression.
<!-- It seems like a doxygen-MathJax-LaTeX related issue causing inline-math formula rendering issues ... => upgrading LaTeX/doxygen may fix this? -->

The Gamma function below is a displaystyle \f( \LaTeX \f) expression ...
\f[
    \Gamma(z) = \int_{0}^{\infty} e^{-t} t^{z} dt
\f]

Furthermore, we can write & align equations like so ...
    
\f{eqnarray*}{
    x &= y + z\\
    r &= s + t\\
\f}
endef
export LATEX_HTML

# define code blocks page to demonstrate how to define code blocks ...
define CODEBLOCKS_HTML
@page codeblocks Code Blocks

> Here is an example of a python (.py) script codeblock to detail the `dummy_function()`

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.py}
# Dummy Class
class Dummy:
    def dummy_function():
        pass
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

> Here is an example of a C++/C (.cpp/c) script codeblock to detail the `alt_function()`

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
// Alt Class
class Alt
{
public:
    void alt_function()
    {
        return;
    }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
endef
export CODEBLOCKS_HTML

# demonstrate how to define tables in doxygen documentation ...
define TABLE_HTML
@page table Table Page

This page demonstrates the use of markdown tables in doxygen

| Left-Align   | Centered          | Right-Aligned   |
| :----------- | :---------------: | --------------: |
| Row 1, Col 1 | Row 1, Col 2      | Row 1, Col 3    |
| Row 2, Col 1 | Row 2, Col 2      | Row 2, Col 3    |
endef
export TABLE_HTML

# change this to something more generic ...
define AUTOMATA_HTML
@page automata Automata

<a href="https://en.wikipedia.org/wiki/Automaton" target="_blank">
    Automata
</a>
is derived from the Greek word <span>&alpha;&upsilon;&tau;o&mu;&alpha;&tau;o&sigmaf;</span> related to the English word "automatic"

<!-- &alpha;&upsilon;&tau;&omicron;&mu;&alpha;&tau;&omicron;&sigmaf; -->


Automata is the study of abstract machines & is an umbrella term encompassing ideas form
<a href="https://en.wikipedia.org/wiki/Theoretical_computer_science" target="_blank">theoretical computer science</a>

Also see <a href="https://en.wikipedia.org/wiki/Automata_theory" target="_blank">
    Automata Theory
</a>

For more information, please see @ref automatagroup "Automata Module"

@defgroup automatagroup Automata

In computer science, a <a href="https://en.wikipedia.org/wiki/High-level_programming_language">High Level Programming Language</a>
is a computer language with strong abstraction, i.e. a layer of abstraction over the computational device/hardware,
with the aim to simplify programming & human interaction with a computational device, by the language specifications.

"High-level language" refers to the higher-level of abstraction from machine language, which may be cumbersome to read/write from a human-perspective

Rather than dealing with registers, memory addresses, and call stacks, high-level languages deal with program-language-objects,
such as variables, arrays, loops & objects/types, simplifying abstract computer-science-related concepts

This allows a wide audience of non-technical individuals to participate in computer programming.

Also see <a href="https://en.wikipedia.org/wiki/History_of_computing_hardware" target="_blank">The history of computation</a>

endef
export AUTOMATA_HTML

# change this to something more generic ...
define ARCHITECTURE_HTML
@page architecture Architecture

<a href="https://en.wikipedia.org/wiki/Software_architecture" target="_blank">
    Software Architecture
</a>
is the abstract application of designing robust software systems that are maintainable & re-usable & extrapolates easily for future project changes.

For more information, please see @ref architecturegroup "Architecture Module"

@defgroup architecturegroup Architecture

The most common/useful software architecture design pattern/principle is <a href="https://en.wikipedia.org/wiki/Object-oriented_programming" target="_blank">Object Oriented Programming (OOP)</a>

The very first Object Oriented Programming Langauge <a href="https://en.wikipedia.org/wiki/C%2B%2B" target="_blank">C++</a> was first released in 1985, as an extension of the <a href="https://en.wikipedia.org/wiki/C_(programming_language)" target="_blank">C</a> programming language to incorporate Object Oriented design principles

Here is a link to the <a href="http://cppreference.com/" target="_blank">official C++ Language Features Documentation</a>

endef
export ARCHITECTURE_HTML

# release notes redirects to changeLOG & lisence agreement etc ...
define RELEASE_NOTES_HTML
@page releasenotes Release Notes

Any changes made to legal matters are expressed here:

First Release Date: <b>@showdate "%A %d-%m-%Y %H:%M"</b>

@copyright <br/> @ref Lisence "copyright & lisence agreement"

For any changes made to the software since the last release see @ref changeLOG "Change LOG"

<a href="$(WEBSITE_URL)">$(COMPANY_NAME)</a>
endef
export RELEASE_NOTES_HTML

# This defines the main page for grouping all the LOG files in order ...
define CHANGELOG_HTML
@page changeLOG Change LOG

This file keeps a record of all changes made
to the software since release, according to LOG numbers,
each written with the following standard date-time format:

    YYYY-MM-DD H:M:S Surname Name <developer@example.com>

        Fixed
            * bug in <some_source_code_file.cpp>
            
        Added
            * somefunction() in <some_source_code_file.cpp>
            
        Changed
            * <some_class.h> to <some_templated_class_class.h>
        
For any changes made to legal matters, see @ref releasenotes "Release Notes" <br/>
        
<a href="$(WEBSITE_URL)">$(COMPANY_NAME)</a>

<br/>

<h3><b>LOGs</b></h3>
<br>

endef
export CHANGELOG_HTML

# release notes redirects to changeLOG & lisence agreement etc ...
define RELEASE_NOTES_HTML
@page releasenotes Release Notes

Any changes made to legal matters are expressed here:

First Release Date: <b>@showdate "%A %d-%m-%Y %H:%M"</b>

@copyright <br/> @ref Lisence "copyright & lisence agreement"

For any changes made to the software since the last release see @ref changeLOG "Change LOG"

<a href="$(WEBSITE_URL)">$(COMPANY_NAME)</a>
endef
export RELEASE_NOTES_HTML

# Automating resource manifest compilation
define MANIFEST_XML
<?xml version="1.0" encoding="utf-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
  <description>
    $(PROJECT_DESCR)
    <!-- <Website><![CDATA[<a href="$(REPO_LINK)">$(REPO_LINK)</a>]]></Website> -->
  </description>
  <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">
    <application>
      <!-- Supports Windows Vista / Server 2008 -->
      <supportedOS Id="{e2011457-1546-43c5-a5fe-008deee3d3f0}"/>
      <!-- Supports Windows 7 / Server 2008 R2 -->
      <supportedOS Id="{35138b9a-5d96-4fbd-8e2d-a2440225f93a}"/>
      <!-- Supports Windows 8 / Server 2012 -->
      <supportedOS Id="{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}"/>
      <!-- Supports Windows 8.1 / Server 2012 R2 -->
      <supportedOS Id="{1f676c76-80e1-4239-95bb-83d0f6d0da78}"/>
      <!-- Supports Windows 10 -->
      <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>
    </application>
  </compatibility>
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
      <requestedPrivileges>
        <!-- toggle between the two states to indicate for shell whether executable requires elevated priviledge to run program -->
        <!-- requireAdministrator => only admin can run the program -->
        <!-- <requestedExecutionLevel level="requireAdministrator" uiAccess="false"/> -->
        <!-- asInvoker => anyone can run the program -->
        <requestedExecutionLevel level="asInvoker" uiAccess="false"/>
      </requestedPrivileges>
    </security>
  </trustInfo>
  <dependency>
    <dependentAssembly>
      <assemblyIdentity
        type="Win32"
        name="Microsoft.Windows.Common-Controls"
        version="6.0.0.0" processorArchitecture="*"
        publicKeyToken="6595b64144ccf1df" language="*"
      />
    </dependentAssembly>
  </dependency>
</assembly>
endef
export MANIFEST_XML

# To compile windows resource files, we use windres as follows ...
# windres -i rsc/info.rc -O coff -o rsc/info.res

# define a comma symbol to perform substritution on project number 1.2.3.4 into 1,2,3,4
comma:=,
# define info.rc resource file
define INFO_RC
// always include for Win32 CONSTANTS
// OFTEN forgotten include when writing .rc files ...
#include <windows.h>

// enable common controls by linking to the application's manifest ...
CREATEPROCESS_MANIFEST_RESOURCE_ID RT_MANIFEST "$(BINARY).exe.manifest"

// icon.ico can be replaced with ANY icon
// of user's choice or set at project runtime ...
// icon maximum size => 96x96 pixels
APPICON ICON "./img/icon.ico"

// Executable version information.
VS_VERSION_INFO    VERSIONINFO
FILEVERSION        $(subst .,$(comma),$(PROJ_NUM))
PRODUCTVERSION     $(subst .,$(comma),$(PROJ_NUM))
FILEFLAGSMASK      VS_FFI_FILEFLAGSMASK
#ifdef _DEBUG
  FILEFLAGS        VS_FF_DEBUG | VS_FF_PRERELEASE
#else
  FILEFLAGS        0
#endif
FILEOS             VOS_NT_WINDOWS32
FILETYPE           VFT_APP
FILESUBTYPE        VFT2_UNKNOWN
BEGIN
  BLOCK "StringFileInfo"
  BEGIN
    BLOCK "040904b0" // => English US
    // BLOCK "080904b0" // => English UK
    // BLOCK "080904b0" // => English SA
    BEGIN
      VALUE "CompanyName", "$(COMPANY_NAME)"
      VALUE "FileDescription", "$(PROJECT_DESCR)"
      VALUE "FileVersion", "$(PROJ_NUM)"
      VALUE "InternalName", "$(BINARY).exe"
      VALUE "LegalCopyright", "\xA9 $(shell date '+%Y') $(COMPANY_NAME)"
      VALUE "OriginalFilename", "$(BINARY).exe"
      VALUE "ProductName", "$(PROJECT_NAME)"
      VALUE "ProductVersion", "$(PROJ_NUM)"
      VALUE "Language", "English (United States)"
    END
  END
  BLOCK "VarFileInfo"
  BEGIN
    VALUE "Translation", 0x409, 1200 // English US
    // VALUE "Translation", 0x809, 1200 // English UK
    // VALUE "Translation", 0x1c09, 1200 // English SA
  END
END
endef
export INFO_RC

# define variables for .rc file path & manifest file path
RSC_PATH := $(ROOT)/rsc/$(BINARY).exe.manifest
INFO_PATH := $(ROOT)/rsc/info.rc

ICON_PATH := ./dependencies/img/ico/icon.ico
ICON_DEST := ./rsc/img/icon.ico

manifest : ## recipe for generating manifest file in ./rsc/ directory when compiling application for visual styles ...
>@mkdir -p $(ROOT)/rsc/img;\
>[ ! -f $(RSC_PATH) ] && (echo "$$MANIFEST_XML" >> $(RSC_PATH)) || echo "If the $(RSC_PATH) file already exists, it will NOT be overwritten!";\
>[ ! -f $(INFO_PATH) ] && (echo "$$INFO_RC" >> $(INFO_PATH)) || echo "If the $(INFO_PATH) file already exists, it will NOT be overwritten!";\
>"$(MAKE)" copyicon;\

.PHONY : copyicon
copyicon : ## a recipe for copying the `xLib` `icon.ico` to be replaced by any other `icon.ico` file of client choice
>[ ! -f $(ICON_DEST) ] && (echo $(shell cp -f $(ICON_PATH) $(ICON_DEST))) || echo "If the $(ICON_DEST) file already exists, it will NOT be overwritten!";
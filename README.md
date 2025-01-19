# Win32xLib

Win32xLib is a C++ OOP API wrapper library/framework & `makefile` automation tool to streamline the Win32 app build process

`Win32xLib` is a Win32 API `C++` OOP wrapper Library/Framework including a complete `makefile` to automate the project-build,
documentation &amp; `windres` resource management, enabling developers to focus on core application logic

---

## Technologies

-  [xLib](https://github.com/lambdadotjoburg/Win32xLib)
-  [MinGW](https://www.mingw-w64.org/)
  1. g++/gcc
  2. makefile (included with mingw)
  3. windres (included with mingw)
-  [Doxygen](https://www.doxygen.nl/)
  1. [LaTeX](https://www.latex-project.org/)
  2. [graphviz](https://graphviz.org/)
- [git](https://git-scm.com/)

---

To see the project under development, visit the youtube project-build playlists:

[Win32xLib](https://www.youtube.com/watch?v=siGZTPcYIdA&list=PLXijQ1MR10inf0cSEOwmxYdj_482PBYP_)
[makefile](https://youtube.com/playlist?list=PLXijQ1MR10imOVytpU524UlY9UTjKYitn&si=tAtHN-0TCHmj4kla)
[typelib](https://youtube.com/playlist?list=PLXijQ1MR10im6NxQjidjEGFfBYUlVy0O3&si=kBY9mmEFEpCdglmj)
[cmd](https://youtube.com/playlist?list=PLXijQ1MR10insgmvjewdS9KlG7TFSUL3a&si=_MB6dj9jcLD5lOKn)

## Core Library Features:

- Quick-launch/setup Win32 `WinMain` GUI & `main` console applications.
- Automates project setup, doxygen, `windres` resource compilation & simplifies the overall Win32 build processes.
- Industrial-quality, scalable project design.
- Provides a minimalistic, developer-centric interface.
- run `make help` to see a list of all useful `make` recipies

---

## Automating `Win32`/`C++` programming using `makefile`

`xLib` is written to build both `ANSI` & `UNICODE` applications

`xLib` contains is a quick-project launch/setup/init `makefile` "library management system"
that automates `C`/`C++` project-related task-management processes.

`xLib` enables programmers to write & build clean, efficient & scalable "production-quality" applications
using a single `makefile` setup for easy deployment, compilation & documentation.

The project aims to *reduce the burden* on programmers by automating repetitive tasks required to build
& maintain scalable applications.

The `makefile` recipes streamline project setup/deployment/compilation
& documentation, allowing developers to focus on core logic.

The project helps maintain simple-yet-powerful "high-quality"/"scalable" `C++` applications
by providing developer-friendly makefile recipies for quick-setup & mundane/routine compilation steps.

It simplifies project maintenance using macro principles and abstracts away from
intricate project-related complexities/nuances for easy maintenance.

---

## `makefile` usage:

Setting up the project:

Use any of the following commands to pull the raw `xLib` `makefile` from the public repository into a `Project` root-folder ...

```bash
curl https://raw.githubusercontent.com/lambdadotjoburg/Win32xLib/main/makefile >> makefile
```

or

```bash
wget https://raw.githubusercontent.com/lambdadotjoburg/Win32xLib/main/makefile >> makefile
```

The following `make` recipe pulls the `xLib` project source code into the `<Project Folder>` ...

```bash
make dependencies
```

It is recommended to only pull the `Win32xLib/dependencies/` folder using the above `make dependencies` recipe,
since this will ensure that all the packaged data respects the project folder hierarchy!

*"Under the hood"* (obscured from the developer) the following `git` (required) instructions are performed by the `makefile`:

```bash
.PHONY : dependencies

# see https://www.github.com/lambdadotjoburg/Win32xLib.git
# define variable to store github repo link
DEPENDENCIES_REPO := https://github.com/lambdadotjoburg/Win32xLib

# https://stackoverflow.com/questions/37353458/cloning-a-single-file-from-github-repo
# wget https://raw.githubusercontent.com/jquery/jquery/master/src/ajax.js
dependencies : ## recipe for setting up the `xLib` dependencies in the correct folder
	mkdir -p Project/dependencies;\
	git init;\
	git remote add origin $(DEPENDENCIES_REPO);\
	git config core.sparsecheckout true;\
	echo "Project/" >> .git/info/sparse-checkout;\
	git pull origin main;\
	rm -rf .git;\
```

To clone the entire repository, do as usualy ...

```bash
git clone https://github.com/lambdadotjoburg/Win32xLib.git
```

To initialize a new project, just simply do:

```bash
make init
```

The developer is then requested to provide the following project details used to automate important project tasks:

  - Enter Project Name: My First Win32 Project
  - Enter Project Number: 1.2.3.4
  - Enter Author Name: your-full-name-and-surname
  - Enter Project Description: This is your project description
  - Enter REPO Link: https://github.com/your-github-username/your-github-repo

After the details are completed, a simple `WinMain` program entry point is generated in the project root folder & automatically links to the appropriate `xLib` resources `./dependencies/xLib/xApp.h` [single include header file]

To explicitly generate a Win32 application program entry-point, run:
```bash
make Win32
```

To compile the application in debug mode, run:

```bash
make
```

This step will produce the executable `main.exe` in the folder `./bin/debug/main.exe` but the resulting .exe is an `ANSI`/`ASCI` build

Alternatively, to compile the application in `UNICODE` mode, run:

```bash
make win
```

To compile the program in release mode, run:

```bash
make app
```

To create a project LOG file, run:

```bash
make LOG
```

The log info is taken into account when generating documentation for the project, using the `make doxygen command`

---

### Unicode Macro Definition

The following macros are defined by default when compiling with `make win` or `make app`:
  
```cpp
#ifndef UNICODE
#define UNICODE
#endif  

#ifndef _UNICODE
#define _UNICODE
#endif
```

So, there's no need to define the `UNICODE` MACRO symbols in the project header, ***in fact***, it should NOT be defined in any header files!

If you're developing on `vscode` with the `C++` extension loaded in the project, locate the file `.vscode/c_cpp_properties.json` & remove the attribute:

```json
"defines": [
  "_DEBUG",
  "UNICODE"
  "_UNICODE"
]
```

The `makefile` will take care of all the `UNICODE` & `_UNICODE` MACRO's when running `make win` for `UNICODE` builds

---

#### Helpful/Common commands:

To see a list of all the `make` commands/recipes available, run:

```bash
make help
```

To clean up the project, run:

```bash
make clean
```

This will remove all object `.o` & linker files `.d` & resource `.res` files once used to compile the executable.

To generate documentation for the source code, first document the code yourself (obviously), then simply run:

```bash
make doxygen
```

This will produce a `Doxyfile` containing the **MOST** important doxygen directives for producing documentation `html` pages.

Visual elements such as `UML` diagrams & dependency-charts are active if the `graphviz` dependency is installed in it's default location. Additionally, you may want to install `LaTeX` [see above technologies]

---

### Getting started with Win32 programming:

- [The Forget's Win32 Program Tutorials](http://www.winprog.org/tutorial/start.html)
- [Making Win32 API - Being Unicode Friendly](https://cplusplus.com/forum/articles/16820/#google_vignette)
- [Transmission Zero - Building Win32 GUI Applications with MinGW](https://www.transmissionzero.co.uk/computing/win32-apps-with-mingw/)
- [Managing Application State - An Object Oriented Approach](https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-)

--- 

### Git Instructions for Repository Setup:

To commit to the repository:

```bash
> echo "# Win32xLib" >> README.md
> git init
> git add README.md
> git commit -m "first commit"
> git branch -M main
> git remote add origin git@github.com:lambdadotjoburg/Win32xLib.git
> git push -u origin main
```

---

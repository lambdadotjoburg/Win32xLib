// single xApp include header file => clean code style ...
#include "./dependencies/xLib/xApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) {

  // initialize application ...
  // init common controls for visual styles ...
  xApp::get().init(hInstance, hPrevInstance, szCmdLine, nCmdShow);

  // xWidget usage demonstration ...

  // Create a single xFrame object ...
  xFrame parentFrame("Parent Frame", 100, 100, 900, 800);
  parentFrame.show();

  xFrame childFrame(&parentFrame, "Child Frame", 200, 200, 400, 300);
  childFrame.show();

  // We want a type of class that is capable of registering a frame
  // as a child frame, but not necessarily as a subframe contained
  // within the borders of its parents ...
  // xFrame siblingFrame(&parentFrame, "Sibling Frame", 100, 100, 300, 200);
  // siblingFrame.show();

  // xButton parentButton;
  // xButton childButton;
  // xButton siblingButton;
  
  // create a button for testing ...
  // Visual styles will NOT yet be applied to the App
  // First, we must create a manifest file
  // We will automate compilation of resources via the makefile
  CreateWindowEx(
    0,
    WC_BUTTON, // WC_BUTTON => MACRO expanding as "BUTTON" [ANSII] or L"BUTTON"" [UNICODE]
    TEXT("OK"),
    WS_VISIBLE | WS_CHILD,
    100, 100, 50, 25,
    parentFrame.Handle(),
    (HMENU)NULL,
    GetModuleHandle(NULL),
    NULL
  );

  // main message loop ...
  xApp::get().messagepump();
	return EXIT_SUCCESS;
}
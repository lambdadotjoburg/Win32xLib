/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*/
/**
  * @file 	  CliendDemoClass.h
  * @author   Client Name & Surname
  * @date     \showdate "%Y-%m-%d"
  * 
  * @brief 	  This file contains an example class to demonstrate
  *           how client-code may go about extending/inheriting `xFrame`
  * 
  * @details  This file contains an example of a client-defined class
  *           extending/inheriting from `xFrame`, including demonstration
  *           of overriding `HandleMessage(...)`
  *           <br/>
  *           This file is typically located in ./src folder!
  */

#include "../Project/dependencies/xLib/xApp.h"

/**
 * @class    ClientDemoClass
 * @brief    This class demonstrates a possible client-code implementation for extending `xFrame`
 * 
 * @details  This class provides a minimal example of a client-class implemenation
 *           extending/inheriting from `xFrame`, including demonstration of
 *           overriding the necessary virtual base class method `HandleMessage`
 * 
 *  To experiment with this file in a project or getting started,
 *  simply write the following in the `main.cpp` stub code file:
 *  
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *  #include "./src/ClientDemoClass.h"
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  & then instantiate an object of your client-defined type:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *  ClientDemoClass demoFrame("Demo Frame", 500, 400, 100, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Then be sure to create & "show" the window ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *  demoFrame.show(); // "create" & "show"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 */
// extend `xFrame`
class ClientDemoClass : public xFrame {

    // client code data ...

public:

    /// @brief client-defined class constructor
    ClientDemoClass(const std::string& title, int w, int h, int x, int y)
        : xFrame(title, w, h, x, y) {
        // client demo class stuff...
    }

protected:

    /// @brief client-overrided `HandleMessage` (basically copy-paste)
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {
        
        // testing client code override base class `HandleMessage(...)` invokation
        std::cout << "ClientDemoClass::HandleMessage" << std::endl;

        // nothing ...

        // if client-class has no need to implement own message-handling logic,
        // then *at least* define this method & return `xFrame` parent class
        // `HandleMessage(...)`

        // simply return default message handling
        // from `xFrame` parent class
        return xFrame::HandleMessage(msg, wParam, lParam);

    } // end of HandleMessage override demo
};

/**
 * @example ClientDemoClass
 * 
 * This class provides a minimal example of a client-class implementation
 * extending/inheriting from `xFrame`, including demonstration of overriding
 * the three necessary virtual base class method `HandleMessage`
 * 
 *  To experiment with this file in a project or getting started,
 *  simply write the following in the `main.cpp` stub code file:
 *  
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *  #include "./src/ClientDemoClass.h"
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  & then instantiate an object of your client-defined type:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *  ClientDemoClass demoFrame("Demo Frame", 500, 400, 100, 100);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Then be sure to create & "show" the window ...
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *  demoFrame.show(); // "create" & "show"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * // class declaration & definition:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * // extend `xFrame`
 * class ClientDemoClass : public xFrame {
 * 
 *      // client data ...
 * 
 * public:
 * 
 *      // client-defined class constructor
 *      ClientDemoClass(const std::string& title, int w, int h, int x, int y)
 *        : xFrame(title, w, h, x, y) {
 *              // client demo class stuff...
 *      }
 * 
 * protected:
 * 
 *      // client-overrided `HandleMessage`
 *      LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {
 *     
 *          // testing client code override base class `HandleMessage(...)` invokation
 *          std::cout << "ClientDemoClass::HandleMessage" << std::endl;
 * 
 *          // nothing ...
 *          
 *          // if client-class has no need to implement own message-handling logic,
 *          // then *at least* define this method & return `xFrame` parent class
 *          // `HandleMessage(...)`
 * 
 *          return xFrame::HandleMessage(msg, wParam, lParam);
 * 
 *      } // end of HandleMessage override demo
 * 
 * }; // end of ClientDemoClass
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
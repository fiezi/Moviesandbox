#ifndef H_CONSOLE
#define H_CONSOLE

#include "textInputButton.h"
#include <functional>


class Console: public TextInputButton{

typedef void (Console::*callBack) (std::string args);

public:

    map<string, callBack>  functionList;

    Console();
    virtual ~Console();

    virtual void registerProperties();
    virtual void focusClick();
    virtual void callFunction(string command);
    void (Console::*toCall) (std::string args);

    virtual void create();

    void randomSize(string args);
    void dropBird(string args);
    void setZero(string args);
    void teleport(string args);
    void loadScene(string args);
    void loadProject(string args);



};
#endif // CONSOLE

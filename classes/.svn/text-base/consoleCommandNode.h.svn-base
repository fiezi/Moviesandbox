#ifndef H_CONSOLECOMMANDNODE
#define H_CONSOLECOMMANDNODE

#include "node.h"

class ConsoleCommandNode: public Node{

public:

    string consoleCommand;


    ConsoleCommandNode();
    virtual ~ConsoleCommandNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // CONSOLECOMMANDNODE

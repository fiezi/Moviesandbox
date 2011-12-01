#ifndef H_WAITFOREVENTNODE
#define H_WAITFOREVENTNODE

#include "node.h"

class WaitForEventNode: public Node{

public:

    string eventName;

    WaitForEventNode();
    virtual ~WaitForEventNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // WAITFOREVENTNODE

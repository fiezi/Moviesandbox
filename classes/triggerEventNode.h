#ifndef H_TRIGGEREVENTNODE
#define H_TRIGGEREVENTNODE

#include "node.h"

class TriggerEventNode: public Node{

public:

    string eventName;

    TriggerEventNode();
    virtual ~TriggerEventNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // TRIGGEREVENTNODE

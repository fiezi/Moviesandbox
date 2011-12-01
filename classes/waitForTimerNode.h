#ifndef H_WAITFORTIMERNODE
#define H_WAITFORTIMERNODE

#include "node.h"

class WaitForTimerNode: public Node{

public:

    float waitTime;
    double passedTime;

    bool bIsCounting;

    WaitForTimerNode();
    virtual ~WaitForTimerNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();

    virtual void execute();
    virtual void update(double deltaTime);

    virtual void create();
};
#endif

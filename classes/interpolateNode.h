#ifndef H_INTERPOLATENODE
#define H_INTERPOLATENODE

#include "node.h"
#include "interpolationHelper.h"

class InterpolateNode: public Node{

public:

    Actor* moveActor;
    Actor* targetActor;

    float moveTime;
    bool bNeedMover;

    InterpolateNode();
    ~InterpolateNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // INTERPOLATENODE

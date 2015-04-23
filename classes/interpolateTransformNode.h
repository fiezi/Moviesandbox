#ifndef H_INTERPOLATETRANSFORMNODE
#define H_INTERPOLATETRANSFORMNODE

#include "node.h"
#include "interpolationHelper.h"

class InterpolateTransformNode: public Node{

public:

    Actor* moveActor;

    float moveTime;

    Vector3f loc;
    Vector3f rot;
    Vector3f scl;

    bool bNeedMover;
    //hack for turning differently...
    bool bTurnOnFly;

    InterpolateTransformNode();
    ~InterpolateTransformNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif //INTERPOLATETRANSFORMNODE

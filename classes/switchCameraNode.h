#ifndef H_SWITCHCAMERANODE
#define H_SWITCHCAMERANODE

#include "node.h"

class SwitchCameraNode: public Node{

public:

    Actor* cameraActor;

    SwitchCameraNode();
    ~SwitchCameraNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);
    virtual void trigger(Actor * other);
    virtual void create();
};
#endif // SWITCHCAMERANODE

#ifndef H_ATTACHTONODE
#define H_ATTACHTONODE

#include "node.h"

class AttachToNode: public Node{

public:

    Actor* applyTo;
    Actor* target;

    Actor* oldBase;

    Vector3f offset;
    Vector3f newRot;
    Vector3f oldLocation;
    Vector3f oldRotation;

    AttachToNode();
    virtual ~AttachToNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // ATTACHTONODE

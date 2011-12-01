#ifndef H_SETANIMATIONNODE
#define H_SETANIMATIONNODE

#include "node.h"

class SetAnimationNode: public Node{

public:

    Actor* applyTo;
    string animName;

    bool bLoop;

    float inPoint;

    SetAnimationNode();
    virtual ~SetAnimationNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // SETANIMATIONNODE

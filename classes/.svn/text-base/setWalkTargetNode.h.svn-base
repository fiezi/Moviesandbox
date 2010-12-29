#ifndef H_SETWALKTARGETNODE
#define H_SETWALKTARGETNODE

#include "node.h"

class SetWalkTargetNode: public Node{

public:

    Actor* applyTo;
    Actor* walkTarget;

    SetWalkTargetNode();
    virtual ~SetWalkTargetNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // SETWALKTARGETNODE

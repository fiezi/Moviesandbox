#ifndef H_WALKTONODE
#define H_WALKTONODE

#include "node.h"

class WalkToNode: public Node{

public:

    Actor* applyTo;
    Actor* target;

    WalkToNode();
    virtual ~WalkToNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // WALKTONODE

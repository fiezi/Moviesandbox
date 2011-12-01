#ifndef H_STOPANIMNODE
#define H_STOPANIMNODE

#include "node.h"

class StopAnimNode: public Node{

public:

    Actor* applyTo;

    StopAnimNode();
    virtual ~StopAnimNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // STOPANIMNODE

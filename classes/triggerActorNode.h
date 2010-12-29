#ifndef H_TRIGGERACTORNODE
#define H_TRIGGERACTORNODE

#include "node.h"

class TriggerActorNode: public Node{

public:

    Actor* applyTo;

    TriggerActorNode();
    virtual ~TriggerActorNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // TRIGGERACTORNODE

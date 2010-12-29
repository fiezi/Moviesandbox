#ifndef H_PERFORMACTIONNODE
#define H_PERFORMACTIONNODE

#include "node.h"
#include "action.h"
#include "interpolationHelper.h"

class PerformActionNode: public Node{

public:

    Actor*                  applyTo;
    std::string             myAction;

    bool                    bWaitToBeFinished;
    bool                    bPlaying;
    bool                    bRelative;

    PerformActionNode();
    virtual ~PerformActionNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void trigger(Actor * other);

    virtual void create();
};
#endif // PERFORMACTIONNODE

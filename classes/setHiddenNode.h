#ifndef H_SETHIDDENNODE
#define H_SETHIDDENNODE

#include "node.h"

class SetHiddenNode: public Node{

public:

    Actor* applyTo;
    bool setHidden;
    bool bHideGroup;

    bool originalHidden;

    SetHiddenNode();
    virtual ~SetHiddenNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // SETHIDDENNODE

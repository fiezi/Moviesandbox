#ifndef H_SETPROPERTYNODE
#define H_SETPROPERTYNODE

#include "node.h"

class SetPropertyNode: public Node{

public:

    string  propertyToSet;
    string  propValue;
    Actor*  applyTo;

    SetPropertyNode();
    virtual ~SetPropertyNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // SETPROPERTYNODE

#ifndef H_LOADNODE
#define H_LOADNODE

#include "node.h"

class LoadNode: public Node{

public:

    string fileName;

    LoadNode();
    virtual ~LoadNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // LOADNODE

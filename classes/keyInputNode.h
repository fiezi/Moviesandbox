#ifndef H_KEYINPUTNODE
#define H_KEYINPUTNODE

#include "rootNode.h"

class KeyInputNode: public RootNode{

public:

    std::string keyInput;

    KeyInputNode();
    ~KeyInputNode();

    virtual void registerProperties();

    virtual void execute();

    virtual void create();
};
#endif // KEYINPUTNODE

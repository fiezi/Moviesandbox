#ifndef H_NODE
#define H_NODE

#include "listButton.h"
#include "nodeIO.h"

class Node: public ListButton{

    public:

    NodeIO * nodeIn;
    NodeIO * nodeOut;

    Node * childNode;
    Node * treeStart;

    bool bKeyInputBranch;               //determines if this node is part of a keyInput tree (those trees don't get deleted after they're done, but will be reset!)


    int nodeBranch;

    Node();
    virtual ~Node();

    virtual void registerProperties();

    virtual void setup();
    virtual void update(double deltaTime);
    virtual void drawPlane();

    virtual void connectChild(Node* connectNode);

    virtual void mouseOver();
    virtual void mouseDrag();

    virtual void clickedLeft();
    virtual void clickedRight();

    virtual void trigger(Actor * other);

    virtual void start();
    virtual void stop();

    virtual void execute();

    virtual void nextNode();
    virtual void removeBranch();
    virtual void resetBranch();         //resets a keyInput tree to make sure you can press your key a couple of times!

    virtual void remove();

    virtual void create();
    virtual TiXmlElement* save(TiXmlElement *root);
};
#endif

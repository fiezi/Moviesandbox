#ifndef H_CREATENODEBUTTON
#define H_CREATENODEBUTTON

#include "basicButton.h"
#include "node.h"

class CreateNodeButton: public BasicButton{

public:

    Node * myNode;

    CreateNodeButton();
    virtual ~CreateNodeButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);
    virtual void create();
};
#endif // CREATENODEBUTTON

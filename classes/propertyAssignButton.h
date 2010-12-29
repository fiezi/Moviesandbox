#ifndef H_PROPERTYASSIGNBUTTON
#define H_PROPERTYASSIGNBUTTON

#include "basicButton.h"

class PropertyAssignButton: public BasicButton{

public:

    PropertyAssignButton();
    virtual ~PropertyAssignButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();

    virtual void mouseDrag();
    virtual void finishDrag();

    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);
    virtual void create();
};
#endif // PROPERTYASSIGNBUTTON

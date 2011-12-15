#ifndef H_PICKWORLDBUTTON
#define H_PICKWORLDBUTTON

#include "basicButton.h"

class PickWorldButton: public BasicButton{

public:

    PickWorldButton();
    virtual ~PickWorldButton();

    virtual void setup();
    virtual void drawPlane();
    virtual void update(double deltaTime);

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);
    virtual void create();
};
#endif // PICKWORLDBUTTON

#ifndef H_SETBASEBUTTON
#define H_SETBASEBUTTON

#include "basicButton.h"

class SetBaseButton: public BasicButton{

public:

    SetBaseButton();
    virtual ~SetBaseButton();

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
#endif // SETBASEBUTTON

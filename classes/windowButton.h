#ifndef H_WINDOWBUTTON
#define H_WINDOWBUTTON

#include "basicButton.h"

class WindowButton: public ListButton{

public:

    WindowButton();
    virtual ~WindowButton();

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
#endif // WINDOWBUTTON

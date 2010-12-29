#ifndef H_USERPOPUP
#define H_USERPOPUP

#include "textInputButton.h"

class UserPopUp: public TextInputButton{

public:

    bool bWaitForInput;

    UserPopUp();
    virtual ~UserPopUp();

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
#endif // USERPOPUP

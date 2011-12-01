#ifndef H_CREATEACTIONBUTTON
#define H_CREATEACTIONBUTTON

#include "textInputButton.h"

class CreateActionButton: public TextInputButton{

public:

    CreateActionButton();
    virtual ~CreateActionButton();

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
#endif // CREATEACTIONBUTTON

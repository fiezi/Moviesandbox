#ifndef H_MENUBAR
#define H_MENUBAR

#include "basicButton.h"

class MenuBar: public BasicButton{

public:

    MenuBar();
    virtual ~MenuBar();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();


    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();

    virtual void trigger(MsbObject* other);

    virtual void deselect(int depth);

    virtual void create();
};
#endif // MENUBAR

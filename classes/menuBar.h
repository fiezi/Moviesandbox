#ifndef H_MENUBAR
#define H_MENUBAR

#include "listButton.h"

class MenuBar: public BasicButton{

public:

    vector<ListButton*> menus;
    bool bToggledMenu;
    int activeMenu;

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

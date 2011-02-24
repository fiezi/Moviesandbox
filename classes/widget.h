#ifndef H_WIDGET
#define H_WIDGET

#include "listButton.h"
#include "particleSystem.h"
#include "brush.h"


/************************************************
*   Widgets are used for the left side menus    *
*                                               *
*   Base Class simulates ListButton             *
*                                               *
************************************************/


class Widget: public ListButton{

public:

    Vector3f    widgetLocation;
    bool        bWidgetOpen;            //is this widget open?
    bool        bToggleWidget;          //does this widget close when another widget opens?

    Widget();
    virtual ~Widget();

    virtual void clickedLeft();

    virtual void openWidget();
    virtual void closeWidget();

    virtual void trigger(Actor* other);

    virtual void deselect(int depth);

    virtual void create();
};
#endif // WIDGET

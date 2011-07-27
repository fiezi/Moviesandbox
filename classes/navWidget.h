#ifndef H_NAVWIDGET
#define H_NAVWIDGET

#include "widget.h"

class NavWidget: public Widget{

public:

    int         useTool;

    NavWidget();
    virtual ~NavWidget();

    virtual void clickedLeft();

    virtual void openWidget();
    virtual void closeWidget();
    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // NAVWIDGET

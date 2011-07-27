#ifndef H_SELECTWIDGET
#define H_SELECTWIDGET

#include "widget.h"

class SelectWidget: public Widget{

public:

    int         useTool;

    SelectWidget();
    virtual ~SelectWidget();

    virtual void clickedLeft();

    virtual void openWidget();
    virtual void closeWidget();
    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // NAVWIDGET

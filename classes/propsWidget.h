#ifndef H_PROPSWIDGET
#define H_PROPSWIDGET

#include "widget.h"

class PropsWidget: public Widget{

public:

    PropsWidget();
    virtual ~PropsWidget();

    virtual void setup();

    virtual void openWidget();
    virtual void closeWidget();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // PROPSWIDGET

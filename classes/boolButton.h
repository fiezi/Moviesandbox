#ifndef H_BOOLBUTTON
#define H_BOOLBUTTON

#include "basicButton.h"

class BoolButton: public BasicButton{

public:

    bool* boolValue;

    BoolButton();
    virtual ~BoolButton();

    virtual void setup();

    virtual void drawPlane();

    virtual void clickedLeft();

    virtual void create();
};
#endif // BOOLBUTTON

#ifndef H_VECTORINPUTBUTTON
#define H_VECTORINPUTBUTTON

#include "listButton.h"

class VectorInputButton: public ListButton{

public:

    int         vecLength;                      //vector3 or vector4?

    VectorInputButton();
    virtual ~VectorInputButton();

    virtual void registerProperties();
    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();

    virtual void mouseDrag();
    virtual void finishDrag();

    virtual void clickedLeft();
    virtual void clickedRight();

    virtual void focusClick();

    virtual void assembleList();

    virtual void remove();
    virtual void create();
};
#endif // VECTORINPUTBUTTON

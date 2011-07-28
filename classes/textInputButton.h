#ifndef H_TEXTINPUTBUTTON
#define H_TEXTINPUTBUTTON

#include "basicButton.h"

class TextInputButton: public BasicButton{

public:

    bool    bEditing;           //are we typing in text?

    bool    bmIDPart;           //only edit a part of a property (e.g. Vector3f)
    int     mIDPartNumber;      //which part of the property to edit

    TextInputButton();
    virtual ~TextInputButton();

    virtual void setup();
    virtual void update(double deltaTime);
    virtual void drawTooltip();

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);
    virtual void create();
};
#endif // TEXTINPUTBUTTON

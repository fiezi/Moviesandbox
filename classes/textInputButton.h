#ifndef H_TEXTINPUTBUTTON
#define H_TEXTINPUTBUTTON

#include "basicButton.h"

class TextInputButton: public BasicButton{

public:

    bool    bEditing;           //are we typing in text?

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

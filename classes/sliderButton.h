#ifndef H_SLIDERBUTTON
#define H_SLIDERBUTTON

#include "basicButton.h"

class SliderButton: public BasicButton{

public:

    float sliderValue;
    bool bVertical;
    bool bFlipValue;

    SliderButton();
    virtual ~SliderButton();

    virtual void setup();
    virtual void update(double deltaTime);
    virtual void drawPlane();

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void finishDrag();
    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);
    virtual void create();
};
#endif // SLIDERBUTTON

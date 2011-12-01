#ifndef H_COLORPICKBUTTON
#define H_COLORPICKBUTTON

#include "basicButton.h"

class ColorPickButton: public BasicButton{

public:

    Vector2f paletteSize;

    ColorPickButton();
    virtual ~ColorPickButton();


    virtual Vector4f pickColor();

    virtual void mouseDrag();
    virtual void finishDrag();
    virtual void clickedLeft();

    virtual void create();
};
#endif // COLORPICKBUTTON

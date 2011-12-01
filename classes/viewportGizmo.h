#ifndef H_VIEWPORTGIZMO
#define H_VIEWPORTGIZMO

#include "basicButton.h"

class ViewportGizmo: public BasicButton{

public:

    GLuint viewport_fb;
    GLuint myDepthBuffer;

    int cubeSide;

    ViewportGizmo();
    virtual ~ViewportGizmo();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();

    virtual void mouseDrag();
    virtual void finishDrag();

    virtual void clickedLeft();
    virtual void clickedRight();

    virtual void focusClick();

    virtual void deselect(int depth);

    virtual void create();
};
#endif // VIEWPORTGIZMO

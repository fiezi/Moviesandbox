#ifndef H_NODEIO
#define H_NODEIO

#include "basicButton.h"

class NodeIO: public BasicButton{
public:

    Vector3f    minScale,
                maxScale;

    bool bDrawLine;
    bool bConnected;

    NodeIO * connector;

    NodeIO();
    virtual ~NodeIO();

    virtual void setup();
    virtual void update(double deltaTime);
    virtual void drawTooltip();
    virtual void drawPlane();

    virtual void mouseOver();
    virtual void clickedLeft();
    virtual void focusClick();

    virtual void remove();
};
#endif

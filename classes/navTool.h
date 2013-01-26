#ifndef H_NAVTOOL
#define H_NAVTOOL

#include "msbTool.h"

class NavTool: public MsbTool{

public:

    Vector2f oldMousePos;
    bool oldBDrawNodes;

    NavTool();
    virtual ~NavTool();

    virtual void setup();

    virtual void start();
    virtual void stop();

    virtual void keyPressed(int key);
    virtual void keyReleased(int key);

    virtual void mousePressed(int btn);
    virtual void mouseReleased(int btn);
    virtual void mouseDragged(int btn);

    virtual void update(double deltaTime);
    virtual void processMove(double deltaTime);
    virtual void processRotation(double deltaTime);
};
#endif // NAVTOOL

#ifndef H_MSBTOOL
#define H_MSBTOOL

#include "msbObject.h"
#include "basicButton.h"

class Control;


class MsbTool: public MsbObject {

public:

    Control* controller;

    BasicButton* myBtn;
    string      highlightBtn;

    bool bPressLeft,bPressRight;
    bool bOverrideOldState;

    MsbTool();
    virtual ~MsbTool();

    virtual void setup();

    virtual void start();
    virtual void stop();

    virtual void keyPressed(int key);
    virtual void keyReleased(int key);

    virtual void mousePressed(int btn);
    virtual void mouseReleased(int btn);
    virtual void mouseDragged(int btn);

    virtual void update(double deltaTime);

    virtual void selectActors(int btn, Actor* other);
    virtual void highlightButton(string btnName);
    virtual void lowlightButton();

    virtual void save();
};
#endif // MSBTOOL

#ifndef H_SETTINGSBUTTON
#define H_SETTINGSBUTTON

#include "inspector.h"

class SettingsButton: public Inspector{

public:

    SettingsButton();
    virtual ~SettingsButton();

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
#endif // SETTINGSBUTTON

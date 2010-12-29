#ifndef H_CREATEPREFABBUTTON
#define H_CREATEPREFABBUTTON

#include "textInputButton.h"

class CreatePrefabButton: public TextInputButton{

public:

    CreatePrefabButton();
    virtual ~CreatePrefabButton();

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
#endif // CREATEPREFABBUTTON

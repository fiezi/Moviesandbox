#ifndef H_CREATEACTORBUTTON
#define H_CREATEACTORBUTTON

#include "basicButton.h"

class CreateActorButton: public BasicButton{

public:

Actor * newActor;

CreateActorButton();
virtual ~CreateActorButton();

virtual void clickedLeft();
virtual void mouseDrag();
virtual void finishDrag();
virtual void focusClick();

virtual void update(double deltaTime);

virtual void create();

};
#endif



#include "assignButton.h"
#include "renderer.h"
#include "input.h"

AssignButton::AssignButton(){}
AssignButton::~AssignButton(){}


void AssignButton::setup(){
BasicButton::setup();

}

void AssignButton::update(double deltaTime){}

void AssignButton::mouseOver(){

BasicButton::mouseOver();
}

void AssignButton::mouseDrag(){

    input->dragButton=this;
    if (bConfineDragX)
        setLocation(Vector3f(input->mouseX, location.y,0));
    else if (bConfineDragY)
        setLocation(Vector3f(location.x, input->mouseY,0));
    else
        setLocation(Vector3f(input->mouseX, input->mouseY,0));

    if (bTriggerWhileDragging && parent)
        parent->trigger(this);
}

void AssignButton::finishDrag(){

if (bResetAfterDrag)
    setLocation(initialLocation);
cout << "setting to: " << initialLocation << endl;
input->dragButton=NULL;

if (parent)
  parent->trigger(this);

}

void AssignButton::clickedLeft(){

if (parent!=NULL)
  parent->trigger(this);
}

void AssignButton::clickedRight(){}
void AssignButton::focusClick(){

BasicButton::focusClick();
}

void AssignButton::deselect(int depth){

BasicButton::deselect(depth);
}

void AssignButton::create(){renderer->addButton(this);}

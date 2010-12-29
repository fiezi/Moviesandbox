

#include "pickWorldButton.h"
#include "renderer.h"
#include "input.h"

PickWorldButton::PickWorldButton(){}
PickWorldButton::~PickWorldButton(){}


void PickWorldButton::setup(){
BasicButton::setup();

}

void PickWorldButton::update(double deltaTime){

if (parent && buttonProperty!="NULL"){
    bOver=true;
    tooltip=memberToString(&parent->property[buttonProperty]);
    }
}

void PickWorldButton::mouseOver(){

BasicButton::mouseOver();
}

void PickWorldButton::mouseDrag(){}
void PickWorldButton::clickedLeft(){

input->focusButton=this;
}

void PickWorldButton::clickedRight(){}
void PickWorldButton::focusClick(){

parent->trigger(this);

//TODO: testing added functionality!
if (parent && buttonProperty!="NULL"){
    if (input->worldTarget && input->worldTarget->name=="ground")
        parent->setActorPropertyTo(&(parent->property[buttonProperty]), NULL);
    else
        parent->setActorPropertyTo(&(parent->property[buttonProperty]), input->worldTarget);
    }

BasicButton::focusClick();
}

void PickWorldButton::deselect(int depth){

BasicButton::deselect(depth);
}

void PickWorldButton::create(){renderer->addButton(this);}

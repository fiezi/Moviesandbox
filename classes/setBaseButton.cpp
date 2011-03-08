

#include "setBaseButton.h"
#include "renderer.h"
#include "input.h"
SetBaseButton::SetBaseButton(){

registerProperties();
}
SetBaseButton::~SetBaseButton(){}


void SetBaseButton::setup(){
BasicButton::setup();

}

void SetBaseButton::update(double deltaTime){}

void SetBaseButton::mouseOver(){

BasicButton::mouseOver();
}

void SetBaseButton::mouseDrag(){}
void SetBaseButton::finishDrag(){}
void SetBaseButton::clickedLeft(){

    input->focusButton=this;
}

void SetBaseButton::clickedRight(){}
void SetBaseButton::focusClick(){

    BasicButton::focusClick();

    if (!parent)
        return;

    parent->setBase(input->worldTarget);
}

void SetBaseButton::deselect(int depth){

BasicButton::deselect(depth);
}

void SetBaseButton::create(){renderer->addButton(this);}

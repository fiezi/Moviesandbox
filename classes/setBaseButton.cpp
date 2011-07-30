

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
    if ( ((Actor*)parent)->base){
        ((Actor*)parent)->base->bHighlight=true;
    }

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

    if (input->worldTarget->base!=(Actor*)parent)
        ((Actor*)parent)->setBase(input->worldTarget);
    else
        sceneData->makeWarningPopUp("Circular connections not allowed!", this);
}

void SetBaseButton::deselect(int depth){

BasicButton::deselect(depth);
}

void SetBaseButton::create(){sceneData->addButton(this);}

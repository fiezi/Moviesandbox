

#include "windowButton.h"
#include "renderer.h"
#include "input.h"
WindowButton::WindowButton(){

registerProperties();
}
WindowButton::~WindowButton(){}


void WindowButton::setup(){
BasicButton::setup();

}

void WindowButton::update(double deltaTime){}

void WindowButton::mouseOver(){

BasicButton::mouseOver();
}

void WindowButton::mouseDrag(){}
void WindowButton::finishDrag(){}
void WindowButton::clickedLeft(){}
void WindowButton::clickedRight(){}
void WindowButton::focusClick(){

BasicButton::focusClick();
}

void WindowButton::deselect(int depth){

BasicButton::deselect(depth);
}

void WindowButton::create(){sceneData->addButton(this);}

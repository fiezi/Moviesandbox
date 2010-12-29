

#include "userPopUp.h"
#include "renderer.h"
#include "input.h"

UserPopUp::UserPopUp(){

name="UserPopUp";
bMessageWindow=true;
bDrawName=true;
bWaitForInput=false;
registerProperties();
}

UserPopUp::~UserPopUp(){}


void UserPopUp::setup(){
TextInputButton::setup();

}

void UserPopUp::update(double deltaTime){

TextInputButton::update(deltaTime);
}

void UserPopUp::mouseOver(){

TextInputButton::mouseOver();
}

void UserPopUp::mouseDrag(){
TextInputButton::mouseDrag();
}

void UserPopUp::finishDrag(){
TextInputButton::finishDrag();
}

void UserPopUp::clickedLeft(){
    input->inputText="";
    TextInputButton::clickedLeft();
}

void UserPopUp::clickedRight(){
TextInputButton::clickedRight();
}

void UserPopUp::focusClick(){

    if (input->inputText=="" && bWaitForInput)
        return;

    if (parent)
        parent->trigger(this);

    //cleanUp
    //we need to pop back before adding the action!
    renderer->buttonList.pop_back();
    input->staticButton=NULL;

    //clean up
    input->inputText="";
    bEditing=false;
    BasicButton::focusClick();
}

void UserPopUp::deselect(int depth){
TextInputButton::deselect(depth);
}

void UserPopUp::create(){renderer->addButton(this);}

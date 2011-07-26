

#include "userPopUp.h"
#include "renderer.h"
#include "input.h"

UserPopUp::UserPopUp(){

name="UserPopUp";
bMessageWindow=true;
bDrawName=true;
bWaitForInput=false;
level=100;
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
    if (bWaitForInput)
        TextInputButton::clickedLeft();
    else
        focusClick();

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
    sceneData->buttonList.pop_back();
    sceneData->staticButton=NULL;

    //clean up
    input->inputText="";
    bEditing=false;
    bWaitForInput=false;
    BasicButton::focusClick();
}

void UserPopUp::deselect(int depth){

    input->inputText="";
    bEditing=false;
    bWaitForInput=false;
    //cleanUp
    //we need to pop back to vanish again!
    sceneData->buttonList.pop_back();
    //remove from sceneData reference!
    sceneData->staticButton=NULL;
    input->focusButton=NULL;
}

void UserPopUp::create(){sceneData->addButton(this);}

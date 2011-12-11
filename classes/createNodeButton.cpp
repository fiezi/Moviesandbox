#include "createNodeButton.h"
#include "renderer.h"
#include "input.h"


CreateNodeButton::CreateNodeButton(){}

CreateNodeButton::~CreateNodeButton(){}


void CreateNodeButton::setup(){

BasicButton::setup();
}

void CreateNodeButton::update(double deltaTime){

    BasicButton::update(deltaTime);

    if (input->focusButton==this && myNode){
      myNode->location.x=input->mouseX;
      myNode->location.y=input->mouseY;
    }

}

void CreateNodeButton::mouseOver(){

BasicButton::mouseOver();
}

void CreateNodeButton::mouseDrag(){}

void CreateNodeButton::clickedLeft(){

    input->focusButton=this;

    createNewActor(buttonProperty);

    //we need to do this to make it possible to generate ScriptNodes (with only one NodeIO)!!!!
    myNode=(Node*) sceneData->nodeList.back();

    cout << "referencing node: " << myNode->name << endl;

    myNode->setup();
}

void CreateNodeButton::clickedRight(){}

void CreateNodeButton::focusClick(){

BasicButton::focusClick();
}

void CreateNodeButton::deselect(int depth){

BasicButton::deselect(depth);
}

void CreateNodeButton::create(){sceneData->addButton(this);}

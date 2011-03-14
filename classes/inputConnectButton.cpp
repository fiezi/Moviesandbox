

#include "inputConnectButton.h"
#include "renderer.h"
#include "udpInput.h"

InputConnectButton::InputConnectButton(){

scale.x=15;
scale.y=15;
scale.z=1;
bDrawName=true;
}
InputConnectButton::~InputConnectButton(){}


void InputConnectButton::setup(){
BasicButton::setup();

}

void InputConnectButton::update(double deltaTime){}

void InputConnectButton::trigger(MsbObject* other){

//cast for parent
UdpInput* p=(UdpInput*)parent;

p->targetActors[listPosition]=(Actor*)other;
p->targetProperties[listPosition]=buttonProperty;

cout << "connected " << p->targetActors[listPosition]-> name << " at " << p->targetProperties[listPosition] << endl;
}


void InputConnectButton::mouseOver(){

BasicButton::mouseOver();
}

void InputConnectButton::mouseDrag(){}
void InputConnectButton::clickedLeft(){}
void InputConnectButton::clickedRight(){}

void InputConnectButton::focusClick(){

BasicButton::focusClick();
}

void InputConnectButton::deselect(int depth){

BasicButton::deselect(depth);
}

void InputConnectButton::create(){sceneData->addButton(this);}


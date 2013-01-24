#include "waitForTimerNode.h"
#include "renderer.h"

WaitForTimerNode::WaitForTimerNode(){
    name="WaitTimer";
    passedTime=0;
    waitTime=0;
    bIsCounting=false;

    listType.push_back("15TextInputButton");
    listName.push_back("WaitTime");
    listProp.push_back("WAITTIME");
    listIcon.push_back("icon_flat");

    registerProperties();
}


WaitForTimerNode::~WaitForTimerNode(){
}

void WaitForTimerNode::registerProperties(){

Node::registerProperties();
createMemberID("WAITTIME",&waitTime,this);
}


void WaitForTimerNode::start(){

Node::start();
passedTime=0;
bIsCounting=false;
}

void WaitForTimerNode::stop(){

Node::stop();
passedTime=0;
bIsCounting=false;
}

void WaitForTimerNode::execute(){

    cout << "waiting... " << passedTime << endl;
    bIsCounting=true;
    if (passedTime>=waitTime){
      passedTime=0.0;
      bIsCounting=false;
      Node::nextNode();
    }
}

void WaitForTimerNode::update(double deltaTime){

    Node::update(deltaTime);
    //cout << "updating..." << passedTime << endl;
    if (bIsCounting)
      passedTime+=deltaTime/1000.0;

}

void WaitForTimerNode::create(){

sceneData->addNode(this);
cout << "created new WaitForTimerButton!" << endl;
}

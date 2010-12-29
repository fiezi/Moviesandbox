#include "waitForTimerNode.h"
#include <renderer.h>

WaitForTimerNode::WaitForTimerNode(){
name="WaitTimer";
passedTime=0;
waitTime=0;
bIsCounting=false;

listType.push_back("15TextInputButton");
listProp.push_back("WAITTIME");

color=Vector4f(0.5,0.5,0.5,1.0);

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

//cout << "waiting... " << passedTime << endl;
bIsCounting=true;
if (passedTime>=waitTime)
  Node::nextNode();
}

void WaitForTimerNode::update(double deltaTime){

Node::update(deltaTime);

if (bIsCounting)
  passedTime+=deltaTime/1000;
}

void WaitForTimerNode::create(){

renderer->addNode(this);
cout << "created new WaitForTimerButton!" << endl;
}

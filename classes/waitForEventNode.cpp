

#include "waitForEventNode.h"
#include "control.h"
#include "renderer.h"
#include "input.h"
WaitForEventNode::WaitForEventNode(){

    color=Vector4f(0.5,0.3,0.5,1);
    name="WaitForEvent";
    eventName="NULL";

    listType.push_back("15TextInputButton");
    listName.push_back("eventName");
    listProp.push_back("EVENTNAME");
    listIcon.push_back("icon_flat");

    registerProperties();
}

WaitForEventNode::~WaitForEventNode(){}

void WaitForEventNode::registerProperties(){

createMemberID("EVENTNAME",&eventName,this);
Node::registerProperties();
}

void WaitForEventNode::start(){

color=Vector4f(0.5,0.3,0.5,1);

Node::start();
}

void WaitForEventNode::stop(){

Node::stop();
}

void WaitForEventNode::execute(){


        if (sceneData->controller->eventTrigger.size()>0){
            for (int i=0;i<(int)sceneData->controller->eventTrigger.size();i++){
                if (sceneData->controller->eventTrigger[i]==eventName){
                    cout << "triggered Event: " << eventName << endl;
                    nextNode();
                }
            }
        }
}


void WaitForEventNode::update(double deltaTime){

	Node::update(deltaTime);
}

void WaitForEventNode::create(){sceneData->addNode(this);}

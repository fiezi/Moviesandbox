

#include "triggerEventNode.h"
#include "control.h"
#include "renderer.h"
#include "input.h"
TriggerEventNode::TriggerEventNode(){


    name="Trigger Event";
    eventName="NULL";

   listType.push_back("15TextInputButton");
   listName.push_back("eventName");
   listProp.push_back("EVENTNAME");
   listIcon.push_back("icon_flat");

    registerProperties();
}

TriggerEventNode::~TriggerEventNode(){}

void TriggerEventNode::registerProperties(){


createMemberID("EVENTNAME",&eventName,this);
Node::registerProperties();
}

void TriggerEventNode::start(){

Node::start();
}

void TriggerEventNode::stop(){

Node::stop();
}

void TriggerEventNode::execute(){

        if (eventName!="NULL")
            sceneData->eventTrigger.push_back(eventName);
        nextNode();
}


void TriggerEventNode::update(double deltaTime){

	Node::update(deltaTime);
}

void TriggerEventNode::create(){sceneData->addNode(this);}

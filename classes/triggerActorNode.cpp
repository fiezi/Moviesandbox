

#include "triggerActorNode.h"
#include "control.h"
#include "renderer.h"

TriggerActorNode::TriggerActorNode(){

    name="triggerActor";
    applyTo=NULL;

    listType.push_back("15PickWorldButton");
    listName.push_back("apply To");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

registerProperties();
}

TriggerActorNode::~TriggerActorNode(){}

void TriggerActorNode::registerProperties(){
createMemberID("APPLYTO",&applyTo,this);
Node::registerProperties();
}

void TriggerActorNode::start(){

Node::start();
}

void TriggerActorNode::stop(){

Node::stop();
}

void TriggerActorNode::execute(){

        if (applyTo)
            applyTo->trigger(this);

        nextNode();
}


void TriggerActorNode::update(double deltaTime){

	Node::update(deltaTime);
}

void TriggerActorNode::create(){sceneData->addNode(this);}

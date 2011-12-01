

#include "attachToNode.h"
#include "control.h"
#include "renderer.h"

AttachToNode::AttachToNode(){
    name="AttachTo";

    applyTo=NULL;
    target=NULL;
    oldBase=NULL;

    listType.push_back("15PickWorldButton");
    listName.push_back("applyTo");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

    listType.push_back("15PickWorldButton");
    listName.push_back("target");
    listProp.push_back("TARGET");
    listIcon.push_back("icon_flat");


    listType.push_back("15TextInputButton");
    listName.push_back("offset");
    listProp.push_back("OFFSET");
    listIcon.push_back("icon_flat");

    listType.push_back("15TextInputButton");
    listName.push_back("rotation");
    listProp.push_back("NEWROT");
    listIcon.push_back("icon_flat");


    registerProperties();}

AttachToNode::~AttachToNode(){}

void AttachToNode::registerProperties(){


createMemberID("NEWROT",&newRot,this);
createMemberID("OFFSET",&offset,this);
createMemberID("APPLYTO",&applyTo,this);
createMemberID("TARGET",&target,this);

Node::registerProperties();
}

void AttachToNode::start(){

    oldLocation=applyTo->location;
    oldRotation=applyTo->rotation;
    oldBase=applyTo->base;

Node::start();
}

void AttachToNode::stop(){

    applyTo->setLocation(oldLocation);
    applyTo->setRotation(oldRotation);
    applyTo->base=oldBase;

Node::stop();
}

void AttachToNode::execute(){

    if (applyTo && target){
        applyTo->base=target;
        applyTo->setLocation(offset);
        applyTo->setRotation(newRot);
    }
    nextNode();

}


void AttachToNode::update(double deltaTime){

	Node::update(deltaTime);
}

void AttachToNode::create(){sceneData->addNode(this);}

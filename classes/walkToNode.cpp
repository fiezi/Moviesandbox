
#include "renderer.h"
#include "walkToNode.h"
#include "control.h"
#include "character.h"

WalkToNode::WalkToNode(){

    applyTo=NULL;
    target=NULL;

    listType.push_back("15PickWorldButton");
    listName.push_back("apply To");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

    listType.push_back("15PickWorldButton");
    listName.push_back("walk target");
    listProp.push_back("TARGET");
    listIcon.push_back("icon_flat");


    registerProperties();
}

WalkToNode::~WalkToNode(){}

void WalkToNode::registerProperties(){

    createMemberID("APPLYTO",&applyTo,this);
    createMemberID("TARGET",&target,this);

Node::registerProperties();
}

void WalkToNode::start(){

Node::start();
}

void WalkToNode::stop(){

Node::stop();
}

void WalkToNode::execute(){

        Character* myChar=NULL;
        myChar=dynamic_cast<Character*>(applyTo);
        if (myChar && target)
            myChar->walkTarget=target;
        nextNode();

}


void WalkToNode::update(double deltaTime){

    Node::update(deltaTime);
}

void WalkToNode::create(){sceneData->addNode(this);}

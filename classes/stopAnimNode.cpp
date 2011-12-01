

#include "stopAnimNode.h"
#include "control.h"
#include "renderer.h"
#include "character.h"

StopAnimNode::StopAnimNode(){

    color=Vector4f(0.1,0.1,0.4,1.0);
    name="stopAnim";
    applyTo=NULL;

    listType.push_back("15PickWorldButton");
    listName.push_back("apply To");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

    registerProperties();
}

StopAnimNode::~StopAnimNode(){}

void StopAnimNode::registerProperties(){

createMemberID("APPLYTO",&applyTo,this);
Node::registerProperties();
}

void StopAnimNode::start(){

color=Vector4f(0.1,0.1,0.4,1.0);
Node::start();
}

void StopAnimNode::stop(){

Node::stop();
}

void StopAnimNode::execute(){

    Character* myChar=NULL;
    myChar=dynamic_cast<Character*>(applyTo);
    if (!myChar)
        cout << "not assigned to character!" << endl;

    if (myChar){
        myChar->stopAnim();
    }

    nextNode();
}


void StopAnimNode::update(double deltaTime){

	Node::update(deltaTime);
}

void StopAnimNode::create(){sceneData->addNode(this);}

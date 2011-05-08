

#include "SetWalkTargetNode.h"
#include "control.h"
#include "renderer.h"
#include "character.h"

SetWalkTargetNode::SetWalkTargetNode(){

    name ="setWalkTarget";
    applyTo=NULL;
    walkTarget=NULL;

    listType.push_back("15PickWorldButton");
    listName.push_back("applyTo");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

    listType.push_back("15PickWorldButton");
    listName.push_back("walkTarget");
    listProp.push_back("WALKTARGET");
    listIcon.push_back("icon_flat");

registerProperties();
}

SetWalkTargetNode::~SetWalkTargetNode(){}

void SetWalkTargetNode::registerProperties(){


createMemberID("APPLYTO",&applyTo,this);
createMemberID("WALKTARGET",&walkTarget,this);

Node::registerProperties();
}

void SetWalkTargetNode::start(){

Node::start();
}

void SetWalkTargetNode::stop(){

    Character* character = dynamic_cast<Character*>(applyTo);
    if (character){
        character->walkTarget=NULL;
    }
    Node::stop();
}

void SetWalkTargetNode::execute(){

    Character* character = dynamic_cast<Character*>(applyTo);
    if (character){
        character->walkTarget=walkTarget;
    }else{
        cout << "applyTo is not a character!" << endl;
    }
    nextNode();
}


void SetWalkTargetNode::update(double deltaTime){

	Node::update(deltaTime);
}

void SetWalkTargetNode::create(){sceneData->addNode(this);}

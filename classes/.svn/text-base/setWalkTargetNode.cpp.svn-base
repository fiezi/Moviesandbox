

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

listType.push_back("15PickWorldButton");
listName.push_back("walkTarget");
listProp.push_back("WALKTARGET");

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

void SetWalkTargetNode::create(){renderer->addNode(this);}

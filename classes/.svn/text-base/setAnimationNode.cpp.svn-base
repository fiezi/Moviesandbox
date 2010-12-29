

#include "setAnimationNode.h"
#include "control.h"
#include "character.h"
#include "renderer.h"

SetAnimationNode::SetAnimationNode(){

name="setAnim";
applyTo=NULL;
color=Vector4f(0.1,0.1,0.8,1.0);
animName="NULL";
bLoop=false;
inPoint=0.0f;

    listType.push_back("15PickWorldButton");
    listName.push_back("apply To");
    listProp.push_back("APPLYTO");

    listType.push_back("15TextInputButton");
    listName.push_back("animName");
    listProp.push_back("ANIMNAME");

    listType.push_back("15TextInputButton");
    listName.push_back("inPoint");
    listProp.push_back("INPOINT");

    listType.push_back("15TextInputButton");
    listName.push_back("bLoop");
    listProp.push_back("BLOOP");

registerProperties();
}

SetAnimationNode::~SetAnimationNode(){}

void SetAnimationNode::registerProperties(){

createMemberID("APPLYTO",&applyTo, this);
createMemberID("ANIMNAME",&animName, this);
createMemberID("BLOOP",&bLoop, this);
createMemberID("INPOINT",&inPoint, this);
Node::registerProperties();
}

void SetAnimationNode::start(){

color=Vector4f(0.1,0.1,0.8,1.0);

Node::start();
}

void SetAnimationNode::stop(){

Node::stop();
}

void SetAnimationNode::execute(){


    Character* myChar=NULL;
    myChar=dynamic_cast<Character*>(applyTo);
    if (!myChar)
        cout << "not assigned to character!" << endl;

    if (myChar && renderer->actionList[animName]){
        myChar->playAnim(renderer->actionList[animName], bLoop, inPoint * 1000);
    }

    nextNode();

}


void SetAnimationNode::update(double deltaTime){

	Node::update(deltaTime);
}

void SetAnimationNode::create(){renderer->addNode(this);}

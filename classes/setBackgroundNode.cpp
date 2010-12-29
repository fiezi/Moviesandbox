

#include "setBackgroundNode.h"
#include "control.h"
#include "renderer.h"

SetBackgroundNode::SetBackgroundNode(){
name="setBackground";
texName="NULL";

listType.push_back("15TextInputButton");
listName.push_back("texName");
listProp.push_back("TEXNAME");

color=Vector4f(0.5,0.5,0.5,1.0);
registerProperties();
}

SetBackgroundNode::~SetBackgroundNode(){}

void SetBackgroundNode::registerProperties(){

createMemberID("TEXNAME",&texName,this);
Node::registerProperties();
}

void SetBackgroundNode::start(){

Node::start();
}

void SetBackgroundNode::stop(){

Node::stop();
}

void SetBackgroundNode::execute(){

    if (texName!="NULL" && renderer->textureList[texName])
        renderer->backgroundTex=texName;

    nextNode();
}


void SetBackgroundNode::update(double deltaTime){

	Node::update(deltaTime);
}

void SetBackgroundNode::create(){renderer->addNode(this);}

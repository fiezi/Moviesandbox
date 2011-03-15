

#include "loadNode.h"
#include "control.h"
#include "renderer.h"

LoadNode::LoadNode(){

fileName="NULL";
registerProperties();

}

LoadNode::~LoadNode(){}

void LoadNode::registerProperties(){

createMemberID("FILENAME",&fileName,this);
Node::registerProperties();
}

void LoadNode::start(){

Node::start();
}

void LoadNode::stop(){

Node::stop();
}

void LoadNode::execute(){}


void LoadNode::update(double deltaTime){

	Node::update(deltaTime);
}

void LoadNode::create(){sceneData->addNode(this);}

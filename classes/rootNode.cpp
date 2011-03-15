#include "rootNode.h"
#include "control.h"
#include "renderer.h"

RootNode::RootNode(){

name="RootNode";
color=Vector4f(0.5,0.5,1.0,1.0);
treeStart=this;
}

RootNode::~RootNode(){
}

void RootNode::setup(){

Node::setup();
nodeIn->remove();
nodeIn=NULL;
}

void RootNode::start(){

Node::start();
nodeBranch=Control::nodeTree.size();
Control::nodeTree.push_back(this);
}

void RootNode::execute(){

Node::nextNode();
}


void RootNode::create(){

sceneData->addNode(this);
std::cout << "creating a RootNode!" << endl;
}

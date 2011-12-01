

#include "loadNode.h"
#include "control.h"
#include "renderer.h"
#include "loadButton.h"

LoadNode::LoadNode(){

fileName="NULL";
registerProperties();

    listType.push_back("15TextInputButton");
    listName.push_back("filename");
    listProp.push_back("FILENAME");
    listIcon.push_back("icon_flat");

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

void LoadNode::execute(){

    if (fileName!="NULL"){
        LoadButton* lb=(LoadButton*)sceneData->actorInfo["10LoadButton"].actorReference;
        sceneData->controller->startMovie();
        lb->loadFile(fileName,true);

    }

}


void LoadNode::update(double deltaTime){

	Node::update(deltaTime);
}

void LoadNode::create(){sceneData->addNode(this);}

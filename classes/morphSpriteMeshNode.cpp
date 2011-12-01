

#include "morphSpriteMeshNode.h"
#include "control.h"
#include "renderer.h"

MorphSpriteMeshNode::MorphSpriteMeshNode(){

name = "morph";
color= Vector4f(0.4, 0.0, 0.0, 1.0);
morphOne=NULL;
morphTwo=NULL;

particleScaleOne=0.0;
particleScaleTwo=0.0;

particleAngleScaleOne=0.0;
particleAngleScaleTwo=0.0;

morphRate= 0.001;
morphAngleRate= 0.0001;

listType.push_back("15PickWorldButton");
listName.push_back("morphOne");
listProp.push_back("MORPHONE");

listType.push_back("15PickWorldButton");
listName.push_back("morphTwo");
listProp.push_back("MORPHTWO");

listType.push_back("15TextInputButton");
listName.push_back("morphRate");
listProp.push_back("MORPHRATE");

listType.push_back("15TextInputButton");
listName.push_back("morphAngleRate");
listProp.push_back("MORPHANGLERATE");

registerProperties();
}

MorphSpriteMeshNode::~MorphSpriteMeshNode(){}

void MorphSpriteMeshNode::registerProperties(){

createMemberID("MORPHRATE",&morphRate,this);
createMemberID("MORPHANGLERATE",&morphAngleRate,this);
createMemberID("MORPHONE",&morphOne,this);
createMemberID("MORPHTWO",&morphTwo,this);
Node::registerProperties();
}

void MorphSpriteMeshNode::start(){

particleScaleTwo=morphTwo->particleScale;
particleScaleOne=morphOne->particleScale;

particleAngleScaleTwo=morphTwo->particleAngleScale;
particleAngleScaleOne=morphOne->particleAngleScale;

morphTwo->particleScale=0.0;
morphTwo->particleAngleScale=0.0;
Node::start();
}

void MorphSpriteMeshNode::stop(){


morphTwo->particleScale=particleScaleTwo;
morphOne->particleScale=particleScaleOne;

morphTwo->particleAngleScale=particleAngleScaleTwo;
morphOne->particleAngleScale=particleAngleScaleOne;
Node::stop();
}

void MorphSpriteMeshNode::execute(){

        if (morphOne->particleScale>0.0)
            morphOne->particleScale-=morphRate;

        if (morphOne->particleAngleScale>0.0)
            morphOne->particleAngleScale-=morphAngleRate;

        if (morphTwo->particleScale<particleScaleTwo)
            morphTwo->particleScale+=morphRate;

        if (morphTwo->particleAngleScale<particleAngleScaleTwo)
            morphTwo->particleAngleScale+=morphAngleRate;

        if (morphOne->particleScale<0.0 &&
            morphOne->particleAngleScale<0.0 &&
            morphTwo->particleScale>particleScaleTwo &&
            morphTwo->particleAngleScale>particleAngleScaleTwo)
            nextNode();
}


void MorphSpriteMeshNode::update(double deltaTime){

	Node::update(deltaTime);
}

void MorphSpriteMeshNode::create(){sceneData->addNode(this);}



#include "SetHiddenNode.h"
#include "control.h"
#include "renderer.h"

SetHiddenNode::SetHiddenNode(){

    name="SetHidden";
    applyTo=NULL;
    setHidden=true;
    bHideGroup=false;
    originalHidden=false;

    listType.push_back("15PickWorldButton");
    listName.push_back("applyTo");
    listProp.push_back("APPLYTO");

    listType.push_back("15TextInputButton");
    listName.push_back("setHidden");
    listProp.push_back("SETHIDDEN");

    listType.push_back("15TextInputButton");
    listName.push_back("hideGroup");
    listProp.push_back("BHIDEGROUP");

    registerProperties();
}

SetHiddenNode::~SetHiddenNode(){}

void SetHiddenNode::registerProperties(){

    createMemberID("APPLYTO",&applyTo,this);
    createMemberID("SETHIDDEN",&setHidden,this);
    createMemberID("BHIDEGROUP",&bHideGroup,this);
    Node::registerProperties();
}

void SetHiddenNode::start(){

    if (applyTo)
        originalHidden=applyTo->bHidden;

    Node::start();
}

void SetHiddenNode::stop(){

    if (applyTo){
            if (bHideGroup){
                for (int i=0;i<(int)renderer->actorList.size();i++){
                    if (renderer->actorList[i]->groupID==applyTo->groupID)
                        renderer->actorList[i]->bHidden=originalHidden;
                }
            }else{
                applyTo->bHidden=originalHidden;
            }
    }
    Node::stop();
}

void SetHiddenNode::execute(){

        if (applyTo){
            if (bHideGroup){
                for (int i=0;i<(int)renderer->actorList.size();i++){
                    if (renderer->actorList[i]->groupID==applyTo->groupID)
                        renderer->actorList[i]->bHidden=setHidden;
                }
            }else{
                applyTo->bHidden=setHidden;
            }

        }
        else
            cout << "no actor selected!" << endl;

        nextNode();
}


void SetHiddenNode::update(double deltaTime){

	Node::update(deltaTime);
}

void SetHiddenNode::create(){renderer->addNode(this);}

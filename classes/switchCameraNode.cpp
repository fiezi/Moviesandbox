

#include "switchCameraNode.h"
#include "control.h"
#include "renderer.h"
#include "input.h"
#include "cameraActor.h"

SwitchCameraNode::SwitchCameraNode(){

    name="SwitchCam";
    cameraActor=NULL;
    focus=15.0;
    oldTool=TOOL_SELECT;
    bCameraShake=false;

    listType.push_back("15PickWorldButton");
    listName.push_back("cameraActor");
    listProp.push_back("CAMERAACTOR");
    listIcon.push_back("icon_flat");

    listType.push_back("15TextInputButton");
    listName.push_back("focus");
    listProp.push_back("FOCUS");
    listIcon.push_back("icon_flat");

    listType.push_back("15TextInputButton");
    listName.push_back("cameraShake");
    listProp.push_back("BCAMERASHAKE");
    listIcon.push_back("icon_flat");

    color=Vector4f(0.5,0.5,0.5,1.0);
    registerProperties();
    controller=sceneData->controller;
}

SwitchCameraNode::~SwitchCameraNode(){}

void SwitchCameraNode::registerProperties(){

Node::registerProperties();
createMemberID("CAMERAACTOR",&cameraActor,this);
createMemberID("FOCUS",&focus,this);
createMemberID("BCAMERASHAKE",&bCameraShake,this);
}

void SwitchCameraNode::start(){

    if (cameraActor){
        cameraActor->drawType=DRAW_NULL;
        sceneData->updateView();
        oldTool=controller->tool;
    }
    Node::start();
}

void SwitchCameraNode::stop(){

    if (cameraActor){
        //controller->switchTool(TOOL_SELECT);
        cameraActor->drawType=DRAW_VBOMESH;
        Node::stop();
        sceneData->controller->controlledActor=sceneData->controller;
        CameraActor* cA=dynamic_cast<CameraActor*>(cameraActor);
        if (cA){
            renderer->fov=45;
            cA->bCameraShake=false;
        }
        renderer->focus=25.0;

        controller->switchTool(oldTool) ;
    }

}

void SwitchCameraNode::execute(){
    sceneData->controller->controlledActor=cameraActor;

    controller->switchTool(TOOL_NAV);
    sceneData->updateView();
    CameraActor* cA=dynamic_cast<CameraActor*>(cameraActor);
    if (cA){
        renderer->fov=cA->fov;
        cA->bCameraShake=bCameraShake;
    }
    renderer->focus=focus;
    nextNode();
}


void SwitchCameraNode::trigger(MsbObject* other){

    if (other==listButton[0] && input->worldTarget){
        cameraActor=input->worldTarget;
        cout << "SwitchCameraNode triggered!!!" << cameraActor->name << endl;
        }
    }

void SwitchCameraNode::update(double deltaTime){
    Node::update(deltaTime);
}

void SwitchCameraNode::create(){sceneData->addNode(this);}

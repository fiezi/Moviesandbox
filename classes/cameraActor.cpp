

#include "cameraActor.h"
#include "renderer.h"
#include "input.h"
CameraActor::CameraActor(){

    drawType=DRAW_VBOMESH;
    vboMeshID="camera";
    color=Vector4f(0.0,0.8,1.0,1.0);
    bPossessed=false;
    bCameraShake=false;

    name="camera";
    sceneShaderID="color";


    menuType.push_back("12AssignButton");
    menuName.push_back("possess");
    menuProp.push_back("NULL");

    registerProperties();
}

CameraActor::~CameraActor(){}

void CameraActor::registerProperties(){


createMemberID("BCAMERASHAKE",&bCameraShake,this);
Actor::registerProperties();
}

void CameraActor::setup(){

noise.setup(4,2.0,1.0,1);

Actor::setup();
}

void CameraActor::trigger(MsbObject* other){

    if (other->name=="possess"){
        cout << "possessing!" << endl;
        input->controller->controlledActor=this;
//        input->controller->navBtn->processMove(0);
        drawType=DRAW_NULL;
        bPossessed=true;
        //input->controller->toggleFlyMode();
    }
    else{
        bCameraShake=!bCameraShake;
    }
}

void CameraActor::update(double deltaTime){

    if (bPossessed && input->startPressLeftBtn>0){
        bPossessed=false;
        input->controller->controlledActor=input->controller;
        drawType=DRAW_VBOMESH;
//        input->controller->navBtn->processMove(0);
    }

    if (Control::bRunning && bCameraShake){
        location.y+=noise.get(renderer->currentTime * 0.01,0.7) * 0.25;
        location.x+=noise.get((renderer->currentTime-0.5) * 0.01,0.3) * 0.25;
    }

    Actor::update(deltaTime);
}

void CameraActor::create(){sceneData->addActor(this);}



#include "cameraActor.h"
#include "renderer.h"
#include "input.h"
CameraActor::CameraActor(){

    drawType=DRAW_VBOMESH;
    vboMeshID="camera";
    color=Vector4f(0.0,0.8,1.0,1.0);
    bPossessed=false;
    bCameraShake=false;

    fov=45;
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
createMemberID("FOV",&fov,this);
Actor::registerProperties();
}

void CameraActor::setup(){

noise.setup(4,2.0,1.0,1);

Actor::setup();
}

void CameraActor::trigger(MsbObject* other){


    if (other && other->name=="possess"){
        cout << "possessing!" << endl;
        sceneData->controller->controlledActor=this;
//        sceneData->controller->navBtn->processMove(0);
        drawType=DRAW_NULL;
        bPossessed=true;
        sceneData->updateView();
        renderer->fov=fov;
        //sceneData->controller->toggleFlyMode();
    }
    else{
        //bCameraShake=!bCameraShake;
    }
}

void CameraActor::update(double deltaTime){

    if (bPossessed)
        sceneData->updateView();


    if (bPossessed && input->startPressLeftBtn>0){
        bPossessed=false;
        sceneData->controller->controlledActor=sceneData->controller;
        renderer->fov=45;

        drawType=DRAW_VBOMESH;
        sceneData->updateView();
    }

    if (Control::bRunning && bCameraShake){
        location.y+=noise.get(renderer->currentTime * 0.01,0.7) * 0.025;
        location.x+=noise.get((renderer->currentTime-0.5) * 0.01,0.3) * 0.025;
        setLocation(location);
    }

    //TODO: hack to make Cameras disappear!
    if (Control::bRunning)
        drawType=DRAW_NULL;
    else
        drawType=DRAW_VBOMESH;


    Actor::update(deltaTime);
}

void CameraActor::create(){sceneData->addActor(this);}

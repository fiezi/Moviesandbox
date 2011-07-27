

#include "orbitTool.h"
#include "renderer.h"
#include "sceneData.h"
#include "input.h"

OrbitTool::OrbitTool(){

    bOverrideOldState=false;
    amountX=-M_PI_4/2.0;
    amountY=M_PI_4/2.0;
    distance=20.0;
}

OrbitTool::~OrbitTool(){}

void OrbitTool::start(){

    NavTool::start();
    //amount=0.0;
}


void OrbitTool::update(double deltaTime){

    MsbTool::update(deltaTime);

    if (sceneData->selectedActors.size()>0)
        lookPoint=sceneData->selectedActors[0]->location;
    else
        lookPoint=Vector3f(0,3,0);

    if (bPressLeft && input->hudTarget==NULL){
        processRotation(deltaTime);
    }
    if (bPressRight && input->hudTarget==NULL){
        input->keyVector=input->mouseVector * 0.5;
        if (!input->bShiftDown){
            input->keyVector.z=-input->keyVector.y;
            input->keyVector.y=0.0;
        }
    }

    processMove(deltaTime);
}

void OrbitTool::processRotation(double deltaTime){

    amountX-=input->mouseVector.x * sceneData->mouseSensitivity * 0.01 * deltaTime;
    amountY-=input->mouseVector.y * sceneData->mouseSensitivity * 0.01 * deltaTime;

    input->bConfineMouse=true;

}

void OrbitTool::processMove(double deltaTime){


    Vector3f xAxis,yAxis,zAxis;


    distance+=input->keyVector.z * 0.01 * deltaTime;

    amountX+=input->keyVector.x * 0.001 * deltaTime;
    amountY+=input->keyVector.y * 0.001 * deltaTime;

    ///moving
    Actor* cam = sceneData->controller->controlledActor;

    Vector3f myLoc;
    myLoc=Vector3f(sin(amountX) * cos(amountY) ,sin(amountY),cos(amountX) * cos(amountY));
    myLoc.normalize();
    zAxis=-myLoc;
    yAxis=Vector3f(0,1,0);
    xAxis=yAxis.crossProduct(zAxis);
    yAxis=zAxis.crossProduct(xAxis);

    myLoc=myLoc * distance + lookPoint;

    xAxis.normalize();
    yAxis.normalize();
    zAxis.normalize();

    cam->setLocation(myLoc);
    cam->setRotation(xAxis,yAxis,zAxis);

    cam->orientation=cam->location+cam->zAxis;

    sceneData->controller->lookPoint=cam->location+cam->zAxis;
    //up Point is absolute!
    sceneData->controller->upPoint=cam->yAxis;

    //TODO: why is this necessary?
    if (cam==sceneData->controller)
        sceneData->controller->upPoint=Vector3f(0,1,0);


}



#include "navTool.h"
#include "renderer.h"
#include "sceneData.h"
#include "input.h"

NavTool::NavTool(){

    bOverrideOldState=false;
}

NavTool::~NavTool(){}

void NavTool::setup(){

    MsbTool::setup();
    processMove(0.0);
}

void NavTool::start(){

    MsbTool::start();
    oldMousePos.x=input->mouseX;
    oldMousePos.y=input->mouseY;

    myBtn->color=COLOR_RED;

}

void NavTool::stop(){

    MsbTool::stop();

    //reset color of navigation button
    myBtn->color=COLOR_WHITE;

    glutWarpPointer(oldMousePos.x,oldMousePos.y);

}

void NavTool::keyPressed(int key){

    MsbTool::keyPressed(key);
}

void NavTool::keyReleased(int key){

    MsbTool::keyReleased(key);
}

void NavTool::mousePressed(int btn){

    MsbTool::mousePressed(btn);
    if (input->startPressLeftBtn==0){
        oldMousePos.x=input->mouseX;
        oldMousePos.y=input->mouseY;
    }


}

void NavTool::mouseReleased(int btn){

    MsbTool::mouseReleased(btn);

    //reset old cursor position

    glutWarpPointer(oldMousePos.x,oldMousePos.y);
}

void NavTool::mouseDragged(int btn){

    MsbTool::mouseDragged(btn);
}


void NavTool::update(double deltaTime){

  MsbTool::update(deltaTime);


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

void NavTool::processRotation(double deltaTime){

    Vector3f axis;

    float amount=0.0f;
    amount=-input->mouseVector.x * sceneData->mouseSensitivity;
    controller->controlledActor->addRotation(amount, Vector3f(0,1,0));


    amount=-input->mouseVector.y * sceneData->mouseSensitivity;
    controller->controlledActor->addRotation(amount, controller->controlledActor->xAxis);

    input->bConfineMouse=true;


}

void NavTool::processMove(double deltaTime){

    Matrix4f moveMatrix;
    Vector3f newLocation;

    deltaTime=deltaTime*0.001;


    ///moving
    Actor* controlledActor = sceneData->controller->controlledActor;

    Vector3f myTranslation=controlledActor->baseMatrix.getTranslation();

    //move forwards
    myTranslation-=controlledActor->zAxis*input->keyVector.z*sceneData->moveSpeed*deltaTime;
    //move sideways
    myTranslation-=controlledActor->xAxis*input->keyVector.x*sceneData->moveSpeed*deltaTime;
    //move up/down
    myTranslation.y+=input->keyVector.y*sceneData->moveSpeed*deltaTime;


    controlledActor->setLocation(myTranslation);
    controlledActor->orientation=controlledActor->zAxis;

    sceneData->controller->lookPoint=myTranslation+controlledActor->zAxis;
    //up Point is absolute!
    sceneData->controller->upPoint=controlledActor->yAxis;

    //TODO: why is this necessary?
    if (controlledActor==sceneData->controller)
        sceneData->controller->upPoint=Vector3f(0,1,0);


}

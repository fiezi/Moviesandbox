

#include "navTool.h"
#include "renderer.h"
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


    #ifdef TARGET_WIN32

	SetCursorPos((int)(input->windowX+oldMousePos.x), (int)(input->windowY+oldMousePos.y));
    #endif

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

    #ifdef TARGET_WIN32

	SetCursorPos((int)(input->windowX+oldMousePos.x), (int)(input->windowY+oldMousePos.y));
    #endif

}

void NavTool::mouseDragged(int btn){

    MsbTool::mouseDragged(btn);
}


void NavTool::update(double deltaTime){

  MsbTool::update(deltaTime);

  if (bPressLeft && input->hudTarget==NULL){
    processRotation(deltaTime);
    input->confineMouse();
  }
  if (bPressRight && input->hudTarget==NULL){
    input->keyVector=input->mouseVector * 0.5;
    if (!input->bShiftDown){
        input->keyVector.z=-input->keyVector.y;
        input->keyVector.y=0.0;
    }
    input->confineMouse();
  }

  processMove(deltaTime);

}

void NavTool::processRotation(double deltaTime){

    Vector3f axis;

    float amount=input->mouseVector.x * renderer->mouseSensitivity;

    amount=-input->mouseVector.x * renderer->mouseSensitivity;
    controller->controlledActor->addRotation(amount, Vector3f(0,1,0));

    amount=-input->mouseVector.y * renderer->mouseSensitivity;
    controller->controlledActor->addRotation(amount, controller->controlledActor->xAxis);

}

void NavTool::processMove(double deltaTime){

    Matrix4f moveMatrix;
    Vector3f newLocation;

    deltaTime=deltaTime*0.001;


    ///moving
    //ha! that would be too easy

    Actor* controlledActor = input->controller->controlledActor;

    Vector3f myTranslation=controlledActor->baseMatrix.getTranslation();

    //move forwards
    myTranslation-=controlledActor->zAxis*input->keyVector.z*renderer->moveSpeed*deltaTime;
    //move sideways
    myTranslation-=controlledActor->xAxis*input->keyVector.x*renderer->moveSpeed*deltaTime;
    //move up/down
    myTranslation.y+=input->keyVector.y*renderer->moveSpeed*deltaTime;

    controlledActor->setLocation(myTranslation);
    controlledActor->orientation=controlledActor->zAxis;

    input->controller->lookPoint=myTranslation+controlledActor->zAxis;
    //up Point is absolute!
    input->controller->upPoint=controlledActor->yAxis;

    //TODO: why is this necessary?
    if (controlledActor==input->controller)
        input->controller->upPoint=Vector3f(0,1,0);


}

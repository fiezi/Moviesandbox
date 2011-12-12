#include "rotateButton.h"
#include "input.h"


RotateButton::RotateButton(){

bActive=false;

name="ROTATION";
}

RotateButton::~RotateButton(){}


//TODO: implement Grid in RotateButton
void RotateButton::update(double deltaTime){

    BasicButton::update(deltaTime);

    if (bActive && parent){
        buttonColor.a=0.15;
        mouseOverColor.a=0.15;

        Actor* actorParent=(Actor*)parent;
        Vector3f axis;

        float amount=input->mouseVector.x * sceneData->mouseSensitivity;

            Vector3f xA,yA,zA;
            actorParent->getAxis(&xA,&yA,&zA,actorParent->transformMatrix);

            if (input->bCtrlDown)
                axis=zA;
            else if (input->bShiftDown)
                axis=xA;
            else
                axis=yA;

            actorParent->addRotation(amount, axis);

        input->bConfineMouse=true;
        return;
    }
}

//start rotation around different axis
void RotateButton::clickedLeft(){

    bActive=true;
    Actor* actorParent=(Actor*)parent;

    if (actorParent==sceneData->controller){
        fineRotation.push_back(actorParent->rotation);
        fineLocation.push_back(actorParent->location);
    }
    else{
        for (int i=0;i<(int)sceneData->selectedActors.size();i++){
            fineRotation.push_back(sceneData->selectedActors[i]->rotation);
            fineLocation.push_back(sceneData->selectedActors[i]->location);
        }
    }

    initialLocation=location;

    if (!input->bWarpMouse){
        bDragable=true;
        scale.x=renderer->windowX;
        scale.y=renderer->windowY;
        location.x=0;
        location.y=0;
        location.z=-1000;
        buttonColor.a=0.15;
        mouseOverColor.a=0.15;
        setLocation(location);
        bActive=false;
    }
    else
        input->focusButton=this;

    glutSetCursor(GLUT_CURSOR_CYCLE);
}

//this is used for connecting to an inputConnectButton
void RotateButton::clickedRight(){

    if (input->focusButton)
        return;

    bActive=false;
    input->focusButton=this;
    cout << "connecting..." << input->hudTarget << endl;
}


void RotateButton::mouseDrag(){

    cout << "dragging" <<  bActive << endl;
    input->focusButton=NULL;
    input->dragButton=this;
    bActive=true;


    //setLocation(Vector3f(input->mouseX-scale.x/2, input->mouseY-scale.y/2,0));

}

void RotateButton::finishDrag(){

    input->focusButton=this;
    bActive=false;

    //setLocation(initialLocation);

    cout << "finishing drag!" << endl;
}

//here we connect to the specific button we clicked on (after right clicking the rotate icon)
void RotateButton::focusClick(){

    BasicButton::focusClick();
    //we're now connecting to a inputConnectButton!!!
    if (!bActive){
        if (input->hudTarget && input->hudTarget->isA("inputConnectButton")){   //connecting to UdpInput
            cout << "connecting with: " << input->hudTarget << endl;
            input->hudTarget->buttonProperty=name;                //which property do we want to connect?
            input->hudTarget->trigger(parent);                          //which actor do we want to connect
        }

     }
    //we've just moved the Actor
    if (bActive)
      bActive=false;
    cout << "Rotation Button inActive! " << endl;

    setLocation(initialLocation);
    scale=Vector3f(30.0f,30.0f,30.0f);
    buttonColor.a=1.0;
    mouseOverColor.a=1.0;

    fineRotation.clear();
    fineLocation.clear();
    bDragable=false;

    glutSetCursor(GLUT_CURSOR_INHERIT);


}

void RotateButton::create(){

sceneData->addButton(this);
cout << "creating a RotateButton!" << endl;
}

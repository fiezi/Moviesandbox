#include "rotateButton.h"
#include "input.h"


RotateButton::RotateButton(){

bActive=false;
name="ROTATION";
}

RotateButton::~RotateButton(){}


//TODO: implement Grid in RotateButton
void RotateButton::update(double deltaTime){

    if (bActive && parent){

        Vector3f axis;

        float amount=input->mouseVector.x * renderer->mouseSensitivity;

            Vector3f xA,yA,zA;
            parent->getAxis(&xA,&yA,&zA,parent->transformMatrix);

            if (input->bCtrlDown)
                axis=zA;
            else if (input->bShiftDown)
                axis=xA;
            else
                axis=yA;

            parent->addRotation(amount, axis);

        input->bConfineMouse=true;
        return;
    }
}

//start rotation around different axis
void RotateButton::clickedLeft(){

    bActive=true;
    input->focusButton=this;

    if (parent==input->controller){
        fineRotation.push_back(parent->rotation);
        fineLocation.push_back(parent->location);
    }
    else{
        for (int i=0;i<(int)input->selectedActors.size();i++){
            fineRotation.push_back(input->selectedActors[i]->rotation);
            fineLocation.push_back(input->selectedActors[i]->location);
        }
    }

    glutSetCursor(GLUT_CURSOR_CYCLE);
}

//this is used for connecting to an inputConnectButton
void RotateButton::clickedRight(){

bActive=false;
input->focusButton=this;
cout << "connecting..." << input->hudTarget << endl;
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
    cout << "inActive!";

    fineRotation.clear();
    fineLocation.clear();
    //input->deselectActors();
    glutSetCursor(GLUT_CURSOR_INHERIT);
}

void RotateButton::create(){

renderer->addButton(this);
cout << "creating a RotateButton!" << endl;
}

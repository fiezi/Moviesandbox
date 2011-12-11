

#include "pickWorldButton.h"
#include "renderer.h"
#include "input.h"

PickWorldButton::PickWorldButton(){}
PickWorldButton::~PickWorldButton(){}


void PickWorldButton::setup(){
    BasicButton::setup();

}

void PickWorldButton::update(double deltaTime){

    BasicButton::update(deltaTime);

    if (parent && buttonProperty!="NULL"){
        bOver=true;
        tooltip=memberToString(&parent->property[buttonProperty]);
    }
}

void PickWorldButton::mouseOver(){

    BasicButton::mouseOver();


    if ( parent && buttonProperty!="NULL" && parent->property[buttonProperty].memberType->name() == typeid(Actor*).name() ){
        if (parent->property[buttonProperty].memberReference){
           Actor* myActor=readActor((char*)memberToString( &(parent->property[buttonProperty])).c_str());
           if (myActor)
                myActor->bHighlight=true;
        }
    }
}

void PickWorldButton::mouseDrag(){}
void PickWorldButton::clickedLeft(){

    input->focusButton=this;
}

void PickWorldButton::clickedRight(){}
void PickWorldButton::focusClick(){

            //make sure we're not selecting gizmos if we want to select an actor!
if (input->worldTarget == sceneData->aGizmo->xAxisGizmo ||
        input->worldTarget == sceneData->aGizmo->yAxisGizmo ||
        input->worldTarget == sceneData->aGizmo->zAxisGizmo ||

        input->worldTarget == sceneData->aGizmo->xRotateGizmo ||
        input->worldTarget == sceneData->aGizmo->yRotateGizmo ||
        input->worldTarget == sceneData->aGizmo->zRotateGizmo
        ){
            //trickery! worldTarget should be our actor to spawn menu for, not the gizmo!
            input->worldTarget=sceneData->selectedActors[0];
    }


    parent->trigger(this);

    //TODO: testing added functionality!
    if (parent && buttonProperty!="NULL"){
        if (input->worldTarget && input->worldTarget->name=="ground")
            parent->setActorPropertyTo(&(parent->property[buttonProperty]), NULL);
        else
            parent->setActorPropertyTo(&(parent->property[buttonProperty]), input->worldTarget);

    }

    BasicButton::focusClick();
}

void PickWorldButton::deselect(int depth){

BasicButton::deselect(depth);
}

void PickWorldButton::create(){sceneData->addButton(this);}

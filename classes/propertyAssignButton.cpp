#include "propertyAssignButton.h"
#include "renderer.h"
#include "input.h"
#include "boneActor.h"

PropertyAssignButton::PropertyAssignButton(){}
PropertyAssignButton::~PropertyAssignButton(){}


void PropertyAssignButton::setup(){
BasicButton::setup();

}

void PropertyAssignButton::update(double deltaTime){}

void PropertyAssignButton::mouseOver(){

BasicButton::mouseOver();
}

void PropertyAssignButton::mouseDrag(){

    input->dragButton=this;
    setLocation(Vector3f(input->mouseX-scale.x/2,input->mouseY-scale.y/2,0));
}

void PropertyAssignButton::finishDrag(){

    cout << "finished dragging..." << endl;

    if (input->worldTarget){
        Actor* a=input->worldTarget;

        //special case for bones! Assign to parent character if present...
        BoneActor* b = dynamic_cast<BoneActor*>(a);
        if (b && b->parent)
            b->parent->memberFromString(&b->parent->property[buttonProperty], name);
        else
            a->memberFromString(&a->property[buttonProperty], name);
    }
    else
        cout << "not on worldTarget" << endl;
    setLocation(initialLocation);
}

void PropertyAssignButton::clickedLeft(){}
void PropertyAssignButton::clickedRight(){}
void PropertyAssignButton::focusClick(){

BasicButton::focusClick();
}

void PropertyAssignButton::deselect(int depth){

BasicButton::deselect(depth);
}

void PropertyAssignButton::create(){sceneData->addButton(this);}

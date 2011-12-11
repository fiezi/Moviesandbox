

#include "boolButton.h"
#include "renderer.h"
#include "input.h"
BoolButton::BoolButton(){


    registerProperties();
}
BoolButton::~BoolButton(){}

void BoolButton::setup(){

    BasicButton::setup();

    if (parent && buttonProperty!="NULL"){
        memberID * mID=&parent->property[buttonProperty];
        if (mID)
            boolValue=(bool*)mID->memberReference;
        }
}

void BoolButton::drawPlane(){

    float oldScaleX= scale.x;
    scale.x-=18;
    BasicButton::drawPlane();
    scale.x=oldScaleX;

    Vector4f tfColor;

    if (*boolValue)
        tfColor=sceneData->boolYesColor;
    else
        tfColor=sceneData->boolNoColor;

    //draw a little square right next to the button
    renderer->drawPlane(scale.x -16.0,0.0,scale.x,scale.y, tfColor );

}

void BoolButton::clickedLeft(){

    *boolValue=!*boolValue;

}


void BoolButton::create(){sceneData->addButton(this);}

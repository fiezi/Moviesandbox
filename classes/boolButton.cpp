

#include "boolButton.h"
#include "renderer.h"
#include "input.h"
BoolButton::BoolButton(){


    registerProperties();
}
BoolButton::~BoolButton(){}

void BoolButton::setup(){

    if (parent && buttonProperty!="NULL"){
        memberID * mID=&parent->property[buttonProperty];
        if (mID)
            boolValue=(bool*)mID->memberReference;
        }
}

void BoolButton::drawPlane(){

    BasicButton::drawPlane();

    Vector4f tfColor;

    if (*boolValue)
        tfColor=Vector4f(1.0,0.0,0.0,1.0);
    else
        tfColor=Vector4f(0.2,0.2,0.2,1.0);

    //draw a little square right next to the button
    renderer->drawPlane(scale.x +2.0,0.0,scale.x+16.0+2.0,scale.y, tfColor );

}

void BoolButton::clickedLeft(){

    *boolValue=!*boolValue;

}


void BoolButton::create(){sceneData->addButton(this);}

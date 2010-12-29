

#include "colorPickButton.h"
#include "renderer.h"
#include "input.h"
#include "drawingWidget.h"

ColorPickButton::ColorPickButton(){

//TODO: Needs bDragable to make color selection smoother
scale.x=256;
scale.y=128;
textureID="icon_palette";
bDragable=true;
}
ColorPickButton::~ColorPickButton(){}



void ColorPickButton::mouseDrag(){

input->dragButton=this;

    if (input->controller->tool==TOOL_SELECT){
        if (input->selectedActors.size()>0){
                for (int i=0;i<(int)input->selectedActors.size();i++){
                    input->selectedActors[i]->color=pickColor();
                }
        }
    }else if (input->controller->tool==TOOL_DRAW){
        renderer->brush->color=pickColor();
    }
    cout << "picked color: "<< pickColor() << endl;

    if (parent){
        parent->color=pickColor();
        parent->trigger(this);
    }

}
void ColorPickButton::finishDrag(){

input->dragButton=NULL;
}

void ColorPickButton::clickedLeft(){


    if (input->controller->tool==TOOL_SELECT){
        if (input->selectedActors.size()>0){
                for (int i=0;i<(int)input->selectedActors.size();i++){
                    input->selectedActors[i]->color=pickColor();
                }
        }
    }else if (input->controller->tool==TOOL_DRAW){
        renderer->brush->color=pickColor();
    }
    cout << "picked color: "<< pickColor() << endl;

    if (parent){
        parent->color=pickColor();
        parent->trigger(this);
    }
}

Vector4f ColorPickButton::pickColor(){

Vector4f myColor;

float hue= input->mouseX-location.x;
float bright= input->mouseY - location.y;

cout << "Color: " << hue << " " << bright << endl;


float greyArea=(scale.x/128)*16;
float colorArea=scale.x-greyArea;
float colorStep=colorArea/3;

myColor=Vector4f(0,0,0,1);

//R
if (hue<colorStep)
  myColor.r=1-(hue/colorStep);
if (hue>2*colorStep)
  myColor.r=(hue-2*colorStep)/(colorStep);
if (myColor.r<0) myColor.r=0;

//G
if (hue<colorStep)
  myColor.g=hue/colorStep;
if (hue>=colorStep)
  myColor.g=1-(hue-colorStep)/colorStep;
if (myColor.g<0) myColor.g=0;

//B
if (hue<2*colorStep)
  myColor.b=(hue-colorStep)/colorStep;
if (hue>=2*colorStep)
  myColor.b=1-(hue-2*colorStep)/colorStep;
if (myColor.b<0) myColor.b=0;

bright=bright/scale.y;        //Values from 0 to 1
bright=(bright-0.5)*(2);            //gives values from -1 to 1

Vector4f brightness=Vector4f(1,1,1,1)*bright;
myColor+=brightness;
myColor.a=1;
if (myColor.r>1) myColor.r=1;
if (myColor.g>1) myColor.g=1;
if (myColor.b>1) myColor.b=1;

return myColor;
}


void ColorPickButton::create(){renderer->addButton(this);}

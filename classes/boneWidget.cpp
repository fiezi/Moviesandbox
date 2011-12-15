

#include "boneWidget.h"
#include "renderer.h"
#include "input.h"
#include "brush.h"
#include "spriteMeshLoader.h"

BoneWidget::BoneWidget(){

name="boneWidget";
tooltip="bones (x)";

bWidgetOpen=false;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;
color=listColor;
//highlightColor=Vector4f(0.8,0.0,0.0,1.0);

useTool=TOOL_BONE;
}

BoneWidget::~BoneWidget(){}


void BoneWidget::clickedLeft(){

    color=COLOR_RED;
    Widget::clickedLeft();

    if (sceneData->controller->tool!=useTool)
        sceneData->controller->switchTool(useTool);

}

void BoneWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
}

void BoneWidget::closeWidget(){

    //input->deselectActors();
    color=COLOR_WHITE;

}

void BoneWidget::trigger(MsbObject* other){

    if (other->name=="Paint Weights"){
        useTool=TOOL_SKIN;
        setTextureID("icon_paintWeights");
    }

    if (other->name=="Create Bone"){
        useTool=TOOL_BONE;
        setTextureID("icon_addBones");
    }

    clickedRight();
    clickedLeft();

}

void BoneWidget::highlight(Actor* other){

    /*
        for (int i=0;i<(int)listButton.size();i++)
            if (listButton[i]!=other && listButton[i]->color==highlightColor)
                listButton[i]->color=listColor;

        other->color=highlightColor;
    */
}

void BoneWidget::create(){sceneData->addButton(this);}

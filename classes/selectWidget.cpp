

#include "selectWidget.h"
#include "renderer.h"
#include "input.h"
#include "brush.h"
#include "spriteMeshLoader.h"

SelectWidget::SelectWidget(){

name="selectWidget";
tooltip="select";

bWidgetOpen=false;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;
color=listColor;

useTool=TOOL_SELECT;
}

SelectWidget::~SelectWidget(){}


void SelectWidget::clickedLeft(){

    Widget::clickedLeft();
    sceneData->controller->switchTool(useTool);
    color=COLOR_RED;
}

void SelectWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
}

void SelectWidget::closeWidget(){

    //input->deselectActors();
    color=COLOR_WHITE;
}

void SelectWidget::trigger(MsbObject* other){

    if (other->name=="Select Objects"){
        useTool=TOOL_SELECT;
        textureID="icon_select";
    }

    if (other->name=="Select Particles"){
        useTool=TOOL_PARTICLESELECT;
        textureID="icon_selectParticles";
    }

    clickedRight();
    clickedLeft();

}

void SelectWidget::create(){sceneData->addButton(this);}

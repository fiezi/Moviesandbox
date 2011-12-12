

#include "navWidget.h"
#include "renderer.h"
#include "input.h"
#include "brush.h"
#include "spriteMeshLoader.h"

NavWidget::NavWidget(){

name="navWidget";
tooltip="navigate";

bWidgetOpen=false;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;
color=listColor;

useTool=TOOL_NAV;
}

NavWidget::~NavWidget(){}


void NavWidget::clickedLeft(){

    if (sceneData->controller->tool!=useTool)
        sceneData->controller->switchTool(useTool);

    color=COLOR_RED;
    Widget::clickedLeft();
}

void NavWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
}

void NavWidget::closeWidget(){

    //input->deselectActors();
    color=COLOR_WHITE;
}

void NavWidget::trigger(MsbObject* other){

    if (other->name=="Fly"){
        useTool=TOOL_NAV;
        setTextureID("icon_fly");
    }

    if (other->name=="Orbit"){
        useTool=TOOL_ORBIT;
        setTextureID("icon_orbit");
    }

    clickedRight();
    clickedLeft();

}

void NavWidget::create(){sceneData->addButton(this);}

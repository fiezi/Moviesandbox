

#include "propsWidget.h"
#include "renderer.h"
#include "input.h"
PropsWidget::PropsWidget(){

registerProperties();
}
PropsWidget::~PropsWidget(){}

void PropsWidget::setup(){

    Widget::setup();
}

void PropsWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
    //sceneData->controller->switchTool(TOOL_SPAWN);
    //listButton[1]->color=COLOR_RED;
}

void PropsWidget::closeWidget(){

    //input->deselectActors();
    sceneData->controller->switchTool(TOOL_SELECT);
}

void PropsWidget::trigger(MsbObject* other){

    if (other->name=="Spawn Meshes"){
        sceneData->controller->switchTool(TOOL_SPAWN);
    }
}

void PropsWidget::create(){sceneData->addButton(this);}

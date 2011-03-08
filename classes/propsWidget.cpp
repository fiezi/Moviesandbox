

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
    //input->controller->switchTool(TOOL_SPAWN);
    //listButton[1]->color=COLOR_RED;
}

void PropsWidget::closeWidget(){

    //input->deselectActors();
    input->controller->switchTool(input->controller->oldTool);
}

void PropsWidget::trigger(Actor* other){

    if (other->name=="Spawn Meshes"){
        input->controller->switchTool(TOOL_SPAWN);
    }
}

void PropsWidget::create(){renderer->addButton(this);}

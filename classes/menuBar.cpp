

#include "menuBar.h"
#include "renderer.h"
#include "input.h"
MenuBar::MenuBar(){

    registerProperties();
    scale.x=64;
    scale.y=16;
    color=Vector4f(0.6,0.6,0.6,0.8);
    name="File";
}

MenuBar::~MenuBar(){}


void MenuBar::setup(){

    BasicButton::setup();
}

void MenuBar::update(double deltaTime){}

void MenuBar::mouseOver(){

    BasicButton::mouseOver();
}

void MenuBar::clickedLeft(){}
void MenuBar::clickedRight(){}

void MenuBar::focusClick(){

    BasicButton::focusClick();
}


void MenuBar::trigger(MsbObject* other){

    if (other->name=="Quit"){

        exit(0);
    }

}


void MenuBar::deselect(int depth){

    BasicButton::deselect(depth);
}

void MenuBar::create(){sceneData->addButton(this);}

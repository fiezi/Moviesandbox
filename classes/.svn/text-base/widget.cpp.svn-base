

#include "widget.h"
#include "renderer.h"
#include "input.h"
Widget::Widget(){

bWidgetOpen=false;
bPermanentList=true;

widgetLocation=Vector3f(0,0,0);
level = 0;
maxListItems=20;

registerProperties();
}

Widget::~Widget(){}


void Widget::clickedLeft(){

    if (!bWidgetOpen){
        bWidgetOpen=true;

        //close all other widgets
        for (int i=0;i<(int)renderer->buttonList.size();i++){
            Widget* widge=dynamic_cast<Widget*>(renderer->buttonList[i]);
            if (widge && widge!=this && widge->bWidgetOpen){
                widge->clickedLeft();
            }
        }

        ListButton::clickedLeft();

        openWidget();
    }
    else{
        bWidgetOpen=false;
        closeWidget();
        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bPermanent=false;
            listButton[i]->level=100;
        }
        input->deselectButtons(level);
        listButton.clear();
    }
}

void Widget::trigger(Actor* other){

    ListButton::trigger(other);
}

void Widget::openWidget(){}

void Widget::closeWidget(){}

void Widget::deselect(int depth){}

void Widget::create(){renderer->addButton(this);}

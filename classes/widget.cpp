

#include "widget.h"
#include "renderer.h"
#include "input.h"
Widget::Widget(){

bWidgetOpen=false;
bToggleWidget=true;
bPermanentList=true;
parent=NULL;

widgetLocation=Vector3f(0,0,0);
level = 0;
maxListItems=20;

registerProperties();
}

Widget::~Widget(){}


void Widget::clickedLeft(){
}


void Widget::clickedRight(){


    if (!bWidgetOpen){
        bWidgetOpen=true;

        //close all other widgets, if our parent is not a widget either
        for (int i=0;i<(int)sceneData->buttonList.size();i++){
            Widget* widge=dynamic_cast<Widget*>(sceneData->buttonList[i]);
            if (widge && widge!=this && widge->bWidgetOpen && widge->bToggleWidget && widge!=parent){
                widge->clickedRight();
                widge->color=COLOR_WHITE;
            }
        }

        ListButton::clickedLeft();

        color=COLOR_YELLOW;
        openWidget();
    }
    else{
        bWidgetOpen=false;
        for (int i=0;i<(int)listButton.size();i++){
            //for cascading widgets:
            Widget* widge=dynamic_cast<Widget*>(listButton[i]);
            if (widge && widge->bWidgetOpen)
                widge->clickedRight();
            listButton[i]->bPermanent=false;
            listButton[i]->level=100;
        }
        closeWidget();
        input->deselectButtons(level);
        listButton.clear();
        color=COLOR_WHITE;
    }
}


void Widget::trigger(MsbObject* other){

    ListButton::trigger(other);
}

void Widget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
    input->deselectButtons(level);
    if (listButton.size()==0){
        cout << "clicked!" << endl;
    assembleList();
    }
}

void Widget::closeWidget(){

    ListButton::deselect(1);

}

void Widget::deselect(int depth){}

void Widget::create(){sceneData->addButton(this);}

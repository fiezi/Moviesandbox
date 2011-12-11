

#include "msbTool.h"
#include "input.h"
#include "control.h"


MsbTool::MsbTool(){

    myBtn=NULL;
    highlightBtn="";
    bPressLeft=false;
    bPressRight=false;
    bOverrideOldState=true;
}

MsbTool::~MsbTool(){}

void MsbTool::setup(){


	renderer=Renderer::getInstance();
    input=Input::getInstance();
    sceneData=SceneData::getInstance();

    controller=sceneData->controller;

    if (myBtn)
        myBtn->buttonColor=sceneData->deselectedToolColor;

}


void MsbTool::start(){

    if (myBtn)
        myBtn->buttonColor=sceneData->selectedToolColor;

    bPressLeft=false;
    bPressRight=false;

}

void MsbTool::stop(){

    if (myBtn)
        myBtn->buttonColor=sceneData->deselectedToolColor;

    bPressLeft=false;
    bPressRight=false;
}

void MsbTool::keyPressed(int key){
}
void MsbTool::keyReleased(int key){

    //CTRL-S
    if (key==19){
        save();
    }
}

void MsbTool::mousePressed(int btn){

}
void MsbTool::mouseReleased(int btn){

        bPressLeft=false;
        bPressRight=false;
}
void MsbTool::mouseDragged(int btn){

    if (btn==MOUSEBTNLEFT)
        bPressLeft=true;
    if (btn==MOUSEBTNRIGHT)
        bPressRight=true;
}



void MsbTool::update(double deltaTime){

        bPressLeft=input->pressedLeft;
        bPressRight=input->pressedRight;

}

void MsbTool::selectActors(int btn, Actor* other){
}

void MsbTool::highlightButton(string btnName){

    ListButton* lst=dynamic_cast<ListButton*>(myBtn);
    if (!lst)
        return;

    //myBtn->color=COLOR_YELLOW;


    for (int i=0;i<(int)lst->listButton.size();i++){
        if (lst->listButton[i]->name==btnName){
            lst->listButton[i]->color=COLOR_RED;
        }
        else{
            lst->listButton[i]->color=COLOR_WHITE;
        }
    }
}


void MsbTool::lowlightButton(){

    ListButton* lst=dynamic_cast<ListButton*>(myBtn);
    if (!lst)
        return;

   // myBtn->color=COLOR_WHITE;
    //only lowlight red buttons!
    for (int i=0;i<(int)lst->listButton.size();i++){
            if (lst->listButton[i]->color==Vector4f(1,0,0,1))
                lst->listButton[i]->color=COLOR_WHITE;
    }
}

void MsbTool::save(){
}


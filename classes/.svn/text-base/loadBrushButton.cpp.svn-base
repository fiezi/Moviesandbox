

#include "loadBrushButton.h"
#include "renderer.h"
#include "input.h"
#include "drawingWidget.h"
#include "skeletalDrawing.h"

LoadBrushButton::LoadBrushButton(){

buttonProperty="NULL";
listDisplayMode=3;
textureID="icon_brushSize";
color=Vector4f(0.5,0.5,0.5,1.0);
}

LoadBrushButton::~LoadBrushButton(){}


void LoadBrushButton::clickedLeft(){

    input->deselectButtons(0);
    bDrawName=true;

    input->getAllBrushes();

    Actor * myParent=parent;

    LoadButton::clickedLeft();

    parent=myParent;

    bDrawName=false;
}

void LoadBrushButton::trigger(Actor *other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    if (parent->isA("DrawingWidget"))
      loadFile(other->name);;
    input->deselectButtons(0);
 }

void LoadBrushButton::assembleLoadList(){

    parent=NULL;
    for (unsigned int i=0;i<input->userBrushes.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(input->userBrushes[i]);
        listProp.push_back("NULL");

        string filename="resources/brushes/";
        filename.append(input->userBrushes[i]);
        renderer->LoadTextureTGA(filename,false, true, input->userBrushes[i]);
        listIcon.push_back(input->userBrushes[i]);
      }
}


void LoadBrushButton::placeScrollBar(){

    scrollBar->location.x=location.x-17;
    scrollBar->location.y=location.y- scale.y -( maxListItems * listButton[0]->scale.y );
    scrollBar->bFlipValue=true;

}

void LoadBrushButton::loadFile(string filename){

renderer->brush->drawing->textureID=filename;
renderer->brush->textureID=filename;
}


void LoadBrushButton::create(){renderer->addButton(this);}

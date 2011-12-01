

#include "loadBrushButton.h"
#include "renderer.h"
#include "input.h"
#include "sceneData.h"
#include "drawingWidget.h"

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

    sceneData->getAllBrushes();

    MsbObject * myParent=parent;

    LoadButton::clickedLeft();

    parent=myParent;

    bDrawName=false;
}

void LoadBrushButton::trigger(MsbObject*other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    if (parent->isA("DrawingWidget"))
      loadFile(other->name);;
    input->deselectButtons(0);
 }

void LoadBrushButton::assembleLoadList(){

    parent=NULL;
    for (unsigned int i=0;i<sceneData->userBrushes.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(sceneData->userBrushes[i]);
        listProp.push_back("NULL");

        string filename="resources/brushes/";
        filename.append(sceneData->userBrushes[i]);
        renderer->LoadTextureTGA(filename,false, true, sceneData->userBrushes[i]);
        listIcon.push_back(sceneData->userBrushes[i]);
      }
}


void LoadBrushButton::placeScrollBar(){

    //scrollBar->location.x=location.x-17;
    //scrollBar->location.y=location.y- scale.y -( maxListItems * listButton[0]->scale.y );
    //scrollBar->bFlipValue=true;

}

void LoadBrushButton::loadFile(string filename){

sceneData->brush->drawing->textureID=filename;
sceneData->brush->textureID=filename;
}


void LoadBrushButton::create(){sceneData->addButton(this);}

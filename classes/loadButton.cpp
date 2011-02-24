#include "loadButton.h"
#include "renderer.h"
#include "input.h"



LoadButton::LoadButton(){

listHeight=20;
listWidth=100;
bDrawListNames=true;
listColor=Vector4f(0.8,0.2,0.2,1.0);
}

LoadButton::~LoadButton(){}

void LoadButton::clickedLeft(){

    listType.clear();
    listName.clear();
    listProp.clear();
    listIcon.clear();

    bDrawName=true;
    input->getAllScenes();

    assembleLoadList();

    ListButton::clickedLeft();
    bDrawName=false;
}

void LoadButton::assembleLoadList(){

    parent=NULL;
    for (unsigned int i=0;i<input->savedScenes.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(input->savedScenes[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_flat");
      }
}

void LoadButton::trigger(Actor * other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    loadFile(other->name);
}

void LoadButton::loadFile(string filename){

input->loadAll(filename);
}

void LoadButton::create(){renderer->addButton(this);}

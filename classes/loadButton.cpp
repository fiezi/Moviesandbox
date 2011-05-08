#include "loadButton.h"
#include "renderer.h"
#include "sceneData.h"



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
    sceneData->getAllScenes();

    assembleLoadList();

    ListButton::clickedLeft();
    bDrawName=false;
}

void LoadButton::assembleLoadList(){

    parent=NULL;
    for (unsigned int i=0;i<sceneData->savedScenes.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(sceneData->savedScenes[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_flat");
      }
}

void LoadButton::trigger(MsbObject* other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    loadFile(other->name);
}

void LoadButton::loadFile(string filename, bool bStart){

    sceneData->loadAll(filename);
    if (bStart){
        sceneData->controller->startMovie();
    }
}

void LoadButton::create(){sceneData->addButton(this);}

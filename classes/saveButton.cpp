#include "saveButton.h"
#include "input.h"



SaveButton::SaveButton(){

listHeight=20;
listWidth=100;
bDrawListNames=true;
listColor=Vector4f(0.8,0.2,0.2,1.0);
}

SaveButton::~SaveButton(){
}

void SaveButton::clickedLeft(){

    input->getAllScenes();


    listType.clear();
    listName.clear();
    listProp.clear();
    listIcon.clear();

    bDrawName=true;
    parent=NULL;

    listType.push_back("15TextInputButton");
    listName.push_back("create new");
    listProp.push_back("NULL");
    listIcon.push_back("icon_flat");

    assembleSaveList();


    ListButton::clickedLeft();

    bDrawName=false;
}

void SaveButton::assembleSaveList(){

    for (unsigned int i=0;i<input->savedScenes.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(input->savedScenes[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_flat");
      }
}

void SaveButton::trigger(MsbObject* other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

   //clicked on the last button
    if (other==listButton[0])
      other->name=((BasicButton*)other)->tooltip;

    //append extension in case the user doesn't want to...
    size_t myPart=other->name.find(".scene");
    if (myPart==string::npos)
      other->name.append(".scene");

    saveFile(other->name);
    input->deselectButtons(0);
}

void SaveButton::saveFile(string filename){

input->saveAll(filename);
}

void SaveButton::create(){sceneData->addButton(this);}

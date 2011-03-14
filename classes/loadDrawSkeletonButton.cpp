#include "loadDrawSkeletonButton.h"
#include "renderer.h"
#include "input.h"
#include "drawingWidget.h"
#include "brush.h"

LoadDrawSkeletonButton::LoadDrawSkeletonButton(){

listWidth=200;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listShader="color";

registerProperties();
}

LoadDrawSkeletonButton::~LoadDrawSkeletonButton(){}


void LoadDrawSkeletonButton::clickedLeft(){

    //input->deselectButtons(0);
    input->getAllDrawings();
    MsbObject * myParent=parent;
    listDisplayMode=3;

    LoadButton::clickedLeft();

    parent=myParent;
    bDrawName=false;
}

void LoadDrawSkeletonButton::placeScrollBar(){

    scrollBar->setLocation( Vector3f(location.x-17, location.y- scale.y -( maxListItems * listButton[0]->scale.y ), 0) );
    scrollBar->bFlipValue=true;

}


void LoadDrawSkeletonButton::assembleLoadList(){

    // in the drawing widget, open the list up towards the top
    // in the system menu, do the standard right-down thing
      parent=NULL;
      for (unsigned int i=0;i<input->prefabs.size();i++)
        {
        listType.push_back("12AssignButton");
        listName.push_back(input->prefabs[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_character");
        }
}

void LoadDrawSkeletonButton::trigger(MsbObject* other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    if (renderer->brush && renderer->brush->drawing){
        SkeletalActor* mySkel= dynamic_cast<SkeletalActor*>(renderer->brush->drawing);
        if (mySkel){
            input->loadPrefab("resources/prefabs/"+other->name);            //load prefab
            for (uint i=0;i<input->selectedActors.size();i++){
                mySkel->bones.push_back(input->selectedActors[i]);                  //push selectedActors into drawing as bones
                if (!mySkel->bones[i]->base)
                    mySkel->bones[i]->base=mySkel;
            }
        }
    }
}

void LoadDrawSkeletonButton::create(){sceneData->addButton(this);}

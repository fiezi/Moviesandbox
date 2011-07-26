

#include "prefabInspector.h"
#include "renderer.h"
#include "input.h"
#include "sceneData.h"
#include "assignButton.h"

PrefabInspector::PrefabInspector(){

    bOpen=false;
    bPermanent=true;
    initialLocation=location;
    tabWidth=350;
    listHeight=64;
    listWidth=64;
    listDisplayMode=4;
    listColumns=4;
    level=0;
    maxListItems=20;

    textureID="icon_error";
    }

PrefabInspector::~PrefabInspector(){}

void PrefabInspector::createInspectorButtons(){

    AssignButton* importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    importButton->name="import";
    importButton->color=Vector4f(0.8,0.8,0.8,1.0);
    importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);
}

void PrefabInspector::refreshList(){

        if (listButton.size()!=sceneData->prefabs.size()){
            ListButton::deselect(level);
            listButton.clear();
            assembleList();
        }

}

void PrefabInspector::assembleList(){

        for (int i=0;(int)listButton.size();i++){
            listButton[i]->remove();
        }
        listButton.clear();

        cout << "creating list..." << endl;

        for (int i=0;i<(int)sceneData->prefabs.size();i++){
            sceneData->actorInfo["12AssignButton"].actorReference->create();
            listButton.push_back(sceneData->buttonList.back());

            listButton[i]->name=sceneData->prefabs[i];
            listButton[i]->textureID="icon_base";
            listButton[i]->level=level+1;
            listButton[i]->bDrawName=true;
            listButton[i]->color=Vector4f(0,0,1,1.0);
            listButton[i]->bPermanent=true;
            listButton[i]->bDragable=true;
            listButton[i]->parent=this;
            if (listWidth>0)
                listButton[i]->scale.x=listWidth;
            if (listHeight>0)
                listButton[i]->scale.y=listHeight;

            listButton[i]->setup();
            placeButton(i,i);
            //set this because we want to drag buttons around!
            listButton[i]->initialLocation=listButton[i]->location;
        }

    cout << "our Button list is: "<< listButton.size() <<" elements long..." << endl;
}


void PrefabInspector::trigger(MsbObject* other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    if (other->name=="import"){
        string fileName=sceneData->openFileDialog();
        cout <<"importing..." << fileName << endl;
        //renderer->LoadTextureTGA(filename,true,true,filename);
        return;
    }


    cout << "placed prefab!" << endl;

    int prefNo=0;

    //clean up sceneData->selectedActors
    input->deselectActors();
    //determine which button called us back
    for (int i=0;i<(int)listButton.size();i++)
        if (other==listButton[i])
            prefNo=i;

     sceneData->loadPrefab("resources/prefabs/",sceneData->prefabs[prefNo]);


    //this should only be called when we drag'n'dropped the prefab somewhere
    if (!input->hudTarget){
        for (int i=0;i< (int)sceneData->selectedActors.size();i++){
            if (sceneData->selectedActors[i]->base==NULL)
                sceneData->selectedActors[i]->setLocation(sceneData->selectedActors[i]->location+ input->mouse3D);
            }
        }

}


void PrefabInspector::create(){sceneData->addButton(this);}

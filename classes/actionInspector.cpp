

#include "actionInspector.h"
#include "action.h"
#include "renderer.h"
#include "input.h"

#include "assignButton.h"
ActionInspector::ActionInspector(){

    listColumns=4;
    level=0;

    textureID="icon_ball";
}

ActionInspector::~ActionInspector(){}


void ActionInspector::createInspectorButtons(){

    AssignButton* importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    importButton->name="import";
    importButton->color=Vector4f(0.8,0.8,0.8,1);
    importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);
}

void ActionInspector::refreshList(){

    if (listButton.size()<sceneData->actionList.size()){
            assembleList();
    }
}

void ActionInspector::assembleList(){

        std::map <std::string, Action*>::iterator it;
        int i=0;

        listButton.clear();
        for ( it=sceneData->actionList.begin() ; it != sceneData->actionList.end(); it++ ){

                if (it->second){

                listButton.push_back(it->second);

                listButton[i]->level=level+1;
                listButton[i]->bDrawName=true;
                listButton[i]->color=Vector4f(1.0,1.0,0.0,1.0);
                //actions are just set to be visible, not actually created!
                listButton[i]->bHidden=false;
                listButton[i]->bDragable=true;

                if (listWidth>0)
                    listButton[i]->scale.x=listWidth;
                if (listHeight>0)
                    listButton[i]->scale.y=listHeight;

                placeButton(i,i);
                //set this because we want to drag buttons around!
                listButton[i]->initialLocation=listButton[i]->location;
                i++;
                }else{
                cout << "problem with:" << it->first << endl;
                }
            }

    cout << "ActionInspector! our Button list is: "<< listButton.size() <<" elements long..." << endl;
/*
    for (int i=0;i<(int)sceneData->buttonList.size();i++){
            cout << "list has: " << sceneData->buttonList[i]->name << endl;
        }
    for (int i=0;i<(int)listButton.size();i++){
            cout << "button has: " << listButton[i]->name << endl;
        }
*/
}

void ActionInspector::trigger(MsbObject* other){

    if (other->name=="import"){
        string fileName=input->openFileDialog();
        cout <<"importing..." << fileName << endl;
    }
}

void ActionInspector::create(){sceneData->addButton(this);}

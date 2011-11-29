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

    //when doing file stuff, always go back to .exe directory!
    sceneData->switchToExePath();

    listType.clear();
    listName.clear();
    listProp.clear();
    listIcon.clear();


    if (buttonProperty=="NEWSCENE"){
        sceneData->newScene();
        return;
    }

    if (buttonProperty=="SAVESCENE"){
        sceneData->saveAll(sceneData->currentScene);
        return;
    }

    if (buttonProperty=="SAVESCENEAS"){
        string fileName=sceneData->saveFileDialog("scene");
        if (fileName=="NULL")
            return;

        sceneData->saveScene(fileName);
        return;
    }

    if (buttonProperty=="LOADSCENELIST"){
        sceneData->getAllScenes();
        assembleLoadList();
        ListButton::clickedLeft();
        return;
    }

    if (buttonProperty=="LOADSCENE"){
        string myScene=sceneData->openFileDialog("scene");
        sceneData->loadScene(myScene,false);
    }

    if (buttonProperty=="NEWPROJECT"){
        string myProject=sceneData->saveFileDialog("project");
        sceneData->newProject(myProject);
    }

    if (buttonProperty=="LOADPROJECT"){
        string myProject=sceneData->openFileDialog("project" );
        if (myProject=="NULL")
            return;
        //myProject=+"/";
        sceneData->loadProject(myProject,false);
    }




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

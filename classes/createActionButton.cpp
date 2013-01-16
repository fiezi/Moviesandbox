

#include "createActionButton.h"
#include "renderer.h"
#include "input.h"
#include "timelineButton.h"
#include "timelineInspector.h"
#include "action.h"

CreateActionButton::CreateActionButton(){

bMessageWindow=true;
registerProperties();
}

CreateActionButton::~CreateActionButton(){}


void CreateActionButton::setup(){
TextInputButton::setup();

}

void CreateActionButton::update(double deltaTime){

    TextInputButton::update(deltaTime);
}

void CreateActionButton::mouseOver(){

    TextInputButton::mouseOver();
}

void CreateActionButton::mouseDrag(){
    TextInputButton::mouseDrag();
}

void CreateActionButton::finishDrag(){
    TextInputButton::finishDrag();
}

void CreateActionButton::clickedLeft(){
    TextInputButton::clickedLeft();
}

void CreateActionButton::clickedRight(){
    TextInputButton::clickedRight();
}

void CreateActionButton::focusClick(){

    TimelineButton* tlBtn=dynamic_cast<TimelineButton*>(parent);

    if (!parent){
        cout << "wrong parent!" << endl;
        sceneData->buttonList.pop_back();
        sceneData->staticButton=NULL;
        return;
        }

    Action* newAction=new Action;
    newAction->name=input->inputText;

    tlBtn->connectAction(newAction);

    sceneData->actionList[newAction->name]=newAction;

    //cout << "created new action: " << newAction->name << endl;

        //assemble TiXmlElement of all selected Actors
        TiXmlElement* myAction = new TiXmlElement("MovieSandbox");
        //save in order of actorList for later referencing!
        myAction->LinkEndChild(newAction->save(myAction));

        TiXmlDocument doc;
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        doc.LinkEndChild( decl );
        doc.LinkEndChild(myAction);
        string stringName=sceneData->startProject+newAction->name+".action";
        doc.SaveFile(stringName);

    //cleanUp
    //we need to pop back before adding the action!
    sceneData->buttonList.pop_back();
    sceneData->staticButton=NULL;

    sceneData->buttonList.push_back(newAction);
    //need to update assetInspector
    for (int i=0;i<(int)sceneData->inspectorManager->inspectors.size();i++)
        if (sceneData->inspectorManager->inspectors[i]->name=="assetInspector")
            sceneData->inspectorManager->inspectors[i]->tabs[sceneData->inspectorManager->inspectors[i]->currentTab]->assembleList();


    //clean up
    input->inputText="";
    bEditing=false;
    BasicButton::focusClick();
}

void CreateActionButton::deselect(int depth){
TextInputButton::deselect(depth);
}

void CreateActionButton::create(){sceneData->addButton(this);}



#include "createPrefabButton.h"
#include "renderer.h"
#include "input.h"
#include "selectTool.h"

CreatePrefabButton::CreatePrefabButton(){

bMessageWindow=true;
registerProperties();
}

CreatePrefabButton::~CreatePrefabButton(){}


void CreatePrefabButton::setup(){
TextInputButton::setup();

}

void CreatePrefabButton::update(double deltaTime){

TextInputButton::update(deltaTime);
}

void CreatePrefabButton::mouseOver(){

TextInputButton::mouseOver();
}

void CreatePrefabButton::mouseDrag(){
TextInputButton::mouseDrag();
}

void CreatePrefabButton::finishDrag(){
TextInputButton::finishDrag();
}

void CreatePrefabButton::clickedLeft(){
TextInputButton::clickedLeft();
}

void CreatePrefabButton::clickedRight(){
TextInputButton::clickedRight();
}

void CreatePrefabButton::focusClick(){


//TODO: hacky!
((SelectTool*)sceneData->controller->myTools[TOOL_SELECT])->makePrefab(input->inputText);                              //set Prefab name to Text

//cleanUp
sceneData->buttonList.pop_back();
sceneData->staticButton=NULL;

TextInputButton::focusClick();


}

void CreatePrefabButton::deselect(int depth){
TextInputButton::deselect(depth);
}

void CreatePrefabButton::create(){sceneData->addButton(this);}

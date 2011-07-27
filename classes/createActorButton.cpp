#include "createActorButton.h"
#include "renderer.h"
#include "input.h"
#include "meshData.h"

CreateActorButton::CreateActorButton(){
    newActor=NULL;
    bDragable=true;
    name="createActorButton";
}

CreateActorButton::~CreateActorButton(){
}

void CreateActorButton::mouseDrag(){

    input->dragButton=this;
    setLocation(Vector3f(input->mouseX-scale.x/2,input->mouseY-scale.y/2,0));

}

void CreateActorButton::finishDrag(){

    cout << "prop is: " << buttonProperty << endl;

    int myActorSize=sceneData->actorList.size();
    int myButtonSize=sceneData->buttonList.size();

    sceneData->actorInfo[buttonProperty].actorReference->create();

	if (myActorSize<(int)sceneData->actorList.size()){
        newActor=sceneData->actorList.back();
		newActor->setup();
	}
    else if (myButtonSize<(int)sceneData->buttonList.size()){
        cout << "creating a new BasicButton too!" << endl;
        newActor=sceneData->buttonList.back();
        newActor->setup();
        newActor->setLocation(Vector3f(input->mouseX,input->mouseY,0.0));
        setLocation(initialLocation);
        newActor=NULL;
        return;
	}

    //draw new actor as VBOMESH if we have one assigned to us and we find it in the vboMeshList
    if (vboMeshID!="NULL" && sceneData->vboList[vboMeshID]){
        newActor->vboMeshID=vboMeshID;
        if (sceneData->vboList[vboMeshID]->bIsSkeletal){          //see if we're skeletal and stuff...
            newActor->sceneShaderID="skeletal";
		}
        else{
            newActor->sceneShaderID="color";
		}
        newActor->drawType=sceneData->vboList[vboMeshID]->drawType;
    }

//	newActor->bPickable=true;

    std::cout << "created new Actor of type " << newActor->name << endl;

    setLocation(initialLocation);
	if (input->mouse3D==Vector3f(1e+06,1e+06,1e+06))
		newActor->setLocation(Vector3f(0,0,0));
	else
		newActor->setLocation(input->mouse3D);


    cout << "set up new Actor at ..." << input->mouse3D << endl;

    newActor=NULL;
}

void CreateActorButton::clickedLeft(){

    input->focusButton=this;

    int myActorSize=sceneData->actorList.size();
    int myButtonSize=sceneData->buttonList.size();

    sceneData->actorInfo[buttonProperty].actorReference->create();
    if (myActorSize < (int)sceneData->actorList.size()){
        newActor=sceneData->actorList.back();
        }

    else if (myButtonSize < (int) sceneData->buttonList.size()){
        newActor=sceneData->buttonList.back();
        newActor->setup();
        setLocation(initialLocation);
        //newActor=NULL;
        return;
        }


    if (vboMeshID!="NULL" && sceneData->vboList[vboMeshID]){
        newActor->vboMeshID=vboMeshID;
        if (sceneData->vboList[vboMeshID]->bIsSkeletal){          //see if we're skeletal and stuff...
            newActor->sceneShaderID="skeletal";
		}
		else{
            newActor->sceneShaderID="color";
		}

        newActor->drawType=sceneData->vboList[vboMeshID]->drawType;
    }
    cout << "created new actor..." << endl;

    newActor->setup();

    SkeletalActor* skel=dynamic_cast<SkeletalActor*>(newActor);
    if (skel){
        for (int i=0;i<(int)skel->bones.size();i++){
            skel->bones[i]->bPickable=false;
        }
    }
    cout << "set up new Actor..." << endl;

    newActor->bPickable=false;
    std::cout << "clicked and: created new Actor of type " << newActor->name << endl;
}

void CreateActorButton::focusClick(){

    BasicButton::focusClick();
    newActor->bPickable=true;
    if (input->mouse3D==Vector3f(1e+06,1e+06,1e+06))
      newActor->setLocation(Vector3f(0,0,0));

    SkeletalActor* skel=dynamic_cast<SkeletalActor*>(newActor);
    if (skel){
        for (int i=0;i<(int)skel->bones.size();i++){
            skel->bones[i]->bPickable=true;
        }
    }

    newActor=NULL;
}

void CreateActorButton::update(double deltaTime){

if (newActor)
    {
     BasicButton* myButton=dynamic_cast<BasicButton*>(newActor);
     if (myButton)
        newActor->setLocation(Vector3f(input->mouseX,input->mouseY, 0.0f));

     else if (input->worldTarget && input->worldTarget->bPickable)
         newActor->setLocation(input->mouse3D);
    }
}

void CreateActorButton::create(){
sceneData->addButton(this);
//std::cout << "creating a createActorButton!";
}

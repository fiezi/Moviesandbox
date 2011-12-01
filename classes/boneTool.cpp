

#include "boneTool.h"
#include "renderer.h"
#include "input.h"
#include "boneActor.h"
#include "brush.h"
#include "spriteMeshLoader.h"

BoneTool::BoneTool(){

    spawnType="9BoneActor";
}

BoneTool::~BoneTool(){}

void BoneTool::start(){
    MsbTool::start();

    input->bKeepSelection=true;

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

    myBtn->color=COLOR_RED;

    brush=sceneData->brush;
    brush->bHidden=false;

    //if we have an actor selcted that is a drawing, use that as our new drawing
    if (sceneData->selectedActors.size()>0){
        SkeletalActor* skel = dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
        if (skel){
            brush->drawing=skel;
        }
    }

	//use already assigned drawing
    if (brush->drawing){
        brush->bHidden=false;
        brush->drawing->sceneShaderID="skinning";
		brush->drawing->drawType=DRAW_PARTICLES;

		//if we already have bones, reset transforms
        brush->drawing->resetBones();
    }
	//no useable drawing found
    else{
        sceneData->makeWarningPopUp("OOPS! \n \nNo Drawing to Skin. Create or Select a Drawing first!", myBtn);
        cout << "no drawing!" << endl;
        input->bTextInput=false;
        sceneData->controller->switchTool(TOOL_SELECT);
    }

}

void BoneTool::stop(){

    MsbTool::stop();

    input->bKeepSelection=false;

    glutSetCursor(GLUT_CURSOR_INHERIT);
    brush->bHidden=true;
    myBtn->color=COLOR_WHITE;

    SkeletalActor* skel=brush->drawing;

    if (!skel) return;

	//determine if we're skeletal or not
	//determine if we're skeletal or not
	if (skel->bones.size()>0)
		skel->sceneShaderID="skeletal";
    else
        skel->sceneShaderID="color";

    for (int i=0;i<(int)skel->bones.size();i++)
        skel->bones[i]->bPickable=true;


	//save vbo and reload, then assign to this actor and switch drawmode to VBO
    //sceneData->drawTool->save();

	skel->drawType=DRAW_VBOMESH;

	skel->resetBones();

    //revert back to our drawing as the only thing selected!
    input->deselectActors();
    skel->bSelected=true;
    sceneData->selectedActors.push_back(skel);
    brush->drawing=NULL;

}

void BoneTool::mouseReleased(int btn){

    if (!brush->drawing)
        return;
    SpawnTool::mouseReleased(btn);


}

void BoneTool::keyReleased(int key){

    MsbTool::keyReleased(key);

    //DELETE
   if (key==127 || key==8){
        if (!input->worldTarget) return;

            for (int i=0;i<(int)brush->drawing->bones.size();i++){
                if (input->worldTarget==brush->drawing->bones[i]){
                    input->deselectActors();
                    brush->drawing->bones.erase(brush->drawing->bones.begin()+i);
                    input->worldTarget->remove();
                }
            }
    }

}

void BoneTool::postSpawn(Actor* myActor){


    BoneActor* bone=(BoneActor*)myActor;
    if (brush->drawing->bones.size()>0)
        bone->base=brush->drawing->bones[brush->drawing->bones.size()-1];
    else
        bone->base=brush->drawing;

    brush->drawing->bones.push_back(bone);

    char newName[64];
    bone->name+=sprintf(newName,"%i",(int)brush->drawing->bones.size()-1);

    bone->setAbsoluteLocation(input->mouse3D);

    cout << "creating bone " << bone->name << endl;

    sceneData->vboList[brush->drawing->vboMeshID]->bUnsavedChanges=true;
}

void BoneTool::save(){

/*
		SkeletalActor* skel=brush->drawing;
		sceneData->spriteMeshLoader->saveSpriteMesh(sceneData->startProject+"/"+skel->vboMeshID+".spriteMesh",skel);

		//open my.library and append this mesh!
		TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
		myElement->SetAttribute("meshID",skel->vboMeshID);
		myElement->SetAttribute("meshFilename",skel->vboMeshID+".spriteMesh");
		sceneData->addToLibrary(myElement);
		free(myElement);
*/
}

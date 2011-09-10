

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
    if (sceneData->selectedActors.size()>0 && !sceneData->specialSelected){
        SkeletalActor* skel = dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
        if (skel){
            brush->drawing=skel;
            sceneData->specialSelected=skel;
        }
    }

	//use already assigned drawing
    if (brush->drawing){
        brush->bHidden=false;
        brush->drawing->sceneShaderID="skinning";
		brush->drawing->drawType=DRAW_PARTICLES;

		//if we already have bones, reset transforms
        if (sceneData->vboList[sceneData->specialSelected->vboMeshID]->bIsSkeletal){

        //if we have an original Matrix, then reset our transformMatrices!
            Matrix4f identityMatrix;
            identityMatrix.identity();

            //reset Matrices...
            for (int i=0;i<(int)sceneData->vboList[sceneData->specialSelected->vboMeshID]->bones.size();i++){
                if (sceneData->brush->drawing->bones[i]->originalMatrix!=identityMatrix){
                    sceneData->brush->drawing->bones[i]->transformMatrix=sceneData->brush->drawing->bones[i]->originalMatrix;
                    sceneData->brush->drawing->bones[i]->originalMatrix.identity();
                }
            }
        }
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

    for (int i=0;i<(int)skel->bones.size();i++)
        skel->bones[i]->bPickable=true;

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
}

void BoneTool::save(){

		SkeletalActor* skel=brush->drawing;
		sceneData->spriteMeshLoader->saveSpriteMesh(sceneData->startProject+"/"+skel->vboMeshID+".spriteMesh",skel);

		//open my.library and append this mesh!
		TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
		myElement->SetAttribute("meshID",skel->vboMeshID);
		myElement->SetAttribute("meshFilename",skel->vboMeshID+".spriteMesh");
		sceneData->addToLibrary(myElement);
		free(myElement);
}

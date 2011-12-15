

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

    SkeletalActor* skel=brush->drawing;

    if (!skel) return;

	//determine if we're skeletal or not
	if (skel->bones.size()>0){
		skel->sceneShaderID="skeletal";
		sceneData->vboList[skel->vboMeshID]->bIsSkeletal=true;
	}else{
        skel->sceneShaderID="color";
		sceneData->vboList[skel->vboMeshID]->bIsSkeletal=false;
	}

    for (int i=0;i<(int)skel->bones.size();i++){
        skel->bones[i]->bPickable=true;

        //setup vboMesh with bones
        MeshData* myData=sceneData->vboList[skel->vboMeshID];
        bone* vboBone = new bone;
        myData->bones.push_back(vboBone);

        vboBone->name= skel->bones[i]->name;
         if (vboBone->name=="mouthUp")
                myData->bIsHead=true;

            //fill the MeshData object with all our DATA
            vboBone->invBoneMatrix=new Matrix4f;
            vboBone->boneMatrix=new Matrix4f;

            *vboBone->invBoneMatrix=(skel->bones[i]->baseMatrix * skel->baseMatrix.inverse()).inverse();
            *vboBone->boneMatrix=skel->bones[i]->transformMatrix * skel->bones[i]->originalMatrix;
              myData->boneCount++;
              myData->bindShapeMatrix=new Matrix4f;
        }

        //it's a little ugly but it gets the job done. Compares every bone against every other and assigns parent
        for (int i=0;i<(int)skel->bones.size();i++){
            bone* vboBone=sceneData->vboList[skel->vboMeshID]->bones[i];
            for (uint parentPos=0;parentPos<skel->bones.size();parentPos++){
                if (skel->bones[parentPos] == skel->bones[i]->base)
                    vboBone->parentBone=sceneData->vboList[skel->vboMeshID]->bones[parentPos];               //parent found!
            }
        }

    skel->postLoad();

	//create vbo Data for faster drawing!
    sceneData->spriteMeshLoader->createVBOs(skel->vboMeshID,false);


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

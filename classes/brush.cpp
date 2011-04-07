#include "brush.h"
#include "input.h"
#include "sceneData.h"
#include "control.h"
#include "boneActor.h"
#include "meshData.h"

Brush::Brush(){

name="brush";
drawType=DRAW_SPRITE;
mode=BRUSH_DRAW;
normalMode=NORMAL_UP;
bNormalFollowDrawing=true;

intensity= 1.0;

color=Vector4f(1,1,1,1);
scale=Vector3f(1.0,1.0,1.0);
pNormal=Vector3f(0.0,0.0,1.0);



sceneShaderID="color";
textureID="NULL";
vboMeshID="teapot";

bTextured=false;
bUseShader=true;
bComputeLight=false;
bDrawOrientation=true;
	bHidden=false;
bPickable=false;

drawing=NULL;
bMouseControlled=true;

calcLoc=Vector3f(0.0,0.0,0.0);                 //the location of the drawn Particle

	bZTest=false;
	bZWrite=false;

menuType.clear();
menuProp.clear();
menuIcon.clear();
}

Brush::~Brush(){

}

void Brush::update(double deltaTime){

	Actor::update(deltaTime);
//    controller =sceneData->controller;

    return;
}


void Brush::select(){

    calcLoc=location-drawing->location;

    MeshData* myData=sceneData->vboList[drawing->vboMeshID];
//go through all particles and see
    for (int i=0;i<(int)myData->vData.size();i++){
        //compute the distance to brush
        Vector3f distance=Vector3f(myData->vData[i].location.x,myData->vData[i].location.y,myData->vData[i].location.z)-calcLoc;

        if ( scale.x>distance.length() ){

            //first one gets a free ride!
            if (selectedData.size()==0){
                selectedData.push_back(i);
                selectedOldColors.push_back(myData->vData[i].color);
                myData->vData[i].color=Vector4f(1.0,0.0,0.0,1.0);
            }
            bool bAlreadySelected=false;

            //see if we already selected this one
            for (int j=0;j<(int)selectedData.size();j++){
                if (selectedData[j]==i){
                    bAlreadySelected=true;
                    cout << "already selected!" << selectedData.size() << endl;
                }
            }

            if (!bAlreadySelected){
                selectedData.push_back(i);
                selectedOldColors.push_back(myData->vData[i].color);
                cout << "selecting: " << selectedData.size() << endl;
                cout << "storing color: " << selectedOldColors.size() << " " <<selectedOldColors.back() << endl;
                myData->vData[i].color=Vector4f(1.0,0.0,0.0,1.0);
            }
        }
    }

}

void Brush::skin(){

    SkeletalActor* skel =dynamic_cast<SkeletalActor*>(drawing);
    if (!skel){
        cout << "drawing cannot be skinned" << endl;
        return;
        }

    calcLoc=location-drawing->location;

    //calcLocation();

    for (unsigned int pID=0;pID<sceneData->vboList[drawing->vboMeshID]->vData.size();pID++){           //go through particles

        //find out where the brush is in relation to the particle
        Vector4f loc=sceneData->vboList[drawing->vboMeshID]->vData[pID].location;
        Vector3f distance=calcLoc - Vector3f(loc.x,loc.y,loc.z);

        ///within brush range
        if (scale.x * 0.1>distance.length()){

            ///single bone skinning
            if (sceneData->selectedActors.size()==1){
                //find bone
                for (int boneID=0; boneID<(int)skel->bones.size();boneID++)
                    if (skel->bones[boneID]==sceneData->selectedActors[0]){
                        if (input->pressedRight)
                            eraseSingleSkin(pID,boneID);                       //erase weights with this bone
                        else
                            singleSkin(pID,boneID);                       //skin particles with this bone
                    }
            }
        }//end particles in brushrange

    }//end all particles
}


void Brush::createBone(){


    //only create bone in new position
    for (int i=0; i<(int)drawing->bones.size();i++){
        if (input->worldTarget==drawing->bones[i])
            return;
    }

    //first bone sets the whole thing to be skeletal!
    if (!sceneData->vboList[drawing->vboMeshID]->bIsSkeletal)
        sceneData->vboList[drawing->vboMeshID]->bIsSkeletal=true;

    BoneActor* bone=(BoneActor*)spawn("9BoneActor");

    if (drawing->bones.size()>0)
        bone->base=drawing->bones[drawing->bones.size()-1];
    else
        bone->base=drawing;

    drawing->bones.push_back(bone);

    bone->setAbsoluteLocation(input->mouse3D);

    char newName[64];
    bone->name+=sprintf(newName,"%i",(int)drawing->bones.size()-1);


    cout << "creating bone " << bone->name << endl;

}

void Brush::createNewDrawing(){

	if (drawing){
		drawing->bPickable=true;
		drawing->bZTest=true;
		drawing->bZWrite=true;
	}
        //setup drawing
        drawing = (SkeletalActor*)spawn("13SkeletalActor");
        //drawing->bPickable=false;
        drawing->name="skeletalActor";
		drawing->drawType=DRAW_PARTICLES;
        drawing->sceneShaderID="drawing";
		drawing->setLocation(input->lastMouse3D);
        drawing->update(0.0);
        cout << "New Drawing: " << drawing->location << endl;
        drawing->controller=sceneData->controller;
        drawing->name=input->inputText;
        drawing->vboMeshID=input->inputText;
        sceneData->vboList[input->inputText]=new MeshData;

        cout << "new drawing name: " << input->inputText << endl;
        drawing->setup();
}

void Brush::calcLocation(){

}


void Brush::singleSkin(int pID, int boneID){


}

void Brush::eraseSingleSkin(int pID, int boneID){


}

void Brush::create(){sceneData->addActor(this);}

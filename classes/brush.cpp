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



void Brush::setColor(Vector4f newColor){


    color=newColor;
    for (int i=0;i<(int)selectedOldColors.size();i++){
        selectedOldColors[i]=newColor;
    }
}



void Brush::create(){sceneData->addActor(this);}

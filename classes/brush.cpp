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






void Brush::setColor(Vector4f newColor){


    color=newColor;
    for (int i=0;i<(int)selectedOldColors.size();i++){
        selectedOldColors[i]=newColor;
    }
}



void Brush::create(){sceneData->addActor(this);}

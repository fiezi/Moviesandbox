

#include "spawnTool.h"
#include "renderer.h"
#include "input.h"

SpawnTool::SpawnTool(){

    spawnType="13SkeletalActor";
    brushMove=BRUSH_ABSOLUTE;
    brushRot=BRUSH_X;
 }

SpawnTool::~SpawnTool(){}

void SpawnTool::start(){

    MsbTool::start();

    brush=renderer->brush;
    brush->bHidden=false;
    brush->drawType=DRAW_VBOMESH;
    brush->color.a=0.5;

    glutSetCursor(GLUT_CURSOR_INHERIT);
    highlightButton(highlightBtn);
}

void SpawnTool::stop(){

    MsbTool::stop();
    brush->bHidden=true;
    brush->drawType=DRAW_SPRITE;
    brush->color.a=1.0;

    glutSetCursor(GLUT_CURSOR_INHERIT);
    lowlightButton();
}


void SpawnTool::keyReleased(int key){

    MsbTool::keyReleased(key);
    if (key=='.'){
        brushRot++;
        if (brushRot>BRUSH_Z)
            brushRot=BRUSH_X;

        if (brushRot==BRUSH_X)
            renderer->brush->setRotation(brushX,brushY,brushZ);
        if (brushRot==BRUSH_Y)
            renderer->brush->setRotation(brushZ,brushY,brushX);
        if (brushRot==BRUSH_Z)
            renderer->brush->setRotation(brushX,brushZ,brushY);

    }
    if (key=='n'){
        if (brushMove==BRUSH_NORMAL)
            brushMove=BRUSH_ABSOLUTE;
        else
            brushMove=BRUSH_NORMAL;

    }

}

void SpawnTool::mouseReleased(int btn){

    MsbTool::mouseReleased(btn);

    renderer->actorInfo[spawnType].actorReference->create();

    Actor* myActor=renderer->actorList.back();


    myActor->setAbsoluteLocation(input->mouse3D);

    postSpawn(myActor);
}

void SpawnTool::update(double deltaTime){

    MsbTool::update(deltaTime);

    renderer->brush->setLocation(input->mouse3D);

    if (brushMove==BRUSH_ABSOLUTE){
        brushX=Vector3f(1,0,0);
        brushY=Vector3f(0,1,0);
        brushZ=Vector3f(0,0,1);
    }

    if (brushMove==BRUSH_NORMAL){
        if (input->worldNormal!=Vector3f(0,1,0)){
            brushZ=input->worldNormal;
            brushZ.normalize();
            brushX=brushZ.crossProduct(Vector3f(0,1,0));
            brushX.normalize();
            brushY= brushZ.crossProduct(brushX);
            brushY.normalize();
        }
        else if (input->worldNormal!=Vector3f(0,0,0)){
            brushY= input->worldNormal;
            brushY.normalize();
            brushX= brushY.crossProduct(Vector3f(0,0,1));
            brushX.normalize();
            brushZ= brushY.crossProduct(brushX);
            brushZ.normalize();
        }else{
            input->worldNormal=Vector3f(0,1,0);
            brushY= input->worldNormal;
            brushY.normalize();
            brushX= brushY.crossProduct(Vector3f(0,0,1));
            brushX.normalize();
            brushZ= brushY.crossProduct(brushX);
            brushZ.normalize();
        }
    }

    if (brushRot==BRUSH_X)
        renderer->brush->setRotation(brushX,brushY,brushZ);
    if (brushRot==BRUSH_Y)
        renderer->brush->setRotation(brushZ,brushY,brushX);
    if (brushRot==BRUSH_Z)
        renderer->brush->setRotation(brushX,brushZ,brushY);

}

void SpawnTool::selectActors(){
}




void SpawnTool::postSpawn(Actor* myActor){


    cout << "Spawning..." << brush->vboMeshID << endl;
    //assign vboMeshID from Brush to new Actor
    myActor->vboMeshID=brush->vboMeshID;
    if (renderer->vboList[brush->vboMeshID]->bIsSkeletal)
        myActor->sceneShaderID="skeletal";

    myActor->setRotation(brush->rotation);
    myActor->setup();
}


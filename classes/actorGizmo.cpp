

#include "actorGizmo.h"
#include "renderer.h"
#include "input.h"

ActorGizmo::ActorGizmo(){

    drawType=DRAW_NULL;
    bGizmoing=false;
    moveAxis=NULL;
    rotAxis=NULL;

    xAxisGizmo=NULL;
    yAxisGizmo=NULL;
    zAxisGizmo=NULL;

    xRotateGizmo=NULL;
    yRotateGizmo=NULL;
    zRotateGizmo=NULL;

    registerProperties();
}

ActorGizmo::~ActorGizmo(){}

void ActorGizmo::registerProperties(){

    Actor::registerProperties();
}

void ActorGizmo::setup(){

    Actor::setup();

    xRotateGizmo=new Actor;
    xRotateGizmo->bComputeLight=false;
    xRotateGizmo->name="xRotateGizmo";
    xRotateGizmo->objectID=-20.0f;
    xRotateGizmo->setLocation(Vector3f(0,0,0));
    xRotateGizmo->setScale(Vector3f(1.5,1.5,1.5));
    xRotateGizmo->setBase(this);
    xRotateGizmo->bRemoveable=false;
    xRotateGizmo->bTextured=true;
    xRotateGizmo->sceneShaderID="texture";
    xRotateGizmo->setTextureID("rotateArrow");
    xRotateGizmo->drawType=DRAW_PLANE;
    xRotateGizmo->color=Vector4f(1.0,0.0,0.0,1);
    sceneData->helperList.push_back(xRotateGizmo);

    yRotateGizmo=new Actor;
    yRotateGizmo->bComputeLight=false;
    yRotateGizmo->name="yRotateGizmo";
    yRotateGizmo->objectID=-21.0f;
    yRotateGizmo->setLocation(Vector3f(0,0,0));
    yRotateGizmo->setRotation(Vector3f(0,90,0));
    yRotateGizmo->setScale(Vector3f(1.5,1.5,1.5));
    yRotateGizmo->setBase(this);
    yRotateGizmo->bRemoveable=false;
    yRotateGizmo->bTextured=true;
    yRotateGizmo->sceneShaderID="texture";
    yRotateGizmo->setTextureID("rotateArrow");
    yRotateGizmo->drawType=DRAW_PLANE;
    yRotateGizmo->color=Vector4f(0.0,1.0,0.0,1);
    sceneData->helperList.push_back(yRotateGizmo);

    zRotateGizmo=new Actor;
    zRotateGizmo->bComputeLight=false;
    zRotateGizmo->name="zRotateGizmo";
    zRotateGizmo->objectID=-22.0f;
    zRotateGizmo->setLocation(Vector3f(0,0,0));
    zRotateGizmo->setRotation(Vector3f(90,0,0));
    zRotateGizmo->setScale(Vector3f(1.5,1.5,1.5));
    zRotateGizmo->setBase(this);
    zRotateGizmo->bRemoveable=false;
    zRotateGizmo->bTextured=true;
    zRotateGizmo->sceneShaderID="texture";
    zRotateGizmo->setTextureID("rotateArrow");
    zRotateGizmo->drawType=DRAW_PLANE;
    zRotateGizmo->color=Vector4f(0.0,0.0,1.0,1);
    sceneData->helperList.push_back(zRotateGizmo);

    //create 3 arrows
    //connect them all to master gizmo
    xAxisGizmo=new Actor;
    xAxisGizmo->bComputeLight=false;
    xAxisGizmo->name="xAxisGizmo";
    xAxisGizmo->objectID=-10.0f;
    xAxisGizmo->setLocation(Vector3f(1,0,0));
    xAxisGizmo->setScale(Vector3f(1,1,1));
    xAxisGizmo->drawType=DRAW_CUBE;
    xAxisGizmo->setBase(this);
    xAxisGizmo->bRemoveable=false;
    xAxisGizmo->bTextured=true;
    xAxisGizmo->sceneShaderID="texture";
    xAxisGizmo->setTextureID("arrow");
    xAxisGizmo->drawType=DRAW_PLANE;
    xAxisGizmo->color=Vector4f(1,0,0,1);
    sceneData->helperList.push_back(xAxisGizmo);


    yAxisGizmo=new Actor;
    yAxisGizmo->bComputeLight=false;
    yAxisGizmo->name="yAxisGizmo";
    yAxisGizmo->objectID=-11.0f;
    yAxisGizmo->setLocation(Vector3f(0,1,0));
    yAxisGizmo->setRotation(Vector3f(0,0,-90));
    yAxisGizmo->drawType=DRAW_CUBE;
    yAxisGizmo->setBase(this);
    yAxisGizmo->bRemoveable=false;
    yAxisGizmo->bTextured=true;
    yAxisGizmo->sceneShaderID="texture";
    yAxisGizmo->setTextureID("arrow");
    yAxisGizmo->drawType=DRAW_PLANE;
    yAxisGizmo->color=Vector4f(0.4,1,0.4,1);
    sceneData->helperList.push_back(yAxisGizmo);


    zAxisGizmo=new Actor;
    zAxisGizmo->bComputeLight=false;
    zAxisGizmo->name="zAxisGizmo";
    zAxisGizmo->objectID=-12.0f;
    zAxisGizmo->setLocation(Vector3f(0,0,-1));
    zAxisGizmo->setRotation(Vector3f(0,-90,0));
    zAxisGizmo->drawType=DRAW_CUBE;
    zAxisGizmo->setBase(this);
    zAxisGizmo->bRemoveable=false;
    zAxisGizmo->bTextured=true;
    zAxisGizmo->sceneShaderID="texture";
    zAxisGizmo->setTextureID("arrow");
    zAxisGizmo->drawType=DRAW_PLANE;
    zAxisGizmo->color=Vector4f(0.4,0.4,1,1);
    sceneData->helperList.push_back(zAxisGizmo);


}

void ActorGizmo::trigger(Actor * other){


}


void ActorGizmo::beginGizmoing(){

        bGizmoing=true;

}

void ActorGizmo::endGizmoing(){

        bGizmoing=false;
        moveAxis=NULL;
        rotAxis=NULL;

}

void ActorGizmo::update(double deltaTime){

    Actor::update(deltaTime);

    //positioning and rotating when a group of actors are selected
    if (sceneData->selectedActors.size()>0 && sceneData->selectedActors[0]->name!="ground" && sceneData->selectedActors[0]->name!="grid"){

    //highlighting and un-highlighting

        if (input->startPressLeftBtn==0){

            //movement
            if (input->worldTarget==xAxisGizmo){
                input->worldTarget->color=COLOR_YELLOW;
            }else{
                xAxisGizmo->color=COLOR_RED;
            }

            if (input->worldTarget==yAxisGizmo){
                input->worldTarget->color=COLOR_YELLOW;
            }else{
                yAxisGizmo->color=COLOR_GREEN;
            }

            if (input->worldTarget==zAxisGizmo){
                input->worldTarget->color=COLOR_YELLOW;
            }else{
                zAxisGizmo->color=COLOR_BLUE;
            }

            //rotation
            if (input->worldTarget==xRotateGizmo){
                input->worldTarget->color=COLOR_YELLOW;
            }else{
                xRotateGizmo->color=COLOR_RED;
            }

            if (input->worldTarget==yRotateGizmo){
                input->worldTarget->color=COLOR_YELLOW;
            }else{
                yRotateGizmo->color=COLOR_GREEN;
            }

            if (input->worldTarget==zRotateGizmo){
                input->worldTarget->color=COLOR_YELLOW;
            }else{
                zRotateGizmo->color=COLOR_BLUE;
            }
        }


        xAxisGizmo->bHidden=false;
        yAxisGizmo->bHidden=false;
        zAxisGizmo->bHidden=false;

        xRotateGizmo->bHidden=false;
        yRotateGizmo->bHidden=false;
        zRotateGizmo->bHidden=false;

        //this should be in between all actors...
        Vector3f centerLocation;
        Vector3f centerAxisX,centerAxisY,centerAxisZ;
        Vector3f centerScale;

        for (int i=0;i<(int)sceneData->selectedActors.size();i++){
            centerLocation+=sceneData->selectedActors[i]->location;
            centerAxisX+=sceneData->selectedActors[i]->xAxis;
            centerAxisY+=sceneData->selectedActors[i]->yAxis;
            centerAxisZ+=sceneData->selectedActors[i]->zAxis;
            centerScale+=sceneData->selectedActors[i]->scale;
        }

        centerLocation=centerLocation * 1.0/(float)(sceneData->selectedActors.size());
        centerScale=centerScale * 1.0/(float)(sceneData->selectedActors.size());
        centerAxisX=centerAxisX * 1.0/(float)(sceneData->selectedActors.size());
        centerAxisY=centerAxisY * 1.0/(float)(sceneData->selectedActors.size());
        centerAxisZ=centerAxisZ * 1.0/(float)(sceneData->selectedActors.size());

        centerAxisX.normalize();
        centerAxisY.normalize();
        centerAxisZ.normalize();


        setLocation(centerLocation);
        setRotation(centerAxisX,centerAxisY,centerAxisZ);
        setScale(centerScale);

        //distance to camera plays a role here...
        Vector3f camDistance=location - sceneData->controller->controlledActor->location;
        float dist=camDistance.length();


        //camera's y-Axis is our screen yAxis
        Vector3f camerayAxis = sceneData->controller->controlledActor->yAxis;
        Vector3f cameraxAxis = sceneData->controller->controlledActor->xAxis;

        Vector3f mouseToCamera = -camerayAxis* input->mouseVector.y+cameraxAxis*input->mouseVector.x;


        mouseToCamera.normalize();

        float lr=0.0;
        //actual movement/rotation
        if (bGizmoing){
            //this is how it kinda behaves in unity, so let's haveit like this...
            for (int i=0;i<(int)sceneData->selectedActors.size();i++){
                if (moveAxis){
                    lr=-mouseToCamera.dotProduct(*moveAxis);
                    if (lr != lr) lr=0.0;
                    sceneData->selectedActors[i]->setLocation(sceneData->selectedActors[i]->location+ (*moveAxis) * 0.001 * abs(dist) * input->mouseVector.length()* lr );
                }
                if (rotAxis){
                    lr=-1.0;
                    if (input->mouseVector.x>0.0)
                      lr=1.0;
                    sceneData->selectedActors[i]->addRotation(0.02*abs(dist)  * input->mouseVector.length()* lr, *rotAxis);
                }
            }
        }

    }else{
        xAxisGizmo->bHidden=true;
        yAxisGizmo->bHidden=true;
        zAxisGizmo->bHidden=true;

        xRotateGizmo->bHidden=true;
        yRotateGizmo->bHidden=true;
        zRotateGizmo->bHidden=true;
    }

}

void ActorGizmo::create(){sceneData->addActor(this);}

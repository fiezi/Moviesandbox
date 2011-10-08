

#include "actorGizmo.h"
#include "renderer.h"
#include "input.h"

ActorGizmo::ActorGizmo(){

    drawType=DRAW_NULL;
    bGizmoing=false;
    moveAxis=Vector3f(0,0,0);

    registerProperties();
}

ActorGizmo::~ActorGizmo(){}

void ActorGizmo::registerProperties(){

    Actor::registerProperties();
}

void ActorGizmo::setup(){

    Actor::setup();
}

void ActorGizmo::trigger(Actor * other){


}


void ActorGizmo::beginGizmoing(){

        bGizmoing=true;

}

void ActorGizmo::endGizmoing(){

        bGizmoing=false;
        moveAxis=Vector3f(0,0,0);

}

void ActorGizmo::update(double deltaTime){

    Actor::update(deltaTime);


    if (sceneData->selectedActors.size()>0){

        xAxisGizmo->bHidden=false;
        yAxisGizmo->bHidden=false;
        zAxisGizmo->bHidden=false;
        //this should be in between all actors...
        Vector3f centerLocation;

        for (int i=0;i<(int)sceneData->selectedActors.size();i++)
            centerLocation+=sceneData->selectedActors[i]->location;

        centerLocation=centerLocation * 1.0/(float)(sceneData->selectedActors.size());

        setLocation(centerLocation);

        //distance to camera plays a role here...
        Vector3f camDistance=location - sceneData->controller->controlledActor->location;
        float dist=camDistance.length();

        float lr;
        if (input->mouseVector.x>0)
            lr=1.0;
        else
            lr=-1.0;


        if (bGizmoing)
            //this is how it kinda behaves in unity, so let's haveit like this...
            for (int i=0;i<(int)sceneData->selectedActors.size();i++)
                sceneData->selectedActors[i]->setLocation(sceneData->selectedActors[i]->location+Vector3f(0.001*abs(dist)* moveAxis.x * input->mouseVector.length()* lr,  0.001*abs(dist)* moveAxis.y * input->mouseVector.y,   0.001*abs(dist) * moveAxis.z * input->mouseVector.length()*lr));
            //sceneData->selectedActors[0]->setLocation(sceneData->selectedActors[0]->location+Vector3f(0.001*abs(dist)* moveAxis.x * input->mouseVector.x,  0.001*abs(dist)* moveAxis.y * input->mouseVector.y,   0.001*abs(dist) * moveAxis.z * input->mouseVector.x));
    }else{
        xAxisGizmo->bHidden=true;
        yAxisGizmo->bHidden=true;
        zAxisGizmo->bHidden=true;
    }

}

void ActorGizmo::create(){sceneData->addActor(this);}



#include "particleSpray.h"
#include "renderer.h"
#include "control.h"
#include "meshData.h"
ParticleSpray::ParticleSpray(){

bSpray=false;
flySpeed=0.005;
sprayDistance=12.0f;
maxParticles=1000;
xStretch= 0.1;
sprayHeight=5.0;
sprayFrequency=0.3;
color=Vector4f(5,5,5,0);
name="ParticleSpray";
registerProperties();
}

ParticleSpray::~ParticleSpray(){}

void ParticleSpray::registerProperties(){

createMemberID("BSPRAY",&bSpray,this,true,"10BoolButton");
createMemberID("FLYSPEED",&flySpeed,this);
createMemberID("SPRAYDISTANCE",&sprayDistance,this);
createMemberID("SPRAYHEIGHT",&sprayHeight,this);
createMemberID("SPRAYFREQUENCY",&sprayFrequency,this);
createMemberID("MAXPARTICLES",&maxParticles,this);
Actor::registerProperties();
}

void ParticleSpray::setup(){

ParticleSystem::setup();
myData=new MeshData;
vboMeshID=name;
sceneData->vboList[name]=myData;
}

void ParticleSpray::trigger(MsbObject* other){

    bSpray=!bSpray;
}

void ParticleSpray::update(double deltaTime){


    if (bSpray){
        sprayParticles(deltaTime);
    }

    ParticleSystem::update(deltaTime);
    for (int i=0;i<(int)myData->vData.size();i++){
        if (myData->vData[i].location.z>sprayDistance){
            myData->vData.erase(myData->vData.begin()+i);
        }
    }


    for (int i=0;i<(int)myData->vData.size();i++){

        float individual=fabs(cos(myData->vData[i].birth) * 0.1);
        myData->vData[i].location.x+=sin((float)i * deltaTime * 0.001) * xStretch;
        myData->vData[i].location.z+=deltaTime* 0.001 *flySpeed + individual * deltaTime * 0.001;
        myData->vData[i].location.y=sin(myData->vData[i].location.z * sprayFrequency) * (sprayHeight + 0.1*individual);
        myData->vData[i].location.w=sin(myData->vData[i].location.z * sprayFrequency) * (sprayHeight + 0.1*individual);
    }


}

void ParticleSpray::sprayParticles(double deltaTime){


    if ((int)myData->vData.size()<maxParticles){
        addParticles(1);
    }





}


void ParticleSpray::create(){sceneData->addActor(this);}

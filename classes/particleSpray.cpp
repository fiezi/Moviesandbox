

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
registerProperties();
}

ParticleSpray::~ParticleSpray(){}

void ParticleSpray::registerProperties(){

createMemberID("BSPRAY",&bSpray,this);
createMemberID("FLYSPEED",&flySpeed,this);
createMemberID("SPRAYDISTANCE",&sprayDistance,this);
createMemberID("SPRAYHEIGHT",&sprayHeight,this);
createMemberID("SPRAYFREQUENCY",&sprayFrequency,this);
createMemberID("MAXPARTICLES",&maxParticles,this);
Actor::registerProperties();
}

void ParticleSpray::setup(){

ParticleSystem::setup();
}

void ParticleSpray::trigger(MsbObject* other){

    bSpray=!bSpray;

}

void ParticleSpray::update(double deltaTime){

/*
    if (bSpray && Control::bRunning){
        sprayParticles(deltaTime);
    }else{
        while((int)myData->vData.size()>0){
            deleteParticle(0);
        }
    }
*/
    ParticleSystem::update(deltaTime);
}

void ParticleSpray::sprayParticles(double deltaTime){

/*
    if ((int)myData->vData.size()<maxParticles)
        addRandomParticles(1,1);


    for (int i=0;i<(int)myData->vData.size();i++){
        float individual=fabs(cos(myData->vData[i].birth) * 0.1);
        myData->vData[i].location.x+=sin((float)i * deltaTime * 0.001) * xStretch;
        myData->vData[i].location.z+=deltaTime*flySpeed + individual;
        myData->vData[i].location.y=sin(myData->vData[i].location.z * sprayFrequency) * (sprayHeight + individual);
        if (myData->vData[i].location.z>sprayDistance){
            myData->vData[i].location.z=0.0f;
            myData->vData[i].location.x=0.0f;
        }
    }
*/
};

void ParticleSpray::create(){renderer->addActor(this);}

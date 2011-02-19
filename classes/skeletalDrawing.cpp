

#include "skeletalDrawing.h"
#include "renderer.h"
#include "control.h"
#include "boneActor.h"
#include "skeletalActor.h"
#include "spriteMeshLoader.h"
#include "input.h"
#include "drawingWidget.h"

SkeletalDrawing::SkeletalDrawing(){


drawType=DRAW_PARTICLES;
skeletonName="NULL";
registerProperties();
}

SkeletalDrawing::~SkeletalDrawing(){}

void SkeletalDrawing::registerProperties(){

createMemberID("SKELETONNAME",&skeletonName,this);
ParticleSystem::registerProperties();
}

void SkeletalDrawing::setup(){

ParticleSystem::setup();
}

void SkeletalDrawing::updateShaders(){

    ParticleSystem::updateShaders();

}

void SkeletalDrawing::trigger(Actor * other){

}



void SkeletalDrawing::create(){renderer->addActor(this);}

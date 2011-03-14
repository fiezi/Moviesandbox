

#include "skeletalHead.h"
#include "renderer.h"

SkeletalHead::SkeletalHead(){

name="SkeletalHead";

mouthUp=NULL;
mouthDown=NULL;

mouthOpen=0.0f;

mouthAmp = 1.0f;
registerProperties();
}

SkeletalHead::~SkeletalHead(){}

void SkeletalHead::registerProperties(){

createMemberID("MOUTHOPEN",&mouthOpen,this);
createMemberID("MOUTHUP",&mouthUp,this);
createMemberID("MOUTHAMP",&mouthAmp,this);
createMemberID("MOUTHDOWN",&mouthDown,this);
SkeletalActor::registerProperties();

}

void SkeletalHead::setup(){

    SkeletalActor::setup();

    if (!mouthUp){
        for (int i=0;i<(int)bones.size();i++){
            if (bones[i]->name=="mouthUp"){
                mouthUp=bones[i];
                baseMouthUpMatrix=mouthUp->transformMatrix;
                cout << "yay!!! Found mouth up!!" << endl;
            }
        }
    }
    if (!mouthDown){
        for (int i=0;i<(int)bones.size();i++){
            if (bones[i]->name=="mouthDown"){
                mouthDown=bones[i];
                baseMouthDownMatrix=mouthDown->transformMatrix;
                cout << "yay!!! Found mouth down!!" << endl;
            }
        }
    }
}

void SkeletalHead::trigger(MsbObject* other){}

void SkeletalHead::update(double deltaTime){

    SkeletalActor::update(deltaTime);

    if (mouthUp){
        mouthUp->transformMatrix.setTranslation(Vector3f(0,2.5 * mouthAmp * mouthOpen,0));
        //mouthUp->transformMatrix=baseMouthMatrix * mouthUp->transformMatrix;
    }
    if (mouthDown){
        mouthDown->transformMatrix.setTranslation(Vector3f(0,-1.5 * mouthAmp * mouthOpen,0));
        //mouthUp->transformMatrix=baseMouthMatrix * mouthUp->transformMatrix;
    }
}

void SkeletalHead::create(){renderer->addActor(this);}

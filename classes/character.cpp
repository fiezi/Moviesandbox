
#include "control.h"
#include "character.h"
#include "renderer.h"
#include "skeletalHead.h"

Character::Character(){

    walkTarget=NULL;
    idleAction=renderer->actionList["std_idle"];
    idleActionName="std_idle";

    lookTarget=NULL;
    walkTarget=NULL;
    mouth=NULL;

    leftFoot=NULL;
    rightFoot=NULL;

    leftHand=NULL;
    rightHand=NULL;
    grabActor=NULL;

    idleMover=NULL;
    animMover=NULL;

    currentStep=NULL;
    lastStep=NULL;

    vboMeshID="sticky";
    sceneShaderID="skeletal";

    footYOffset=0.0;

    //for now
    bFacingTarget=false;
    bBreathing=false;
    bKnowStep=false;
    bFirstStep=true;

    walkSpeed= 4;
    turnSpeed =40;
    idleAnimTimeScale= 0.75;

    currentStep=0;
    stepStart=0;
    walkDirection=Vector3f(0,0,-1);
    walkDirection.normalize();
    startBodyLocation=Vector3f(0,0,0);
    targetBodyLocation=Vector3f(0,0,0);
    stepSize=1.0;
    stepWidth=0.0;
    stepDuration = 0.5;           //in seconds

    bobStrength = 0.15;
    hipNess=0.1;
    leanForward= 0.5;
    liftLegs = 0.5;


    breathStrength=0.00025;

    registerProperties();
}

Character::~Character(){}

void Character::registerProperties(){

    SkeletalActor::registerProperties();
    createMemberID("IDLEACTIONNAME",&idleActionName,this);
    createMemberID("IDLEANIMTIMESCALE",&idleAnimTimeScale,this);
    createMemberID("WALKTARGET",&walkTarget,this);
    createMemberID("LOOKTARGET",&lookTarget,this);
    createMemberID("WALKSPEED",&walkSpeed,this);
    createMemberID("TURNSPEED",&turnSpeed,this);
    createMemberID("BBREATHING",&bBreathing,this);
    createMemberID("BREATHSTRENGTH",&breathStrength,this);
    createMemberID("MOUTH",&mouth,this);
    createMemberID("LEFTFOOT",&leftFoot,this);
    createMemberID("RIGHTFOOT",&rightFoot,this);

    createMemberID("LEFTHAND",&leftHand,this);
    createMemberID("RIGHTHAND",&rightHand,this);
    createMemberID("GRABACTOR",&grabActor,this);

    createMemberID("STEPSIZE",&stepSize,this);
    createMemberID("STEPWIDTH",&stepWidth,this);
    createMemberID("STEPDURATION",&stepDuration,this);
    createMemberID("BOBSTRENGTH",&bobStrength,this);
    createMemberID("HIPNESS",&hipNess,this);
    createMemberID("LEANFORWARD",&leanForward,this);
    createMemberID("LIFTLEGS",&liftLegs,this);
}

void Character::setup(){

    SkeletalActor::setup();

    idleAction=renderer->actionList[idleActionName];

    //find feet and hands!

    for (int i=0; i<(int)bones.size();i++){

        if (bones[i]->name=="rightFoot"){
            rightFoot=bones[i];
        }
        if (bones[i]->name=="rightHand"){
            rightHand=bones[i];
        }
        if (bones[i]->name=="leftFoot"){
            leftFoot=bones[i];
        }
        if (bones[i]->name=="leftHand"){
            leftHand=bones[i];
        }

    }

    if (leftFoot && rightFoot){
        leftFootDelta=leftFoot->baseMatrix.getTranslation() - baseMatrix.getTranslation();
        rightFootDelta=rightFoot->baseMatrix.getTranslation() - baseMatrix.getTranslation();
        footYOffset = leftFootDelta.y;
    }
}


///updates idleAction to reflect possible change in name!
void Character::trigger(Actor * other){

    idleAction=renderer->actionList[idleActionName];

    cout << "IdleAction: " << idleAction->name << endl;

    SkeletalActor::trigger(other);
}

void Character::update(double deltaTime){

    /*
    if (idleMover->bFinished)
        idleMover=NULL;
    if (walkMover->bFinished)
        walkMover=NULL;
    */

    SkeletalActor::update(deltaTime);

    //Vector3f myLoc=Vector3f(sin(renderer->currentTime),0,0);

    //setLocation(location+myLoc);

    if (Control::bRunning){

        if (lookTarget)
            lookAt(lookTarget);

        if (grabActor)
            grab();

        if (walkTarget)
            walkTowards(walkTarget, deltaTime * 0.001);
        else
            idle(deltaTime);
     }
}


void Character::stop(){

    bKnowStep=false;
    bFirstStep=true;
    if (currentStep){
        currentStep->remove();
        currentStep=NULL;
    }
    if (lastStep){
        lastStep->remove();
        lastStep=NULL;
    }
    SkeletalActor::stop();
    idleMover=NULL;
}

void Character::updateShaders(){

    if (bBreathing){

        //breathing
        float   sinNoise=sin(  renderer->currentTime * 0.0012 );


        for (int i=0;i<(int)bones.size();i++){
                if (bones[i]->name=="spine"){
                    bones[i]->transformMatrix.data[15]+=sinNoise * breathStrength;
                }

                if (bones[i]->name=="upperTorso"){
                    bones[i]->transformMatrix.data[15]-=sinNoise * breathStrength;
                }
        }
    }



    SkeletalActor::updateShaders();
}

void Character::idle(double deltaTime){


    if (!idleMover){
        createIdleMover();
    }

}


void Character::grab(){

    //BoneActor* grabHand = (BoneActor*) rightHand;
    BoneActor* grabHand = (BoneActor*) rightFoot;
    grabHand->doIK(grabActor->baseMatrix.getTranslation(),true);
}


///figure out if we need to turn or compute the next step here
void Character::walkTowards(Actor* target, double deltaTime){


    Vector3f walkDistance=baseMatrix.getTranslation() - walkTarget->baseMatrix.getTranslation();

    if (walkDistance.length()>1.0){

    turnTowards(target, deltaTime);

        if (!bKnowStep)
            computeStep();

        walk(deltaTime);
    }
    //finish stepping here - by not computing new steps!
    else{
        //walk(deltaTime);
    }
}


///targets for feet are computed here
void Character::computeStep(){

    walkDirection=walkTarget->location - location;

    //for now:
    walkDirection.y=0.0;

    walkDirection.normalize();


    //let's say, we walk forward, in -z
    float halfStep= stepSize * 0.5;

    Vector3f footLine=walkDirection.crossProduct(yAxis);

    //rotate footDeltas
    Vector3f newLeftFootDelta=footLine * leftFootDelta.x + walkDirection * leftFootDelta.z + yAxis * leftFootDelta.y;
    Vector3f newRightFootDelta=footLine * rightFootDelta.x + walkDirection * rightFootDelta.z + yAxis * rightFootDelta.y;

    Vector3f leftFootPos=baseMatrix.getTranslation() + newLeftFootDelta;
    Vector3f rightFootPos=baseMatrix.getTranslation() + newRightFootDelta;

    //leftFootPos=leftFoot->baseMatrix.getTranslation();
    //rightFootPos=rightFoot->baseMatrix.getTranslation();


    // leftFootPos.y=0.0;
    // rightFootPos.y=0.0;
   if (bFirstStep){
        //left Foot
        Actor* l=spawn("5Actor");
        l->name="leftStep";
        l->bHidden=true;
        l->drawType=DRAW_CUBE;
        l->setLocation(leftFootPos);
        l->setScale(Vector3f(0.5,0.5,0.5));
        lastStep=l;

        //right Foot
        //here we start with a half step!
        Actor* r=spawn("5Actor");
        r->name="rightStep";
        r->bHidden=true;
        r->drawType=DRAW_CUBE;
        r->setLocation(rightFootPos /*- Vector3f(stepWidth,0,0)*/ + walkDirection * halfStep);
        r->setScale(Vector3f(0.5,0.5,0.5));
        currentStep=r;

        //made the first step
        bFirstStep=false;
    }else{
        //last current becomes new old
        lastStep->remove();
        lastStep=currentStep;

        if (lastStep->name=="rightStep"){
            Actor* l=spawn("5Actor");
            l->name="leftStep";
            l->drawType=DRAW_CUBE;
            l->setLocation(leftFootPos /*- Vector3f(stepWidth,0,0)*/ + walkDirection * stepSize );
            l->setScale(Vector3f(0.5,0.5,0.5));
            l->bHidden=true;
            currentStep=l;
        }else{
            Actor* r=spawn("5Actor");
            r->name="rightStep";
            r->drawType=DRAW_CUBE;
            r->setLocation(rightFootPos /*- Vector3f(stepWidth,0,0)*/ + walkDirection * stepSize );
            r->setScale(Vector3f(0.5,0.5,0.5));
            r->bHidden=true;
            currentStep=r;
        }

    }

    stepStart=renderer->currentTime/1000.0;

    targetBodyLocation= walkDirection * stepSize * 0.5;// * leanForward;
    startBodyLocation=baseMatrix.getTranslation();

    bFirstStep=false;
    bKnowStep=true;
}


///actual moving of bones is done here
void Character::walk(double deltaTime){

    BoneActor* activeFoot;
    BoneActor* passiveFoot;
    //how far?
    float stepTime=renderer->currentTime/1000.0 - stepStart;
    //make linear between 0 - 1 to stepTime
    stepTime=stepTime/stepDuration;
    //is left step?

    deltaTime=deltaTime/1000.0;

    //determine active and passive foot
    if (currentStep->name=="leftStep"){
        //step finished!
            activeFoot=(BoneActor*)leftFoot;        //moving foot
            passiveFoot=(BoneActor*)rightFoot;         //resting foot
        }
    else{
            activeFoot=(BoneActor*)rightFoot;       //moving foot
            passiveFoot=(BoneActor*)leftFoot;          //resting foot
    }



    float yPart= liftLegs * sin(stepTime*M_PI);
    float yPartBody= bobStrength * cos(stepTime* M_PI * 2) - bobStrength ;

    float bodyY=originalMatrix.getTranslation().z - hipNess;
    Vector3f newBodyLocation=startBodyLocation + targetBodyLocation * stepTime;
    newBodyLocation.y=bodyY+yPartBody - footYOffset;
    //first move body linear
    setLocation(newBodyLocation);

    //update feet matrices
    passiveFoot->baseMatrix=calcMatrix(passiveFoot);
    passiveFoot->matrixToVectors();

    activeFoot->baseMatrix=calcMatrix(activeFoot);
    activeFoot->matrixToVectors();

    //then move other Foot back this amount
    Vector3f passivePlace= lastStep->baseMatrix.getTranslation();// - otherFoot->baseMatrix.getTranslation();
    //otherFoot->doIK( otherPlace, true );
    passiveFoot->doIK( passivePlace, true );
    //then move forward foot
    //find step distance - from other foot to finished step

    Vector3f stepDistance = walkDirection * stepSize;

    Vector3f stepPosition = currentStep->baseMatrix.getTranslation() - stepDistance * (1.0-stepTime);
    activeFoot->doIK( stepPosition + Vector3f(0,yPart,0), true   );


    if (stepTime + deltaTime> 1.0){
        bKnowStep=false;
        return;
    }

   // addRotation(0.1,Vector3f(0,1,0));
}

void Character::createIdleMover(){

    InterpolationHelper * interpolator=new InterpolationHelper;

    interpolator->bInterpolateMatrix=true;

    interpolator->startTime=renderer->currentTime;
    interpolator->keyFrames=idleAction->keyFrames;

     //apply current Location/rotation if we want our action to be performed relative

    interpolator->currentKey=0;
    interpolator->bFinished=false;
    interpolator->bLooping=true;
    interpolator->bLinear=false;
    interpolator->moveActor=this;

    interpolator->bMerge=true;
    interpolator->mergeDirection=+1.0f;

    interpolator->timeScale=idleAnimTimeScale;
    idleMover=interpolator;
    movers.push_back(interpolator);
}

void Character::createAnimMover(Action* anim,bool bLoop, float inPoint){

    cout << "CREATING ANIM MOVER!" << endl;

    InterpolationHelper * interpolator=new InterpolationHelper;

    interpolator->bInterpolateMatrix=true;

    interpolator->startTime=renderer->currentTime;
    interpolator->keyFrames=anim->keyFrames;

     //apply current Location/rotation if we want our action to be performed relative

    interpolator->currentKey=0;
    interpolator->bFinished=false;

    interpolator->bLooping=bLoop;
    interpolator->bLinear=false;
    interpolator->moveActor=this;
    interpolator->bMerge=true;
    interpolator->mergeDirection=+1.0;
    interpolator->inPoint=inPoint;
    animMover=interpolator;
    movers.push_back(interpolator);
}


void Character::turnTowards(Actor* target, double deltaTime){

   // cout << "turning..." << endl;

    Vector3f orientationToTarget=target->location - location;
    orientationToTarget.y=0;
    orientationToTarget.normalize();

    Vector3f myOrientation = zAxis;

    myOrientation.y=0;
    myOrientation.normalize();

    float rotAmount;

    if (orientationToTarget.crossProduct(myOrientation).y>0.0)
        rotAmount=turnSpeed * deltaTime;
    else
        rotAmount=-turnSpeed * deltaTime;

    addRotation(rotAmount,yAxis);
}

void Character::lookAt(Actor* target){
    //generate look axis
    Vector3f lookZ= target->location-location;
    lookZ.normalize();
    Vector3f lookY=Vector3f(0,1,0);
    Vector3f lookX=lookZ.crossProduct(lookY);
    lookY=lookX.crossProduct(lookZ);
    //assign rotation to transformMatrix of neckBone
    Actor* neckBone=NULL;

    for (int i=0;i<(int)bones.size();i++){
        if (bones[i]->name=="neck")
            neckBone=bones[i];
    }

    if (!neckBone){
        cout << "cannot lookat - no neck found!" << endl;
        return;
    }

    Matrix4f lookMatrix;
    lookMatrix.identity();

    lookMatrix.data[0]=lookX.x;
    lookMatrix.data[1]=lookX.y;
    lookMatrix.data[2]=lookX.z;

    lookMatrix.data[4]=lookY.x;
    lookMatrix.data[5]=lookY.y;
    lookMatrix.data[6]=lookY.z;

    lookMatrix.data[8]=lookZ.x;
    lookMatrix.data[9]=lookZ.y;
    lookMatrix.data[10]=lookZ.z;

    neckBone->transformMatrix=lookMatrix * neckBone->transformMatrix;

}

void Character::playAnim(Action* anim, bool bLoop, float inPoint){

//directly playing animation!
    //idleMover->bFinished=true;
    //idleMover=NULL;


    createAnimMover(anim, bLoop, inPoint);
}

//stop last action
void Character::stopAnim(){

    if (movers.size()>1){
        movers[movers.size()-1]->bMerge=true;
        movers[movers.size()-1]->bLooping=false;
        movers[movers.size()-1]->mergeDirection=-1.0;
        movers[movers.size()-1]->mergeFactor=1.0;

    }
}

void Character::create(){renderer->addActor(this);}

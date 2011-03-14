

#include "boneActor.h"
#include "renderer.h"
#include "input.h"
#include "character.h"
#include "meshData.h"

BoneActor::BoneActor(){

name="Bone";
drawType=DRAW_SPRITE;
bTextured=true;
bComputeLight=false;
particleScale=5;
sceneShaderID="texture";
textureID="sprite_bone";
scale=Vector3f(1, 1, 1);
mySystem=NULL;
influence=10;
collisionCubeSize=2;

bComputeLight=false;
bZTest=true;
bZWrite=true;

bPhysicsBone=false;
bJointedToBase=true;
bFixToWorld=false;

bDoIK=false;


sideRotation = 0.0;
f=0.0;

registerProperties();
}

BoneActor::~BoneActor(){}

void BoneActor::registerProperties(){

createMemberID("BDOIK",&bDoIK,this);
Actor::registerProperties();
}

void BoneActor::setup(){

Actor::setup();

}

void BoneActor::trigger(MsbObject* other){

}

void BoneActor::setLocation(Vector3f loc){

    //has two parents, is vboMesh and skeletal
    if (1==0)//bDoIK)
        doIK(loc, false);
    else{
        Actor::setLocation(loc);
    }
    //make sure base's x-Axis always points to this bone!

}

void BoneActor::update(double deltaTime){

    Actor::update(deltaTime);

}


void BoneActor::alignBaseRotation(){

    //update base so it has its x-Axis always pointed towards us!
    //and update our own rotation to compensate accordingly...

    Vector3f nLoc=transformMatrix.getTranslation();
    //Vector3f nLoc=baseMatrix.getTranslation() - base->baseMatrix.getTranslation();
    float dist=nLoc.length();
    nLoc.normalize();

    //create base axis
    Vector3f xA,yA,zA;

    xA=nLoc;

    //use zAxis or yAxis?
    if (abs(xA.y) > 0.8){
        zA=Vector3f(0,0,1);
        yA=xA.crossProduct(zA);
        zA=yA.crossProduct(xA);
    }
    else{
        yA=Vector3f(0,1,0);
        zA=xA.crossProduct(yA);
        yA=zA.crossProduct(xA);
    }

    Vector3f oldBaseRotation=base->rotation;

    base->setRotationFromAxis(xA,yA,zA);

    Vector3f newLoc= Vector3f(1,0,0) * dist;

    Actor::setLocation(newLoc);
    Actor::setRotation(-oldBaseRotation);
}

void BoneActor::doIK(Vector3f loc, bool bAbsolute){

    /*********************************************************************************

        3 part process:

        1. gather all necessary numbers
        2. rotate the system into target plane (plane between target point and shoulder)
        3. do 2D IK

    *********************************************************************************/

    Actor* actorParent=(Actor*)parent;

    //leave if we don't have two parents
    if (!base || !base->base ){
        cout << "no double-base!!! Cannot do IK" << endl;
        Actor::setLocation(loc);
        return;
    }

    //for debug stuff
    f+=0.01;

    ///clear transform Matrices!

    Matrix4d initialTransform;

    initialTransform=base->base->transformMatrix;

    base->base->transformMatrix.identity();
    base->transformMatrix.identity();
    transformMatrix.identity();
    base->base->baseMatrix=calcMatrix(base->base);
    base->baseMatrix=calcMatrix(base);
    baseMatrix=calcMatrix(this);

    ///gathering numbers

    Vector3f currentUpperVec, currentLowerVec, initialUpperVec, initialLowerVec;

    MeshData* myMesh=renderer->vboList[actorParent->vboMeshID];
    //find original angles for non-aligned bones - from mesh data!
    for (int i=0;i<(int)myMesh->bones.size();i++){
        if (myMesh->bones[i]->name==base->name)
            initialUpperVec=myMesh->bones[i]->boneMatrix->getTranslation();
        if (myMesh->bones[i]->name==name)
            initialLowerVec=myMesh->bones[i]->boneMatrix->getTranslation();
    }

    float upperLength = initialUpperVec.length();
    float lowerLength = initialLowerVec.length();

    //initial Full - without any transformations
    //this is for the relative positions of the bones to each other
    Vector3f initialFull= initialUpperVec + initialLowerVec;
    //project upper on full vector to get upper Angle

    initialUpperVec.normalize();
    initialLowerVec.normalize();
    initialFull.normalize();

    Vector3f currentFull=base->base->baseMatrix.getTranslation() - baseMatrix.getTranslation();
    Vector3f targetFull=base->base->baseMatrix.getTranslation() - loc;

    float targetFullLength = targetFull.length();


    ///out of bounds!!!
    if (targetFullLength>=upperLength+lowerLength){
        cout << "IK on " << name << " is out of bounds!" << endl;
        cout << targetFullLength << endl;
        return;
    }

    currentFull.normalize();
    targetFull.normalize();

    ///rotating into IK plane
    Matrix4f parentMatrix=actorParent->baseMatrix;
    parentMatrix.setTranslation(Vector3f(0,0,0));

    Vector3f rotAxis = currentFull.crossProduct(targetFull);
    rotAxis=parentMatrix.inverse() * rotAxis;
    rotAxis.normalize();

    float rot= acos(currentFull.dotProduct(targetFull));

    //check if they're on each other!
    if (rot!=rot){
        cout << "already in plane!" << endl;
        rot=0.0;    //no rotation in that case!
    }
    Vector3f xA,yA,zA;
    yA=rotAxis;
    xA=currentFull;
    zA=yA.crossProduct(xA);

    zA.normalize();

    Matrix4f rotMatrix,angleMatrix;

    rotMatrix[0]=xA.x;
    rotMatrix[1]=xA.y;
    rotMatrix[2]=xA.z;

    rotMatrix[4]=yA.x;
    rotMatrix[5]=yA.y;
    rotMatrix[6]=yA.z;

    rotMatrix[8]=zA.x;
    rotMatrix[9]=zA.y;
    rotMatrix[10]=zA.z;


    angleMatrix=angleMatrix.createRotationAroundAxis(0,rot,0);

    rotMatrix=rotMatrix * angleMatrix * rotMatrix.inverse();

    //now rotate into IK plane
    base->base->transformMatrix=  base->base->transformMatrix * rotMatrix; //note: we can add the transformMatrix here, since rot should be 0 anyway after this!
    base->base->baseMatrix=calcMatrix(base->base);
    base->baseMatrix=calcMatrix(base);
    baseMatrix=calcMatrix(this);    //update all our matrices after transform!

    ///Math for 2D IK

    float upperAngle=0.0, middleAngle=0.0, lowerAngle=0.0;

    //using the following 2D stuff:

    //sine rule:
    // a/sinA = b/sinB = c/sinC

    //cosine rule:
    //c*c = b*b + a*a + 2ba*cosC

    //find biggest angle first
    if (upperLength>lowerLength){
        if (upperLength>targetFullLength){
            //upperLength = longest!
            lowerAngle=acos( (lowerLength*lowerLength + targetFullLength*targetFullLength - upperLength*upperLength) / (2* lowerLength * targetFullLength) );
            middleAngle=asin( targetFullLength  * sin(lowerAngle) / upperLength);
            upperAngle=asin( lowerLength  * sin(lowerAngle)/ upperLength);

        }else{
            //fullLength = longest!
            middleAngle=acos( (lowerLength*lowerLength + upperLength*upperLength - targetFullLength*targetFullLength) / (2* lowerLength * upperLength) );
            lowerAngle=asin( upperLength  * sin(middleAngle) / targetFullLength);
            upperAngle=asin( lowerLength  * sin(middleAngle) / targetFullLength);
        }
    }else if (lowerLength > targetFullLength){
            //lowerLength = longest!
            upperAngle=acos( ( targetFullLength*targetFullLength + upperLength*upperLength - lowerLength*lowerLength) / (2* upperLength * targetFullLength) );
            middleAngle=asin( targetFullLength  * sin(lowerAngle) / lowerLength);
            lowerAngle=asin( upperLength  * sin(lowerAngle) / lowerLength);
        }
    else{
        //fullLength = longest!
        middleAngle=acos( (lowerLength*lowerLength + upperLength*upperLength - targetFullLength*targetFullLength)/(2* lowerLength * upperLength) );
        lowerAngle=asin( upperLength  * sin(middleAngle) / targetFullLength);
        upperAngle=asin( lowerLength  * sin(middleAngle) / targetFullLength);
    }

    //phew!!!
    //middle rotation
    //NaN test!
    if (middleAngle!=middleAngle)
        middleAngle=0.0;

    //test NaN
    if (upperAngle!=upperAngle)
        upperAngle=0.0;

    ///gather current Info
    currentUpperVec=base->base->baseMatrix.getTranslation() - base->baseMatrix.getTranslation();
    currentUpperVec.normalize();

    currentLowerVec=base->baseMatrix.getTranslation() - baseMatrix.getTranslation();
    currentLowerVec.normalize();

    //origUpperAngle is the current 2D rotation!
    float currentUpperAngle=acos(targetFull.dotProduct(currentUpperVec));
    //check for NaN
    if (currentUpperAngle!=currentUpperAngle)
        currentUpperAngle=0.0;

    //project upper on lower Vector to get middle Angle
    float currentMiddleAngle=acos(currentLowerVec.dotProduct(currentUpperVec));
    //check for NaN
    if (currentMiddleAngle!=currentMiddleAngle)
        currentMiddleAngle=0.0;


    //add original values!
    upperAngle= upperAngle - currentUpperAngle;
    middleAngle= M_PI + middleAngle + currentMiddleAngle;// -;


    ///rotate 2D IK
    Vector3f initialRotYAxis,initialRotZAxis, initialRotXAxis;

    //now find our rotation Axis for 2D IK!
    if (currentUpperAngle==0.0){
        initialRotYAxis=Vector3f(0,1,0);
        initialRotZAxis=Vector3f(0,0,1);
        initialRotXAxis=Vector3f(1,0,0);
    }else{
        initialRotYAxis=initialFull.crossProduct(initialUpperVec);
        initialRotYAxis=initialUpperVec.crossProduct(initialFull);
        initialRotYAxis.normalize();
        initialRotXAxis=initialUpperVec;
        initialRotZAxis=initialRotXAxis.crossProduct(initialRotYAxis);
        initialRotZAxis.normalize();
    }

    xA=initialRotXAxis;
    yA=initialRotYAxis;
    zA=initialRotZAxis;

    Matrix4d initialRotMatrix;
    initialRotMatrix[0]=xA.x;
    initialRotMatrix[1]=xA.y;
    initialRotMatrix[2]=xA.z;

    initialRotMatrix[4]=yA.x;
    initialRotMatrix[5]=yA.y;
    initialRotMatrix[6]=yA.z;

    initialRotMatrix[8]=zA.x;
    initialRotMatrix[9]=zA.y;
    initialRotMatrix[10]=zA.z;


    //create a rotation matrix that applies the rotations to the x-Axis
    Matrix4f midMatrix,upMatrix;

    midMatrix=midMatrix.createRotationAroundAxis(0,middleAngle,0);
    upMatrix=upMatrix.createRotationAroundAxis(0,upperAngle,0);


    //rotate around arbitrary axis by rotating to the axis, then rotating your desired rotation, then rotating back
    midMatrix=  initialRotMatrix * midMatrix * initialRotMatrix.inverse();
    upMatrix=   initialRotMatrix * upMatrix  * initialRotMatrix.inverse();

    //rotate around first angle first
    base->base->transformMatrix=  base->base->transformMatrix * upMatrix;
    base->transformMatrix= base->transformMatrix * midMatrix;

    ///update matrices
    base->base->baseMatrix=calcMatrix(base->base);
    base->baseMatrix=calcMatrix(base);
    baseMatrix=calcMatrix(this);

}

void BoneActor::remove(){

    //TODO: very Hacky!
    //clear parent!
    if (base){
        SkeletalActor* myChar=dynamic_cast<SkeletalActor*>(base);
        if (myChar){
            for (int i=0;i<(int)myChar->bones.size();i++){
                if (myChar->bones[i]==this){
                    myChar->bones[i]=NULL;
                }
            }
        }
    }
    Actor::remove();
}

void BoneActor::create(){renderer->addActor(this);}

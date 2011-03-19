#include "interpolationHelper.h"
#include "skeletalActor.h"
#include "renderer.h"

InterpolationHelper::InterpolationHelper(){

    renderer=Renderer::getInstance();
    bFinished=false;
    bRelative=false;
    bAdditive=false;
    bLinear=false;
    bLooping=false;

    moveActor=NULL;
    targetActor=NULL;

    mergeFactor=0.0;
    mergeDirection=1.0;
    bMerge=false;

    inPoint=0.0f;
    timeScale= 0.75f;
    startTime=0.0f;
    currentTime=0;

    currentKey=0;
    currentKeyMatrix=0;
    currentKeyVectors=0;

    moveTime=1.0f;

    bInterpolateVectors=false;
    bInterpolateMatrix=false;
    bInterpolateActor=false;
    bInterpolateProperty=false;

    bFinishedMatrix=false;
    bFinishedVectors=false;
}

InterpolationHelper::~InterpolationHelper(){
}

void InterpolationHelper::interpolate(){


    //make sure we're not waiting for something that isn't going to happen
    if (!bInterpolateMatrix)
        bFinishedMatrix=true;

    if (!bInterpolateVectors)
        bFinishedVectors=true;

    //as long as there's work to do...
    if (!bFinished){

        //this gets called from interpolate node - moves one actor to another actor!
        if (bInterpolateActor)
                interpolateActor();

        if (keyFrames.size()>1){

            //bone rotations
            if (bInterpolateMatrix && !bFinishedMatrix)
                interpolateMatrix();

            //location/rotation of underlying actor
            if (bInterpolateVectors && !bFinishedVectors)
                interpolateVectors();

            //only if both are finished can we be destroyed!
			if (bFinishedMatrix && bFinishedVectors)
				bFinished=true;
        }

    }
}

void InterpolationHelper::interpolateActor(){

    Vector3f locationOne, locationTwo;
    Vector3f rotationOne, rotationTwo;

    Vector3f resultingLocation, resultingRotation;

    currentTime=renderer->currentTime-startTime;

    float relativeTime=currentTime/moveTime;

    locationOne=baseLocation;
    locationTwo=targetActor->location;

    rotationOne=baseRotation;
    rotationTwo=targetActor->rotation;

    if (currentTime>moveTime){
        resultingLocation=locationOne.lerp(1.0,locationTwo); //calculate resulting position
        resultingRotation=rotationOne.lerp(1.0,rotationTwo); //calculate resulting position
        bFinished=true;
        return;
    }

        float x=relativeTime*2.0f;
        float y=0.0f;

        if (x< 1)
            y=x*x;
        else
            y=2-( (x-2) * (x-2) );

        y=y*0.5;

        if (bLinear)
            y=relativeTime;

    resultingLocation=locationOne.lerp(y,locationTwo); //calculate resulting position
    resultingRotation=rotationOne.lerp(y,rotationTwo); //calculate resulting position

    moveActor->setLocation(resultingLocation);
    moveActor->setRotation(resultingRotation);
}

void InterpolationHelper::interpolateVectors(){

    Vector3f locationOne, locationTwo;
    Vector3f rotationOne, rotationTwo;

    Vector3f resultingLocation, resultingRotation;

    double timeKeyOne;
    double timeKeyTwo;

    currentTime=renderer->currentTime-startTime;

    //move forward if we are at end of key
    while (currentTime> keyFrames[currentKeyVectors+1]->timeKey ){
    //if (currentTime> keyFrames[currentKey+1]->timeKey ){
          currentKeyVectors++;
          if (currentKeyVectors+1 >= (int)keyFrames.size()){
            bFinishedVectors=true;
            return;
            }
          }

    timeKeyOne=keyFrames[currentKeyVectors]->timeKey;
    timeKeyTwo=keyFrames[currentKeyVectors+1]->timeKey;

    //get the time difference
    double timeDifference=timeKeyTwo-timeKeyOne;

    //map time difference to 0.0 - 1.0
    double keyTime=currentTime-timeKeyOne; //-> current Position, in the beginning 0.0;
    float relativeTime=(float) (keyTime/timeDifference); //-> will go from 0.0 to 1.0 between the keys


    if (bAdditive){
        locationOne=moveActor->location;                //
        rotationOne=moveActor->rotation;
        relativeTime*=relativeTime;      //
    }else{
        locationOne=keyFrames[currentKeyVectors]->locationKey;
        rotationOne=keyFrames[currentKeyVectors]->rotationKey;
    }


    locationTwo=keyFrames[currentKeyVectors+1]->locationKey;
    rotationTwo=keyFrames[currentKeyVectors+1]->rotationKey;

    //interpolate between them
    resultingLocation=locationOne.lerp(relativeTime,locationTwo); //calculate resulting position
    resultingRotation=rotationOne.lerp(relativeTime,rotationTwo); //calculate resulting position

    //apply resulting position
    if (bRelative){
        moveActor->setLocation(resultingLocation+baseLocation);
        moveActor->setRotation(resultingRotation+baseRotation);
        }
    else{
        moveActor->setLocation(resultingLocation);
        moveActor->setRotation(resultingRotation);
        }
}

void InterpolationHelper::interpolateMatrix(){


    if (bMerge){

        if (mergeFactor<=1.0)
            mergeFactor+=2.0 * renderer->deltaTime * 0.001 * mergeDirection;

        if (mergeFactor>1.0 ){
            mergeFactor=1.0;
        }

        if (mergeFactor<0.0 ){
            mergeFactor=1.0;
            bFinishedMatrix=true;
            return;
        }
    }


    Matrix4f matrixOne;
    Matrix4f matrixTwo;

    Matrix4f resultingMatrix;

    Matrix3f rotationOne;
    Matrix3f rotationTwo;

    Matrix3f resultingRotation;


    double timeKeyOne;
    double timeKeyTwo;

    currentTime=(renderer->currentTime-startTime) + inPoint;
    currentTime*=timeScale;
    //currentTime=startTime+1;
    //currentTime++;
    //move forward if we are at end of key

    while (currentTime> keyFrames[currentKeyMatrix+1]->timeKey ){
        currentKeyMatrix++;
        if ((uint)currentKeyMatrix+1 >= keyFrames.size()){
            if (bLooping){
                currentKeyMatrix=0;
                startTime=renderer->currentTime;
                currentTime=0.0;
                //cout << "looped keyframes size: " << keyFrames.size() << endl;
                //return;
            }else if (bMerge){
                if (mergeDirection > 0.0){
                    mergeDirection=-1.0 * mergeDirection;
                    mergeFactor=1.0f;
                }
                currentKeyMatrix=keyFrames.size()-2;
                fadeOut(keyFrames.size()-1);
                return;
            }else{
                bFinishedMatrix=true;
                return;
            }
        }
    }

    //cout << "key: " << currentKeyMatrix << " max Keys: " << (*timeKeys).size() << endl;
    //if we are at the end of all keys, we're finished!

    //get the two closest interpolation points
    timeKeyOne=keyFrames[currentKeyMatrix]->timeKey;
    timeKeyTwo=keyFrames[currentKeyMatrix+1]->timeKey;

        //get the time difference
        double timeDifference=timeKeyTwo-timeKeyOne;

        //map time difference to 0.0 - 1.0
        double keyTime=currentTime-timeKeyOne; //-> current Position, in the beginning 0.0;
        float relativeTime=(float) (keyTime/timeDifference); //-> will go from 0.0 to 1.0 between the keys

    SkeletalActor* skel=(SkeletalActor*)moveActor;

    for (uint i=0;i<skel->bones.size();i++){

        matrixOne=keyFrames[currentKeyMatrix]->boneMatrices[skel->bones[i]->name];
        matrixTwo=keyFrames[currentKeyMatrix+1]->boneMatrices[skel->bones[i]->name];

        //ease in - ease out
        float x=relativeTime*2.0f;
        float y=0.0f;

        if (x< 1)
            y=x*x;
        else
            y=2-( (x-2) * (x-2) );

        y=y* 0.5;

        if (bLinear)
            y=relativeTime;
        //interpolate between them
        //resultingMatrix=matrixOne.lerp(relativeTime,matrixTwo); //calculate resulting position


        rotationOne=getRotationMatrix(matrixOne);
        rotationTwo=getRotationMatrix(matrixTwo);
        resultingRotation=rotationOne.lerp(y,rotationTwo);


        resultingMatrix=matrixOne.lerp(y,matrixTwo); //calculate resulting position

        if (bMerge && mergeFactor < 1.0 && mergeFactor > 0.0){
            Matrix3f sourceMatrix=getRotationMatrix(skel->bones[i]->transformMatrix);
            resultingRotation = sourceMatrix.lerp(mergeFactor,resultingRotation);
        }

        //normalize rotations!
        resultingRotation=normalizeRotations(resultingRotation);

        //apply rotation
        skel->bones[i]->transformMatrix.setRotation(resultingRotation);
    }

}


void InterpolationHelper::reset(){

    currentKey=0;
    currentKeyVectors=0;
    currentKeyMatrix=0;

    bFinished=false;
    bFinishedMatrix=false;
    bFinishedVectors=false;
}

Matrix3f InterpolationHelper::getRotationMatrix(Matrix4f source){

    Matrix3f dest;

    dest.data[0]=source.data[0];
    dest.data[1]=source.data[1];
    dest.data[2]=source.data[2];

    dest.data[3]=source.data[4];
    dest.data[4]=source.data[5];
    dest.data[5]=source.data[6];

    dest.data[6]=source.data[8];
    dest.data[7]=source.data[9];
    dest.data[8]=source.data[10];

    return dest;
}

Matrix3f InterpolationHelper::normalizeRotations(Matrix3f source){

    Vector3f xAxis,yAxis,zAxis;

    xAxis=Vector3f(source[0],source[1],source[2]);
    yAxis=Vector3f(source[3],source[4],source[5]);
    zAxis=Vector3f(source[6],source[7],source[8]);

    xAxis.normalize();
    yAxis.normalize();
    zAxis.normalize();

    source[0]=xAxis.x;
    source[1]=xAxis.y;
    source[2]=xAxis.z;

    source[3]=yAxis.x;
    source[4]=yAxis.y;
    source[5]=yAxis.z;

    source[6]=zAxis.x;
    source[7]=zAxis.y;
    source[8]=zAxis.z;

    return source;

}

void InterpolationHelper::fadeOut(int key){

    if (bMerge){
        if (mergeFactor<=1.0)
            mergeFactor+=1.0 * renderer->deltaTime * 0.001 * mergeDirection;
        if (mergeFactor<=0.0 ){
            mergeFactor=1.0;
            bFinished=true;
            return;
        }

    //cout << "mergeFactor " << mergeFactor << endl;
    }

    SkeletalActor* skel=(SkeletalActor*)moveActor;

    for (uint i=0;i<skel->bones.size();i++){

        Matrix4f matrixOne=keyFrames[key]->boneMatrices[skel->bones[i]->name];
        Matrix3f resultingRotation=getRotationMatrix(matrixOne);

        Matrix3f sourceMatrix=getRotationMatrix(skel->bones[i]->transformMatrix);
        resultingRotation = sourceMatrix.lerp(mergeFactor,resultingRotation);

        resultingRotation =normalizeRotations(resultingRotation );

        skel->bones[i]->transformMatrix.setRotation(resultingRotation);

    }
}

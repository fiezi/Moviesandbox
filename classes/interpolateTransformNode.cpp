

#include "interpolateTransformNode.h"
#include "control.h"
#include "input.h"

InterpolateTransformNode::InterpolateTransformNode(){

name="Interpolate";

moveActor=NULL;
moveTime=1.0;

scl=Vector3f(0,0,0);
rot=Vector3f(0,0,0);
loc=Vector3f(0,0,0);

bNeedMover=true;
bTurnOnFly=false;

listType.push_back("15PickWorldButton");
listName.push_back("moveActor");
listProp.push_back("MOVEACTOR");
listIcon.push_back("icon_flat");
/*
listType.push_back("15PickWorldButton");
listName.push_back("targetActor");
listProp.push_back("TARGETACTOR");
listIcon.push_back("icon_flat");
*/

listType.push_back("15TextInputButton");
listName.push_back("moveTime");
listProp.push_back("MOVETIME");
listIcon.push_back("icon_flat");

listType.push_back("15TextInputButton");
listName.push_back("loc");
listProp.push_back("LOC");
listIcon.push_back("icon_flat");

listType.push_back("15TextInputButton");
listName.push_back("scl");
listProp.push_back("SCL");
listIcon.push_back("icon_flat");

listType.push_back("15TextInputButton");
listName.push_back("rot");
listProp.push_back("ROT");
listIcon.push_back("icon_flat");


listType.push_back("10BoolButton");
listName.push_back("turnOnFly");
listProp.push_back("BTURNONFLY");
listIcon.push_back("icon_flat");

color=Vector4f(0.5,0.5,0.5,1.0);
registerProperties();
}

InterpolateTransformNode::~InterpolateTransformNode(){}

void InterpolateTransformNode::registerProperties(){

Node::registerProperties();
createMemberID("MOVETIME",&moveTime,this);
createMemberID("MOVEACTOR",&moveActor,this);
//createMemberID("TARGETACTOR",&targetActor,this);
createMemberID("LOC",&loc,this);
createMemberID("ROT",&rot,this);
createMemberID("SCL",&scl,this);
createMemberID("BTURNONFLY",&bTurnOnFly,this);
}

void InterpolateTransformNode::start(){


bNeedMover=true;
//create interpolationHelper
//moveActor->movers.push_back(new InterpolationHelper);
//moveActor->mover->targetActor=targetActor;
//cout << "executing interpolation: " << moveActor->name << "with target: " << moveActor->mover->targetActor->name << endl;
}

void InterpolateTransformNode::stop(){

}

void InterpolateTransformNode::execute(){


        cout << "executing Interpolation Transform..." << endl;

    //if (bNeedMover){
        InterpolationHelper* lerp= new InterpolationHelper;
        key* startKey= new key;
        key* targetKey= new key;

        Matrix4f startMatrix=moveActor->transformMatrix;

        startKey->transformKey=startMatrix;
        startKey->timeKey=0.0;
        lerp->keyFrames.push_back(startKey);

        Matrix4f targetMatrix;

        //set Location
        targetMatrix.setTranslation(loc);

        //set translation
        Vector3f relTranslation=targetMatrix.getTranslation();
        targetMatrix=Matrix4f::createRotationAroundAxis(rot.x*M_PI/180,rot.y*M_PI/180,rot.z*M_PI/180);
        targetMatrix.setTranslation(relTranslation);

        //set scale
        targetMatrix[0]=scl.x;
        targetMatrix[5]=scl.y;
        targetMatrix[10]=scl.z;

        targetKey->transformKey=targetMatrix;
        targetKey->timeKey=moveTime * 1000.0;

        lerp->keyFrames.push_back(targetKey);
        lerp->bInterpolateTransform=true;
        lerp->bAdditive=true;
        lerp->bInterpolateActor=false;
        lerp->moveActor=moveActor;
        //lerp->moveTime=moveTime * 1000.0;
        lerp->startTime=renderer->currentTime;
        lerp->baseTransform=moveActor->transformMatrix;
        lerp->bLooping=false;
        lerp->bLinear=false;
        moveActor->movers.push_back(lerp);
        bNeedMover=false;

        if (bTurnOnFly)
            sceneData->controller->myTools[TOOL_NAV]->myBtn->clickedLeft();

    //}else{

        nextNode();
    //}
    //TODO: if (bWaitForMoveActor && moveActor->mover)

}


void InterpolateTransformNode::update(double deltaTime){

Node::update(deltaTime);
}

void InterpolateTransformNode::trigger(MsbObject* other){

Node::trigger(other);
}

void InterpolateTransformNode::create(){sceneData->addNode(this);}

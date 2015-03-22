
#include "performActionNode.h"
#include "control.h"
#include "renderer.h"
#include "input.h"
#include "skeletalActor.h"

PerformActionNode::PerformActionNode(){

    applyTo=NULL;
    myAction="NULL";
    bWaitToBeFinished=false;
    bPlaying=false;
    bRelative=false;
    name="Perform";

    listType.push_back("15PickWorldButton");
    listName.push_back("APPLYTO");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

    listType.push_back("10BoolButton");
    listName.push_back("WaitToFinish");
    listProp.push_back("BWAITTOBEFINISHED");
    listIcon.push_back("icon_flat");

    listType.push_back("10BoolButton");
    listName.push_back("Relative");
    listProp.push_back("BRELATIVE");
    listIcon.push_back("icon_flat");

    listType.push_back("15TextInputButton");
    listName.push_back("Action Name");
    listProp.push_back("MYACTION");
    listIcon.push_back("icon_flat");


    color=Vector4f(0.5,0.5,0.5,1.0);
    registerProperties();

}

PerformActionNode::~PerformActionNode(){}

void PerformActionNode::registerProperties(){

    Actor::registerProperties();
    createMemberID("APPLYTO",&applyTo,this);
    createMemberID("MYACTION",&myAction,this);
    createMemberID("BWAITTOBEFINISHED",&bWaitToBeFinished, this);
    createMemberID("BRELATIVE",&bRelative, this);
}

void PerformActionNode::start(){

    Node::start();
}

void PerformActionNode::stop(){

    Node::stop();
}

void PerformActionNode::execute(){



    if (!bPlaying){
        if (sceneData->actionList[myAction] && applyTo){
            InterpolationHelper * interpolator=new InterpolationHelper;

            if (dynamic_cast<SkeletalActor*>(applyTo))
                interpolator->bInterpolateMatrix=true;
            interpolator->bInterpolateTransform=true;

            interpolator->startTime=renderer->currentTime;
            interpolator->keyFrames=sceneData->actionList[myAction]->keyFrames;
            interpolator->bRelative=bRelative;

             //apply current Location/rotation if we want our action to be performed relative
            if (bRelative){
                //starts new ation from the position/rotation the actor is currently in
                interpolator->baseTransform=applyTo->transformMatrix * (sceneData->actionList[myAction]->keyFrames[0]->transformKey).inverse();
                interpolator->baseMatrix=applyTo->transformMatrix;

                //apply current bone matrices if we want our action to be performed relative
                if (dynamic_cast<SkeletalActor*>(applyTo)){
                    SkeletalActor* skeleton=(SkeletalActor*)applyTo;
                    for (uint i=0;i<skeleton->bones.size();i++){
                        //starts new ation from the position/rotation the bone is currently in
                        interpolator->baseBoneMatrices[skeleton->bones[i]->name]=skeleton->bones[i]->transformMatrix - sceneData->actionList[myAction]->keyFrames[0]->boneMatrices[skeleton->bones[i]->name];
                        }
                    }
                }

            interpolator->currentKey=0;
            interpolator->bFinished=false;
            interpolator->moveActor=applyTo;
            applyTo->movers.push_back(interpolator);
            bPlaying=true;
            }
        }
    else{
        if (bWaitToBeFinished){
            cout << "should be waiting..." << endl;
            if (applyTo->movers[0]->bFinished){
                applyTo->transformMatrix=applyTo->movers[0]->baseMatrix;
                bPlaying=false;
                nextNode();
            }
            return;
        }
        bPlaying=false;
        nextNode();
        }

}


void PerformActionNode::update(double deltaTime){

    Node::update(deltaTime);
}

void PerformActionNode::trigger(MsbObject* other){

    cout << "trigger!" << other->name << endl;

    if (other==listButton[0]){
        applyTo=input->worldTarget;
    }
}

void PerformActionNode::create(){sceneData->addNode(this);}

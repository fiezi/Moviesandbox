

#include "interpolateNode.h"
#include "control.h"
#include "input.h"

InterpolateNode::InterpolateNode(){

name="Interpolate";

moveActor=NULL;
targetActor=NULL;
moveTime=1.0;

bNeedMover=true;
bTurnOnFly=false;

listType.push_back("15PickWorldButton");
listName.push_back("moveActor");
listProp.push_back("MOVEACTOR");
listIcon.push_back("icon_flat");

listType.push_back("15PickWorldButton");
listName.push_back("targetActor");
listProp.push_back("TARGETACTOR");
listIcon.push_back("icon_flat");

listType.push_back("15TextInputButton");
listName.push_back("moveTime");
listProp.push_back("MOVETIME");
listIcon.push_back("icon_flat");


listType.push_back("10BoolButton");
listName.push_back("turnOnFly");
listProp.push_back("BTURNONFLY");
listIcon.push_back("icon_flat");

color=Vector4f(0.5,0.5,0.5,1.0);
registerProperties();
}

InterpolateNode::~InterpolateNode(){}

void InterpolateNode::registerProperties(){

Node::registerProperties();
createMemberID("MOVETIME",&moveTime,this);
createMemberID("MOVEACTOR",&moveActor,this);
createMemberID("TARGETACTOR",&targetActor,this);
createMemberID("BTURNONFLY",&bTurnOnFly,this);
}

void InterpolateNode::start(){


bNeedMover=true;
//create interpolationHelper
//moveActor->movers.push_back(new InterpolationHelper);
//moveActor->mover->targetActor=targetActor;
//cout << "executing interpolation: " << moveActor->name << "with target: " << moveActor->mover->targetActor->name << endl;
}

void InterpolateNode::stop(){

}

void InterpolateNode::execute(){


        cout << "executing Interpolation..." << endl;

    //if (bNeedMover){
        InterpolationHelper* lerp= new InterpolationHelper;
        lerp->bInterpolateActor=true;
        lerp->moveActor=moveActor;
        lerp->targetActor=targetActor;
        lerp->moveTime=moveTime * 1000.0;
        lerp->startTime=renderer->currentTime;
        lerp->baseTransform=moveActor->transformMatrix;
        lerp->bLooping=true;
        lerp->bLinear=true;
        moveActor->movers.push_back(lerp);
        bNeedMover=false;

        if (bTurnOnFly)
            sceneData->controller->myTools[TOOL_NAV]->myBtn->clickedLeft();

    //}else{

        nextNode();
    //}
    //TODO: if (bWaitForMoveActor && moveActor->mover)

}


void InterpolateNode::update(double deltaTime){

Node::update(deltaTime);
}

void InterpolateNode::trigger(MsbObject* other){

Node::trigger(other);
}

void InterpolateNode::create(){sceneData->addNode(this);}

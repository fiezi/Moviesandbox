#include "timelineButton.h"
#include "renderer.h"
#include "input.h"
#include "interpolationHelper.h"
#include "timelineInspector.h"

bool sort_time_keys( key * a, key * b) {

	float timea = a->timeKey;
	float timeb = b->timeKey;

	return (timea < timeb);
}

TimelineButton::TimelineButton(){

    sceneShaderID="color";
    selectedKey=NULL;
    bSkeletalTrack=false;
    bPlaying=false;
    skeleton=NULL;

    timePos = 0.0;
    timeSize = 10.0f;

    zoomTime=1.0f;
    secondMark=100.0f;
    snapToMark=5.0f;

    scrubMark= 0.0;

    connectedAction=NULL;
    startPressedLeft=0;
    bDragable=true;

    interpolator.bAdditive=false;
}

TimelineButton::~TimelineButton(){}

void TimelineButton::setup(){
BasicButton::setup();
if (bSkeletalTrack)
    skeleton=dynamic_cast<SkeletalActor*>(connectedActor);
}

void TimelineButton::update(double deltaTime){

    if (selectedKey){
        if (bSkeletalTrack){
            for (uint i=0;i<skeleton->bones.size();i++){
                selectedKey->boneMatrices[skeleton->bones[i]->name]=skeleton->bones[i]->transformMatrix;
                }
		}

		selectedKey->locationKey=connectedActor->location;
		selectedKey->rotationKey=connectedActor->rotation;

        updateAction();
		deleteKey();
	}

    if (!interpolator.bFinished){
        if (bPlaying)
            interpolator.interpolate();
        }
    else{
        //don't destroy interpolator!
        interpolator.bFinished=false;
        bPlaying=false;
        }



}

void TimelineButton::drawPlane(){

    BasicButton::drawPlane();

    drawKeys();

}

void TimelineButton::drawKeys(){

    renderer->setupShading("color");


    float timeLineWidth= timeSize * secondMark;

    //now zoom in ( timeline gets wider the more we're zoomed in
    timeLineWidth = timeLineWidth / zoomTime;

    float startPos = timePos * timeLineWidth;

    glTranslatef(-startPos, 0, 0);

    glPushMatrix();
    for (uint i=0;i<keyFrames.size();i++){

        float keyXpos=keyFrames[i]->timeKey /10.0;
        keyXpos /=zoomTime;

        Vector4f keyColor;
        if (keyFrames[i]==selectedKey)
            keyColor=Vector4f(1.0,0.0,0.0,1.0);
        else
            keyColor=Vector4f(1.0,1.0,1.0,1.0);

        //cout << "drawing keystuff!" << endl;
        float keySize = 10;//1/zoomTime;
        float keyYpos = .5 * (scale.y - keySize);

        if (keyXpos >= startPos )
            renderer->drawPlane(keyXpos,keyYpos, keyXpos+keySize,keySize+keyYpos, keyColor);
        }

    //draw scrubber
    glColor4f(0.0,0.0,0.0,1.0);
    glLineWidth(1.0);
    glBegin( GL_LINES );
        if (scrubMark >= startPos){
            glVertex3f(scrubMark, 0, 0);
            glVertex3f(scrubMark, scale.y, 0);
        }
    glEnd();

    glPopMatrix();
}



void TimelineButton::mouseOver(){

BasicButton::mouseOver();
}

void TimelineButton::mouseDrag(){

   float timeLineWidth= timeSize * secondMark;

    //now zoom in ( timeline gets wider the more we're zoomed in
    timeLineWidth = timeLineWidth / zoomTime;

    float startPos = timePos * timeLineWidth;

    if(input->pressedLeft && startPressedLeft==0){

        float mouseX=input->mouseX;

        for (uint i=0;i<keyFrames.size();i++){
            float myTime = keyFrames[i]->timeKey/10.0;
            float keyX= location.x + myTime/zoomTime - startPos;
            if (mouseX > keyX && mouseX< keyX+10)
                draggedKey=keyFrames[i];
        }
    }

    if(input->pressedLeft && draggedKey){
        if (!isOccupied() ){
            draggedKey->timeKey  = (input->mouseX-location.x)*zoomTime  + startPos;
            draggedKey->timeKey  *= 10.0;
        }

        startPressedLeft = input->startPressLeftBtn;
    }


}

void TimelineButton::clickedLeft(){

    cout << "selecting closest keyFrame..." << endl;


    float timeLineWidth= timeSize * secondMark;

    //now zoom in ( timeline gets wider the more we're zoomed in
    timeLineWidth = timeLineWidth / zoomTime;

    float startPos = timePos * timeLineWidth;


    float mouseX=input->mouseX;

    key* clickedOn=NULL;
    for (uint i=0;i<keyFrames.size();i++){
        float myKey = keyFrames[i]->timeKey/10.0;
        float keyX= location.x + myKey/zoomTime  - startPos;
        if (mouseX > keyX && mouseX < keyX+10)
                clickedOn=keyFrames[i];
    }

    //deactivate key

    if (clickedOn){
        if (clickedOn==selectedKey){
            selectedKey=NULL;
		}
        else{
             selectedKey=clickedOn;
             if (bSkeletalTrack){
                for (uint i=0;i<skeleton->bones.size();i++){
                    skeleton->bones[i]->transformMatrix=selectedKey->boneMatrices[skeleton->bones[i]->name];
				}
			}
			connectedActor->setLocation(selectedKey->locationKey);
			connectedActor->setRotation(selectedKey->rotationKey);

		}
	}
    else
        selectedKey=NULL;


    // for dragging
    if(startPressedLeft>0){
        sort(keyFrames.begin(),keyFrames.end(),sort_time_keys);
    }
    startPressedLeft = 0;
    draggedKey = NULL;
}

void TimelineButton::clickedRight(){

    cout << "creating new keyframe!" << endl;


    float timeLineWidth= timeSize * secondMark;

    //now zoom in ( timeline gets wider the more we're zoomed in
    timeLineWidth = timeLineWidth / zoomTime;

    float startPos = timePos * timeLineWidth; // in Pixels

    //check if we already have a key in this location
    bool bOccupied=isOccupied();


    if (!bOccupied){


        key* myKey=new key;

        myKey->timeKey  = (input->mouseX-location.x)*zoomTime + startPos; //time in seconds
        myKey->timeKey  *= 10.0;

        if( keyFrames.size() == 0 )
            keyFrames.push_back(myKey);
        else{

           int myPos = keyFrames.size();
           for( int i = 0; i < (int)keyFrames.size(); i++)
                if( myKey->timeKey <  keyFrames[i]->timeKey)
                {
                    myPos = i;
                    break;
                }

           keyFrames.insert( keyFrames.begin() + myPos , myKey );

        }

        if (bSkeletalTrack){
            for (uint i=0;i<skeleton->bones.size();i++){
                myKey->boneMatrices[skeleton->bones[i]->name]=skeleton->bones[i]->transformMatrix;
                cout << "TimelineButton: boneMatrix: " << myKey->boneMatrices[skeleton->bones[i]->name] << endl;
			}
		}

		myKey->locationKey=connectedActor->location;
        myKey->rotationKey=connectedActor->rotation;

        selectedKey=myKey;
        cout << "TimelineButton: timekey: " << keyFrames.back()->timeKey << endl;

        updateAction();

    }


}

bool TimelineButton::isOccupied(){

    float mouseX=input->mouseX;

    bool bOccupied=false;

    for (uint i=0;i<keyFrames.size();i++){
        float keyX= location.x + keyFrames[i]->timeKey * zoomTime;
        if (mouseX > keyX && mouseX< keyX+10)
                bOccupied=true;
        }

    return bOccupied;
}

void TimelineButton::playTimeline(){


        //load first keyFrame
        bPlaying=true;
        selectedKey=NULL;
        interpolator.reset();
        interpolator.moveActor=connectedActor;
		if (bSkeletalTrack){
            interpolator.bInterpolateMatrix=true;
		}

		interpolator.bInterpolateVectors=true;

		interpolator.startTime=renderer->currentTime;
        interpolator.keyFrames=keyFrames;
        interpolator.currentKey=0;

}

void TimelineButton::scrub(float scrubber){


        float timeLineWidth= timeSize * secondMark;

        //now zoom in ( timeline gets wider the more we're zoomed in
        timeLineWidth = timeLineWidth / zoomTime;

        float startPos = timePos * timeLineWidth;

        float scrubPos = startPos + scrubber * zoomTime;

        scrubPos*=10.0;

        selectedKey=NULL;
        //scrubMark*=10.0;

        //no interpolating to keys we don't have
        if (keyFrames.size()<1 || keyFrames.back()->timeKey<=scrubPos)
            return;
        interpolator.reset();
        interpolator.moveActor=connectedActor;
		if (bSkeletalTrack){
            interpolator.bInterpolateMatrix=true;
		}
		interpolator.bInterpolateVectors=true;
        interpolator.startTime=renderer->currentTime-scrubPos;
        interpolator.keyFrames=keyFrames;

        //interpolator.interpolate();
        cout << "interpolating scrub " << scrubPos << endl;
        interpolator.interpolate();
        connectedActor->update(0.0);
}

void TimelineButton::pauseTimeline(){

    bPlaying=!bPlaying;
}

void TimelineButton::focusClick(){

BasicButton::focusClick();
}

void TimelineButton::deselect(int depth){

cout << "timelineDeselect!" << endl;

BasicButton::deselect(depth);
}

void TimelineButton::create(){renderer->addButton(this);}

float TimelineButton::convertToTimeGrid( float val )
{
    return (snapToMark)*(int)( val / (snapToMark) );
}

void TimelineButton::connectAction(Action* newAction)
{

    //here, we actually need to copy the contents of our keys, not just the references!
    for (uint i=0;i<keyFrames.size();i++)
    {
        newAction->keyFrames.push_back(new key);
        *(newAction->keyFrames[i])=*(keyFrames[i]);
    }

    newAction->bSkeletal=bSkeletalTrack;
    newAction->color = Vector4f(1,0,0,1);

    //add connected Action to my.library
    TiXmlElement* myElement = new TiXmlElement("Action");
	myElement->SetAttribute("actionFilename",newAction->name+".action");
	input->addToLibrary(myElement);

    connectedAction = newAction;

}

void TimelineButton::disconnectAction()
{
    if(connectedAction) connectedAction->color = Vector4f(1.0,1.0,0.0,1);
    connectedAction=NULL;
}

void TimelineButton::updateAction()
{
    if( connectedAction )
    {

        for (uint i=0;i<connectedAction->keyFrames.size();i++)
            delete connectedAction->keyFrames[i];

        connectedAction->keyFrames.clear();

        for (uint i=0;i<keyFrames.size();i++)
        {
        connectedAction->keyFrames.push_back(new key);
        *(connectedAction->keyFrames[i])=*(keyFrames[i]);
        }

        //cout << "connAction keyFrames: " << connectedAction->keyFrames.size() << endl;

    }

}


void TimelineButton::deleteKey()
{
    if( input->lastKey == 127 || input->lastKey == 8  )
    {
        for( uint i = 0; i < keyFrames.size(); i++ )
        {
            if( keyFrames[i]->timeKey == selectedKey->timeKey )
            {
                keyFrames.erase( keyFrames.begin() + i );
                updateAction();
                break;
            }
        }
    }

}


void TimelineButton::populateFromAction(Action* other)
{
    for (uint i=0;i<keyFrames.size();i++)
            delete keyFrames[i];

    keyFrames.clear();

    for (uint i=0;i<other->keyFrames.size();i++)
        {
        keyFrames.push_back(new key);
        *(keyFrames[i])=*(other->keyFrames[i]);
        }

    connectAction(other);

    // connect buttons in parent
    for( uint i = 0; i < ((TimelineInspector*)parent)->listButton.size(); i++)
    {
        if( ((TimelineInspector*)parent)->listButton[i]->name=="make" )
        {
            ((TimelineInspector*)parent)->listButton[i]->name="connect";
            ((TimelineInspector*)parent)->listButton[i]->color = Vector4f(1,0,0,1);
        }
   }

}

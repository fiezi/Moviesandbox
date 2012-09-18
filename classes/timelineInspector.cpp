
#include "timelineInspector.h"
#include "assignButton.h"
#include "renderer.h"
#include "input.h"
#include "sliderButton.h"
#include "skeletalActor.h"

//this inspector does not remove its buttons when closed!
TimelineInspector::TimelineInspector(){

    bOpen=false;
    bToggleInspector=false;
    initialLocation=location;
    tabWidth= 0.0;
    tabHeight=(renderer->screenY * 0.3f);
    level=0;

    timeSize = 10.0f;               //10 seconds in timeline!
    timePos = 0.0f;
    zoomTime=1.0f;

    secondMark=100.0f;
    snapToMark=5.0f;

    listHeight=15;
    listWidth=40;
    listDisplayMode=4;
    listColumns=3;

    listOffsetY=70;

    bDrawName=false;

    bRecording=false;
    startRecordingTime=0;

}

TimelineInspector::~TimelineInspector(){}


void TimelineInspector::setup(){

    Inspector::setup();

    buttonColor=sceneData->timelineButtonColor;
    backgroundButton->buttonColor=sceneData->menuBackgroundColor;
}

void TimelineInspector::update(double deltaTime){

    BasicButton::update(deltaTime);

    if (input->lastKey==127 || input->lastKey==8){
        for (int i=0;i<(int)listButton.size();i++){
            if (i%3==0 && input->hudTarget==listButton[i])
                removeTimeline(i/3);
        }
    }

    if (bRecording){
        for (int i=0;i<(int)timelines.size();i++){
            int timeStep=(renderer->currentTime - startRecordingTime);
            //TODO: make frame recording resolution a setting!
            if (timelines[i]->keyFrames.size() * 50.0< timeStep){
                timelines[i]->createKey(  timeStep/10.0  );
            }
        }
    }else{
        startRecordingTime=renderer->currentTime;
    }

    //timeSize= 6.0f * zoomTime;//+ 6.0f * zoomTime* timePos ;
}


void TimelineInspector::createInspectorButtons(){

    ///ADD
    //is picking, so do it differently
    sceneData->actorInfo["15PickWorldButton"].actorReference->create();
    inspectorButtons.push_back(sceneData->buttonList.back());

    /// Zoom, focus
    //6 assign buttons
    for( int i = 1; i < 7; i++){
        AssignButton * myButton = new AssignButton;
        inspectorButtons.push_back(myButton);
        sceneData->buttonList.push_back(myButton);
    }

    //setup all buttons
    for (int i=0; i < 7; i++){
        inspectorButtons[i]->name="";
        inspectorButtons[i]->bDrawName=true;
        inspectorButtons[i]->parent=this;
        inspectorButtons[i]->level=level+1;
        inspectorButtons[i]->tooltip="";
        inspectorButtons[i]->bPermanent=true;
        inspectorButtons[i]->sceneShaderID="buttonColor";
        inspectorButtons[i]->scale=Vector3f(40,15,1);
        inspectorButtons[i]->setup();
        inspectorButtons[i]->buttonColor=Vector4f(0.3,0.6,0.3,1.0);
    }

    //set names and colors
    Vector3f loc = location;

    loc.y+=scale.y;
    inspectorButtons[0]->name="Add";
    inspectorButtons[0]->buttonColor=Vector4f(0.5,0.4,0.4,1.0);
    inspectorButtons[0]->setLocation( loc );

    loc.x+=listWidth + 2;
    inspectorButtons[1]->name="play";
    inspectorButtons[1]->buttonColor=Vector4f(0.3,0.6,0.3,1.0);
    inspectorButtons[1]->setLocation( loc );

    loc.x+=listWidth + 2;
    inspectorButtons[2]->name="pause";
    inspectorButtons[2]->buttonColor=Vector4f(0.3,0.6,0.3,1.0);
    inspectorButtons[2]->setLocation( loc );

    loc.x+=listWidth + 2;
    inspectorButtons[3]->name="zmIn";
    inspectorButtons[3]->buttonColor=Vector4f(0.5,0.6,0.3,1.0);
    inspectorButtons[3]->setLocation( loc );

    loc.x+=listWidth + 2;
    inspectorButtons[4]->name="zmOut";
    inspectorButtons[4]->buttonColor=Vector4f(0.3,0.6,0.5,1.0);
    inspectorButtons[4]->setLocation( loc );

    ///Record
    loc.x+=listWidth + 2;
    inspectorButtons[5]->name="rec";
    inspectorButtons[5]->buttonColor=Vector4f(0.5,0.5,0.5,1.0);
    inspectorButtons[5]->setLocation( loc );

    ///scrubber
    inspectorButtons[6]->name="scrubber";
    inspectorButtons[6]->bDragable=true;
    inspectorButtons[6]->bResetAfterDrag=false;
    inspectorButtons[6]->bTriggerWhileDragging=true;

    inspectorButtons[6]->bDrawName=false;
    inspectorButtons[6]->bConfineDragX=true;

    inspectorButtons[6]->sceneShaderID="buttonColor";
    inspectorButtons[6]->scale=Vector3f(5,400,0);

    loc.x=location.x + (2* listWidth +2) + 96;
    loc.y=location.y + 55;
    inspectorButtons[6]->buttonColor=Vector4f(0.0,0.0,0.0,1.0);
    inspectorButtons[6]->mouseOverColor=Vector4f(0.1,0.1,0.1,1.0);
    inspectorButtons[6]->setLocation( loc );

    ///scrolling timelines
    SliderButton* mySlider = new SliderButton;
    mySlider->parent=this;
    mySlider->bVertical=false;
    mySlider->scale.x=renderer->windowX-location.x - (2*listWidth+2);
    mySlider->scale.y=8;
    mySlider->sceneShaderID="buttonColor";
    mySlider->setLocation(Vector3f(location.x + (2*listWidth+2) +96, location.y + 47, 0) );
    mySlider->name="timePos";
    mySlider->bDrawName=false;
    mySlider->buttonProperty="timePos";
    mySlider->setup();
    mySlider->buttonColor=sceneData->scrollBarColor;
    mySlider->mouseOverColor=sceneData->scrollBarColor;
    mySlider->slidePointColor=sceneData->scrollBarIndicatorColor;
    inspectorButtons.push_back(mySlider);
    sceneData->buttonList.push_back(mySlider);

}

void TimelineInspector::addTimeline(int pos, bool bSkeletal){


    cout << "TimlineInspector: add Actor: " << input->worldTarget->name << endl;

    /** NAME **/
    //Button for displaying the actor's name
    BasicButton*  nameButton = new AssignButton;
    sceneData->buttonList.push_back(nameButton);
    listButton.push_back(nameButton);

    nameButton->level=level+1;
    nameButton->bDrawName=true;
    nameButton->bPermanent=true;
    nameButton->parent=this;

    nameButton->buttonProperty=memberToString(&input->property["WORLDTARGET"]);
    nameButton->sceneShaderID="buttonColor";
    nameButton->setup();
    nameButton->name=timelineActors[pos]->name;

    if (bSkeletal){
        nameButton->buttonColor=Vector4f(0.3,0.4,0.3,1.0);
        }
    else{
        nameButton->buttonColor=Vector4f(0.3,0.4,0.3,1.0);
        }


    /** MAKE ACTION **/

    BasicButton*  makeAction = new AssignButton;
    sceneData->buttonList.push_back(makeAction);
    listButton.push_back(makeAction);

    makeAction->name="make";

    makeAction->level=level+1;
    makeAction->bDrawName=true;
    makeAction->bPermanent=true;
    makeAction->sceneShaderID="buttonColor";
    makeAction->parent=this;
    makeAction->setup();
    makeAction->buttonColor=Vector4f(0.3,0.3,0.5,1.0);


    /** TIMELINE **/

    TimelineButton* tlBtn= new TimelineButton;
    listButton.push_back(tlBtn);
    sceneData->buttonList.push_back(tlBtn);
    timelines.push_back(tlBtn);

    tlBtn->tooltip="Timeline for" + timelineActors[pos]->name;
    tlBtn->parent=this;
    tlBtn->connectedActor=timelineActors[pos];
    tlBtn->level=level+1;
    tlBtn->bPermanent=true;

    if (bSkeletal){
        tlBtn->bSkeletalTrack=true;
        }
    tlBtn->setup();
    tlBtn->buttonColor=sceneData->timelineColor;

    ///add initial keyframe
    tlBtn->createKey(0.0);

    ///scaling
    nameButton->scale=Vector3f( listWidth+96, listHeight, 0);
    makeAction->scale=Vector3f( listWidth, listHeight, 0);
    tlBtn->scale=Vector3f( renderer->windowX- location.x, listHeight, 0);

    ///locations
    Vector3f loc;

    loc=location + Vector3f( 0, listOffsetY+2, 0) + Vector3f( 0, pos * (listHeight+10), 0);
    nameButton->setLocation(loc);

    //give more space to names
    loc.x+=listWidth + 96 + 2;
    makeAction->setLocation(loc);

    loc.x+=listWidth + 2;
    tlBtn->setLocation(loc);

}

void TimelineInspector::trigger(MsbObject* other){

    //is a callback from the buttons from our list!


//    cout << "triggered timelineInspector!" << endl;

    if (other->name=="timePos"){
        timePos=((SliderButton*)other)->sliderValue;
        cout << "changing timePos to: " << timePos << endl;
        for(uint i=0;i<listButton.size();i++){
            if (listButton[i]->isA("TimelineButton")){
                ((TimelineButton*)listButton[i])->timePos=timePos;
            }
        }
    }

    if (other->name=="scrubber"){

        float myTime;
        //calculate time from scrubber position
        myTime=((Actor*)other)->location.x - 2* (listWidth +2) - (location.x + 96);
        if (myTime<0.0){
            ((Actor*)other)->setLocation( Vector3f( location.x + 2* (listWidth +2)+ 96, ((Actor*)other)->location.y, 0.0) );
            return;
        }

        for(uint i=0;i<listButton.size();i++){
            if (listButton[i]->isA("TimelineButton")){
                ((TimelineButton*)listButton[i])->scrub(myTime);
            }
        }

    }

    //manually create new timeline...
    if (other->name=="Add"){

        // check if we have that actor already!! or if we selected the ground
        bool bOkayToAdd = true;

        if( input->worldTarget->name == "ground")
            bOkayToAdd = false;

        for( uint i=0; i < timelineActors.size(); i++)
        {
            if( timelineActors[i] == input->worldTarget )
                bOkayToAdd = false;
        }

        if( bOkayToAdd )
        {
            int pos=0;
            if (timelineActors.size()>0)
                pos=timelineActors.size();

            //if it's a regular actor...
            SkeletalActor* skel=dynamic_cast<SkeletalActor*>(input->worldTarget);
            if (skel){
                if (input->bShiftDown){
                    timelineActors.push_back(input->worldTarget);
                    addTimeline(pos, false);
                    }
                else{
                    timelineActors.push_back(input->worldTarget);
                    addTimeline(pos, true);
                    }
                }
            else{
                timelineActors.push_back(input->worldTarget);
                addTimeline(pos, false);
                }
        }
    }

    //PLAY!
    if (other->name=="play"){
      playTimelines();
    }

    //PAUSE!
    if (other->name=="pause")
      for(uint i=0;i<listButton.size();i++){
        if (listButton[i]->isA("TimelineButton")){
            ((TimelineButton*)listButton[i])->pauseTimeline();
        }
      }

    if (other->name=="rec"){
        bRecording=!bRecording;
        if (bRecording)
            other->color=Vector4f(1.0,0.2,0.2,1.0);
        else
            other->color=Vector4f(0.5,0.5,0.5,1.0);
    }

    //MAKE NEW ACTION!
    if (other->name=="make"){
        TimelineButton* tlBtn;
        cout << "listSize: " << listButton.size() << endl;

        for (uint i=0;i<listButton.size();i++){
            if (other==listButton[i])
                tlBtn=(TimelineButton*)listButton[i+1];
        }
        //use MAKEUSERPOPUP here!!!
        sceneData->staticButton=(BasicButton*)sceneData->actorInfo["18CreateActionButton"].actorReference;
        sceneData->staticButton->setLocation(Vector3f(renderer->windowX/2-200,renderer->windowY/2-50,0));
        sceneData->staticButton->color=Vector4f(1.0,1.0,0.0,1.0);
        sceneData->staticButton->name="name your action";
        sceneData->staticButton->parent=tlBtn;
        sceneData->staticButton->clickedLeft();
        sceneData->buttonList.push_back(sceneData->staticButton);

        other->color = Vector4f(1.0,0.0,0.0,1.0);
        other->name="connect";

    }else if (other->name=="connect"){
        TimelineButton* tlBtn;
        for (uint i=0;i<listButton.size();i++){
            if (other==listButton[i])
                tlBtn=(TimelineButton*)listButton[i+1];
        }

        //save updated action before disconnecting
        TiXmlElement* myAction = new TiXmlElement("MovieSandbox");
        //save in order of actorList for later referencing!
        myAction->LinkEndChild(tlBtn->connectedAction->save(myAction));
        TiXmlDocument doc;
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        doc.LinkEndChild( decl );
        doc.LinkEndChild(myAction);
        string stringName=sceneData->startProject+tlBtn->connectedAction->name+".action";
        doc.SaveFile(stringName);

        tlBtn->disconnectAction();
        other->color = Vector4f(0.3 , 0.3 , 0.6 , 1.0);
        other->name="make";
    }

    // ZOOM IN TIMESCALE
    if (other->name=="zmIn")
    {
        zoomTimeScale(0.4f);
    }

    // ZOOM OUT TIMESCALE
    if (other->name=="zmOut")
    {
        zoomTimeScale(2.5f);
    }

}

void TimelineInspector::playTimelines(){

    for(uint i=0;i<listButton.size();i++){
        TimelineButton* tiBut=dynamic_cast<TimelineButton*>(listButton[i]);
        if (tiBut){
            ((TimelineButton*)listButton[i])->playTimeline();
        }
    }
    cout << "playing!" << endl;
}

void TimelineInspector::create(){sceneData->addButton(this);}


void TimelineInspector::drawPlane(){

    Inspector::drawPlane();
}

void TimelineInspector::drawTooltip(){

    Inspector::drawTooltip();
    drawTimeScale();
}

void TimelineInspector::drawTimeScale(){

    glPushMatrix();

    glTranslatef(location.x,location.y,location.z);

    float tlButtonOffset= 2* (listWidth + 4) +96;

    glTranslatef( tlButtonOffset , 65, 0);

    //how many seconds is our timeLine long?
    //this will determine how long we will draw our timeline!
    float timeLineWidth= timeSize * secondMark ;

    //now zoom in ( timeline gets wider the more we're zoomed in

    float startPos = timePos * timeLineWidth;

    glTranslatef(-startPos, 0, 0);


    ///keyFrame snaps
    int nLines = (int) (  (timeLineWidth * max(1.0f,zoomTime ) +   timePos *  timeLineWidth * max(1.0f,zoomTime) ) / snapToMark );

    renderer->setupShading("color");

    glColor4f(0.5,0.5,0.5,1.0);
    glLineWidth(1.0);
    glBegin( GL_LINES );

        for( int i = 0; i < nLines; i++)
        {
            if (i*snapToMark > startPos){
                glVertex3f(i*(snapToMark), -2, 0);
                glVertex3f(i*(snapToMark), 5+timelineActors.size() * (listHeight +10), 0);
            }
        }

    glEnd();

    float zoomedSecond = secondMark;

    ///seconds
    nLines = (int) ( (timeLineWidth * max(1.0f,zoomTime )  +   timePos *  timeLineWidth * max(1.0f,zoomTime )  ) / zoomedSecond );
    glLineWidth(2.0);

    glColor4f(0.8,0.0,0.0,1.0);
    glBegin( GL_LINES );
        for( int i = 0; i < nLines; i++)
        {
            if (i* zoomedSecond > startPos){
                glVertex3f(i*(zoomedSecond), -10.0, 0);
                glVertex3f(i*(zoomedSecond), 5+timelineActors.size() * (listHeight +10), 0);
            }

        }
    glEnd();


    ///half seconds
    nLines = (int) ( 2.0 * (timeLineWidth * max(1.0f,zoomTime )   +   timePos *  timeLineWidth * max(1.0f,zoomTime )  )  / zoomedSecond );
    glLineWidth(2.0);

    glColor4f(0.6,0.4,0.4,1.0);
    glBegin( GL_LINES );
        for( int i = 1; i < nLines; i+=2)
        {
            if (i*zoomedSecond * 0.5 > startPos){
                glVertex3f(i*zoomedSecond * 0.5, -5.0, 0);
                glVertex3f(i*zoomedSecond * 0.5, 5+timelineActors.size() * (listHeight +10), 0);
            }
        }
    glEnd();

    renderer->setupShading("font");

    glColor4f(1.0,1.0,1.0,1.0);

        for( int i = 0; i < nLines; i+=max(1,(int)zoomTime)  ){
            if (i*zoomedSecond * 0.5 > startPos){
                char secTxt[8];
                if (i%2==0){
                    sprintf(secTxt,"%i",i/2);
                    renderer->drawText(secTxt,i*zoomedSecond * 0.5,0);
                }
                else{
                    sprintf(secTxt,"%1.1f",(float)i/2.0);
                    renderer->drawText(secTxt,i*zoomedSecond * 0.5,0);
                }
            }
        }
    glPopMatrix();
}

void TimelineInspector::zoomTimeScale( float val ){

    //if( timeScale < .1 ) timeScale = .1;
    zoomTime *= val;
    secondMark /=val;

    cout << "timeStuff: millisecond per pixel" << zoomTime << " px per second: " << secondMark << endl;

    snapToMark=secondMark/10.0;

    //make sure we always have enough fields to snap in!

    //if snapTo is to large, shrink!
    while(snapToMark>2.5)
        snapToMark*=0.4;

    //if snapTo is too small, embiggen!
    //this makes sure that snapto is always 2.5 or bigger
    while (snapToMark<2.5)
        snapToMark*=2.5;

    for(uint i=0;i<listButton.size();i++)
    {
        if (listButton[i]->isA("TimelineButton")){
            ((TimelineButton*)listButton[i])->zoomTime=zoomTime;
            ((TimelineButton*)listButton[i])->secondMark=secondMark;
            ((TimelineButton*)listButton[i])->snapToMark=snapToMark;
        }
    }
}


void TimelineInspector::removeTimeline(int i){
    cout << "removing timeline" << endl;
    timelineActors.erase(timelineActors.begin()+i);
    timelines.erase(timelines.begin()+i);


    //name button
    listButton[i*3+0]->remove();
    //make button
    listButton[i*3+1]->remove();
    //timelineButton
    listButton[i*3+2]->remove();

    //erase removes all between first and last, not iincluding the one pointed to by last!

    //move remaining timelinebuttons up!
    for (int n=i*3+3;n<(int)listButton.size();n++){
        listButton[n]->setLocation(Vector3f(listButton[n]->location.x, listButton[n]->location.y - (listHeight+10),listButton[n]->location.z));
    }
    listButton.erase(listButton.begin()+i*3, listButton.begin()+3+i*3);
    //placeTimelineButtons();

}

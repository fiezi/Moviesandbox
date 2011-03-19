#ifndef H_TIMELINEBUTTON
#define H_TIMELINEBUTTON

#include "basicButton.h"
#include "interpolationHelper.h"
#include "skeletalActor.h"
#include "action.h"

///by Friedrich Kirschner and Chris Sugrue

class TimelineButton: public BasicButton{

public:

    vector <key*>   keyFrames;
    key*            selectedKey;

    // key dragging
    int             startPressedLeft;
    key*            draggedKey;

    InterpolationHelper    interpolator;

    SkeletalActor*  skeleton;
    bool bPlaying;
    bool bSkeletalTrack;

    float   timePos;                            //position in time we start from, is between 0 and 1 of timeSize
    float   zoomTime;                           //zoom Level
    float   timeSize;                           //size of the timeline in seconds

    float   secondMark;                         //how many pixels represent a unit of time (second)
    float   snapToMark;                         //where to put the keyframe so it makes sense.
    float   scrubMark;                          //position of scrubber

    float   beginTime;                          //start point of timeline - used internally

    Action*         connectedAction;
    Actor*          connectedActor;

    TimelineButton();
    virtual ~TimelineButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void drawPlane();
    virtual void drawKeys();

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();

    virtual void scrub(float scrubTime);
    virtual void playTimeline();
    virtual void pauseTimeline();

    virtual void deselect(int depth);
    virtual void create();

    virtual float convertToTimeGrid( float val );       //


    virtual void createKey(float keyTime);
    virtual bool isOccupied();                          // checks if key is occupying this place already

    virtual void deleteKey();                           // deletes selected keyFrames from timeline
    virtual void connectAction(Action* newAction);      // connects new action with timeline
    virtual void disconnectAction();                    // disconnects action
    virtual void updateAction();                        // updates connected actions so that changes to timeline occur in action
    virtual void populateFromAction(Action* other);     // replaces keyFrames of timeline and re-populates with keys from action


};
#endif // TIMELINEBUTTON

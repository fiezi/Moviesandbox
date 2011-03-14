#ifndef H_TIMELINEINSPECTOR
#define H_TIMELINEINSPECTOR

#include "inspector.h"
#include "action.h"
#include "timelineButton.h"

class TimelineInspector: public Inspector{

public:

    vector<Actor*>  timelineActors;
    vector<Action*> timelineActions;

    float   timeSize;                           //timeLine size in seconds

    float   timePos;                            //time section we look at

    float   zoomTime;                            //zoom Level
    float   secondMark;                           //how many pixels represent a unit of time (second)
    float   snapToMark;                           //where to put the keyframe so it makes sense.

    TimelineInspector();
    virtual ~TimelineInspector();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void createInspectorButtons();
//    virtual void assembleList();
//    virtual void refreshList();
    //virtual void placeTimelineButtons();


    virtual void trigger(MsbObject* other);

    virtual void playTimelines();

    virtual void create();

    virtual void addTimeline(int pos, bool bSkeletal);
    virtual void removeTimeline(int i);

    virtual void drawPlane();
    virtual void drawTooltip();
    virtual void drawTimeScale();

    virtual void zoomTimeScale( float val );
};
#endif // TIMELINEINSPECTOR

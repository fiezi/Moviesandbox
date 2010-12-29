#ifndef H_ACTION
#define H_ACTION

#include "listButton.h"
#include "skeletalActor.h"
#include "timelineButton.h"

class Action: public ListButton{

public:

    vector<key*> keyFrames;

    double inPoint;                 //animation starting offset
    double outPoint;                //animation ending offset
    double timeScale;               //animation speed

    Actor*  animActor;
    SkeletalActor*  skeleton;

    bool bSkeletal;
    bool bRelative;

    Action();
    virtual ~Action();

    virtual void registerProperties();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void finishDrag();

    virtual void trigger(Actor* other);

    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);

    virtual void importAction(string filename);
    virtual Matrix4f setRotationComponent(Matrix4f dest, Matrix3f source);


    virtual void create();
    virtual void load(TiXmlElement* myInfo);
    virtual TiXmlElement* save(TiXmlElement *root);
};
#endif // ACTION

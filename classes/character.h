#ifndef H_CHARACTER
#define H_CHARACTER

#include "skeletalActor.h"
#include "interpolationHelper.h"

class Character: public SkeletalActor{

public:

    Action* idleAction;
    string idleActionName;

    Actor* walkTarget;
    Actor* currentStep;
    Actor* lastStep;

    Actor* mouth;
    Actor* lookTarget;

    Actor* leftFoot;
    Actor* rightFoot;

    Actor* leftHand;
    Actor* rightHand;

    Actor* grabActor;

    float   footYOffset;

    InterpolationHelper*    idleMover;
    InterpolationHelper*    animMover;

    bool bFacingTarget;
    bool bBreathing;
    bool bKnowStep;
    bool bFirstStep;


    float walkSpeed;
    float turnSpeed;
    float idleAnimTimeScale;

    float breathStrength;


    float stepStart;            //in seconds
    float stepSize;             //in units
    float stepWidth;
    float stepDuration;        //in seconds
    float bobStrength;
    float hipNess;
    float leanForward;
    float liftLegs;

    Vector3f walkDirection;
    Vector3f startBodyLocation;
    Vector3f targetBodyLocation;

    Vector3f leftFootDelta;
    Vector3f rightFootDelta;

    Character();
    virtual ~Character();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);
    virtual void start();
    virtual void stop();
    virtual void updateShaders();

    virtual void idle(double deltaTime);
    virtual void computeStep();
    virtual void walk(double deltaTime);
    virtual void grab();

    virtual void createIdleMover();
    virtual void createAnimMover(Action* anim, bool bLoop, float inPoint);

    virtual void walkTowards(Actor* target, double deltaTime);
    virtual void turnTowards(Actor* target, double deltaTime);
    virtual void lookAt(Actor* target);
    virtual void playAnim(Action* anim, bool bLoop, float inPoint);
    virtual void stopAnim();

    virtual void create();
};
#endif // CHARACTER

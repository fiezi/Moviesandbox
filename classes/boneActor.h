#ifndef H_BONEACTOR
#define H_BONEACTOR

#include "actor.h"
#include "particleSystem.h"
#include "ofxPerlin.h"

class BoneActor: public Actor{

public:

    Actor*      mySystem;
    float       influence;

    bool        bPhysicsBone;
    bool        bJointedToBase;
    bool        bFixToWorld;

    bool        bDoIK;
    Vector3f    oldIKLoc;

    float       sideRotation;

    float       f;

    BoneActor();
    virtual ~BoneActor();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(Actor * other);

    virtual void update(double deltaTime);

    virtual void alignBaseRotation();

    virtual void doIK(Vector3f loc, bool bAbsolute);

    virtual void setLocation(Vector3f loc);

    virtual void create();

    virtual void remove();
};
#endif // BONEACTOR

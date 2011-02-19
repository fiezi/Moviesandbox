#ifndef H_SKELETALACTOR
#define H_SKELETALACTOR

#include "particleSystem.h"
#include "boneActor.h"

class SkeletalActor: public ParticleSystem{

public:

    vector<Actor*>  bones;           //bone Actors
    Matrix4f*       boneTransforms;
    Matrix4f*       invBoneTransforms;

    bool    bPhysicsBones;
    bool    bDelayedConvert;

    std::string     skeletonName;               //reference to Prefab that is used as skeleton


    SkeletalActor();
    virtual ~SkeletalActor();

    virtual void registerProperties();


    virtual void postLoad();
    virtual void setup();

    virtual void trigger(Actor * other);

    virtual void update(double deltaTime);
    virtual void updateShaders();

    virtual void start();
    virtual void stop();

    virtual void convertToPhysicsBones();

    virtual void reset();

    virtual void remove();
    virtual void create();
};
#endif // SKELETALACTOR

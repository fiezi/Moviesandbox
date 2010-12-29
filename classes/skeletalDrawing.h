#ifndef H_SKELETALDRAWING
#define H_SKELETALDRAWING

#include "particleSystem.h"
#include "boneActor.h"

class SkeletalDrawing: public ParticleSystem{

public:

    vector<Actor*>  bones;
    Matrix4f*       boneTransforms;
    Matrix4f*       invBoneTransforms;

    std::string     skeletonName;               //reference to Prefab that is used as skeleton

    SkeletalDrawing();
    virtual ~SkeletalDrawing();

    virtual void registerProperties();

    virtual void setup();

    virtual void updateShaders();

    virtual void trigger(Actor * other);

    virtual void create();
};
#endif // SKELETALDRAWING

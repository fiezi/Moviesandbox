#ifndef H_SKELETALHEAD
#define H_SKELETALHEAD

#include "skeletalActor.h"

class SkeletalHead: public SkeletalActor{

public:

    float mouthOpen;
    Matrix4f baseMouthUpMatrix;
    Matrix4f baseMouthDownMatrix;

    Actor* mouthUp;
    Actor* mouthDown;
    float mouthAmp;

    SkeletalHead();
    virtual ~SkeletalHead();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // SKELETALHEAD

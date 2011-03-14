#ifndef H_CAMERAACTOR
#define H_CAMERAACTOR

#include "actor.h"
#include "ofxPerlin.h"

class CameraActor: public Actor{

public:

    ofxPerlin   noise;

    bool bPossessed;
    bool bCameraShake;

    CameraActor();
    virtual ~CameraActor();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // CAMERAACTOR

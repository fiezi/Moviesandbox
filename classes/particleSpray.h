#ifndef H_PARTICLESPRAY
#define H_PARTICLESPRAY

#include "particleSystem.h"

class ParticleSpray: public ParticleSystem{

public:

    bool bSpray;
    float flySpeed;
    float sprayDistance;
    float xStretch;

    float sprayHeight;
    float sprayFrequency;
    int maxParticles;

    ParticleSpray();
    virtual ~ParticleSpray();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(Actor * other);

    virtual void update(double deltaTime);

    virtual void sprayParticles(double deltaTime);

    virtual void create();
};
#endif // PARTICLESPRAY

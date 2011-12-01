#ifndef H_HITBOX
#define H_HITBOX

#include "actor.h"

class HitBox: public Actor{

public:

    Actor*                  collisionActor;

    std::vector<Vector3f>   boxLocations;

    int                     currentLoc;


    TiXmlElement * root;
    TiXmlDocument doc;

    string savename;


    HitBox();
    virtual ~HitBox();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // HITBOX

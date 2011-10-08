#ifndef H_ACTORGIZMO
#define H_ACTORGIZMO

#include "actor.h"

class ActorGizmo: public Actor{

public:

    Actor*      xAxisGizmo;
    Actor*      yAxisGizmo;
    Actor*      zAxisGizmo;

    Actor*      xRotateGizmo;
    Actor*      yRotateGizmo;
    Actor*      zRotateGizmo;

    Vector3f*   moveAxis;
    Vector3f*   rotAxis;

    bool        bGizmoing;

    ActorGizmo();
    virtual ~ActorGizmo();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(Actor * other);

    virtual void beginGizmoing();
    virtual void endGizmoing();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // ACTORGIZMO

#ifndef H_CHARACTERCONTROLLER
#define H_CHARACTERCONTROLLER

#include "actor.h"

class CharacterController: public Actor{

public:

    Actor*  controlledCharacter;

    CharacterController();
    virtual ~CharacterController();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // CHARACTERCONTROLLER

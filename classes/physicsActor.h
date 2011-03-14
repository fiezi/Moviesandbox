#ifndef H_PHYSICSACTOR
#define H_PHYSICSACTOR

//Physics drawing defines
#define CUBESHAPE 1
#define CAPSULESHAPE 2
#define FLATSHAPE 3


#include "actor.h"
#include "particleSystem.h"

class PhysicsActor: public Actor{

public:

      Actor* oldBase;

      dBodyID body;
      dGeomID geom;
      dJointID joint;


      bool bInit;

      bool bJointedToBase;

      bool bFixToWorld;
      bool bSkeletalPart;

      GLuint capsuleList;

      dWorldID world;
      dSpaceID space;

      int type;
      float angleDamp;
      float linearDamp;

      Vector3f shape;

      dReal    mass;

      PhysicsActor();
      PhysicsActor(Vector3f myShape,dReal myMass, int myType);

      virtual ~PhysicsActor();

      virtual void registerProperties();

      virtual void postLoad();                     //gets called immediately after all variables have been loaded, but before setup
      virtual void setup();
      virtual void update(double deltaTime);
      virtual void trigger(MsbObject* other);

      virtual void makeJoint();

      virtual void drawCollisionBox() const;
      virtual void generateCapsuleList();

      void setRotationODE(Vector3f vec);

      virtual void create();

};
#endif

#ifndef H_PARTICLESYSTEM
#define H_PARTICLESYSTEM

#include "includes.h"
#include "actor.h"
#include "textfile.h"

class MeshData;

class ParticleSystem: public Actor
{
public:

      ParticleSystem();

      virtual void setup();

      virtual void update(double deltaTime);
      virtual void updateShaders();

      void addParticles(int amount);
      void deleteParticle(int particleNumber);
      void addRandomParticles(int amount, float diameter);

      void create();
      virtual void load(TiXmlElement *myInfo);
};
#endif

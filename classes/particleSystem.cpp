#include "particleSystem.h"
#include "control.h"
#include "input.h"
#include "meshData.h"

ParticleSystem::ParticleSystem(){
name="ParticleSystem";
sceneShaderID="color";
textureID="NULL";
bTextured=false;
bUseShader=true;

drawType=DRAW_PARTICLES;
registerProperties();
}

void ParticleSystem::addParticles(int amount){

    for (int i=0;i<amount; i++){
        vertexData myVData;
        myVData.birth=(0.0);
        myVData.color=Vector4f(1,1,1,1);
        myVData.secondaryColor=Vector3f(0.0,0.0,0.0);

        myVData.normal=Vector3f(0,0,1);
        myVData.texCoord=Vector3f(1,1,1);

        renderer->vboList[vboMeshID]->vData.push_back(myVData);
    }

}

void ParticleSystem::deleteParticle(int particleNumber){

      //delete the particle
      renderer->vboList[vboMeshID]->vData.erase(renderer->vboList[vboMeshID]->vData.begin()+particleNumber);
}

void ParticleSystem::addRandomParticles(int amount, float diameter){

    for (int i=0;i<amount;i++){

        vertexData myVData;

        myVData.location.x=((double)rand()/(double)RAND_MAX) * diameter;
        myVData.location.y=((double)rand()/(float)RAND_MAX) * diameter;
        myVData.location.z=((double)rand()/(float)RAND_MAX) * diameter;
        myVData.location.w=1.0; //scale!!!

        myVData.normal.x=((double)rand()/(double)RAND_MAX) * diameter;
        myVData.normal.y=((double)rand()/(float)RAND_MAX) * diameter;
        myVData.normal.z=((double)rand()/(float)RAND_MAX) * diameter;

        myVData.color=Vector4f((float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,1);
        myVData.texCoord=Vector3f(myVData.color.r, myVData.color.g, myVData.color.b) ;

        myVData.secondaryColor=Vector3f((float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,1);
        myVData.birth=i;

        renderer->vboList[vboMeshID]->vData.push_back(myVData);
    }

}

void ParticleSystem::setup(){
     Actor::setup();
}

void ParticleSystem::update(double deltaTime){
     Actor::update(deltaTime);
     elapsedTime+=deltaTime;
}

void ParticleSystem::updateShaders(){
     Actor::updateShaders();
}




void ParticleSystem::create(){

renderer->addActor(this);
std::cout << "created a ParticleSystem! \n";
}


void ParticleSystem::load(TiXmlElement *myInfo){

Actor::load(myInfo);

}



/*
      for (unsigned int i=0;i<segments.size();i++){
        if (particleNumber<segments[i]){
            mySegment=i;
            break;
            }
        //or, if we are deleting from the last segment...
        else{
            cout << "deleting from the last segment!" << endl;
            mySegment=segments.size()-1;
            }
        }
        //make sure we draw one particle less in every segment above the one we delete from
        for (unsigned int i=mySegment;i<segments.size();i++){
          //dont delete from the last segment
          if (mySegment<(int)segments.size()-1)
            //segments[i]--;
          //check for unnecessary segments
          if (i>0)
            if (segments[i]==segments[i-1])
              {
              segments.erase(segments.begin()+i-1);
              i--;
              }
        }
        particles.erase(particles.begin()+particleNumber);
        //particles[particleNumber].color=Vector4f(1,0,0,1);

*/

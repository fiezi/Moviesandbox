#include "flexGrid.h"
#include "renderer.h"
#include "meshData.h"

FlexGrid::FlexGrid(){

gridHeight=10;
name="grid";
bTextured=false;
bUseShader=true;
drawType=DRAW_PLANE;
}

void FlexGrid::setup(){
}

void FlexGrid::trigger(MsbObject* other){

cout << "triggered!" << endl;
ParticleSystem * p=dynamic_cast<ParticleSystem *>(other);

if (p)
  makeGridFromParticles( p );

}

void FlexGrid::makeGridFromParticles(ParticleSystem * p){

MeshData * myMesh =  renderer->vboList[p->vboMeshID];

for (unsigned int i=0;i<myMesh->vData.size(); i++)
  {
      cout << "number of verts in: " << vertLocations.size() << endl;

      Vector3f v = Vector3f(myMesh->vData[i].location.x,myMesh->vData[i].location.y,myMesh->vData[i].location.z);
      vertLocations.push_back(v);
      v.y+=gridHeight;
      vertLocations.push_back(v);
  }
location=p->location;
rotation=p->rotation;
scale=p->scale;

}

void FlexGrid::draw(){

Actor::draw();
}

void FlexGrid::drawPlane(){

drawQuadstrip();
}

void FlexGrid::drawQuadstrip(){

    if (vertLocations.size()>0)
    {
        GLfloat *verts=&vertLocations[0].x;

        glVertexPointer(3, GL_FLOAT,0,verts);

        glEnableClientState(GL_VERTEX_ARRAY);

        glDrawArrays(GL_QUAD_STRIP,0,vertLocations.size());

        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

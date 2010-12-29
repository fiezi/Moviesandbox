

#include "meshData.h"
#include "renderer.h"

MeshData::MeshData(){	
	
bIsSkeletal=false;
bIsHead=false;
bVertexColor=true;
bEditable=true;

boneCount=0;
texCoordPerVertexCount=3;
verticesPerShapeCount=4;
vertexInterpretation=GL_POINTS;
bindShapeMatrix=NULL;

drawType=DRAW_PARTICLES;

registerProperties();
}

MeshData::~MeshData(){}

void MeshData::registerProperties(){
Actor::registerProperties();
}

void MeshData::setup(){

Actor::setup();
}

void MeshData::trigger(Actor * other){}

void MeshData::update(double deltaTime){}

void MeshData::create(){renderer->addActor(this);}

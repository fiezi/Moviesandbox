
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

meshType=MESH_VBO;
vertexInterpretation=GL_POINTS;
bindShapeMatrix=NULL;

lowerLeftBack=Vector3f(0,0,0);
upperRightFront=Vector3f(0,0,0);
center=Vector3f(0,0,0);
registerProperties();
}

MeshData::~MeshData(){}

void MeshData::registerProperties(){
    MsbObject::registerProperties();
}

void MeshData::setup(){

    MsbObject::setup();
}

void MeshData::trigger(MsbObject* other){}

void MeshData::update(double deltaTime){}


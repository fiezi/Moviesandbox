

#include "brushFilter.h"
#include "renderer.h"
#include "input.h"
#include "sceneData.h"
#include "drawTool.h"



BrushFilter::BrushFilter(){}

BrushFilter::BrushFilter(DrawTool* myTool){

    parent=myTool;
    setup();
}

BrushFilter::~BrushFilter(){}


void BrushFilter::setup(){

    input=Input::getInstance();
    renderer=Renderer::getInstance();
    sceneData=SceneData::getInstance();
    brush=parent->brush;
}

void BrushFilter::filter(vertexData* myVData){

    if (!parent){
        cout <<"filter not set up! No assigned tool found!" << endl;
        return;
    }


    myVData->location=Vector4f(parent->calcLoc.x,parent->calcLoc.y,parent->calcLoc.z,parent->brush->scale.x*0.1);


    myVData->color=brush->color;

    //standard bone references - nothing (set to -1)
    myVData->boneReferences=Vector4f(-1.0,-1.0,-1.0,-1.0);
    myVData->vertexWeights=Vector4f(0.0,0.0,0.0,0.0);

    myVData->texCoord=Vector3f(0.0,0.0,0.0);     //texCoord holds color
    myVData->birth=float(glutGet(GLUT_ELAPSED_TIME))/1000.0; //particle creation time in seconds

	oldMouse3D=input->mouse3D;


}


void BrushFilter::reset(){
}

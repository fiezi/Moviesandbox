

#include "brushFilter.h"
#include "renderer.h"
#include "input.h"
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
    brush=parent->brush;
}

void BrushFilter::filter(vertexData* myVData){

    if (!parent){
        cout <<"filter not set up! No assigned tool found!" << endl;
        return;
    }


    myVData->location=Vector4f(parent->calcLoc.x,parent->calcLoc.y,parent->calcLoc.z,parent->brush->scale.x*0.1);


    //TODO: figure out normal generation for drawing!
        //myVData->normal=input->worldNormal;

    vertexData oldData;
    oldData.normal=Vector3f(0,0,0);
    oldData.location=Vector4f(0,0,0,brush->scale.x*0.1);

    if (renderer->vboList[brush->drawing->vboMeshID]->vData.size()>0)
        oldData=renderer->vboList[brush->drawing->vboMeshID]->vData.back();

    Vector4f pDistance=myVData->location - oldData.location;
    //this is the drawing vector
    Vector3f pDist3=Vector3f(pDistance.x,pDistance.y,pDistance.z);
    pDist3.normalize();





    switch (brush->normalMode){

        case NORMAL_UP:

            if (brush->bNormalFollowDrawing){
                    myVData->normal=renderer->grid->zAxis.crossProduct(pDist3);
                    cout << "normal: " << myVData->normal << endl;
            }else{
                myVData->normal=renderer->grid->yAxis;
            }
            break;


        case NORMAL_FRONT:

//            if (brush->bNormalFollowDrawing){
//                myVData->normal=pDist3.crossProduct(renderer->grid->yAxis);
//            }else{
                myVData->normal=renderer->grid->zAxis;
//            }
            break;

    }


    myVData->color=brush->color;

    //standard bone references - nothing (set to -1)
    myVData->boneReferences=Vector4f(-1.0,-1.0,-1.0,-1.0);
    myVData->vertexWeights=Vector4f(0.0,0.0,0.0,0.0);;

    myVData->texCoord=Vector3f(0.0,0.0,0.0);     //texCoord holds color
    myVData->birth=float(glutGet(GLUT_ELAPSED_TIME))/1000.0; //particle creation time in seconds


	Vector3f myAxis=Vector3f(0.0,1.0,0.0);
	if (abs(myVData->secondaryColor.y)>0.9)
		myAxis=Vector3f(0.0,0.0,1.0);

	Vector3f biNormal=input->mouse3D - oldMouse3D;
	biNormal.normalize();
//	myVData->normal=biNormal.crossProduct(myAxis);
	//myVData->normal=myAxis * 0.0;
	myVData->secondaryColor=biNormal;

	oldMouse3D=input->mouse3D;


}


void BrushFilter::reset(){
}

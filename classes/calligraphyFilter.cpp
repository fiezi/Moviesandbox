

#include "calligraphyFilter.h"
#include "renderer.h"
#include "input.h"
#include "sceneData.h"

CalligraphyFilter::CalligraphyFilter(){}

CalligraphyFilter::CalligraphyFilter(DrawTool* myTool){

    parent=myTool;
    setup();
}

CalligraphyFilter::~CalligraphyFilter(){}

void CalligraphyFilter::filter(vertexData* myVData){

    BrushFilter::filter(myVData);
    myVData->location.w*=input->mouseVector.length() * renderer->deltaTime / 100.0;

}

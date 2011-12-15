

#include "brushInspector.h"
#include "renderer.h"
#include "input.h"
#include "colorPickButton.h"
#include "sliderButton.h"


BrushInspector::BrushInspector(){

level=0;
currentSwatch=0;
numSwatches=5;

brushViz=0;

registerProperties();
}

BrushInspector::~BrushInspector(){}


void BrushInspector::setup(){
    Inspector::setup();
}


void BrushInspector::createInspectorButtons(){


    ColorPickButton* colorPick= new ColorPickButton;
    colorPick->name="pick Color";
    colorPick->bPermanent=true;
    colorPick->level=level+1;
    colorPick->setLocation(location + Vector3f(scale.x+5 ,5,0));

    colorPick->initialLocation=colorPick->location;
    colorPick->parent=this;

    colorPick->setup();
    colorPick->buttonColor=COLOR_WHITE;
    colorPick->mouseOverColor=COLOR_WHITE;
    sceneData->buttonList.push_back(colorPick);
    inspectorButtons.push_back(colorPick);

    for (int i=0;i<numSwatches;i++){
        AssignButton* showColor = new AssignButton;
        showColor->name="show color";
        showColor->parent=this;
        showColor->bPermanent=true;
        showColor->level=level+1;
        showColor->setLocation(location+Vector3f(35 + i*34,160,0));
        showColor->initialLocation=showColor->location;
        showColor->scale=Vector3f(32,32,1);
        showColor->setTextureID("icon_flat");
        showColor->setup();
        showColor->buttonColor=COLOR_WHITE;
        showColor->mouseOverColor=COLOR_WHITE;

        sceneData->buttonList.push_back(showColor);
        inspectorButtons.push_back(showColor);
        colorSwatches.push_back(showColor);
    }

    colorSwatches[0]->setTextureID("icon_base");

    SliderButton* brushScaleBtn= new SliderButton;
    brushScaleBtn->name="brush scale";
    brushScaleBtn->parent=this;
    brushScaleBtn->bPermanent=true;
    brushScaleBtn->level=level+1;
    brushScaleBtn->bDrawName=true;
    brushScaleBtn->setLocation(location+Vector3f(35,200,0));
    brushScaleBtn->initialLocation=brushScaleBtn->location;
    brushScaleBtn->scale=Vector3f(256,15,1);
    brushScaleBtn->setTextureID("icon_flat");
    brushScaleBtn->sceneShaderID="buttonColor";
    brushScaleBtn->bVertical=false;
    brushScaleBtn->setup();
    brushScaleBtn->buttonColor=sceneData->scrollBarColor;
    brushScaleBtn->mouseOverColor=sceneData->scrollBarColor;
    brushScaleBtn->slidePointColor=sceneData->scrollBarIndicatorColor;

    sceneData->buttonList.push_back(brushScaleBtn);
    inspectorButtons.push_back(brushScaleBtn);

    SliderButton* brushIntensityBtn= new SliderButton;
    brushIntensityBtn->name="brush intensity";
    brushIntensityBtn->parent=this;
    brushIntensityBtn->bPermanent=true;
    brushIntensityBtn->level=level+1;
    brushIntensityBtn->bDrawName=true;
    brushIntensityBtn->setLocation(location+Vector3f(35,220,0));
    brushIntensityBtn->initialLocation=brushIntensityBtn->location;
    brushIntensityBtn->scale=Vector3f(256,15,1);
    brushIntensityBtn->setTextureID("icon_flat");
    brushIntensityBtn->sceneShaderID="buttonColor";
    brushIntensityBtn->sliderValue=1.0;
    brushIntensityBtn->bVertical=false;
    brushIntensityBtn->setup();
    brushIntensityBtn->buttonColor=sceneData->scrollBarColor;
    brushIntensityBtn->mouseOverColor=sceneData->scrollBarColor;
    brushIntensityBtn->slidePointColor=sceneData->scrollBarIndicatorColor;
    sceneData->buttonList.push_back(brushIntensityBtn);
    inspectorButtons.push_back(brushIntensityBtn);

    brushViz = new BasicButton;
    brushViz->name="brushViz";
    brushViz->parent=this;
    brushViz->bPermanent=true;
    brushViz->level=level+1;
    brushViz->bDrawName=false;
    brushViz->setLocation(location+Vector3f(40,240,0)   );
    brushViz->initialLocation=brushViz->location;
    brushViz->scale=Vector3f(32,32,1);
    brushViz->setTextureID("brushTwo");
    brushViz->setup();
    brushViz->buttonColor=COLOR_WHITE;
    brushViz->mouseOverColor=COLOR_WHITE;
    sceneData->buttonList.push_back(brushViz);
    inspectorButtons.push_back(brushViz);


    ///Grid specific buttons


        AssignButton* gridAssign = new AssignButton;
        gridAssign->name="Grid Plane XY";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(35,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->setTextureID("icon_gridXY");
        gridAssign->setup();
        gridAssign->buttonColor=sceneData->meanButtonColor;

        sceneData->buttonList.push_back(gridAssign);
        inspectorButtons.push_back(gridAssign);

        gridAssign = new AssignButton;
        gridAssign->name="Grid Plane ZY";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(70,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->setTextureID("icon_gridZY");
        gridAssign->setup();
        gridAssign->buttonColor=sceneData->meanButtonColor;

        sceneData->buttonList.push_back(gridAssign);
        inspectorButtons.push_back(gridAssign);

        gridAssign = new AssignButton;
        gridAssign->name="Grid Plane XZ";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(105,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->setTextureID("icon_gridXZ");
        gridAssign->setup();
        gridAssign->buttonColor=sceneData->meanButtonColor;

        sceneData->buttonList.push_back(gridAssign);
        inspectorButtons.push_back(gridAssign);

}

void BrushInspector::refreshList(){

/*
    if (bOpen && listButton.size()==0){
        cout << "assembling brushinspector..." << endl;
        assembleList();
    }
*/

}

void BrushInspector::assembleList(){



}

void BrushInspector::trigger(MsbObject* other){

    Inspector::trigger(other);


    if (other->name=="Grid Plane XY"){
        sceneData->gridTool->gridRot=GRID_X-1;
        sceneData->gridTool->keyReleased('.');
    }

    if (other->name=="Grid Plane ZY"){
        sceneData->gridTool->gridRot=GRID_Y-1;
        sceneData->gridTool->keyReleased('.');
    }

    if (other->name=="Grid Plane XZ"){
        sceneData->gridTool->gridRot=GRID_Z-1;
        sceneData->gridTool->keyReleased('.');
    }

    if (other->name=="brush scale"){
        sceneData->brush->scale.x= pow(4.0 * ((SliderButton*)other)->sliderValue,2.0) *1.0 + 0.1;
        sceneData->brush->scale.y= pow(4.0 * ((SliderButton*)other)->sliderValue,2.0) *1.0 + 0.1;
        sceneData->brush->scale.z= pow(4.0 * ((SliderButton*)other)->sliderValue,2.0) *1.0 + 0.1;
        sceneData->brush->setScale(sceneData->brush->scale);
      }

    if (other->name=="brush intensity"){
        sceneData->brush->intensity=min(1.0, 0.01 + ((SliderButton*)other)->sliderValue * 1.1); //slightly exaggerate slidervalue so we can get to 1.0 earlier!
    }

    if (other->name=="pick Color"){
        cout << "triggered from pickColor!" << endl;
        color=Vector4f( max(min(color.r, 1.0f),0.0f),max(min(color.g, 1.0f),0.0f), max(min(color.b, 1.0f),0.0f), max(min(color.a, 1.0f),0.0f) );
        colorSwatches[currentSwatch]->buttonColor=color;
        colorSwatches[currentSwatch]->mouseOverColor=color;
        sceneData->brush->setColor(color);
        color=Vector4f(1.0,0.6,0.1,1.0);
    }

    if (other->name=="show color"){

        if (sceneData->controller->tool==TOOL_SELECT){
            if (sceneData->selectedActors.size()>0){
                    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
                        sceneData->selectedActors[i]->color=other->color;
                    }
            }
        }

        sceneData->brush->color=other->color;

        for (int i=0;i<numSwatches;i++){
            colorSwatches[i]->setTextureID("icon_flat");
            if (other==colorSwatches[i])
                currentSwatch=i;
        }
        ((Actor*)other)->setTextureID("icon_base");
    }

    //update brush preview
     brushViz->buttonColor=sceneData->brush->color;
     brushViz->mouseOverColor=sceneData->brush->color;
     brushViz->buttonColor.a=sceneData->brush->intensity+ 0.1;
     brushViz->mouseOverColor.a=sceneData->brush->intensity+ 0.1;
     brushViz->scale.x=sceneData->brush->scale.x * 10.0 + 10.0;
     brushViz->scale.y=sceneData->brush->scale.x * 10.0 +10.0;

}

void BrushInspector::create(){sceneData->addButton(this);}

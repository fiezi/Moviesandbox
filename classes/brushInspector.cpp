

#include "brushInspector.h"
#include "renderer.h"
#include "input.h"
#include "colorPickButton.h"
#include "sliderButton.h"


BrushInspector::BrushInspector(){

level=0;
currentSwatch=0;
numSwatches=5;
maxListItems=20;

registerProperties();
}

BrushInspector::~BrushInspector(){}


void BrushInspector::setup(){
Inspector::setup();
}

void BrushInspector::refreshList(){

    if (bOpen && listButton.size()==0){
        cout << "assembling brushinspector..." << endl;
        assembleList();
    }
}

void BrushInspector::assembleList(){

    ColorPickButton* colorPick= new ColorPickButton;
    colorPick->name="pick Color";
    colorPick->bPermanent=true;
    colorPick->level=level+1;
    colorPick->setLocation(location + Vector3f(scale.x+5 ,5,0));

    colorPick->initialLocation=colorPick->location;
    colorPick->parent=this;

    renderer->buttonList.push_back(colorPick);
    listButton.push_back(colorPick);

    for (int i=0;i<numSwatches;i++){
        AssignButton* showColor = new AssignButton;
        showColor->name="show color";
        showColor->parent=this;
        showColor->bPermanent=true;
        showColor->level=level+1;
        showColor->setLocation(location+Vector3f(35 + i*34,160,0));
        showColor->initialLocation=showColor->location;
        showColor->scale=Vector3f(32,32,1);
        showColor->textureID="icon_flat";
        showColor->color=Vector4f(1,1,1,1);


        renderer->buttonList.push_back(showColor);
        listButton.push_back(showColor);
        colorSwatches.push_back(showColor);
    }

    colorSwatches[0]->textureID="icon_base";

    SliderButton* brushScaleBtn= new SliderButton;
    brushScaleBtn->name="brush scale";
    brushScaleBtn->parent=this;
    brushScaleBtn->bPermanent=true;
    brushScaleBtn->level=level+1;
    brushScaleBtn->bDrawName=true;
    brushScaleBtn->setLocation(location+Vector3f(35,200,0));
    brushScaleBtn->initialLocation=brushScaleBtn->location;
    brushScaleBtn->scale=Vector3f(256,15,1);
    brushScaleBtn->textureID="icon_flat";
    brushScaleBtn->bVertical=false;

    renderer->buttonList.push_back(brushScaleBtn);
    listButton.push_back(brushScaleBtn);

    SliderButton* brushIntensityBtn= new SliderButton;
    brushIntensityBtn->name="brush intensity";
    brushIntensityBtn->parent=this;
    brushIntensityBtn->bPermanent=true;
    brushIntensityBtn->level=level+1;
    brushIntensityBtn->bDrawName=true;
    brushIntensityBtn->setLocation(location+Vector3f(35,220,0));
    brushIntensityBtn->initialLocation=brushIntensityBtn->location;
    brushIntensityBtn->scale=Vector3f(256,15,1);
    brushIntensityBtn->textureID="icon_flat";
    brushIntensityBtn->bVertical=false;

    renderer->buttonList.push_back(brushIntensityBtn);
    listButton.push_back(brushIntensityBtn);

    ///Normal alignment buttons!

        AssignButton* normalAssign = new AssignButton;
        normalAssign->name="normals up";
        normalAssign->parent=this;
        normalAssign->bPermanent=true;
        normalAssign->level=level+1;
        normalAssign->setLocation(location+Vector3f(35,250,0));
        normalAssign->initialLocation=normalAssign->location;
        normalAssign->scale=Vector3f(32,32,1);
        normalAssign->textureID="normals_up";
        normalAssign->color=Vector4f(1,1,0,1);

        renderer->buttonList.push_back(normalAssign);
        listButton.push_back(normalAssign);

        normalAssign = new AssignButton;
        normalAssign->name="normals front";
        normalAssign->parent=this;
        normalAssign->bPermanent=true;
        normalAssign->level=level+1;
        normalAssign->setLocation(location+Vector3f(70,250,0));
        normalAssign->initialLocation=normalAssign->location;
        normalAssign->scale=Vector3f(32,32,1);
        normalAssign->textureID="normals_front";
        normalAssign->color=Vector4f(1,1,1,1);

        renderer->buttonList.push_back(normalAssign);
        listButton.push_back(normalAssign);

        normalAssign = new AssignButton;
        normalAssign->name="normals follow drawing";
        normalAssign->parent=this;
        normalAssign->bPermanent=true;
        normalAssign->level=level+1;
        normalAssign->setLocation(location+Vector3f(35,300,0));
        normalAssign->initialLocation=normalAssign->location;
        normalAssign->scale=Vector3f(32,32,1);
        normalAssign->textureID="normals_follow_drawing";
        normalAssign->color=Vector4f(1,1,0,1);

        renderer->buttonList.push_back(normalAssign);
        listButton.push_back(normalAssign);

}

void BrushInspector::trigger(Actor* other){

    if (other->name=="brush scale"){
        renderer->brush->scale.x= pow(0.4+ ((SliderButton*)other)->sliderValue,2.0) ;
    }

    if (other->name=="brush intensity"){
        renderer->brush->intensity=min(1.0, 0.01 + ((SliderButton*)other)->sliderValue);
    }

    if (other->name=="pick Color"){
        cout << "triggered from pickColor!" << endl;
        color=Vector4f( max(min(color.r, 1.0f),0.0f),max(min(color.g, 1.0f),0.0f), max(min(color.b, 1.0f),0.0f), max(min(color.a, 1.0f),0.0f) );
        colorSwatches[currentSwatch]->color=color;
        renderer->brush->color=color;
        color=Vector4f(1.0,0.6,0.1,1.0);
    }

    if (other->name=="show color"){

        if (input->controller->tool==TOOL_SELECT){
            if (input->selectedActors.size()>0){
                    for (int i=0;i<(int)input->selectedActors.size();i++){
                        input->selectedActors[i]->color=other->color;
                    }
            }
        }else if (input->controller->tool==TOOL_DRAW){
            renderer->brush->color=other->color;
        }

        for (int i=0;i<numSwatches;i++){
            colorSwatches[i]->textureID="icon_flat";
            if (other==colorSwatches[i])
                currentSwatch=i;
        }
        other->textureID="icon_base";
    }

    if (other->name=="normals up"){

        renderer->brush->normalMode=NORMAL_UP;
        other->color=Vector4f(1,1,0,1);
    }

    if (other->name=="normals front"){

        renderer->brush->normalMode=NORMAL_FRONT;
        other->color=Vector4f(1,1,0,1);

    }

    if (other->name=="normals follow drawing"){

        renderer->brush->bNormalFollowDrawing=!renderer->brush->bNormalFollowDrawing;

        if (renderer->brush->bNormalFollowDrawing)
            other->color=Vector4f(1,1,0,1);
        else
            other->color=Vector4f(1,1,1,1);


    }


}

void BrushInspector::create(){renderer->addButton(this);}

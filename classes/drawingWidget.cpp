#include "drawingWidget.h"
#include "input.h"
#include "renderer.h"
#include "sliderButton.h"
#include "spriteMeshLoader.h"
#include "drawTool.h"

DrawingWidget::DrawingWidget(){

name="drawWidget";
tooltip="draw";

bWidgetOpen=false;
bKinectToolOpen=false;

color=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;

useTool=TOOL_DRAW;

}

DrawingWidget::~DrawingWidget(){

}

void DrawingWidget::setup(){

    Widget::setup();
    drawTool=(DrawTool*)sceneData->controller->myTools[TOOL_DRAW];
}

void DrawingWidget::clickedLeft(){

    Widget::clickedLeft();
    if (sceneData->controller->tool!=useTool)
        sceneData->controller->switchTool(useTool);
    color=COLOR_RED;
}

void DrawingWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
    //sceneData->controller->switchTool(TOOL_DRAW);
/*
    if (bKinectToolOpen){
        listButton[4]->color=COLOR_YELLOW;
    }
*/


}

void DrawingWidget::closeWidget(){

    //input->deselectActors();
    color=COLOR_WHITE;
}



///creates new drawing here!
void DrawingWidget::trigger(MsbObject* other){

    if (other->name=="Draw Particles"){
        useTool=TOOL_DRAW;
        sceneData->drawTool->bPaintMesh=false;
        sceneData->brush->drawType=DRAW_SPRITE;
        setTextureID("icon_paint");
    }

    if (other->name=="Draw Calligraphy"){
        useTool=TOOL_CALLIGRAPHY;
        sceneData->drawTool->bPaintMesh=false;
        sceneData->brush->drawType=DRAW_SPRITE;
        setTextureID("icon_paintFancy");
    }

    if (other->name=="Draw Meshes"){
        useTool=TOOL_DRAW;
        sceneData->drawTool->bPaintMesh=true;
        sceneData->brush->drawType=DRAW_VBOMESH;
        setTextureID("icon_drawMesh");
    }


    //close widget and select again
    clickedRight();
    clickedLeft();

}

void DrawingWidget::closeKinectTool(){

    sceneData->externalInputList["kinectInput"]->stopProgram();

}

void DrawingWidget::openKinectTool(){

    if (!sceneData->brush->drawing){
        sceneData->makeWarningPopUp("No drawing! \n please place a drawing first!", this);
        return;
    }
    sceneData->brush->drawing->drawType=DRAW_POINTPATCH;
    sceneData->brush->drawing->bTextured=true;
    sceneData->brush->drawing->setTextureID("sharedMemory");
    sceneData->brush->drawing->sceneShaderID="heightfield";
    sceneData->brush->drawing->particleScale=100;

    sceneData->externalInputList["kinectInput"]->startProgram();
    listButton[5]->color=Vector4f(1,1,0,1);
    bKinectToolOpen=true;
}

void DrawingWidget::importKinect(){

            static float kinectContent[1024*1024*4];
            glBindTexture(GL_TEXTURE_2D,sceneData->textureList["sharedMemory"]->texture);
            glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,&kinectContent);
            glBindTexture(GL_TEXTURE_2D,0);
            sceneData->brush->drawing->drawType=DRAW_PARTICLES;
            sceneData->brush->drawing->bTextured=false;
            sceneData->brush->drawing->setTextureID("NULL");
            sceneData->brush->drawing->sceneShaderID="color";
            sceneData->brush->drawing->bPickable=true;
            sceneData->brush->drawing->particleScale=2;
            for (int i=0;i<1024*1024*4;i+=4){
                Vector3f myLoc;
                myLoc.z=kinectContent[i+3] * 10.0f;
                if (myLoc.z==0.0f)
                    continue;
                myLoc.x= (i/4)%1024;
                myLoc.y= (int)((i/4)/1024 );

                myLoc.x=myLoc.x/1024.0f - 0.5f;
                myLoc.y=myLoc.y/1024.0f - 0.5f;

                myLoc.x*=myLoc.z;
                myLoc.y*=myLoc.z;

                sceneData->brush->setLocation(myLoc);
                sceneData->brush->color=Vector4f(kinectContent[i],kinectContent[i+1],kinectContent[i+2],1.0);
                sceneData->brush->normalMode=NORMAL_FRONT;
                input->mouseVector.x=1.0f;

                drawTool->paint();
            }
}

void DrawingWidget::create(){sceneData->addButton(this);}


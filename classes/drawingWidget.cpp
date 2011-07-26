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

}

DrawingWidget::~DrawingWidget(){

}

void DrawingWidget::setup(){

    Widget::setup();
    drawTool=(DrawTool*)sceneData->controller->myTools[TOOL_DRAW];
}

void DrawingWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
    sceneData->controller->switchTool(TOOL_DRAW);

    if (bKinectToolOpen){
        listButton[4]->color=COLOR_YELLOW;
    }



}

void DrawingWidget::closeWidget(){

    input->deselectActors();


    if (sceneData->brush->drawing){
        sceneData->makeWarningPopUp("saving drawing...",this);
        renderer->draw();
        sceneData->controller->myTools[TOOL_DRAW]->save();
        sceneData->brush->drawing->drawType=DRAW_VBOMESH;
        sceneData->brush->drawing->reset();
        input->bTextInput=false;
    }


    sceneData->controller->switchTool(TOOL_SELECT);
    if (sceneData->staticButton){
        sceneData->staticButton->focusClick();
    }

    color=COLOR_WHITE;

}



///creates new drawing here!
void DrawingWidget::trigger(MsbObject* other){

    if (other->name=="New Drawing" && (sceneData->controller->tool==TOOL_DRAW || sceneData->controller->tool==TOOL_CALLIGRAPHY) ){
        //save old drawing first!
        if (sceneData->brush->drawing)
            sceneData->controller->currentTool->save();
        sceneData->makeUserPopUp("Name your new drawing:",this);
    }

    //from staticButton
    if (other->name=="Name your new drawing:"){
        sceneData->brush->createNewDrawing();
        sceneData->specialSelected=sceneData->brush->drawing;
    }

    if (other->name=="Draw Particles (p)"){
        sceneData->controller->switchTool(TOOL_DRAW);
    }

    if (other->name=="Draw Calligraphy"){
        sceneData->controller->switchTool(TOOL_CALLIGRAPHY);
    }

    if (other->name=="Select Particles"){
        sceneData->controller->switchTool(TOOL_PARTICLESELECT);
    }

	if (other->name=="save"){
        sceneData->controller->currentTool->save();
	}

    if (other->name=="save As..."){
        cout << "creating VBO..." << endl;
        sceneData->spriteMeshLoader->saveSpriteMesh(sceneData->startProject+"/"+input->inputText+".spriteMesh",(SkeletalActor*)(sceneData->brush->drawing));
        sceneData->spriteMeshLoader->loadSpriteMesh(sceneData->startProject+"/"+input->inputText+".spriteMesh",input->inputText);

		sceneData->brush->drawing->name=input->inputText;
		sceneData->brush->drawing->vboMeshID=input->inputText;

        TiXmlElement * newMesh= new TiXmlElement("SpriteMesh");
        newMesh->SetAttribute("meshID",input->inputText);
        newMesh->SetAttribute("meshFilename",input->inputText+".spriteMesh");
        sceneData->addToLibrary(newMesh);
        free(newMesh);
    }


    if (other->name=="Select Drawing"){
        if (input->worldTarget && input->worldTarget->name!="ground" && input->worldTarget->name!="grid"){
            //save old drawing
            if (sceneData->brush->drawing)
                sceneData->drawTool->save();
            //select new drawing
            drawTool->stop();
            sceneData->specialSelected=input->worldTarget;
            sceneData->brush->drawing=(SkeletalActor*)sceneData->specialSelected;
            drawTool->start();
        }
    }

    if (other->name=="merge Drawings"){
        drawTool->mergeDrawings();
    }
///scale paticle size

    if (other->name=="scaleZ x2"){
        drawTool->scaleZ(2.0);
    }

    if (other->name=="scaleZ x0.5"){
        drawTool->scaleZ(0.5);
    }

    if (other->name=="Import Kinect"){
        if (other->color==Vector4f(1,1,0,1)){
            //TODO: move these functions to drawTool!
            importKinect();
            other->color=COLOR_WHITE;
            closeKinectTool();
            bKinectToolOpen=false;
        }
        else{
            openKinectTool();
        }
    }

   if (other->name=="clear Drawing"){
        drawTool->clearDrawing();
   }

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
    sceneData->brush->drawing->textureID="sharedMemory";
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
            sceneData->brush->drawing->textureID="NULL";
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


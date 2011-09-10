

#include "boneWidget.h"
#include "renderer.h"
#include "input.h"
#include "brush.h"
#include "spriteMeshLoader.h"

BoneWidget::BoneWidget(){

name="boneWidget";
tooltip="bones";

bWidgetOpen=false;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;
color=listColor;
//highlightColor=Vector4f(0.8,0.0,0.0,1.0);

useTool=TOOL_BONE;
}

BoneWidget::~BoneWidget(){}


void BoneWidget::clickedLeft(){

    color=COLOR_RED;
    Widget::clickedLeft();

    if (sceneData->controller->tool!=useTool)
        sceneData->controller->switchTool(useTool);

}

void BoneWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);


    /*
    widgetLocation=Vector3f(location.x,location.y,0);

    //switch to create bone mode
    sceneData->controller->switchTool(TOOL_BONE);
    //unless our drawing is already boned (tee-he-he), then we might want to switch to skinning mode!
    if (sceneData->specialSelected && sceneData->vboList[sceneData->specialSelected->vboMeshID]->bIsSkeletal)
        sceneData->controller->switchTool(TOOL_SKIN);
    */

}

void BoneWidget::closeWidget(){

    //input->deselectActors();
    color=COLOR_WHITE;

    /*
    if (sceneData->brush->drawing){
        sceneData->makeWarningPopUp("saving drawing...",this);
        renderer->draw();
        sceneData->controller->myTools[TOOL_BONE]->save();
        sceneData->brush->drawing->drawType=DRAW_VBOMESH;
        sceneData->brush->drawing->reset();
        sceneData->staticButton->focusClick();
        input->bTextInput=false;
    }


    sceneData->controller->switchTool(TOOL_SELECT);

    */
}

void BoneWidget::trigger(MsbObject* other){


/*
	SkeletalActor* skel=sceneData->brush->drawing;
	if (!skel) return;


    if (other->name=="Create Bone (b)"){
        sceneData->controller->switchTool(TOOL_BONE);
    }

    if (other->name=="Paint Weights"){
        if (sceneData->brush->drawing){
            sceneData->makeWarningPopUp("saving drawing...",this);
            renderer->draw();
            sceneData->controller->myTools[TOOL_BONE]->save();
            sceneData->brush->drawing->drawType=DRAW_VBOMESH;
            sceneData->brush->drawing->reset();
            sceneData->staticButton->focusClick();
            input->bTextInput=false;
        }
        sceneData->controller->switchTool(TOOL_SKIN);
    }


	if (other->name=="Load Bones"){

		cout << "loading bones..." << endl;

		string meshID;
		string filename=sceneData->openFileDialog();
		//check for file ending
		size_t found, appendix;
		appendix=filename.find(".spriteMesh");
		if (appendix!=string::npos){
			//generate meshID
			found=filename.find_last_of(DIRECTORY_SEPARATION);
			found+=1;
			meshID=filename.substr(found, appendix-found);
			cout << "found following meshID:" << meshID << endl;
		}else{
			return;
		}
		//resetting bones of drawing...
		string oldMeshID=sceneData->brush->drawing->vboMeshID;

		sceneData->brush->drawing->vboMeshID=meshID;
		sceneData->brush->drawing->reset();
		sceneData->brush->drawing->vboMeshID=oldMeshID;

		//set to skeletal if loaded mesh was skeletal too!
		if (sceneData->vboList[meshID]->bIsSkeletal)
			sceneData->vboList[oldMeshID]->bIsSkeletal=true;

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

    */


    if (other->name=="Paint Weights"){
        useTool=TOOL_SKIN;
        textureID="icon_paintWeights";
    }

    if (other->name=="Create Bone (b)"){
        useTool=TOOL_BONE;
        textureID="icon_addBones";
    }

    clickedRight();
    clickedLeft();

}

void BoneWidget::highlight(Actor* other){

    /*
        for (int i=0;i<(int)listButton.size();i++)
            if (listButton[i]!=other && listButton[i]->color==highlightColor)
                listButton[i]->color=listColor;

        other->color=highlightColor;
    */
}

void BoneWidget::create(){sceneData->addButton(this);}

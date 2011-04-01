

#include "skinTool.h"
#include "renderer.h"
#include "input.h"
#include "spriteMeshLoader.h"
#include "skeletalActor.h"
#include "brush.h"

SkinTool::SkinTool(){

}

SkinTool::~SkinTool(){}


void SkinTool::start(){

    MsbTool::start();

    input->bKeepSelection=true;

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    brush->bHidden=false;

    highlightButton("Paint Weights");


	//use selectedActor as drawing
	if (sceneData->selectedActors.size()>0){
		SkeletalActor* skel=dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
		if (skel){
			sceneData->specialSelected=skel;
			brush->drawing=skel;
		}
	}

	//use already assigned drawing
    if (brush->drawing){
        brush->bHidden=false;
        brush->drawing->sceneShaderID="skinning";
		brush->drawing->drawType=DRAW_PARTICLES;

       //if we have an original Matrix, then reset our transformMatrices!
        if (sceneData->vboList[sceneData->specialSelected->vboMeshID]->bIsSkeletal){
            Matrix4f identityMatrix;
            identityMatrix.identity();

            //reset Matrices...
            for (int i=0;i<(int)sceneData->vboList[sceneData->specialSelected->vboMeshID]->bones.size();i++){
                if (sceneData->brush->drawing->bones[i]->originalMatrix!=identityMatrix){
                    sceneData->brush->drawing->bones[i]->transformMatrix=sceneData->brush->drawing->bones[i]->originalMatrix;
                    sceneData->brush->drawing->bones[i]->originalMatrix.identity();
                }
            }
        }
        //color=highlightColor;
        //highlight(listButton[0]);
    }
	//no useable drawing found
    else{
        sceneData->makeWarningPopUp("OOPS! \n \nNo Drawing to Skin. Create or Select a Drawing first!", myBtn);
        cout << "no drawing!" << endl;
        //sceneData->controller->switchTool(TOOL_SELECT);
        input->bTextInput=false;
    }


}

void SkinTool::stop(){

    MsbTool::stop();

    input->bKeepSelection=false;
    glutSetCursor(GLUT_CURSOR_INHERIT);
    brush->bHidden=true;
    lowlightButton();

    SkeletalActor* skel=brush->drawing;

    if (!skel) return;

	//determine if we're skeletal or not
	if (skel->bones.size()>0)
		skel->sceneShaderID="skeletal";
    else
        skel->sceneShaderID="drawing";

    for (int i=0;i<(int)skel->bones.size();i++)
        skel->bones[i]->bPickable=true;


	//save vbo and reload, then assign to this actor and switch drawmode to VBO
    sceneData->spriteMeshLoader->saveSpriteMesh("resources/meshes/"+skel->vboMeshID+".spriteMesh",skel);
	sceneData->spriteMeshLoader->loadSpriteMesh("resources/meshes/"+skel->vboMeshID+".spriteMesh",skel->vboMeshID);

    //open my.library and append this mesh!
    TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
    myElement->SetAttribute("meshID",skel->vboMeshID);
    myElement->SetAttribute("meshFilename","resources/meshes/"+skel->vboMeshID+".spriteMesh");
    sceneData->addToLibrary(myElement);

	skel->drawType=DRAW_VBOMESH;

	skel->reset();


}


void SkinTool::keyReleased(int key){

    MsbTool::keyReleased(key);

    //DELETE
   if (key==127 || key==8){
        if (!input->worldTarget) return;

            for (int i=0;i<(int)brush->drawing->bones.size();i++){
                if (input->worldTarget==brush->drawing->bones[i]){
                    input->deselectActors();
                    brush->drawing->bones.erase(brush->drawing->bones.begin()+i);
                    input->worldTarget->remove();
                }
            }
    }
}

void SkinTool::mouseReleased(int btn){

    DrawTool::mouseReleased(btn);

    SkeletalActor* skel=brush->drawing;
    for (int i=0;i<(int)skel->bones.size();i++)
        skel->bones[i]->bPickable=true;

    input->bKeepSelection=false;
}

void SkinTool::selectActors(int btn, Actor* other){


       //we can select and move bones
       //we can select and move the grid
       //we can special-select the currently active drawing
       //we deselect when clicking on ground or on nothing!

    if (fabs(input->mouseVector.length())>0.0)
        return;

    if (!input->worldTarget)
        return;

    //don't do anything to selection if we're just finishing a move or rotate
    if (sceneData->actorMenu && sceneData->actorMenu->listButton.size()>0)
        return;

    //only allow selection of bones in skinTool!!
    BoneActor* myBone=dynamic_cast<BoneActor*>(input->worldTarget);

    if (myBone){

        //no drawing present!
        if (!brush->drawing)
            return;

        //selecting bones
        //only select one bone at a time right now...
        for (int i=0;i<(int)brush->drawing->bones.size();i++){
            if (input->worldTarget==brush->drawing->bones[i]){
                input->deselectActors();
                sceneData->selectedActors.push_back(input->worldTarget);
                input->worldTarget->bSelected=true;
            }
        }

        if (input->worldTarget->name=="ground" || input->worldTarget==sceneData->grid)
            input->deselectActors();

    }

    //right Button creates menu if on selected actor
    if (btn==MOUSEBTNRIGHT && input->worldTarget && input->worldTarget->bSelected){
        for (int i=0;i<(int)sceneData->selectedActors.size();i++)
            if (input->worldTarget==sceneData->selectedActors[i])
                sceneData->createActorMenu();
    }

}

void SkinTool::erase(){
    paint();
}

void SkinTool::paint(){



    if (fabs(input->mouseVector.length())==0.0)
        return;

    input->bKeepSelection=true;

    SkeletalActor* skel =dynamic_cast<SkeletalActor*>(brush->drawing);
    if (!skel){
        cout << "drawing cannot be skinned" << endl;
        return;
        }

    for (int i=0;i<(int)skel->bones.size();i++)
        skel->bones[i]->bPickable=false;

    calcLoc=brush->location-skel->location;

    //calcLocation();

    for (unsigned int pID=0;pID<sceneData->vboList[skel->vboMeshID]->vData.size();pID++){           //go through particles

        //find out where the brush is in relation to the particle
        Vector4f loc=sceneData->vboList[skel->vboMeshID]->vData[pID].location;
        Vector3f distance=calcLoc - Vector3f(loc.x,loc.y,loc.z);

        ///within brush range
        if (brush->scale.x * 0.1>distance.length()){

            ///single bone skinning
            if (sceneData->selectedActors.size()==1){
                //find bone
                for (int boneID=0; boneID<(int)skel->bones.size();boneID++)
                    if (skel->bones[boneID]==sceneData->selectedActors[0]){
                        if (input->pressedRight)
                            eraseSingleSkin(pID,boneID);                       //erase weights with this bone
                        else
                            singleSkin(pID,boneID);                       //skin particles with this bone
                    }
            }
        }//end particles in brushrange

    }//end all particles
}

void SkinTool::save(){

		SkeletalActor* skel=brush->drawing;
		sceneData->spriteMeshLoader->saveSpriteMesh("resources/meshes/"+skel->vboMeshID+".spriteMesh",skel);

		//open my.library and append this mesh!
		TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
		myElement->SetAttribute("meshID",skel->vboMeshID);
		myElement->SetAttribute("meshFilename","resources/meshes/"+skel->vboMeshID+".spriteMesh");
		sceneData->addToLibrary(myElement);
		free(myElement);
}


void SkinTool::singleSkin(int pID,int boneID){

    MeshData * myData = sceneData->vboList[brush->drawing->vboMeshID];
    //find distance from brush
    Vector3f bLoc = brush->location;
    Vector3f pLoc=Vector3f(myData->vData[pID].location.x,myData->vData[pID].location.y,myData->vData[pID].location.z) + brush->drawing->location;
    float bSize=brush->scale.x * 2;

    float distance=fabs( (pLoc-bLoc).length() );

    //cout << "distance: " << distance << endl;
    //cout << "bSize: " << bSize << endl;

    //depending on brush size, fade intensity to edge of brush
    float intensity=max( (bSize-distance), 0.0f);
    //intensity=0.1;

    // see if this particle already has weights painted on
    for (int i=0;i<4;i++){
        //if already painted in slot
        if (myData->vData[pID].vertexWeights[i]>0.0){
            //see if it was us
            if ((int)myData->vData[pID].boneReferences[i]==boneID){
                //and add to painting
                myData->vData[pID].vertexWeights[i]+=intensity;
                //we're done
                //cout << " adding in slot: " << i << " with ID: " << boneID << "and intensity: " << intensity << endl;

                return;
            }
        }else{
        //noone has painted in this slot before
        //claim the slot
        myData->vData[pID].boneReferences[i]=boneID;
        //add our intensity
        myData->vData[pID].vertexWeights[i]=intensity;
        //we're done

        cout << "painting in slot: " << i << " with ID: " << boneID << "and intensity: " << intensity << endl;
        return;
        }
    }

    cout << "all 4 slots are taken! " << endl;
}

void SkinTool::eraseSingleSkin(int pID, int boneID){


    MeshData * myData = sceneData->vboList[brush->drawing->vboMeshID];

    //find distance from brush
    Vector3f bLoc = brush->location;
    Vector3f pLoc = Vector3f(myData->vData[pID].location.x,myData->vData[pID].location.y,myData->vData[pID].location.z) + brush->drawing->location;
    float bSize=sceneData->brush->scale.x * 2;

    float distance=fabs( (pLoc-bLoc).length() );

    //depending on brush size, fade intensity to edge of brush
    float intensity=max( (bSize-distance), 0.0f);

    // see if this particle already has weights painted on

    //someone already painted on this one
    //see if it's us
    for (int i=0;i<4;i++){
        //if already painted in slot
        if (myData->vData[pID].vertexWeights[i]>0.0){
            //see if it was us
            if ((int)myData->vData[pID].boneReferences[i]==boneID){
                //and add to painting
                myData->vData[pID].vertexWeights[i]-=intensity;
                myData->vData[pID].vertexWeights[i]=max(0.0f, myData->vData[pID].vertexWeights[i]);
                if (myData->vData[pID].vertexWeights[i]==0.0)
                    myData->vData[pID].boneReferences[i]=-1;
                                //we're done
                return;
            }

        }
    }

    cout << "no data to erase! " << endl;

}

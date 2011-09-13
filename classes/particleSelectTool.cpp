

#include "particleSelectTool.h"
#include "renderer.h"
#include "input.h"

ParticleSelectTool::ParticleSelectTool(){

}

ParticleSelectTool::~ParticleSelectTool(){}

void ParticleSelectTool::setup(){

  DrawTool::setup();
}

void ParticleSelectTool::start(){

    MsbTool::start();

    input->bKeepSelection=true;

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    highlightButton("Select Particles");

    cout << "now selecting particles!" << endl;

	//use selectedActor as drawing
	if (sceneData->selectedActors.size()>0){
		SkeletalActor* skel=dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
		if (skel){
			brush->drawing=skel;
		}
	}

	//use already assigned drawing
    if (brush->drawing){
        brush->bHidden=false;
    	brush->drawing->drawType=DRAW_PARTICLES;
    }
	//no useable drawing found
    else{
        sceneData->makeWarningPopUp("OOPS! \n \nNo Drawing to Select Particles in. Create or Select a Drawing first!", myBtn);
        cout << "no drawing!" << endl;
        input->bTextInput=false;
    }

    if (brush->selectedData.size()>0){

        MeshData* myData=sceneData->vboList[brush->drawing->vboMeshID];

        for (int i=0; i<(int)brush->selectedData.size(); i++){
            myData->vData[brush->selectedData[i]].color=Vector4f(1,0,0,1);
        }

    }

}

void ParticleSelectTool::stop(){


    MsbTool::stop();

    input->bKeepSelection=false;
    glutSetCursor(GLUT_CURSOR_INHERIT);
    brush->bHidden=true;

    if (brush->drawing){
        MeshData* myData=sceneData->vboList[brush->drawing->vboMeshID];

        for (int i=0; i<(int)brush->selectedData.size(); i++){
                myData->vData[brush->selectedData[i]].color=brush->selectedOldColors[i];
        }
    }

}

void ParticleSelectTool::keyReleased(int key){

}


void ParticleSelectTool::paint(){

   if (fabs(input->mouseVector.length())==0.0)
        return;

    input->bKeepSelection=true;

    SkeletalActor* drawing =dynamic_cast<SkeletalActor*>(brush->drawing);
    if (!drawing){
        cout << "drawing cannot be selected" << endl;
        return;
        }


    calcLoc=brush->location-drawing->location;
    MeshData* myData=sceneData->vboList[drawing->vboMeshID];

//go through all particles and see
    for (int i=0;i<(int)myData->vData.size();i++){
        //compute the distance to brush
        Vector3f distance=Vector3f(myData->vData[i].location.x,myData->vData[i].location.y,myData->vData[i].location.z)-calcLoc;

        if ( brush->scale.x>distance.length() ){

            cout << "selecting..." << endl;

            //first one gets a free ride!
            if (brush->selectedData.size()==0){
                brush->selectedData.push_back(i);
                brush->selectedOldColors.push_back(myData->vData[i].color);
                myData->vData[i].color=Vector4f(1.0,0.0,0.0,1.0);
            }
            bool bAlreadySelected=false;

            //see if we already selected this one
            for (int j=0;j<(int)brush->selectedData.size();j++){
                if (brush->selectedData[j]==i){
                    bAlreadySelected=true;
                    //cout << "already selected!" << brush->selectedData.size() << endl;
                }
            }

            if (!bAlreadySelected){
                brush->selectedData.push_back(i);
                brush->selectedOldColors.push_back(myData->vData[i].color);
                //cout << "selecting: " << brush->selectedData.size() << endl;
                //cout << "storing color: " << brush->selectedOldColors.size() << " " <<brush->selectedOldColors.back() << endl;
                myData->vData[i].color=Vector4f(1.0,0.0,0.0,1.0);
            }
        }
    }

}

void ParticleSelectTool::erase(){

   if (fabs(input->mouseVector.length())==0.0)
        return;

    input->bKeepSelection=true;

    SkeletalActor* drawing =dynamic_cast<SkeletalActor*>(brush->drawing);
    if (!drawing){
        cout << "drawing cannot be selected" << endl;
        return;
        }


    calcLoc=brush->location-drawing->location;
    MeshData* myData=sceneData->vboList[drawing->vboMeshID];

//go through all particles and see
    for (int i=0;i<(int)myData->vData.size();i++){
        //compute the distance to brush
        Vector3f distance=Vector3f(myData->vData[i].location.x,myData->vData[i].location.y,myData->vData[i].location.z)-calcLoc;

        if ( brush->scale.x>distance.length() ){

            cout << "selecting..." << endl;

            //first one gets a free ride!
            bool bAlreadySelected=false;

            //see if we already selected this one
            for (int j=0;j<(int)brush->selectedData.size();j++){
                if (brush->selectedData[j]==i){
                    bAlreadySelected=true;
                    myData->vData[i].color=brush->selectedOldColors[j];
                    brush->selectedData.erase(brush->selectedData.begin()+j);
                    brush->selectedOldColors.erase(brush->selectedOldColors.begin()+j);
                    j--;
                }
            }
        }
    }

}


#include "drawTool.h"
#include "renderer.h"
#include "input.h"
#include "sceneData.h"
#include "skeletalActor.h"
#include "spriteMeshLoader.h"
#include "drawingWidget.h"
#include "brushFilter.h"


DrawTool::DrawTool(){

    brush=NULL;
    bMouseControlled=true;
    bDrawing=false;
	bStartStroke=true;
    pNormal=Vector3f(0,1,0);

    highlightBtn="Draw Particles (p)";
}

DrawTool::~DrawTool(){}

void DrawTool::setup(){

    MsbTool::setup();
    brush=sceneData->brush;
    filters.push_back(new BrushFilter(this));
}

void DrawTool::start(){

    MsbTool::start();


    highlightButton(highlightBtn);

    //if we have an actor selcted that is a drawing, use that as our new drawing
    if (sceneData->selectedActors.size()>0 && !sceneData->specialSelected){
        SkeletalActor* skel = dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
        if (skel){
            brush->drawing=skel;
            sceneData->specialSelected=skel;
        }
    }

    //switch drawing to particleMode
	if (sceneData->specialSelected){
		sceneData->specialSelected->drawType=DRAW_PARTICLES; //important!
		sceneData->specialSelected->sceneShaderID="drawing";   //also important!
		cout << "already have drawing and switched to Particle draw mode!!!!!" << endl;
	}



    if (brush->drawing)
        for (int i=0;i<(int)brush->drawing->bones.size();i++)
            brush->drawing->bones[i]->bPickable=false;

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    brush->bHidden=false;

}

void DrawTool::stop(){

    MsbTool::stop();
    brush->bHidden=true;

    lowlightButton();

    if (brush->drawing){
        brush->drawing->bPickable=true;
        brush->drawing->bZTest=true;
        brush->drawing->bZWrite=true;
    }
    bDrawing=false;

    SkeletalActor* skel=brush->drawing;

    if (!skel) return;

    for (int i=0;i<(int)skel->bones.size();i++)
        skel->bones[i]->bPickable=false;

	glutSetCursor(GLUT_CURSOR_INHERIT);


}

void DrawTool::keyPressed(int key){

    MsbTool::keyPressed(key);

}

void DrawTool::keyReleased(int key){

    MsbTool::keyReleased(key);

}

void DrawTool::mousePressed(int btn){

    MsbTool::mousePressed(btn);

    if (!input->hudTarget && brush->drawing){
        bDrawing=true;
    }else{
        bDrawing=false;
    }


}

void DrawTool::mouseReleased(int btn){

    MsbTool::mouseReleased(btn);
/*
    if (brush->drawing){
        brush->drawing->bPickable=true;
        brush->drawing->bZTest=true;
        brush->drawing->bZWrite=true;
    }
*/
    bDrawing=false;
	bStartStroke=true;

    selectActors(btn, input->worldTarget);
}

void DrawTool::mouseDragged(int btn){

    MsbTool::mouseDragged(btn);
}


void DrawTool::selectActors(int btn, Actor* other){

    ///selection stuff

    //we can special-select any drawing
    //we create a new drawing if we have none selected


    SkeletalActor* skel=dynamic_cast<SkeletalActor*>(other);

    //figure out if we released on a drawing
    //don't use that drawing as our new drawing if we already have one...
    if (skel){
        if (brush->drawing)
            return;
        input->deselectActors();
        sceneData->specialSelected=skel;
        brush->drawing=skel;
        skel->drawType=DRAW_PARTICLES;
        skel->sceneShaderID="drawing";   //also important!
    }
    else{

        //if no drawing present
        //means that we want to create a new drawing!
        if (!brush->drawing){
            sceneData->makeUserPopUp("Name your new drawing:", myBtn);
        }
    }

    bPressLeft=false;
    bPressRight=false;

}

void DrawTool::update(double deltaTime){

    MsbTool::update(deltaTime);

    brush->setLocation(input->mouse3D);

    if (brush->drawing){
        brush->drawing->bPickable=true;
        brush->drawing->bZTest=true;
        brush->drawing->bZWrite=true;

    }

    if (bDrawing){
        if (bPressLeft)
            paint();
        if (bPressRight)
            erase();
    }
}

void DrawTool::paint(){

    if (fabs(input->mouseVector.length())==0.0)
        return;

    calcLocation();
    vertexData myVData;
	vertexData oldVData;

	int mySize=sceneData->vboList[brush->drawing->vboMeshID]->vData.size();
	if (mySize>0)
		oldVData=sceneData->vboList[brush->drawing->vboMeshID]->vData[mySize-1];

    brush->drawing->bPickable=false;
    brush->drawing->bZTest=false;
    //brush->drawing->bZWrite=false;

    for (int i=0;i<(int)filters.size();i++){
        filters[i]->filter(&myVData);
    }
	if (!sceneData->brush->bMouseControlled)
		myVData.normal=sceneData->brush->pNormal;

    myVData.vertexID=sceneData->vboList[brush->drawing->vboMeshID]->vData.size();

    sceneData->vboList[brush->drawing->vboMeshID]->vData.push_back(myVData);

    //count particles
    sceneData->numParticles++;
}

void DrawTool::erase(){

    cout << "erasing!" << endl;

    calcLoc=brush->location-brush->drawing->location;

    for (int i=0;i<(int)sceneData->vboList[brush->drawing->vboMeshID]->vData.size();i++)
      {
      Vector4f loc=sceneData->vboList[brush->drawing->vboMeshID]->vData[i].location;
      Vector3f distance=calcLoc - Vector3f(loc.x,loc.y,loc.z);
      if (brush->scale.x * 0.1>distance.length())
          brush->drawing->deleteParticle(i);
      }
    sceneData->numParticles--;
}

void DrawTool::save(){

		SkeletalActor* skel=brush->drawing;
		sceneData->spriteMeshLoader->saveSpriteMesh("resources/meshes/"+skel->vboMeshID+".spriteMesh",skel);

		//open my.library and append this mesh!
		TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
		myElement->SetAttribute("meshID",skel->vboMeshID);
		myElement->SetAttribute("meshFilename","resources/meshes/"+skel->vboMeshID+".spriteMesh");
		sceneData->addToLibrary(myElement);
}

void DrawTool::scaleZ(float factor){

    MeshData* myMesh;
    if (brush->drawing)
        myMesh=sceneData->vboList[brush->drawing->vboMeshID];

    if (brush->drawing){
        for (int i=0;i<(int)myMesh->vData.size();i++){
            myMesh->vData[i].location.z*=factor;
        }
    }
}

void DrawTool::selectParticles(){


    calcLoc=brush->location-brush->drawing->location;

    MeshData* myData=sceneData->vboList[brush->drawing->vboMeshID];
//go through all particles and see
    for (int i=0;i<(int)myData->vData.size();i++){
        //compute the distance to brush
        Vector3f distance=Vector3f(myData->vData[i].location.x,myData->vData[i].location.y,myData->vData[i].location.z)-calcLoc;

        if ( brush->scale.x>distance.length() ){

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
                    cout << "already selected!" << brush->selectedData.size() << endl;
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

void DrawTool::deselectAllParticles(){

 //deselect!
    for (int i=0;i<(int)brush->selectedData.size();i++){
        sceneData->vboList[brush->drawing->vboMeshID]->vData[brush->selectedData[i]].color=brush->selectedOldColors[i];
    }
    brush->selectedData.clear();
    brush->selectedOldColors.clear();
}

void DrawTool::mergeDrawings(){

    //merge drawings here...
    cout << "merging selected drawings..." << endl;

    //check if we have only particleSystems selected!
    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
        ParticleSystem* myPS=dynamic_cast<ParticleSystem*>(sceneData->selectedActors[i]);
        if (!myPS){
            cout << "wrong actors selected! Couldn't merge, sorry." << endl;
            return;
        }
    }

    //copy particles over
    ParticleSystem* receivePS;
    ParticleSystem* readPS;
    receivePS=dynamic_cast<ParticleSystem*>(sceneData->selectedActors[0]);

    for (int i=1;i<(int)sceneData->selectedActors.size();i++){
        readPS=dynamic_cast<ParticleSystem*>(sceneData->selectedActors[i]);
        //copy values
        MeshData * readMesh = sceneData->vboList[readPS->vboMeshID];
        MeshData * receiveMesh = sceneData->vboList[receivePS->vboMeshID];
        for (int i=0;i<(int)readMesh->vData.size();i++){
            receiveMesh->vData.push_back(readMesh->vData[i]);

            //calculate offset between particleSystems
            Vector4f readLoc=readMesh->vData[i].location;
            //taking into account that we encode scale in the 4th location value
            float myScale=readLoc.w;
            readLoc.w=1.0;
            Vector4f locationOffset= readPS->baseMatrix * readLoc;
            locationOffset=receivePS->baseMatrix.inverse() * locationOffset;
            receiveMesh->vData[receiveMesh->vData.size()-1].location=locationOffset;
            receiveMesh->vData[receiveMesh->vData.size()-1].location.w=myScale;
        }
    }
}

void DrawTool::clearDrawing(){

    sceneData->vboList[brush->drawing->vboMeshID]->vData.clear();
 //   for (int i=0;i<(int)sceneData->vboList[brush->drawing->vboMeshID]->vData.size();i++){
 //       brush->drawing->deleteParticle(i);
 //   }
}

void DrawTool::flipNormals(){

    MeshData* myData=sceneData->vboList[brush->drawing->vboMeshID];

    for (int i=0;i<(int)brush->selectedData.size();i++){
        myData->vData[brush->selectedData[i]].normal=-myData->vData[brush->selectedData[i]].normal;
    }

}

void DrawTool::calcLocation(){

    Matrix4f invmDrawing;
    Matrix4f mResult;
    Vector4f calc;


    invmDrawing=brush->drawing->baseMatrix.inverse();     //todo: use a faster inverse() function
    mResult=invmDrawing * brush->baseMatrix ;

    calc = mResult * Vector4f(0,0,0,1);
    calcLoc = Vector3f(calc.x/calc.w , calc.y/calc.w , calc.z/calc.w);

}



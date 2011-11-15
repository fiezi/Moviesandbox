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

    //always start mousecontrolled!
    bMouseControlled=true;
    //highlightButton(highlightBtn);

    //if we have an actor selcted that is a drawing, use that as our new drawing
    if (sceneData->selectedActors.size()>0){
        SkeletalActor* skel = dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
        if (skel){
            brush->drawing=skel;
            //switch drawing to particleMode
            skel->drawType=DRAW_PARTICLES; //important!
            skel->sceneShaderID="color";   //also important!
            cout << "already have drawing and switched to Particle draw mode!!!!!" << endl;
        }
    }


    //auto-generate drawing when we don't have one selected!
    if (!brush->drawing){
        createNewDrawing(true);
    }


    if (brush->drawing){
        brush->drawing->resetBones();
        for (int i=0;i<(int)brush->drawing->bones.size();i++)
            brush->drawing->bones[i]->bPickable=false;
    }

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    brush->bHidden=false;

}

void DrawTool::stop(){

    MsbTool::stop();
    save();
    brush->bHidden=true;

    bDrawing=false;

    SkeletalActor* skel=brush->drawing;


    if (!skel) return;
    int mySize=sceneData->vboList[skel->vboMeshID]->vData.size();

	if (mySize>0 && skel->bones.size()>0){
	    save();

        for (int i=0;i<(int)skel->bones.size();i++)
            skel->bones[i]->bPickable=false;
    }

    //forget our drawing!
    brush->drawing=NULL;

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

    //we cannot select a drawing! Either we create one or we are out of luck!

    return;

    //we can special-select any drawing
    //we create a new drawing if we have none selected


    SkeletalActor* skel=dynamic_cast<SkeletalActor*>(other);

    //figure out if we released on a drawing
    //don't use that drawing as our new drawing if we already have one...
    if (skel){
        if (brush->drawing)
            return;
        input->deselectActors();
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

    int buffer=(renderer->frames%BRUSHBUFFER);
    brushLocBuffer[buffer]=input->mouse3D;
    Vector3f bufferedBrushLoc;
    for (int i=0;i<BRUSHBUFFER;i++){
        bufferedBrushLoc+=brushLocBuffer[i]/BRUSHBUFFER;
    }


    if (bMouseControlled){
        //buffer brush location!
        brush->setLocation(bufferedBrushLoc);

    }


    if (bDrawing){
        if (bPressLeft)
            paint();
        if (bPressRight)
            erase();
    }



    if (brush->drawing && !bDrawing){
        brush->drawing->bPickable=true;
        brush->drawing->bZTest=true;
        brush->drawing->bZWrite=true;

    }
}

void DrawTool::paint(){

    brush->drawing->bPickable=false;
    brush->drawing->bZTest=true;
    brush->drawing->bZWrite=false;

    if (fabs(input->mouseVector.length())==0.0)
        return;

    if (input->worldTarget==brush->drawing)
        return;

    calcLocation();

    vertexData myVData;
	vertexData oldVData;

	int mySize=sceneData->vboList[brush->drawing->vboMeshID]->vData.size();
	if (mySize>0)
		oldVData=sceneData->vboList[brush->drawing->vboMeshID]->vData[mySize-1];

    //brush->drawing->bPickable=false;
    //brush->drawing->bZTest=false;

    for (int i=0;i<(int)filters.size();i++){
        filters[i]->filter(&myVData);
    }

    myVData.vertexID=sceneData->vboList[brush->drawing->vboMeshID]->vData.size();

    sceneData->vboList[brush->drawing->vboMeshID]->vData.push_back(myVData);

    //count particles
    sceneData->numParticles++;

    //adjust bounding box

    brush->drawing->lowerLeftBack.x=min(brush->drawing->lowerLeftBack.x,(brush->drawing->baseMatrix * myVData.location).x);
    brush->drawing->lowerLeftBack.y=min(brush->drawing->lowerLeftBack.y,(brush->drawing->baseMatrix * myVData.location).y);
    brush->drawing->lowerLeftBack.z=min(brush->drawing->lowerLeftBack.z,(brush->drawing->baseMatrix * myVData.location).z);

    brush->drawing->upperRightFront.x=max(brush->drawing->upperRightFront.x,(brush->drawing->baseMatrix * myVData.location).x);
    brush->drawing->upperRightFront.y=max(brush->drawing->upperRightFront.y,(brush->drawing->baseMatrix * myVData.location).y);
    brush->drawing->upperRightFront.z=max(brush->drawing->upperRightFront.z,(brush->drawing->baseMatrix * myVData.location).z);

}

void DrawTool::erase(){

    cout << "erasing!" << endl;

    //calcLoc=brush->location-brush->drawing->location;
    calcLocation();

    for (int i=0;i<(int)sceneData->vboList[brush->drawing->vboMeshID]->vData.size();i++)
      {
      Vector4f loc=sceneData->vboList[brush->drawing->vboMeshID]->vData[i].location;
      Vector3f distance=calcLoc - Vector3f(loc.x,loc.y,loc.z);
      if (brush->scale.x * 0.25>distance.length())
          brush->drawing->deleteParticle(i);
      }
    sceneData->numParticles--;
}

//we have a separate directory for untitled drawings!
void DrawTool::save(){

		SkeletalActor* skel=brush->drawing;
		string myPath="";
		if (skel->vboMeshID.find("untitled")!=string::npos && (int)skel->vboMeshID.find("untitled")==0){
            myPath+="untitled/";
		}

		sceneData->spriteMeshLoader->saveSpriteMesh(sceneData->startProject+"/"+myPath+skel->vboMeshID+".spriteMesh",skel);

		//open my.library and append this mesh!
		TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
		myElement->SetAttribute("meshID",skel->vboMeshID);
		myElement->SetAttribute("meshFilename",myPath+skel->vboMeshID+".spriteMesh");
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

void DrawTool::createNewDrawing(bool bUnnamed){

    //setup drawing
    brush->drawing = (SkeletalActor*)brush->spawn("13SkeletalActor");
    brush->drawing->drawType=DRAW_PARTICLES;
    brush->drawing->sceneShaderID="drawing";
    brush->drawing->setLocation(input->lastMouse3D);
    brush->drawing->update(0.0);
    cout << "New Drawing: " << brush->drawing->location << endl;
    brush->drawing->controller=sceneData->controller;

    if (bUnnamed){
        string myName="untitled";
        char buffer [16];
        cout << "amount untitled drawings: " << sceneData->numberOfUntitledDrawings << endl;
        sprintf(buffer,"%d",sceneData->numberOfUntitledDrawings);
        //itoa(, ,10);
        myName=myName+buffer;
        brush->drawing->name=myName;
        sceneData->numberOfUntitledDrawings++;
    }else{
        brush->drawing->name=input->inputText;
    }

    sceneData->vboList[brush->drawing->name]=new MeshData;
    brush->drawing->vboMeshID=brush->drawing->name;
    cout << "new drawing name: " << brush->drawing->name << endl;
    brush->drawing->setup();

    //also quite important - we want to select the drawing we create!
    brush->drawing->bSelected=true;
    sceneData->selectedActors.push_back(brush->drawing);

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



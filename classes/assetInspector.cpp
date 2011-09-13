

#include "assetInspector.h"
#include "renderer.h"
#include "input.h"
#include "importBitmapButton.h"
#include "assignButton.h"
#include "colladaLoader.h"
#include "spriteMeshLoader.h"

AssetInspector::AssetInspector(){

    listColumns=4;
    level=0;
    listDisplaySize=300;
    textureID="icon_props";
    listOffsetY=64;
    bKinectToolOpen=false;
}

AssetInspector::~AssetInspector(){}


void AssetInspector::setup(){

    Inspector::setup();
    tabs.clear();
    tabs.push_back( new MeshTab(this) );
    tabs.push_back( new TextureTab(this) );
    tabs.push_back( new ActionTab(this) );
    tabs.push_back( new PrefabTab(this) );

    tabs[currentTab]->assembleList();
}

void AssetInspector::createInspectorButtons(){

/// Import
    BasicButton* importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+30.0f,location.y+20, 0.0f));
    importButton->name="import";
    importButton->color=Vector4f(0.8,0.8,0.8,1.0);
    importButton->bDrawName=true;
    importButton->sceneShaderID="color";
    importButton->scale.x=64;
    inspectorButtons.push_back(importButton);


/// Import from Bitmap, Kinect, Milkscanner, etc...

    importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+100.0f,location.y+20, 0.0f));
    importButton->name="Import Kinect";
    importButton->tooltip="Import Kinect";
    importButton->textureID="icon_kinect";
    //importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);


    importButton= new ImportBitmapButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+140.0f,location.y+20, 0.0f));
    importButton->name="Import Bitmap";
    importButton->tooltip="Import Bitmap";
    importButton->textureID="icon_importBitmap";
    //importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);


/// Tabs

    AssignButton* tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="color";
    tabButton->name="Meshes";
    tabButton->color=Vector4f(0.7,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+100.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="color";
    tabButton->name="Textures";
    tabButton->color=Vector4f(0.7,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+170.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="color";
    tabButton->name="Actions";
    tabButton->color=Vector4f(0.6,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+240.0f,location.y, 0.0f));
    tabButton->scale.x=200;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="color";
    tabButton->name="Prefabs";
    tabButton->color=Vector4f(0.6,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

}

void AssetInspector::refreshList(){

/*
    if (listButton.size()<sceneData->vboList.size()){
        tabs[currentTab]->assembleList();
    }

*/
}


void AssetInspector::MeshTab::assembleList(){

        cout << "assembling mesh list..." << endl;
        std::map <std::string, MeshData*>::iterator it;
        int i=0;

        if (mine->scrollBar){
            mine->scrollBar->remove();
            mine->scrollBar=NULL;
        }


        /*
        for (int i=0; i<(int)mine->listButton.size();i++){
            mine->listButton[i]->bPermanent=false;
            mine->listButton[i]->deselect(0);
        }
        */

        for (int i=0; i<(int)mine->listButton.size();i++){
            mine->listButton[i]->remove();
        }


        cout << "old mesh list size : " << mine->listButton.size() << endl;

        mine->listButton.clear();

        for ( it=mine->sceneData->vboList.begin() ; it != mine->sceneData->vboList.end(); it++ ){

            if (it->second && it->first!="NULL"){

                mine->sceneData->actorInfo["12AssignButton"].actorReference->create();
                mine->listButton.push_back(mine->sceneData->buttonList.back());

                mine->listButton[i]->name="string " + it->first;
                mine->listButton[i]->tooltip= it->first;
                mine->listButton[i]->buttonProperty="VBOMESHID";
                mine->listButton[i]->textureID="icon_props";
                mine->listButton[i]->level=mine->level+1;
                mine->listButton[i]->bDrawName=true;
                mine->listButton[i]->color=Vector4f(0.75,0.75,0.75,1.0);
                mine->listButton[i]->bPermanent=true;
                mine->listButton[i]->bDragable=true;
                mine->listButton[i]->parent=mine;
                mine->listButton[i]->bDrawPrefix=false;
                mine->listButton[i]->drawNameOffset.y=mine->listHeight/2.0;
                if (mine->listWidth>0)
                    mine->listButton[i]->scale.x=mine->listWidth;

                //leave room for name beneath icon
                if (mine->listHeight>0)
                    mine->listButton[i]->scale.y=mine->listHeight-16.0;

                mine->listButton[i]->setup();
                mine->placeButton(i,i);
                //set this because we want to drag buttons around!
                mine->listButton[i]->initialLocation=mine->listButton[i]->location;

				cout << "create new mesh list button: " << mine->listButton[i]->name << endl;
                i++;
            }
        }


    cout << "our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void AssetInspector::MeshTab::trigger(MsbObject* other){

    if (other==mine->scrollBar)
        return;

    cout << "triggering..." << endl;

       for (int i=0;i<(int)mine->listButton.size();i++){
            mine->listButton[i]->color=COLOR_WHITE;
            if (other==mine->listButton[i]){
                ///set vboMeshID of worldTarget to
                if (mine->input->worldTarget){

                    if (mine->input->worldTarget!=mine->sceneData->grid && mine->input->worldTarget->name!="ground"){
                        Actor* a=mine->input->worldTarget;
                        //special case for bones! Assign to parent character if present...
                        BoneActor* b = dynamic_cast<BoneActor*>(a);
                        if (b && b->parent)
                            a=(Actor*)b->parent;

                        ///setting the vboMesh
                        a->memberFromString(&a->property["VBOMESHID"], mine->listButton[i]->name);

                        //figure out if we should change the shader too!
                        if (mine->sceneData->vboList[a->vboMeshID]->bIsSkeletal)
                            a->sceneShaderID="skeletal";
                        //now, also reset the actor
                        a->reset();
                    }else{

                        //create a skeletal actor of this type here please!
                        Actor* a=mine->spawn("13SkeletalActor");
                        a->drawType=DRAW_VBOMESH;

                        a->memberFromString(&a->property["VBOMESHID"], mine->listButton[i]->name);

                        if (mine->sceneData->vboList[a->vboMeshID]->bIsSkeletal){
                            a->sceneShaderID="skeletal";
                        }
                        a->setAbsoluteLocation(mine->input->mouse3D);
                        //now, also set up the actor
                        a->setup();
                    }
                }
                ///but always set the brushes mesh id!
                mine->sceneData->brush->memberFromString(&mine->sceneData->brush->property["VBOMESHID"], mine->listButton[i]->name);
                mine->listButton[i]->color=COLOR_RED;
            }
        }

    if (other->name=="import"){

        //TODO: also copy to project folder!

        string filename=sceneData->openFileDialog();
        //check for file ending
        size_t found;
        found=filename.find(".dae");
        if (found!=string::npos){
            //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION);
            string meshID=filename.substr(found+1);
            sceneData->colladaLoader->loadColladaMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("ColladaMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(myElement);

        }
        found=filename.find(".DAE");
        if (found!=string::npos){
            cout << "found a COLLADA mesh!" << filename << endl;
           //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION);
            string meshID=filename.substr(found+1);
            sceneData->colladaLoader->loadColladaMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("ColladaMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(myElement);
        }
        found=filename.find(".spriteMesh");
        if (found!=string::npos){
            //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION);
            string meshID=filename.substr(found+1);
            sceneData->spriteMeshLoader->loadSpriteMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(myElement);
        }
    }


}


void AssetInspector::TextureTab::assembleList(){

    for (int i=0;i<(int)mine->listButton.size();i++){
        mine->listButton[i]->remove();
    }
    mine->listButton.clear();

    if (mine->scrollBar){
        mine->scrollBar->remove();
        mine->scrollBar=NULL;
    }

        cout << "creating list..." << endl;
        std::map <std::string, textureObject*>::iterator it;
        int i=0;

        for ( it=mine->sceneData->textureList.begin() ; it != mine->sceneData->textureList.end(); it++ ){

            mine->sceneData->actorInfo["20PropertyAssignButton"].actorReference->create();
            mine->listButton.push_back(mine->sceneData->buttonList.back());

            mine->listButton[i]->name="string " + it->first;
            mine->listButton[i]->tooltip=it->first;
            mine->listButton[i]->buttonProperty="TEXTUREID";
            mine->listButton[i]->textureID="icon_base";
            mine->listButton[i]->level=mine->level+1;
            mine->listButton[i]->bDrawName=true;
            mine->listButton[i]->color=Vector4f(1,1,1,1.0);
            mine->listButton[i]->textureID=it->first;
            mine->listButton[i]->bPermanent=true;
            mine->listButton[i]->bDragable=true;

                mine->listButton[i]->bDrawPrefix=false;
                mine->listButton[i]->drawNameOffset.y=mine->listHeight/2.0;
                if (mine->listWidth>0)
                    mine->listButton[i]->scale.x=mine->listWidth;

                //leave room for name beneath icon
                if (mine->listHeight>0)
                    mine->listButton[i]->scale.y=mine->listHeight-16.0;

            mine->listButton[i]->setup();
            mine->placeButton(i,i);
            //set this because we want to drag buttons around!
            mine->listButton[i]->initialLocation=mine->listButton[i]->location;

            i++;
        }

    cout << "TextureInspector: our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void AssetInspector::TextureTab::trigger(MsbObject* other){

   if (other->name=="import"){
        string filename=mine->sceneData->openFileDialog();

        cout <<"importing..." << filename << endl;

        size_t found;
        found=filename.find(".tga");
        if (found == string::npos){
            found=filename.find(".TGA");

            if (found==string::npos){
                cout << "Not the right image format, sorry!" << endl;
                return;
            }
        }

        //generate texID
        found=filename.find_last_of(DIRECTORY_SEPARATION);
        string texID=filename.substr(found+1);

        mine->renderer->LoadTextureTGA(filename,true,true,texID);
        TiXmlElement * texElement = new TiXmlElement("Texture");
        texElement->SetAttribute("textureID",texID);
        texElement->SetAttribute("filename",filename);
        texElement->SetAttribute("bAlpha",1);
        texElement->SetAttribute("bWrap",1);
        mine->sceneData->addToLibrary(texElement);
        free(texElement);
    }
}


void AssetInspector::ActionTab::assembleList(){

        std::map <std::string, Action*>::iterator it;
        int i=0;

        for (int i=0;i<(int)mine->listButton.size();i++){
            mine->listButton[i]->remove();
        }
        mine->listButton.clear();

        if (mine->scrollBar){
            mine->scrollBar->remove();
            mine->scrollBar=NULL;
        }

        for ( it=mine->sceneData->actionList.begin() ; it != mine->sceneData->actionList.end(); it++ ){

                if (it->second){

                mine->listButton.push_back(it->second);
                mine->listButton[i]->tooltip=mine->listButton[i]->name;
                mine->listButton[i]->level=mine->level+1;
                mine->listButton[i]->bDrawName=true;
                mine->listButton[i]->color=Vector4f(1.0,1.0,0.0,1.0);
                //actions are just set to be visible, not actually created!
                mine->listButton[i]->bHidden=false;
                mine->listButton[i]->bDragable=true;

                mine->listButton[i]->drawNameOffset.y=mine->listHeight/2.0;
                if (mine->listWidth>0)
                    mine->listButton[i]->scale.x=mine->listWidth;

                //leave room for name beneath icon
                if (mine->listHeight>0)
                    mine->listButton[i]->scale.y=mine->listHeight-16.0;

                mine->placeButton(i,i);
                //set this because we want to drag buttons around!
                mine->listButton[i]->initialLocation=mine->listButton[i]->location;
                i++;
                }else{
                cout << "problem with:" << it->first << endl;
                }
            }

    cout << "ActionInspector! our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;


}

void AssetInspector::ActionTab::trigger(MsbObject* other){

    //TODO: implement Action importing...

    if (other->name=="import"){
        string fileName=sceneData->openFileDialog();
        cout <<"importing..." << fileName << endl;
    }
}



void AssetInspector::PrefabTab::assembleList(){

        if (mine->scrollBar){
            mine->scrollBar->remove();
            mine->scrollBar=NULL;
        }

        for (int i=0;i<(int)mine->listButton.size();i++){
            mine->listButton[i]->remove();
        }
        mine->listButton.clear();

        cout << "creating list..." << endl;

        for (int i=0;i<(int)mine->sceneData->prefabs.size();i++){
            mine->sceneData->actorInfo["12AssignButton"].actorReference->create();
            mine->listButton.push_back(mine->sceneData->buttonList.back());

            mine->listButton[i]->name=mine->sceneData->prefabs[i];
            mine->listButton[i]->textureID="icon_base";
            mine->listButton[i]->level=mine->level+1;
            mine->listButton[i]->bDrawName=true;
            mine->listButton[i]->color=Vector4f(0,0,1,1.0);
            mine->listButton[i]->bPermanent=true;
            mine->listButton[i]->bDragable=true;
            mine->listButton[i]->parent=this;
                mine->listButton[i]->drawNameOffset.y=mine->listHeight/2.0;
                if (mine->listWidth>0)
                    mine->listButton[i]->scale.x=mine->listWidth;

                //leave room for name beneath icon
                if (mine->listHeight>0)
                    mine->listButton[i]->scale.y=mine->listHeight-16.0;

            mine->listButton[i]->setup();
            mine->placeButton(i,i);
            //set this because we want to drag buttons around!
            mine->listButton[i]->initialLocation=mine->listButton[i]->location;
        }

    cout << "our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void AssetInspector::PrefabTab::trigger(MsbObject* other){}


void AssetInspector::trigger(MsbObject* other){


    if (other->name=="Meshes"){
        currentTab=0;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="Textures"){
        currentTab=1;
        tabs[currentTab]->assembleList();

    }

    if (other->name=="Actions"){
        currentTab=2;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="Prefabs"){
        currentTab=3;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="Import Kinect"){
        if (other->color==Vector4f(0.8,0.8,0,1)){
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

    Inspector::trigger(other);

}


void AssetInspector::closeKinectTool(){

    sceneData->externalInputList["kinectInput"]->stopProgram();

}

void AssetInspector::openKinectTool(){

    if (!sceneData->brush->drawing){
        //TODO: maybe make this optional?
        sceneData->drawTool->createNewDrawing(true);

        //sceneData->makeWarningPopUp("No drawing! \n please place a drawing first!", this);
        //return;
    }
    sceneData->brush->drawing->drawType=DRAW_POINTPATCH;
    sceneData->brush->drawing->bTextured=true;
    sceneData->brush->drawing->textureID="sharedMemory";
    sceneData->brush->drawing->sceneShaderID="heightfield";
    sceneData->brush->drawing->particleScale=100;

    sceneData->externalInputList["kinectInput"]->startProgram();
    inspectorButtons[1]->color=Vector4f(1,1,0,1);
    bKinectToolOpen=true;
}

void AssetInspector::importKinect(){

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

                sceneData->drawTool->paint();
            }
}

void AssetInspector::create(){sceneData->addButton(this);}
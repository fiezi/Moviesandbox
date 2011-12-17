

#include "assetInspector.h"
#include "renderer.h"
#include "input.h"
#include "importBitmapButton.h"
#include "assignButton.h"
#include "boolButton.h"
#include "colladaLoader.h"
#include "spriteMeshLoader.h"

AssetInspector::AssetInspector(){

    name="assetInspector";
    tooltip="Asset Inspector";
    listColumns=4;
    level=0;
    scrollSize=250.0;
    listDisplaySize=500;
    textureID="icon_props";
    listOffsetY=64;
    bKinectToolOpen=false;
    bShowUntitled=false;
    bDrawName=false;
    registerProperties();
}

AssetInspector::~AssetInspector(){}


void AssetInspector::registerProperties(){

    createMemberID("BSHOWUNTITLED",&bShowUntitled,this);
}

void AssetInspector::setup(){

    Inspector::setup();
    //clean up
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
    importButton->bDrawName=false;
    importButton->sceneShaderID="buttonTexture";
    importButton->textureID="icon_import";
    importButton->scale.x=32;
    importButton->setup();
    importButton->buttonColor=sceneData->meanButtonColor;
    inspectorButtons.push_back(importButton);


/// Import from Bitmap, Kinect, Milkscanner, etc...

    importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+70.0f,location.y+20, 0.0f));
    importButton->name="Import Kinect";
    importButton->tooltip="Import Kinect";
    importButton->setTextureID("icon_kinect");
    //importButton->bDrawName=true;
    importButton->setup();
    importButton->buttonColor=sceneData->meanButtonColor;

    inspectorButtons.push_back(importButton);


    importButton= new ImportBitmapButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+110.0f,location.y+20, 0.0f));
    importButton->name="Import Bitmap";
    importButton->tooltip="Import Bitmap";
    importButton->setTextureID("icon_importBitmap");
    importButton->setup();
    importButton->buttonColor=sceneData->meanButtonColor;
    inspectorButtons.push_back(importButton);

    importButton= new BoolButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+tabWidth-130,location.y+28, 0.0f));
    importButton->name="Show Untitled";
    importButton->tooltip="";
    importButton->buttonProperty="BSHOWUNTITLED";
    importButton->scale.y=12;
    importButton->scale.x=110;
    importButton->setTextureID("icon_base");
    importButton->bDrawName=true;
    importButton->sceneShaderID="buttonColor";
    importButton->setup();
    importButton->drawNameOffset=Vector2f(0,0);
    importButton->buttonColor=sceneData->selectedTabColor;
    inspectorButtons.push_back(importButton);

/// Tabs

    AssignButton* tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="buttonColor";
    tabButton->name="Meshes";
    tabButton->buttonColor=Vector4f(0.7,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    tabButton->setup();
    tabButton->buttonColor=sceneData->selectedTabColor;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+100.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="buttonColor";
    tabButton->name="Textures";
    tabButton->buttonColor=Vector4f(0.7,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    tabButton->setup();
    tabButton->buttonColor=sceneData->tabColor;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+170.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="buttonColor";
    tabButton->name="Actions";
    tabButton->buttonColor=Vector4f(0.6,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    tabButton->setup();
    tabButton->buttonColor=sceneData->tabColor;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+240.0f,location.y, 0.0f));
    tabButton->scale.x=200;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="buttonColor";
    tabButton->name="Prefabs";
    tabButton->buttonColor=Vector4f(0.6,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    tabButton->setup();
    tabButton->buttonColor=sceneData->tabColor;
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
                size_t found;
                found = (it->first).rfind("untitled");
                AssetInspector* ass = (AssetInspector*)mine;
                if (found!=string::npos && !ass->bShowUntitled)    //if our drawing has "untitled" in its name, do not show it!
                    continue;
                mine->sceneData->actorInfo["12AssignButton"].actorReference->create();
                mine->listButton.push_back(mine->sceneData->buttonList.back());

                mine->listButton[i]->name="string " + it->first;
                mine->listButton[i]->tooltip= it->first;
                mine->listButton[i]->buttonProperty="VBOMESHID";
                mine->listButton[i]->setTextureID("icon_props");
                mine->listButton[i]->level=mine->level+1;
                mine->listButton[i]->bDrawName=true;
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
                mine->listButton[i]->buttonColor=mine->sceneData->deselectedElementColor;

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
            mine->listButton[i]->buttonColor=mine->sceneData->deselectedElementColor;
            if (other==mine->listButton[i]){
                ///set vboMeshID of worldTarget to
                if (mine->input->worldTarget && !mine->input->hudTarget){

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
                mine->listButton[i]->buttonColor=mine->sceneData->selectedElementColor;
            }
        }

    if (other->name=="import"){

        //TODO: also copy to project folder!

        string filename=mine->sceneData->openFileDialog("mesh");
        //check for file ending
        size_t found;
        found=filename.find(".dae");
        if (found!=string::npos){
            //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION)+1;
            string meshID=filename.substr(found);
            found=meshID.rfind('.');
            meshID=meshID.substr(0,found);

            replace(filename.begin(), filename.end(), '\\', '/');

            mine->sceneData->colladaLoader->loadColladaMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("ColladaMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            mine->sceneData->addToLibrary(myElement);

        }
        found=filename.find(".DAE");
        if (found!=string::npos){
            cout << "found a COLLADA mesh!" << filename << endl;
           //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION)+1;
            string meshID=filename.substr(found);
            found=meshID.rfind('.');
            meshID=meshID.substr(0,found);

            replace(filename.begin(), filename.end(), '\\', '/');

            mine->sceneData->colladaLoader->loadColladaMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("ColladaMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            mine->sceneData->addToLibrary(myElement);
        }
        found=filename.find(".spriteMesh");
        if (found!=string::npos){
            //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION)+1;
            //change directory separation to something tinyXML can deal with...
            string meshID=filename.substr(found);
            found=meshID.rfind('.');
            meshID=meshID.substr(0,found);

            replace(filename.begin(), filename.end(), '\\', '/');

            mine->sceneData->spriteMeshLoader->loadSpriteMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            mine->sceneData->addToLibrary(myElement);
            assembleList();
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

            size_t found;
            found = (it->first).rfind("icon_");
            if (found!=string::npos)    //if our drawing has "icon_" in its name, do not show it!
                continue;
            mine->sceneData->actorInfo["20PropertyAssignButton"].actorReference->create();
            mine->listButton.push_back(mine->sceneData->buttonList.back());

            mine->listButton[i]->name="string " + it->first;
            mine->listButton[i]->tooltip=it->first;
            mine->listButton[i]->buttonProperty="TEXTUREID";
            mine->listButton[i]->setTextureID("icon_base");
            mine->listButton[i]->level=mine->level+1;
            mine->listButton[i]->bDrawName=true;
            mine->listButton[i]->setTextureID(it->first);
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
            mine->listButton[i]->buttonColor=COLOR_WHITE;

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
        string filename=mine->sceneData->openFileDialog("tga");

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
        found=texID.rfind('.');
        texID=texID.substr(0,found);

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
                //mine->sceneData->buttonList.push_back(it->second);
                mine->listButton[i]->tooltip=mine->listButton[i]->name;
                mine->listButton[i]->level=mine->level+1;
                mine->listButton[i]->bDrawName=true;
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
                mine->listButton[i]->setup();
                //actions are just set to be visible, not actually created!
                mine->listButton[i]->bHidden=false;
                mine->listButton[i]->buttonColor=mine->sceneData->deselectedElementColor;

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
/*
    if (other->name=="import"){
        string fileName=sceneData->openFileDialog("action");
        cout <<"importing..." << fileName << endl;
    }
*/
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
            mine->listButton[i]->setTextureID("icon_base");
            mine->listButton[i]->level=mine->level+1;
            mine->listButton[i]->bDrawName=true;
            mine->listButton[i]->buttonColor=mine->sceneData->deselectedElementColor;
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
        if (bKinectToolOpen){
            importKinect();
            ((BasicButton*)other)->buttonColor=sceneData->meanButtonColor;
            closeKinectTool();
            bKinectToolOpen=false;
        }
        else{
            openKinectTool();
        }
    }

    if (other->name=="Show Untitled" && currentTab== 0){
        cout << "switched Untitled!"<< endl;
        tabs[currentTab]->assembleList();
    }

    Inspector::trigger(other);

}


void AssetInspector::closeKinectTool(){

    sceneData->externalInputList["kinectInput"]->stopProgram();
    clickedLeft();

}

void AssetInspector::openKinectTool(bool bHighZRes){

    if (!sceneData->brush->drawing){
        sceneData->drawTool->createNewDrawing(true);
    }

    sceneData->brush->drawing->drawType=DRAW_POINTPATCH;
    sceneData->brush->drawing->bTextured=true;
    sceneData->brush->drawing->setTextureID("sharedMemory");
    sceneData->brush->drawing->sceneShaderID="kinectHeightfield";
    sceneData->brush->drawing->particleAngleScale=256;
    sceneData->brush->drawing->particleScale=12;
     sceneData->brush->drawing->setScale(Vector3f(3,3,3));
    if (!sceneData->textureList["sharedMemory"] ){
        if (bHighZRes)
            renderer->createEmptyTexture("sharedMemory",GL_RGBA, GL_FLOAT,1024,512);
        else
            renderer->createEmptyTexture("sharedMemory",GL_RGBA, GL_UNSIGNED_BYTE,1024,512);

    }

    sceneData->externalInputList["kinectInput"]->startProgram();
    inspectorButtons[1]->buttonColor=sceneData->focusButtonColor;
    bKinectToolOpen=true;
}

void AssetInspector::importKinect(bool bHighZRes){

            //TODO: this needs to change for HighRez!
            static unsigned char kinectContent[1024*512*4];
            //static float kinectContentFloat[1024*512*4];

            glBindTexture(GL_TEXTURE_2D,sceneData->textureList["sharedMemory"]->texture);
            if (bHighZRes)
                glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,&kinectContent);
            else
                glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,&kinectContent);

            glBindTexture(GL_TEXTURE_2D,0);

            sceneData->controller->switchTool(TOOL_DRAW);

            sceneData->brush->drawing->drawType=DRAW_PARTICLES;
            sceneData->brush->drawing->bTextured=false;
            sceneData->brush->drawing->setTextureID("NULL");
            sceneData->brush->drawing->sceneShaderID="color";
            sceneData->brush->drawing->bPickable=true;
            sceneData->brush->drawing->particleScale=1;
            sceneData->brush->drawing->particleAngleScale=0;
            sceneData->brush->setScale(Vector3f(1,1,1));
            Vector3f oldScale=sceneData->brush->drawing->scale;
            Matrix4f oldTransform = sceneData->brush->drawing->baseMatrix;

            sceneData->brush->drawing->baseMatrix.identity();

            for (int i=0;i<1024*512*4;i+=4){
                Vector3f myLoc;
                myLoc.z= kinectContent[i+3];
                myLoc.z/=255.0f ;

                //don't draw extremes
                if (myLoc.z==0.0f || myLoc.z> 0.99f)
                    continue;

                myLoc.z*=8.0f;
                float x= (i/4)%1024 - 512;
                float y= (int)((i/4)/1024 ) - 256;


                myLoc.x=x/512.0f;
                myLoc.y =y/512.0f;


                myLoc.x*=myLoc.z * 1.5;
                myLoc.y*=myLoc.z * 1.5;


                sceneData->brush->setLocation(myLoc);
                sceneData->brush->color=Vector4f(kinectContent[i]/255.0,kinectContent[i+1]/255.0,kinectContent[i+2]/255.0,1.0);
                input->mouseVector.x=1.0f;

                //only draw the 640x480 that we actually  get from the kinect!
                if (x>-320 && x<320 && y>-240 && y<240 )
                    sceneData->drawTool->paint();
            }
            sceneData->brush->drawing->baseMatrix=oldTransform;
            sceneData->brush->drawing->bPickable=true;
            sceneData->brush->drawing->bZTest=true;
            sceneData->brush->drawing->bZWrite=true;

            sceneData->spriteMeshLoader->createVBOs(sceneData->brush->drawing->vboMeshID,false);

            sceneData->controller->switchTool(TOOL_SELECT);

}

void AssetInspector::create(){sceneData->addButton(this);}

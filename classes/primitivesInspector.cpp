

#include "primitivesInspector.h"
#include "renderer.h"
#include "input.h"
#include "assignButton.h"
#include "colladaLoader.h"
#include "spriteMeshLoader.h"

PrimitivesInspector::PrimitivesInspector(){

    listColumns=4;
    level=0;
    scrollSize=250.0;
    listDisplaySize=400;
    textureID="icon_props";
    listOffsetY=64;
}

PrimitivesInspector::~PrimitivesInspector(){}


void PrimitivesInspector::setup(){

    Inspector::setup();
    tabs.clear();
    tabs.push_back( new PrimitivesTab(this) );
    tabs.push_back( new NodeTab(this) );

    tabs[currentTab]->assembleList();

}

void PrimitivesInspector::createInspectorButtons(){

    AssignButton* tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="buttonColor";
    tabButton->name="Primitives";
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
    tabButton->name="Nodes";
    tabButton->bDrawName=true;
    tabButton->setup();
    tabButton->buttonColor=sceneData->tabColor;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);


}

void PrimitivesInspector::refreshList(){

/*
    if (listButton.size()<sceneData->vboList.size()){
        tabs[currentTab]->assembleList();
    }

*/
}


void PrimitivesInspector::PrimitivesTab::assembleList(){

    for (int i=0;i<(int)mine->listButton.size();i++){
        mine->listButton[i]->remove();
    }
    mine->listButton.clear();
    mine->listName.clear();
    mine->listProp.clear();
    mine->listIcon.clear();

    if (mine->scrollBar){
        mine->scrollBar->remove();
        mine->scrollBar=NULL;
    }

        cout << "creating list..." << endl;


    //a plane
    //a character
    //a camera
    //a sprite
    //a light

    mine->listName.push_back("Character");
    mine->listProp.push_back("9Character");
    mine->listIcon.push_back("icon_character");

    mine->listName.push_back("Camera");
    mine->listProp.push_back("11CameraActor");
    mine->listIcon.push_back("icon_camera");

    mine->listName.push_back("Plane");
    mine->listProp.push_back("13SkeletalActor");
    mine->listIcon.push_back("icon_plane");

    mine->listName.push_back("Cube");
    mine->listProp.push_back("13SkeletalActor");
    mine->listIcon.push_back("icon_cube");

    mine->listName.push_back("Sprite");
    mine->listProp.push_back("13SkeletalActor");
    mine->listIcon.push_back("icon_target");

    mine->listName.push_back("PointPatch");
    mine->listProp.push_back("13SkeletalActor");
    mine->listIcon.push_back("icon_pointPatch");

    mine->listName.push_back("ParticleSpray");
    mine->listProp.push_back("13ParticleSpray");
    mine->listIcon.push_back("icon_pointPatch");


//--- test video actor
#ifdef TARGET_WIN32
    mine->listName.push_back("VideoTexture");
    mine->listProp.push_back("17VideoTextureActor");
    mine->listIcon.push_back("icon_texture");
#endif

    mine->listName.push_back("Light");
    mine->listProp.push_back("8MsbLight");
    mine->listIcon.push_back("icon_light");

    for (int i=0; i<(int)mine->listProp.size(); i++){
        mine->sceneData->actorInfo["17CreateActorButton"].actorReference->create();
        mine->listButton.push_back(mine->sceneData->buttonList.back());


        mine->listButton[i]->name=mine->listName[i];
        mine->listButton[i]->buttonProperty=mine->listProp[i];
        mine->listButton[i]->setTextureID(mine->listIcon[i]);

        mine->listButton[i]->level=mine->level+1;
        mine->listButton[i]->bDrawName=true;
        mine->listButton[i]->color=Vector4f(0.8,0.8,0.8,1.0);
        mine->listButton[i]->bPermanent=true;
        mine->listButton[i]->bDragable=true;
        mine->listButton[i]->parent=mine;

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

    cout << "PrimitivesInspector: our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void PrimitivesInspector::PrimitivesTab::trigger(MsbObject* other){

    if (other->name=="Plane"){
        Actor* a=mine->sceneData->actorList.back();
        a->drawType=DRAW_PLANE;
    }

    if (other->name=="Cube"){
        Actor* a=mine->sceneData->actorList.back();
        a->drawType=DRAW_CUBE;
    }

    if (other->name=="PointPatch"){
        Actor* a=mine->sceneData->actorList.back();
        a->drawType=DRAW_POINTPATCH;
        a->bTextured=true;
        a->sceneShaderID="heightfield";
        a->particleAngleScale=100;
    }

if (other->name=="ParticleSpray"){
        Actor* a=mine->sceneData->actorList.back();
        a->drawType=DRAW_PARTICLES;
        a->bTextured=false;
        a->sceneShaderID="color";
        a->particleScale=10;
    }

    if (other->name=="Sprite"){
        Actor* a=mine->sceneData->actorList.back();
        a->drawType=DRAW_SPRITE;
        a->particleScale=10.0;
    }

}

void PrimitivesInspector::NodeTab::assembleList(){


    for (int i=0;i<(int)mine->listButton.size();i++){
        mine->listButton[i]->remove();
    }
    mine->listButton.clear();
    mine->listName.clear();
    mine->listProp.clear();
    mine->listIcon.clear();

    if (mine->scrollBar){
        mine->scrollBar->remove();
        mine->scrollBar=NULL;
    }

        cout << "creating list..." << endl;

    mine->listName.push_back("RootNode");
    mine->listName.push_back("WaitTime");
    mine->listName.push_back("WalkTo");
    mine->listName.push_back("KeyInput");
    mine->listName.push_back("SwitchCamera");
    mine->listName.push_back("SetAnimation");
    mine->listName.push_back("StopAnim");
    mine->listName.push_back("WaitForEvent");
    mine->listName.push_back("TriggerEvent");
    mine->listName.push_back("ConsoleCommand");
    mine->listName.push_back("Interpolate");
    mine->listName.push_back("SetWalkTarget");
    mine->listName.push_back("SetHidden");
    mine->listName.push_back("TriggerActor");
    mine->listName.push_back("AttachTo");
    mine->listName.push_back("PerformAction");
    mine->listName.push_back("SetProperty");
    mine->listName.push_back("SetBackground");
    mine->listName.push_back("MorphSpriteMesh");
    mine->listName.push_back("load new");

    mine->listProp.push_back("8RootNode");
    mine->listProp.push_back("16WaitForTimerNode");
    mine->listProp.push_back("10WalkToNode");
    mine->listProp.push_back("12KeyInputNode");
    mine->listProp.push_back("16SwitchCameraNode");
    mine->listProp.push_back("16SetAnimationNode");
    mine->listProp.push_back("12StopAnimNode");
    mine->listProp.push_back("16WaitForEventNode");
    mine->listProp.push_back("16TriggerEventNode");
    mine->listProp.push_back("18ConsoleCommandNode");
    mine->listProp.push_back("15InterpolateNode");
    mine->listProp.push_back("17SetWalkTargetNode");
    mine->listProp.push_back("13SetHiddenNode");
    mine->listProp.push_back("16TriggerActorNode");
    mine->listProp.push_back("12AttachToNode");
    mine->listProp.push_back("17PerformActionNode");
    mine->listProp.push_back("15SetPropertyNode");
    mine->listProp.push_back("17SetBackgroundNode");
    mine->listProp.push_back("19MorphSpriteMeshNode");
    mine->listProp.push_back("8LoadNode");

    int i;
    for (i=0; i<(int)mine->listProp.size(); i++){
        mine->sceneData->actorInfo["16CreateNodeButton"].actorReference->create();
        mine->listButton.push_back(mine->sceneData->buttonList.back());


        mine->listButton[i]->name=mine->listName[i];
        mine->listButton[i]->buttonProperty=mine->listProp[i];
        mine->listButton[i]->setTextureID("icon_nodeMenu");

        mine->listButton[i]->level=mine->level+1;
        mine->listButton[i]->bDrawName=true;
        mine->listButton[i]->tooltip=mine->listName[i];
        mine->listButton[i]->color=Vector4f(1,1,1,1.0);
        mine->listButton[i]->bPermanent=true;
        mine->listButton[i]->bDragable=true;

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

    //add udpInput as non-node at the end
    mine->listName.push_back("UDPInput");
    mine->listProp.push_back("8UdpInput");
    mine->sceneData->actorInfo["17CreateActorButton"].actorReference->create();
    mine->listButton.push_back(mine->sceneData->buttonList.back());


        mine->listButton[i]->name=mine->listName[i];
        mine->listButton[i]->buttonProperty=mine->listProp[i];
        mine->listButton[i]->setTextureID("icon_base");

        mine->listButton[i]->level=mine->level+1;
        mine->listButton[i]->bDrawName=true;
        mine->listButton[i]->color=Vector4f(1,1,1,1.0);
        mine->listButton[i]->bPermanent=true;
        mine->listButton[i]->bDragable=true;

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



    cout << "NodeInspector: our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void PrimitivesInspector::NodeTab::trigger(MsbObject* other){

}

void PrimitivesInspector::trigger(MsbObject* other){

    if (other->name=="Primitives"){
        currentTab=0;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="Nodes"){
        currentTab=1;
        tabs[currentTab]->assembleList();
    }

    Inspector::trigger(other);
}

void PrimitivesInspector::create(){sceneData->addButton(this);}

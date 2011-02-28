#include "content.h"
#include "input.h"

#include "drawingWidget.h"
#include "boneWidget.h"
#include "propsWidget.h"

#include "listButton.h"
#include "sliderButton.h"
#include "rotateButton.h"
#include "moveButton.h"
#include "saveButton.h"

#include "console.h"

#include "particleSystem.h"
#include "physicsActor.h"
#include "ragDoll.h"
#include "flexGrid.h"
#include "interpolationHelper.h"

#include "createActorButton.h"

#include "node.h"
#include "rootNode.h"
#include "waitForTimerNode.h"
#include "textInputButton.h"
#include "createNodeButton.h"


#include "propertyInspector.h"
#include "textureInspector.h"
#include "meshInspector.h"
#include "actionInspector.h"
#include "timelineInspector.h"
#include "prefabInspector.h"
#include "brushInspector.h"

#include "layerInspector.h"

#include "udpInput.h"
#include "pilot.h"
#include "action.h"


#include "spriteCharacter.h"

void Content::setup(){

    renderer=Renderer::getInstance();
    input=Input::getInstance();

    createMonitors();

    createConsole();

    //offset for left hand side menu
    int yPos=10;
    int xPos=10;

    createSelectButton(xPos,yPos);

    yPos+=30;

    createNavButton(xPos,yPos);

    yPos+=30;

    createGridButton(xPos,yPos);

    yPos+=30;

    createPropList(xPos,yPos);

    yPos+=30;

    createDrawWidget(xPos,yPos);

    yPos+=30;

    createBoneWidget(xPos,yPos);

    yPos+=40;

    createNodeList(xPos,yPos);

    yPos+=40;

    createSysMenu(xPos,yPos);

    createInspectors();


 }

void Content::createConsole(){

    BasicButton *but;

    //console
    but= new Console;
    but->setLocation(Vector3f( 0, input->screenY-but->scale.y, 0));
    but->bTextured=false;
    but->sceneShaderID="color";
    but->setup();
    input->console=(Console*)but;
    renderer->buttonList.push_back(but);
}

void Content::createMonitors(){

        BasicButton* monitor;

    //*************************************************
    // Shader Test
    //*************************************************




    monitor=new BasicButton;
    /*
    monitor->location.x=360.0f;
    monitor->location.y=700.0f;
    monitor->scale.x=800.0;
    monitor->scale.y=-600.0;
    */

    monitor->location.x=0.0f;
    monitor->location.y=renderer->screenY;
    monitor->scale.x=renderer->screenX;
    monitor->scale.y=-renderer->screenY;

    monitor->textureID="sceneTexture";
    monitor->name="sfx";
    monitor->sceneShaderID="post";
    monitor->sceneShaderID="texture";
    renderer->postOverlay=monitor;
    monitor->bScreenOverlay=true;

}



void Content::createSelectButton(int x, int y){

    BasicButton *but;

    but= new AssignButton;
    but->location.x=x;
    but->location.y=y;
    but->name="select";
    but->tooltip="select (i)";
    but->setLocation(but->location);
    but->textureID="icon_select";
    but->color=COLOR_RED;
    but->setup();
    but->parent=input->controller;
    input->controller->myTools[TOOL_SELECT]->myBtn=but;
    renderer->buttonList.push_back(but);
}

void Content::createNavButton(int x, int y){

    BasicButton *but;

    but= new AssignButton;
    but->location.x=x;
    but->location.y=y;
    but->name="navigate";
    but->tooltip="navigate (TAB)";
    but->setLocation(but->location);
    but->textureID="icon_fly";
    but->setup();
    but->parent=input->controller;
    input->controller->myTools[TOOL_NAV]->myBtn=but;
    renderer->buttonList.push_back(but);

}


void Content::createGridButton(int x, int y){


    BasicButton *but;

    but= new AssignButton;
    but->name="grid";
    but->tooltip="grid (g)";
    but->location.x=x;
    but->location.y=y;
    but->setLocation(but->location);
    but->textureID="icon_grid";
    but->setup();
    but->parent=input->controller;
    input->controller->myTools[TOOL_GRID]->myBtn=but;
    renderer->buttonList.push_back(but);

}

void Content::createPropList(int x, int y){

        //PropList
    ListButton *lbut;

    lbut= new PropsWidget;
    lbut->location.x=x;
    lbut->location.y=y;
    lbut->setLocation(lbut->location);
    //lbut->listWidth=100;

    lbut->textureID="icon_props";
    lbut->name="propsMenu";

    input->controller->myTools[TOOL_SPAWN]->myBtn=lbut;
    input->controller->myTools[TOOL_SPAWN]->highlightBtn="Spawn Meshes";
    //populate propList

    lbut->listType.push_back("17CreateActorButton");
    lbut->listName.push_back("Place Mesh");
    lbut->listProp.push_back("13SkeletalActor");
    lbut->listIcon.push_back("icon_props");

 ///spawn meshes!
    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Spawn Meshes");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_drawMesh");



    lbut->listType.push_back("17CreateActorButton");
    lbut->listName.push_back("place Character");
    lbut->listProp.push_back("9Character");
    lbut->listIcon.push_back("icon_character");


    lbut->listType.push_back("17CreateActorButton");
    lbut->listName.push_back("place camera");
    lbut->listProp.push_back("11CameraActor");
    lbut->listIcon.push_back("icon_camera");

//--- test video actor
    lbut->listType.push_back("17CreateActorButton");
    lbut->listName.push_back("place bone");
    lbut->listProp.push_back("9BoneActor");
    lbut->listIcon.push_back("icon_target");

    lbut->listType.push_back("17CreateActorButton");
    lbut->listName.push_back("place light");
    lbut->listProp.push_back("8MsbLight");
    lbut->listIcon.push_back("icon_light");

    lbut->setup();
    renderer->buttonList.push_back(lbut);
}

void Content::createDrawWidget(int x, int y){

    ListButton* lbut;
    //DRAWING
    lbut= new DrawingWidget;
    lbut->location.x=x;
    lbut->location.y=y;
    lbut->setLocation(lbut->location);
    lbut->textureID="icon_brush";

    //assign Widget to corresponding tools
    input->controller->myTools[TOOL_DRAW]->myBtn=lbut;
    input->controller->myTools[TOOL_CALLIGRAPHY]->myBtn=lbut;
    input->controller->myTools[TOOL_PARTICLESELECT]->myBtn=lbut;
    lbut->setup();
    renderer->buttonList.push_back(lbut);


    lbut->listType.push_back("15PickWorldButton");
    lbut->listName.push_back("New Drawing");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_new");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Draw Particles (p)");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_paint");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Draw Calligraphy");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_paintFancy");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Select Particles");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_selectDrawing");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Import Kinect");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_kinect");

    lbut->listType.push_back("18ImportBitmapButton");
    lbut->listName.push_back("Import Bitmap");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_importBitmap");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("merge Drawings");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_mergeDrawings");


    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("scaleZ x2");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_grow");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("scaleZ x0.5");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_shrink");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("save");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_save");

    lbut->listType.push_back("15TextInputButton");
    lbut->bDrawName=false;
    lbut->listName.push_back("save As...");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_saveAs");
}

void Content::createBoneWidget(int x, int y){

    ListButton* lbut;
    //DRAWING
    lbut= new BoneWidget;
    lbut->location.x=x;
    lbut->location.y=y;
    lbut->setLocation(lbut->location);
    lbut->textureID="icon_character";
    input->controller->myTools[TOOL_SKIN]->myBtn=lbut;
    input->controller->myTools[TOOL_BONE]->myBtn=lbut;
    lbut->setup();
    renderer->buttonList.push_back(lbut);

    //this way, we create new bones by clicking!
    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Create Bone (b)");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_addBones");

    //here, we skin bones
    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Paint Weights");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_paintWeights");

    //here we load a bone set
    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Load Bones");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_load");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("save");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_save");

    //here we convert to a VBO
    lbut->listType.push_back("15TextInputButton");
    lbut->listName.push_back("save As...");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_saveAs");
    /*
    lbut->listType.push_back("22SaveDrawSkeletonButton");
    lbut->listName.push_back("Load Skeleton");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_save");
    */
}

void Content::createNodeList(int x, int y){

    ListButton* lbut;
    //NODELIST
    lbut= new Widget;
    lbut->location.x=x;
    lbut->location.y=y;
    lbut->setLocation(lbut->location);
    lbut->listWidth=110;
    lbut->listHeight=20;
    lbut->listColor=Vector4f(0.7,0.7,0.7,1.0);

    lbut->maxListItems=10;
    lbut->bDrawListNames=true;
    lbut->textureID="icon_nodeMenu";
    lbut->name="nodeMenu";

    //populate nodeList
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("16CreateNodeButton");
    lbut->listType.push_back("17CreateActorButton");

    lbut->listName.push_back("RootNode");
    lbut->listName.push_back("WaitTime");
    lbut->listName.push_back("WalkTo");
    lbut->listName.push_back("KeyInput");
    lbut->listName.push_back("SwitchCamera");
    lbut->listName.push_back("SetAnimation");
    lbut->listName.push_back("StopAnim");
    lbut->listName.push_back("WaitForEvent");
    lbut->listName.push_back("TriggerEvent");
    lbut->listName.push_back("ConsoleCommand");
    lbut->listName.push_back("Interpolate");
    lbut->listName.push_back("SetWalkTarget");
    lbut->listName.push_back("SetHidden");
    lbut->listName.push_back("TriggerActor");
    lbut->listName.push_back("AttachTo");
    lbut->listName.push_back("PerformAction");
    lbut->listName.push_back("SetProperty");
    lbut->listName.push_back("SetBackground");
    lbut->listName.push_back("MorphSpriteMesh");
    lbut->listName.push_back("place UDPInput");

    lbut->listProp.push_back("8RootNode");
    lbut->listProp.push_back("16WaitForTimerNode");
    lbut->listProp.push_back("10WalkToNode");
    lbut->listProp.push_back("12KeyInputNode");
    lbut->listProp.push_back("16SwitchCameraNode");
    lbut->listProp.push_back("16SetAnimationNode");
    lbut->listProp.push_back("12StopAnimNode");
    lbut->listProp.push_back("16WaitForEventNode");
    lbut->listProp.push_back("16TriggerEventNode");
    lbut->listProp.push_back("18ConsoleCommandNode");
    lbut->listProp.push_back("15InterpolateNode");
    lbut->listProp.push_back("17SetWalkTargetNode");
    lbut->listProp.push_back("13SetHiddenNode");
    lbut->listProp.push_back("16TriggerActorNode");
    lbut->listProp.push_back("12AttachToNode");
    lbut->listProp.push_back("17PerformActionNode");
    lbut->listProp.push_back("15SetPropertyNode");
    lbut->listProp.push_back("17SetBackgroundNode");
    lbut->listProp.push_back("19MorphSpriteMeshNode");
    lbut->listProp.push_back("8UdpInput");


    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");
    lbut->listIcon.push_back("icon_flat");

    renderer->buttonList.push_back(lbut);
}

void Content::createSysMenu(int x, int y){

    ListButton* lbut;
    //SYSTEM
    lbut= new Widget;
    lbut->location.x=x;
    lbut->location.y=y;
    lbut->setLocation(lbut->location);
    lbut->textureID="icon_system";
    lbut->name="sysMenu";

    lbut->listType.push_back("10SaveButton");
    lbut->listName.push_back("save");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_save");

    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("load");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_load");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("exit");
    lbut->listProp.push_back("NULL");
    lbut->listIcon.push_back("icon_exit");
    lbut->setup();
    renderer->buttonList.push_back(lbut);
}

void Content::createInspectors(){

    ListButton* lbut;

    lbut= new PropertyInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->location.y=0;
    lbut->name="propertyInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();

    lbut= new BrushInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->location.y=30;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_brush";
    lbut->name="brushInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();

    lbut= new TextureInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->location.y=60;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_texture";
    lbut->name="textureInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();

    lbut= new MeshInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->location.y=90;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_props";
    lbut->name="meshInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();


    lbut= new ActionInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->location.y=120;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_ball";
    lbut->name="actionInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();

    lbut= new PrefabInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->location.y=150;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_prefab";
    lbut->name="prefabInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();

    lbut= new LayerInspector;
    lbut->location.x=renderer->screenX-30;
    lbut->location.y=180;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_layers";
    lbut->name="layerInspector";

    renderer->buttonList.push_back(lbut);
    lbut->setup();


    lbut= new TimelineInspector;
    lbut->location.x=200;
    lbut->location.y=renderer->screenY -lbut->scale.y;
    lbut->setLocation(lbut->location);
    lbut->color=Vector4f(1.0,0.6,0.1,1.0);
    lbut->textureID="icon_timeline";
    lbut->name="TimelineInspector";

    input->controller->timeline=(TimelineInspector*)lbut;
    renderer->buttonList.push_back(lbut);
    lbut->setup();
}


void Content::createActorContent(){



    // not in stable branch!
    return;

    Renderer* renderer=Renderer::getInstance();
    Input* input=Input::getInstance();

    SkeletalActor* drawing;
    drawing = (SkeletalActor*)renderer->grid->spawn("13SkeletalActor");

    drawing->name="kinectTest";
    drawing->drawType=DRAW_PARTICLES;
    drawing->sceneShaderID="drawing";
    drawing->setLocation(Vector3f(0,0,0));
    cout << "New Drawing: " << drawing->location << endl;
    drawing->controller=input->controller;
    //drawing->texture=renderer->textureList[1];
    drawing->vboMeshID="kinectTest";
    drawing->setRotation(Vector3f(0,180,90));
    drawing->setLocation(Vector3f(4,12,0));
   // drawing->myData=new MeshData;
    renderer->vboList["kinectTest"]=new MeshData;
    renderer->vboList["kinectTest"]->boneCount=0;

    for (int row=0;row<240;row++){
        for (int col=0;col<320;col++){

            //next, fill drawing with 640*480 particles
            vertexData myVData;
            myVData.location=Vector4f((float)row * 0.04f,(float)col * 0.04f,0.0,0.01f);
            myVData.color=Vector4f(1.0,0.0,0.0,1.0);

            //standard bone references - nothing (set to -1)
            myVData.boneReferences=Vector4f(-1.0,-1.0,-1.0,-1.0);
            myVData.vertexWeights=Vector4f(0.0,0.0,0.0,0.0);;

            myVData.texCoord=Vector3f(0.0,0.0,0.0);     //texCoord holds color
            myVData.birth=float(glutGet(GLUT_ELAPSED_TIME))/1000.0; //particle creation time in seconds


            myVData.normal=Vector3f(0,0,1);
            myVData.secondaryColor=Vector3f(0,1,0);
            renderer->vboList[drawing->vboMeshID]->vData.push_back(myVData);
        }
    }
    //drawing->setup();

}

void Content::reset(){

renderer->actorList.clear();
renderer->buttonList.clear();
setup();
}

#include "sceneData.h"
#include "input.h"
#include "renderer.h"


//loaders and the like
#include "colladaLoader.h"
#include "spriteMeshLoader.h"
#include "spriteMeshLoaderXML.h"

//actors
#include "particleSystem.h"
#include "physicsActor.h"
#include "ragDoll.h"
#include "skeletalActor.h"
#include "hitBox.h"
#include "cameraActor.h"
#include "spriteCharacter.h"
#include "boneActor.h"
#include "skeletalHead.h"

#include "character.h"
#include "characterController.h"

#ifdef TARGET_WIN32
    #include "videoTextureActor.h"
#endif

#include "msbLight.h"

//#include "videoTextureActor.h"

//buttons
#include "console.h"
#include "createActorButton.h"
#include "createNodeButton.h"
#include "moveButton.h"
#include "rotateButton.h"
#include "setBaseButton.h"
#include "listButton.h"
#include "sliderButton.h"
#include "saveButton.h"
#include "loadButton.h"
#include "userPopUp.h"

#include "drawingWidget.h"
#include "importBitmapButton.h"
#include "loadBrushButton.h"
#include "loadDrawingButton.h"
#include "loadDrawSkeletonButton.h"

#include "assignButton.h"
#include "propertyAssignButton.h"
#include "textInputButton.h"
#include "colorPickButton.h"
#include "pickWorldButton.h"
#include "vectorInputButton.h"

#include "boolButton.h"

//static buttons for windows and messages and stuff
#include "createPrefabButton.h"
#include "createActionButton.h"



//inspectors
#include "inspector.h"
#include "propertyInspector.h"
#include "textureInspector.h"
#include "actionInspector.h"
#include "timelineInspector.h"
#include "meshInspector.h"
#include "prefabInspector.h"
#include "brushInspector.h"

#include "timelineButton.h"

#include "layerInspector.h"

#include "windowButton.h"
//communication
#include "udpInput.h"

//nodes
#include "node.h"
#include "rootNode.h"
#include "waitForTimerNode.h"
#include "interpolateNode.h"
#include "keyInputNode.h"
#include "switchCameraNode.h"
#include "performActionNode.h"
#include "setPropertyNode.h"
#include "walkToNode.h"
#include "setAnimationNode.h"
#include "stopAnimNode.h"
#include "waitForEventNode.h"
#include "triggerEventNode.h"
#include "consoleCommandNode.h"
#include "setWalkTargetNode.h"
#include "setHiddenNode.h"
#include "triggerActorNode.h"
#include "attachToNode.h"
#include "setBackgroundNode.h"
#include "morphSpriteMeshNode.h"
#include "loadNode.h"
#include "layer.h"

//animation
#include "action.h"
#include "particleSpray.h"


//SceneData
#include "meshData.h"

#ifdef TARGET_WIN32

    #include <Commdlg.h>
    #include <stdio.h>
    #include <conio.h>
    #include <tchar.h>

    TCHAR szName[]=TEXT("Global\\MyFileMappingObject");

#else

	#include <sys/mman.h>

#endif

#define BUF_SIZE 640*480*4*32


//static link
SceneData* SceneData::sceneDataInstance=NULL;

//************************************************************
//
//RTTY actorInfo - Information about every Actor:
//                 Class Name, Type, Size
//
//************************************************************


void SceneData::fillGlobalLists(){



    //actors
    createActorID(new Actor);
    //createActorID(new VideoTextureActor);
    createActorID(new ParticleSystem);
    createActorID(new ParticleSpray);
    createActorID(new PhysicsActor);
    createActorID(new RagDoll);
    createActorID(new SkeletalActor);
    createActorID(new SkeletalHead);
    createActorID(new Brush);
    createActorID(new CameraActor);
    createActorID(new HitBox);
    createActorID(new SpriteCharacter);
    createActorID(new BoneActor);

    createActorID(new Character);
    createActorID(new CharacterController);

    createActorID(new MsbLight);

    #ifdef TARGET_WIN32
    createActorID(new VideoTextureActor);
    #endif

    ////buttons
    createActorID(new BasicButton);
    createActorID(new Layer);
    createActorID(new Console);
    createActorID(new CreateActorButton);
    createActorID(new CreateNodeButton);

    createActorID(new ListButton);
    createActorID(new SliderButton);

    createActorID(new SaveButton);
    createActorID(new LoadButton);

    createActorID(new MoveButton);
    createActorID(new RotateButton);
    createActorID(new SetBaseButton);

    createActorID(new ImportBitmapButton);
    createActorID(new UserPopUp);

    createActorID(new LoadBrushButton);

    createActorID(new LoadDrawingButton);
    createActorID(new LoadDrawSkeletonButton);
    createActorID(new DrawingWidget);

    createActorID(new PropertyAssignButton);
    createActorID(new TextInputButton);
    createActorID(new AssignButton);

    createActorID(new BoolButton);
    createActorID(new VectorInputButton);

    createActorID(new ColorPickButton);
    createActorID(new PickWorldButton);
    createActorID(new CreatePrefabButton);
    createActorID(new CreateActionButton);

    createActorID(new UdpInput);


    //inspectors
    createActorID(new Inspector);
    createActorID(new PropertyInspector);
    createActorID(new TextureInspector);
    createActorID(new MeshInspector);
    createActorID(new ActionInspector);
    createActorID(new PrefabInspector);
    createActorID(new BrushInspector);

    createActorID(new TimelineInspector);
    createActorID(new TimelineButton);

    createActorID(new LayerInspector);

    createActorID(new WindowButton);

    //nodes and actions
    createActorID(new Action);

    createActorID(new Node);
    createActorID(new RootNode);
    createActorID(new KeyInputNode);
    createActorID(new WaitForTimerNode);
    createActorID(new WalkToNode);
    createActorID(new InterpolateNode);
    createActorID(new SwitchCameraNode);
    createActorID(new PerformActionNode);
    createActorID(new SetPropertyNode);
    createActorID(new SetAnimationNode);
    createActorID(new StopAnimNode);
    createActorID(new WaitForEventNode);
    createActorID(new TriggerEventNode);
    createActorID(new ConsoleCommandNode);
    createActorID(new SetWalkTargetNode);
    createActorID(new SetHiddenNode);
    createActorID(new TriggerActorNode);
    createActorID(new AttachToNode);
    createActorID(new SetBackgroundNode);
    createActorID(new MorphSpriteMeshNode);
    createActorID(new LoadNode);
}


//************************************************************
//
//Constructor and Destructor
//
//************************************************************

SceneData::SceneData(){


    name="SceneManager";

    currentLayer=0;
    startProject="NULL";
    startSceneFilename="NULL";
    currentShader="color";                //currently bound shader

    backgroundTex="NULL";
    backgroundColor=Vector4f(0.25,0.5,0.8,1);

    grid=NULL;                        //direct pointer to Grid
    brush=NULL;                       //direct pointer to Brush

    drawTool=NULL;
    navTool=NULL;
    gridTool=NULL;

	frames=0;
    currentTime=0.0;
	deltaTime=0.0;
    physicsTime=0.0;                 //time for physics step (sometimes deltaTime is too small to advance physics...)

        //Control stuff
    mouseSensitivity=0.005;
    moveSpeed=0.1;

    content=NULL;                   //the list of things we create at program start
    colladaLoader=NULL;       //helper object for loading meshes and animations
    spriteMeshLoader=NULL; //helper Object for loading sprite meshes from Base64
    spriteMeshLoaderXML=NULL; //helper Object for loading old sprite meshes from XML

    controller=NULL;
    console=NULL;

    inspectorManager=NULL;
    actorMenu=NULL;
    staticButton=NULL;    //in case we want to access Button functionality without actually displaying a Button

    gridSize=0.00010f;                      //grid!
    debugText="Moviesandbox 2.0";
    numParticles=0;
    invertMouse=-1;

    numberOfUntitledDrawings=0;

}

SceneData::~SceneData(){}

SceneData* SceneData::getInstance(){

    if (sceneDataInstance)
        return sceneDataInstance;
    else{
        sceneDataInstance=new SceneData;
        return sceneDataInstance;
        }
}


void SceneData::setup(){

    renderer=Renderer::getInstance();
    input=Input::getInstance();

    //do this first, so we don't get an error when initialising our physicsActor in the global list!
	renderer->physicsSetup();

        //generate Class and Type Lists
    fillGlobalLists();

    colladaLoader=new ColladaLoader();
    spriteMeshLoader=new SpriteMeshLoader();
    spriteMeshLoaderXML=new SpriteMeshLoaderXML();

    inspectorManager=new InspectorManager;
	controller=new Control;

}

// load render settings
void SceneData::loadPreferences(){

#ifdef TARGET_MACOSX
	//switch to working directory!!
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);

	string myPath=path;
	//get rid of Moviesandbox.app at the end!
	myPath.erase(myPath.end()-16, myPath.end() );

	chdir(myPath.c_str());
#endif

    //open config xml file
    //configure renderer
    cout << "Loading Config file" <<endl;

    TiXmlDocument doc( "config.xml" );
    if (!doc.LoadFile()) {
        cout << "Cannot find config file, or config file corrupt. Exiting..." << endl;
        exit(0);
        }


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Get the "Moviesandbox" element
    //***********************************************************************
    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //now load the configuration
    cout << "loading render settings" << endl;

    element=hRoot.FirstChild( "Rendersettings" ).Element();

    int val=0;
    double dVal=0.0;
    string mStr;

    //resolution
    element->Attribute("WindowSizeX", &val);
    renderer->windowX=val;
    element->Attribute("WindowSizeY", &val);
    renderer->windowY=val;

    //renderscreen
    element->Attribute("ScreenSizeX", &val);
    renderer->screenX=val;
    element->Attribute("ScreenSizeY", &val);
    renderer->screenY=val;

    //fullscreen on/off
    element->Attribute("bFullScreen", &val);
    renderer->bFullscreen=bool(val);

    backgroundTex=element->Attribute("BackgroundTex");


    //light drawing on/off
    mStr=element->Attribute("bDrawLighting", &val);
    renderer->bDrawLighting=bool(val);

    //stereo Render on/off
    mStr=element->Attribute("bRenderStereo", &val);
    renderer->bRenderStereo=bool(val);

    //multisampling on/off
    mStr=element->Attribute("bMultisample", &val);
    renderer->bMultisample=bool(val);

    element->Attribute("numSamples", &val);
    renderer->numSamples=val;

    //rendertarget texture resolutions
    element->Attribute("ShadowSize", &val);
    renderer->shadow_size=val;

    element->Attribute("LightingSize", &val);
    renderer->lighting_size=val;

    element->Attribute("SceneSize", &val);
    renderer->scene_size=val;


    element->Attribute("MouseSensitivity", &dVal);
    mouseSensitivity=dVal;
    element->Attribute("MoveSpeed", &dVal);
    moveSpeed=dVal;

    element->Attribute("FOV", &dVal);
    renderer->fov=dVal;

    //setting start scene
    startProject=element->Attribute("StartProject");

    startSceneFilename=element->Attribute("StartSceneFile");

    //setting external Program connections
    element=hRoot.FirstChild( "ExternalInput" ).Element();
    while(element){
        string myTaskName=element->Attribute("taskName");

        //for now, just windows and OSX
#ifdef TARGET_WIN32
        string myTaskLocation=element->Attribute("filenameWin32");
#else
        string myTaskLocation=element->Attribute("filenameOSX");
#endif

        externalInputList[myTaskName]=new externalInputData;
        externalInputList[myTaskName]->taskName=myTaskName;
        externalInputList[myTaskName]->filename=myTaskLocation;
        element=element->NextSiblingElement("ExternalInput");
    }


    //setting library
    element=hRoot.FirstChild( "Library" ).Element();
    library=element->Attribute("Library");

    //check for my.project and create if not exists

    TiXmlDocument myLib( startProject + "/" + "my.project" );

    if (!myLib.LoadFile()){

        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        myLib.LinkEndChild( decl );
        TiXmlElement * root = new TiXmlElement( "Moviesandbox" );

        TiXmlElement * untitled = new TiXmlElement ("untitled");
        untitled->SetAttribute("amount", 0);
        root->LinkEndChild(untitled);
        myLib.LinkEndChild( root );
        myLib.SaveFile( startProject + "/" + "my.project");

        #ifdef TARGET_WIN32
        string myName=startProject + "/" +"untitled";
        CreateDirectory(myName.c_str(),NULL);
        #endif

        #ifdef TARGET_MACOSX
        #endif

    }


}


//************************************************************
//
//  Setting up and calling all Actors' update function
//
//************************************************************

void SceneData::createScene(){


   //create base layer
    addLayer("baseLayer");

    FreeImage_Initialise();

    //add Brush
    addBrush();

    //add one grid - allow for adding more!
    addGrid();

    input->setup();          //controller gets created here!
    controller->setup();

    //set convenience pointers to drawTool and navTool in sceneData:
    navTool=(NavTool*)controller->myTools[TOOL_NAV];
    drawTool=(DrawTool*)controller->myTools[TOOL_DRAW];
    gridTool=(GridTool*)controller->myTools[TOOL_GRID];

    //setting up menu
    cout << "setting up menu" << endl;
    content= new Content;
    content->setup();

    //load library stuff
    getAllPrefabs();

    //load basic library
    loadMeshes("resources/meshes/",library);
    loadTextures("resources/icons/",library);
    loadShaders("shaders/",library);
    loadActionList("resources/actions/",library);

    //load project library
    loadMeshes(startProject+"/",startProject+"/my.project");          //this one also sets the number of untitled drawings!
    loadTextures(startProject+"/",startProject+"/my.project");
    loadShaders(startProject+"/",startProject+"/my.project");
    loadActionList(startProject+"/",startProject+"/my.project");


    //then load scene
    cout << "loading basic stuff..." << endl;
    loadAll(startSceneFilename, false);
    cout << "finished loading basic stuff" << endl;


    //now set up custom actors from content
    content->createActorContent();

    //shared memory texture
    renderer->setup();
}

void SceneData::update(float deltaTime){


    readSharedMemory();           //convert shared memory to a texture...

	//first update Nodes!
	for (unsigned int i=0;i<buttonList.size();i++){
		Node* myNode=dynamic_cast<Node*>(buttonList[i]);
		NodeIO* myNodeIO=dynamic_cast<NodeIO*>(buttonList[i]);
		if (myNode || myNodeIO){
            if (renderer->bDrawNodes){
                buttonList[i]->update(deltaTime);
            }
		}
		else{
			buttonList[i]->update(deltaTime);
		}
	}

	//then update Actors!
	for (int i=0;i<(int)actorList.size();i++){
        actorList[i]->objectID=(float)i;
        actorList[i]->update(deltaTime);
	}

	//then helpers - we are using these for brush and grid and stuff
	for (int i=0;i<(int)helperList.size();i++){
        helperList[i]->update(deltaTime);
	}

    //for actor menu animation
    if (actorMenu)
        actorMenu->update(deltaTime);

	input->update(deltaTime);

    //always enable moving around
    if (input->bPressedMovementKeys && controller->tool!=TOOL_NAV)
        controller->myTools[TOOL_NAV]->update(deltaTime);

    //update controller after assigning all hudTargets, staticButtons, etc...
    controller->update(deltaTime);

    renderer->update(deltaTime);

	input->resetInputVectors();


    glutPostRedisplay();
}



void SceneData::addLayer(string layerName){

        Layer* lay = new Layer;
        lay->setup();
        lay->name=layerName;
        lay->sceneTextureID=lay->name+"_Scene";
        lay->colorTextureID=lay->name+"_Color";
        lay->depthTextureID=lay->name+"_Depth";
        lay->pickTextureID=lay->name+"_Pick";
		lay->lightDataTextureID=lay->name+"_lightData";

        layerList.push_back(lay);
        renderer->createFBO(&(lay->sceneFBO), &(lay->sceneTex), NULL, renderer->scene_size, false, lay->sceneTextureID);
        renderer->createFBO(&(lay->colorFBO), &(lay->colorTex), NULL, renderer->scene_size, false, lay->colorTextureID);
        renderer->createFBO(&(lay->depthFBO), &(lay->depthTex), NULL, renderer->scene_size, false, lay->depthTextureID);
        renderer->createFBO(&(lay->pickFBO),  &(lay->pickTex),  NULL, renderer->scene_size, false, lay->pickTextureID);
		renderer->createFBO(&(lay->lightDataFBO),  &(lay->lightDataTex),  NULL, renderer->scene_size, false, lay->lightDataTextureID);
        currentLayer=layerList.size()-1;

        cout << "Added new Layer:" << layerName << endl;

}

void SceneData::addGrid(){

    grid= new Actor;
    grid->scale=Vector3f(50,50,50);
    grid->texScale=Vector3f(10,10,10);
    grid->setLocation(Vector3f(0,25,0));
    grid->setRotation(Vector3f(0,0,0));

    grid->sceneShaderID="texture";

    grid->bTextured=true;
    grid->bUseShader=true;
    grid->bComputeLight=true;
    grid->bHidden=true;
    grid->color=Vector4f(0.5,0.5,0.5,0.8);
    grid->textureID="grid_solid";
    grid->name="grid";

    grid->drawType=DRAW_PLANE;
    grid->objectID=-2.0f;
	grid->bRemoveable=false;

    helperList.push_back(grid);

 }

 void SceneData::addBrush(){

 //brush creation code here
    brush = new Brush;
    brush->drawing=NULL;
    brush->bPickable=false;
    brush->name="brush";
    brush->controller=controller;
    brush->bHidden=true;

    brush->menuType.empty();
    helperList.push_back(brush);
}



int SceneData::readSharedMemory(){

#ifdef TARGET_WIN32

   HANDLE hMapFile;
   LPCTSTR pBuf;

   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object


   if (hMapFile == NULL)
   {
     // printf(TEXT("Could not open file mapping object (%d).\n"),(int)GetLastError());
      return 1;
   }

   pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_READ,  // read/write permission
               0,
               0,
               BUF_SIZE);

   if (pBuf != NULL)
   {
        renderer->copyMemoryToTexture((void*)pBuf,"sharedMemory", 640,480);

        UnmapViewOfFile((void*)pBuf);
        CloseHandle(hMapFile);

        return 1;
   }else{

      CloseHandle(hMapFile);
   }
#else

	//attach shared memory file
	int fd = open("/tmp/msbRamFile", O_RDWR);
	if(fd<0){
		//cout << "Could not open '/tmp/msbRamFile'"<<endl;
		return 0;
	}

	// load the file into memory, shared, read & write access
	void* sourcebuffer = mmap( 0, BUF_SIZE, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);
	if(!sourcebuffer){
		cout << "Could not mmap '%s.vga'"<< endl;
		return 0;
	}

	// once the file is mapped, we can dispose of the filehandle
	close(fd);

	renderer->copyMemoryToTexture((void*)sourcebuffer,"sharedMemory", 640,480);

	if (sourcebuffer)
		munmap(sourcebuffer, BUF_SIZE);

	return 1;
#endif
   return 0;


}


void SceneData::updateView(){

    navTool->processRotation(0.0);
    navTool->processMove(0.0);

}


//Button related stuff

void SceneData::createActorMenu(){

	cout << "Creating radial menu for: " << input->worldTarget->name << endl;

    Actor* myActor=input->worldTarget;

	//make actor menu if we don't have one, else clean it up
	if (actorMenu)
		actorMenu->deselect(0);
	else
		actorMenu=new ListButton;

	actorMenu->setLocation( Vector3f( input->mouseX, input->mouseY, 0.0 ) );
	actorMenu->listDisplayMode=2;
	actorMenu->listType.assign(myActor->menuType.begin(),myActor->menuType.end());
	actorMenu->listName.assign(myActor->menuName.begin(),myActor->menuName.end());
	actorMenu->listIcon.assign(myActor->menuIcon.begin(),myActor->menuIcon.end());
	actorMenu->listProp.assign(myActor->menuProp.begin(),myActor->menuProp.end());
	actorMenu->parent=myActor;
	actorMenu->clickedLeft();
}


void SceneData::makeUserPopUp(string text, Actor* parent){

    staticButton=(BasicButton*)actorInfo["9UserPopUp"].actorReference;
    ((UserPopUp*)staticButton)->bWaitForInput=true;
    staticButton->setLocation(Vector3f(renderer->screenX/2-200,renderer->screenY/2-50,0));
    staticButton->color=Vector4f(0.0,0.0,1.0,1.0);
    staticButton->name=text;
    staticButton->parent=parent;
    cout << "UserInput->parent is: " << parent->name << endl;
    staticButton->clickedLeft();
    buttonList.push_back(staticButton);
    input->lastMouse3D=input->mouse3D;
}

void SceneData::makeWarningPopUp(string message, Actor* parent){

    staticButton=(BasicButton*)actorInfo["9UserPopUp"].actorReference;
    staticButton->setLocation(Vector3f(renderer->screenX/2-200,renderer->screenY/2-50,0));
    staticButton->color=Vector4f(1.0,0.0,0.0,1.0);
    staticButton->name=message;
    staticButton->parent=parent;
    cout << "UserInput->parent is: " << parent->name << endl;
    //staticButton->clickedLeft();
    buttonList.push_back(staticButton);
}

//**************************************************************************************
// loading

//TODO: load textures/meshes/stuff seperately or save it always too!
void SceneData::saveAll(std::string filename){

    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    TiXmlElement * root = new TiXmlElement( "Moviesandbox" );
    doc.LinkEndChild( root );

    //save controller location and rotation
    TiXmlElement * controlElement=new TiXmlElement("Controller");
    controlElement->SetAttribute("TransformMatrix",controller->memberToString(&controller->property["TRANSFORMMATRIX"]));
    root->LinkEndChild(controlElement);

    //first Actors
    for (unsigned int i=0;i<actorList.size();i++)
      {
      TiXmlElement * actorElement=actorList[i]->save(root);
      root->LinkEndChild(actorElement);
      }

    //then saveable Buttons
    for (unsigned int i=0;i<saveableButtonList.size();i++)
      {
      TiXmlElement * buttonElement=saveableButtonList[i]->save(root);
      root->LinkEndChild(buttonElement);
      }
    //then nodes
    for (unsigned int i=0;i<nodeList.size();i++)
      {
      TiXmlElement * nodeElement=nodeList[i]->save(root);
      root->LinkEndChild(nodeElement);
      }

    string saveString=startProject + "/";
    saveString.append(filename);

    cout << "saving filename: " << saveString << endl;

    doc.SaveFile( saveString );
}

void SceneData::loadAll(std::string fileName, bool bCleanUp){


    renderer->bUpdatePhysics=false;

    if (bCleanUp){
        //clean Up
        //let's not remove actions, okay? But why? Now with the ne
        //let's not remove buttons? unless they are nodes?
        //Button first because of udpInput and threading

        //remove udpInputs
        if (buttonList.size()>0){
            for (int i=(int)buttonList.size()-1;i>=0;i--){
                //Action * act=NULL;
                //act=dynamic_cast<Action*>(buttonList[i]);
                UdpInput* udp=NULL;
                udp=dynamic_cast<UdpInput*>(buttonList[i]);
                if (buttonList[i]->level>0 && udp)
                    buttonList[i]->remove();
            }
        }

        if (actorList.size()>0){
            for (int i=(int)actorList.size()-1;i>=0;i--)
                actorList[i]->remove();
        }

        if (nodeList.size()>0){
            for (int i=(int)nodeList.size()-1;i>=0;i--)
                nodeList[i]->remove();
        }
        //clean selection stuff!
        selectedActors.clear();
    }


    string stringName=startProject + "/";
    stringName.append(fileName);

    cout << "Loading file..." << stringName <<endl;


    TiXmlDocument doc( stringName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Make sure this is a Moviesandbox file!
    //***********************************************************************

    element=hDoc.FirstChildElement().Element();
    if (!element) return;

    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load Actor List
    //***********************************************************************

    //loading is a two step process
    int listPos=0;

    //load controller locations
    cout << "loading camera Position..." << endl;
    element=hRoot.FirstChild("Controller").Element();
    if (element){
        string cLoc=element->Attribute("TransformMatrix");
        controller->memberFromString(&controller->property["TRANSFORMMATRIX"], cLoc);
        controller->postLoad();
        //controller->processMove(0.0);
    }
    else cout << "no position info found... skipping..." << endl;

		cout << "loading actors..." << endl;

    //first create all Actors

      element=hRoot.FirstChild( "Actor" ).Element();
      string myType;
      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        cout << "next element: " << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Actor * A=actorInfo[myType].actorReference;
        A->create();
        }


    //then load all properties - for referencing reasons
      element=hRoot.FirstChild( "Actor" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        Actor* A=actorList[listPos];
        myType=element->GetText();
        cout << "Loading property type: " << myType << endl;
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        A->load(element);
        listPos++;
        }

    //***********************************************************************
    //Load Node List
    //***********************************************************************
    listPos=0;

    cout << "loading nodes..." << endl;

      element=hRoot.FirstChild( "Node" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Node"))
        {
        cout << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Node* N=(Node*)actorInfo[myType].actorReference;
        N->create();
        }

      element=hRoot.FirstChild( "Node" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Node"))
        {
        Node* N=nodeList[listPos];
        myType=element->GetText();
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        N->load(element);
        listPos++;
        }
      for (uint i=0; i< nodeList.size();i++){
        nodeList[i]->setup();
        }


    //***********************************************************************
    //Setup Actor List
    //***********************************************************************
    for (int i=0;i<(int)actorList.size();i++){
        actorList[i]->postLoad();
    }
    //then setup all actors, so properties are already present!
    for (int i=0;i<(int)actorList.size();i++){
            actorList[i]->setup();
    }

    //***********************************************************************
    //Load Button List
    //***********************************************************************
    listPos=0;

    cout << "loading buttons..." << endl;

      element=hRoot.FirstChild( "Button" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Button"))
        {
        cout << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        BasicButton* B=(BasicButton*)actorInfo[myType].actorReference;
        B->create();
        }

      element=hRoot.FirstChild( "Button" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Button"))
        {
        BasicButton* B=saveableButtonList[listPos];
        myType=element->GetText();
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        B->load(element);
        B->setup();
        listPos++;
        }

    inspectorManager->setup();

    //do a camera update!
    controller->myTools[TOOL_NAV]->update(deltaTime);

}

void SceneData::loadMeshes(std::string path, std::string fileName){

    cout<< "loading project: " << fileName << " from path: " << path << endl;

    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);
    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

/*
    for (int i=0;i<numberOfUntitledDrawings;i++){
        string myName="untitled";
        char buffer [16];
        itoa(i, buffer,10);
        myName=myName+buffer;
        spriteMeshLoader->loadSpriteMesh(path+"/untitled/"+myName, myName);
        cout << "loading untitled drawing: " << myName << endl;

    }
*/
    //***********************************************************************
    //Load OBJs
    //***********************************************************************


	  element=hRoot.FirstChild( "Mesh" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Mesh")){
    /*
		  string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        renderer->meshList[meshID]=LoadOBJ(meshFileName.c_str());
        cout << "loading mesh " << meshID << endl;
     */

	 }

    //***********************************************************************
    //Load complex Meshes
    //***********************************************************************
      element=hRoot.FirstChild( "ColladaMesh" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("ColladaMesh")){
        string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        colladaLoader->loadColladaMesh(path+"/"+meshFileName, meshID);
        cout << "loading mesh " << meshID << endl;
      }

    //***********************************************************************
    //Load sprite Meshes
    //***********************************************************************
      element=hRoot.FirstChild( "SpriteMesh" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("SpriteMesh")){
        string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        spriteMeshLoader->loadSpriteMesh(path+"/"+meshFileName, meshID);
        cout << "loading sprite mesh " << meshID << endl;
      }

      element=hRoot.FirstChild( "SpriteMeshXML" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("SpriteMeshXML")){
        string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        spriteMeshLoaderXML->loadSpriteMesh(path+"/"+meshFileName, meshID);
        cout << "loading legacy XML sprite mesh " << meshID << endl;
      }

    //load number of untitled drawings
    element=hRoot.FirstChild( "untitled" ).Element();
    if (element)
        element->Attribute("amount",&numberOfUntitledDrawings);

}

void SceneData::loadPrefab(std::string path, std::string fileName){

    //loading is a two step process
    int listPos=actorList.size();


    cout << "loading Prefab from... "<< fileName << endl;
    //first create all Actors
    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    element=hDoc.FirstChildElement().Element();
    element= element->FirstChildElement("Actor");

      string myType;
      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        cout << "next element: " << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Actor * A=actorInfo[myType].actorReference;
        A->create();
        }


    //then load all properties - for referencing reasons
    element=hDoc.FirstChildElement().Element();
    element= element->FirstChildElement("Actor");

    //for relative loading
    int loadPos=listPos;

      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        Actor* A=actorList[listPos];

        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        A->actorOffset=loadPos;         //for relative actor references

        A->load(element);

//        if (A->base)
//            A->baseMatrix=A->baseMatrix;
            //cout << A->base->name << endl;


        A->actorOffset=0;               //set back to zero
        A->setup();
        selectedActors.push_back(A);                                         //push newly created buttons in selection stack
        listPos++;
        }

 //   makeGroup();
}

void SceneData::loadAction(std::string path, std::string fileName){


    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Load Action
    //***********************************************************************
    cout << "loading actions..." << endl;

    element=hDoc.FirstChildElement().Element();
    element= element->FirstChildElement("Action");
    std::string myType=element->GetText();
    Action* AC=(Action*)actorInfo[myType].actorReference;
    AC->create();

    AC=(Action*)buttonList.back();
    AC->load(element);
    actionList[AC->name]=AC;
    AC->setup();
}

void SceneData::loadTextures(std::string path, string fileName){

    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);


    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load textures
    //***********************************************************************
      element=hRoot.FirstChild( "Texture" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Texture")){
        int val=0;

        string texID=element->Attribute("textureID");
        string texFileName=element->Attribute("filename");
        string nextFrame="NULL";
        if (element->Attribute("nextFrame"))
            nextFrame=element->Attribute("nextFrame");

        float frameRate = 0.0f;
        if (element->Attribute("frameRate"))
        {
            element->Attribute("frameRate",&val);
            frameRate=float(val);
        }

        element->Attribute("bAlpha", &val);
        bool bAlpha=bool(val);

        element->Attribute("bWrap", &val);
        bool bWrap=bool(val);

        renderer->LoadTextureTGA(path+texFileName,bWrap,bAlpha, texID);
        textureList[texID]->nextTexture=nextFrame;
        textureList[texID]->frameRate=frameRate;
        cout << "loading texture " << texID << endl;
      }
}

void SceneData::loadShaders(std::string path, string fileName){

    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);


    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load shaders
    //***********************************************************************
      element=hRoot.FirstChild( "Shader" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Shader")){
        string shaderID=element->Attribute("shaderID");
        string vertexFileName=element->Attribute("vertexShaderFilename");
        string fragmentFileName=element->Attribute("fragmentShaderFilename");
        cout << "loading shader " << shaderID << endl;
        renderer->loadShader(path+vertexFileName,path+fragmentFileName, shaderID);
        cout << "finished loading shader " << shaderID << "-------------------------------------------------- " << endl;
      }
}

void SceneData::loadActionList(std::string path, string fileName){

    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);


    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load Actions
    //***********************************************************************
      element=hRoot.FirstChild( "Action" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Action")){
        string actionFileName=path+ "/" + element->Attribute("actionFilename");
        loadAction(path,actionFileName);
        cout << "loading action " << actionFileName << endl;
      }

}

void SceneData::addToLibrary(TiXmlElement* myElement){

    TiXmlDocument doc( startProject + "/my.project" );
    if (!doc.LoadFile()){
        cout << "could not find my.project!" << endl;
        return;
    }

    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Make sure this is a Moviesandbox file!
    //***********************************************************************

    element=hDoc.FirstChildElement().Element();
    if (!element) return;

    hRoot=TiXmlHandle(element);

    //check for duplicates!!
    element=element->FirstChildElement(myElement->Value());
    string myName=myElement->FirstAttribute()->Name();

    while (element && element->Attribute(myName)){
        string myAttribContent;
        string existingAttribContent;
        myAttribContent=*myElement->Attribute(myName);
        existingAttribContent=*(element->Attribute(myName));
        if (myAttribContent==existingAttribContent){
            TiXmlElement* yesNo = (TiXmlElement*)hRoot.Element()->ReplaceChild(element,*myElement);
            if (yesNo){
                doc.SaveFile(startProject + "/my.project");
                cout << "replaced in my.project!!" << endl;
            }
            else
                exit(0);
            return;
        }
        element=element->NextSiblingElement();
    }
    //append our element to that - or just dump it in here.
    element=hDoc.FirstChildElement().Element();
    TiXmlElement* lastElementOfSameType=element->FirstChildElement(myElement->Value());
    if (lastElementOfSameType)
        element->InsertBeforeChild(lastElementOfSameType,*myElement);
    else
        element->LinkEndChild(myElement);

    //update number of untitled drawings
    element=hRoot.FirstChild("untitled").Element();
    element->SetAttribute("amount",numberOfUntitledDrawings);

    doc.SaveFile(startProject + "/my.project");
}

void SceneData::getAllDrawings(){

    cout << "getting drawings..." << endl;

    savedDrawings.clear();

#ifdef TARGET_WIN32

    //not used?
    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\drawings\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
          {
		  cout << fd.cFileName << endl;
		  savedDrawings.push_back(fd.cFileName);
          }
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif

#ifdef TARGET_MACOSX
	string dir="resources/drawings";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

    while ((dirp = readdir(dp)) != NULL) {
        savedDrawings.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif

}

void SceneData::getAllScenes(){

    cout << "getting scenes..." << endl;

    savedScenes.clear();

#ifdef TARGET_WIN32

    //never used?
    //char path[MAX_PATH];
    string myPath=startProject + DIRECTORY_SEPARATION + "*.scene";
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( myPath.c_str(), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  savedScenes.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}

#endif

#ifdef TARGET_MACOSX
    //TODO: only list .scene files on Mac OSX!!
	string dir=startProject+DIRECTORY_SEPARATION;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
	    if (dirp->d_name.contains(".scene"))
            savedScenes.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif
}

void SceneData::getAllImages(){

    cout << "getting Images..." << endl;

    userImages.clear();

#ifdef TARGET_WIN32

    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\images\\*.tga", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  userImages.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif

#ifdef TARGET_MACOSX
	string dir="resources/images";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        userImages.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif

}

void SceneData::getAllBrushes(){


    cout << "getting Brushes..." << endl;

    userBrushes.clear();

#ifdef TARGET_WIN32

    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\brushes\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  userBrushes.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif
#ifdef TARGET_MACOSX
	string dir="resources/brushes";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        userBrushes.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif
}

void SceneData::getAllPrefabs(){

#ifdef TARGET_WIN32

    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\prefabs\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  prefabs.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif
#ifdef TARGET_MACOSX
	string dir="resources/prefabs";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        prefabs.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif
}


//************************************************************
//
//       3D and general Helper functions
//
//
//************************************************************

float SceneData::convertToGrid(float nonGrid){

    return (gridSize*(int)(nonGrid/gridSize));// - nonGridRounded%gridSizeRounded)*0.001);
}

float SceneData::setToRange(float min, float max, float value){
    //multiply by difference of max and min     //and add minimum
    return( value   *   (max-min)     +   min);
}


#ifdef TARGET_WIN32
string SceneData::openFileDialog(){


    string myFileName;

    typedef BOOL ( WINAPI *GetFileNameFromBrowse )( HWND hwnd,
                                                    LPWSTR pszFilePath,
                                                    UINT cchFilePath,
                                                    LPCWSTR pszWorkingDir,
                                                    LPCWSTR pszDefExt,
                                                    LPCWSTR pszFilters,
                                                    LPCWSTR szTitle );

    // Path buffer, specifies starting directory and on successful return
    // from open dialog also holds the selected file name
    wchar_t wszPath[MAX_PATH] = L".";

    // Load shell32 dll
    HMODULE hModule = LoadLibrary( "Shell32.dll" );
    if( !hModule ){
       return "NULL";
    }

    // Get procedure address
    GetFileNameFromBrowse GetFileNameFromBrowsePtr = ( GetFileNameFromBrowse )GetProcAddress( hModule, "GetFileNameFromBrowse" );

    // Show browse dialog
    if( GetFileNameFromBrowsePtr && GetFileNameFromBrowsePtr( 0, wszPath, MAX_PATH, 0, 0, L"", L"Nibu Open" )){

        int i = 0;
        while (wszPath[i] != 0)
        {
            myFileName += wszPath[i];
            i++;
        }

    }else
		myFileName="NULL";

    // Free loaded library
    FreeLibrary( hModule );

    return myFileName;
}
#endif

#ifdef TARGET_MACOSX
string SceneData::openFileDialog(){

	NavDialogCreationOptions dialogOptions;
	NavDialogRef dialog;
	NavReplyRecord replyRecord;
	CFURLRef cfURL = NULL;
	FSRef fileAsFSRef;
	OSStatus status;

	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions(&dialogOptions);
	require_noerr( status, CantGetNavOptions );

	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;

	// Create the dialog
	status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	require_noerr( status, CantCreateDialog );

	// Show it
	status = NavDialogRun(dialog);
	require_noerr( status, CantRunDialog );

	// Get the reply
	status = NavDialogGetReply(dialog, &replyRecord);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );

	// If the user clicked "Cancel", just bail
	if ( status == userCanceledErr ) goto UserCanceled;

	// Get the file
	status = AEGetNthPtr(&(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
	require_noerr( status, CantExtractFSRef );

	// Convert it to a CFURL
	cfURL = CFURLCreateFromFSRef(NULL, &fileAsFSRef);


   // cleanup dialog


	// Cleanup
	CantExtractFSRef:
	UserCanceled:
		verify_noerr( NavDisposeReply(&replyRecord) );
	CantGetReply:
	CantRunDialog:
		NavDialogDispose(dialog);
	CantCreateDialog:
	CantGetNavOptions:

	CFStringRef cfString = NULL;
	if ( cfURL != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfURL, kCFURLPOSIXPathStyle );
		CFRelease( cfURL );
	}

    // copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
	const int kBufferSize = 255;

	char folderURL[kBufferSize];
	Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);

	// append strings together

	string url1 = folderURL;
	string finalURL = url1;

	//printf("url %s\n", finalURL.c_str());

	return finalURL;
}
#endif



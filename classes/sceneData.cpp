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
}


//************************************************************
//
//Constructor and Destructor
//
//************************************************************

SceneData::SceneData(){


    name="SceneManager";

    backgroundTex="NULL";
    backgroundColor=Vector4f(0.25,0.5,0.8,1);

    currentLayer=0;
    startSceneFilename="";


    mouseSensitivity=0.005;
    moveSpeed=0.1;

	deltaTime=0.0;
	frames=0;

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

    input->screenX=renderer->screenX;
    input->screenY=renderer->screenY;

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

    element->Attribute("SceneSize", &val);
    renderer->scene_size=val;


    element->Attribute("MouseSensitivity", &dVal);
    mouseSensitivity=dVal;
    element->Attribute("MoveSpeed", &dVal);
    moveSpeed=dVal;

    element->Attribute("FOV", &dVal);
    renderer->fov=dVal;

    //setting start scene
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


    //setting libraries
    element=hRoot.FirstChild( "Library" ).Element();

    while (element){
        library.push_back(element->Attribute("Library"));
        element=element->NextSiblingElement("Library");
    }

    //check for myLibrary and create if not exists

    TiXmlDocument myLib( "resources/my.library" );

    if (!myLib.LoadFile()){

        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        myLib.LinkEndChild( decl );
        TiXmlElement * root = new TiXmlElement( "Moviesandbox" );
        myLib.LinkEndChild( root );
        myLib.SaveFile( "resources/my.library");
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


    //setting up menu
    cout << "setting up menu" << endl;
    content= new Content;
    content->setup();

    //load library stuff
    input->getAllPrefabs();
    for (int i=0;i<(int)library.size();i++){
        input->loadMeshes(library[i]);
        input->loadTextures(library[i]);
        input->loadShaders(library[i]);
        input->loadActionList(library[i]);
    }


    //then load scene
    cout << "loading basic stuff..." << endl;
    input->loadAll(startSceneFilename, false);
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

	input->update(deltaTime);
    renderer->update(deltaTime);

    glutPostRedisplay();
}



void SceneData::addLayer(string layerName){

        Layer* lay = new Layer;
        lay->setup();
        lay->name=layerName;
        lay->textureID=lay->name+"_Color";
        lay->depthTextureID=lay->name+"_Depth";
        lay->pickTextureID=lay->name+"_Pick";
		lay->lightDataTextureID=lay->name+"_lightData";

        layerList.push_back(lay);
        renderer->createFBO(&(lay->colorFBO), &(lay->colorTex), NULL, renderer->scene_size, false, lay->textureID);
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
    grid->setRotation(Vector3f(0,180,0));

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
    brush->controller=input->controller;
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
        //renderer->copyMemoryToTexture(pBuf,textureList["sharedMemory"], 640,480);

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
		cout << "Could not open '/tmp/msbRamFile'"<<endl;
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

        //renderer->copyMemoryToTexture(pBuf,textureList["sharedMemory"], 640,480);

	if (sourcebuffer)
		munmap(sourcebuffer, BUF_SIZE);

	return 1;
#endif
   return 0;


}


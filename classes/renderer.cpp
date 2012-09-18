#include "input.h"

#include "FreeImage.h"

//loaders and the like
#include "colladaLoader.h"
#include "spriteMeshLoader.h"
#include "spriteMeshLoaderXML.h"

//actors
#include "particleSystem.h"
//#include "physicsActor.h"
//#include "ragDoll.h"
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
#include "timelineInspector.h"
#include "assetInspector.h"
#include "primitivesInspector.h"

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


//data
#include "meshData.h"


//static link
Renderer* Renderer::rendererInstance=NULL;

//************************************************************
//
//RTTY actorInfo - Information about every Actor:
//                 Class Name, Type, Size
//
//************************************************************


//************************************************************
//
//Constructor and Destructor
//
//************************************************************

Renderer::Renderer(){

    lastShader="NULL";

    bDrawLighting=true;
    bDrawNormals=false;
    bDrawSmudge=false;
    bRenderStereo=true;
    bDrawMenu=true;
    bDrawNodes=true;
    bUseBlending=true;
    bMultisample=true;
    bSSAO=false;
    bDrawColor=true;
    bShadowPass=false;
    bDOF=false;


    bFullscreen=false;
    bUpdatePhysics=false;

    lightLoc=Vector3f(0,3,15);                  //light Location
    ambient=Vector3f(1,1,1);

    nearClip=0.2;
    farClip=512.0;

    screenX=0;
    screenY=0;

    windowX=1024;
    windowY=768;

	windowPosX=0;
	windowPosY=0;
    fov=45;
    focus=25.0;

    normalBlur=10;
    dofBlur=1;

    frustumTop=0.083;
    frustumBottom=-0.083;
    eyeDistance=0.10;
    bkgOffset = 50.0;

    //depthPrecision = GL_RGBA8;
    depthPrecision = GL_RGBA16F_ARB;
    //depthPrecision = GL_RGBA32F;

    texFilterMin = GL_LINEAR_MIPMAP_LINEAR;
    texFilterMag = GL_LINEAR;
    //texFilter = GL_NEAREST;
    texTGAFilterMin=GL_LINEAR_MIPMAP_LINEAR;
    texTGAFilterMag=GL_LINEAR;

    dataType= GL_FLOAT;
    //dataType= GL_INT;

    lighting_tx = 0; // the light texture
    lighting_fb = 0; // the framebuffer object to render to that texture
    lighting_size = 1.0;

    normal_tx = 0; // the light texture
    normal_fb = 0; // the framebuffer object to render to that texture
    normal_size = 1.0;

    normalBlur_tx = 0; // the light texture
    normalBlur_fb = 0; // the framebuffer object to render to that texture


    shadow_tx = 0;
    shadow_fb = 0;
    shadow_size = 1.0;

    depth_tx = 0;
    depth_fb = 0;

    scene_tx = 0;
    scene_fb = 0;
    scene_size = 1.0;

    multiSample_fb = 0;
    multiSample_db = 0;
    multiSample_depth = 0;
    multiSample_color = 0;

    postOverlay=NULL;

	deltaTime=0.0;
	frames=0;

	drawBuffers[0] = GL_COLOR_ATTACHMENT0_EXT;
	drawBuffers[1] = GL_COLOR_ATTACHMENT1_EXT;
	//drawBuffers[2] = GL_COLOR_ATTACHMENT2_EXT;
	//drawBuffers[3] = GL_COLOR_ATTACHMENT3_EXT;

	registerProperties();
}

Renderer::~Renderer(){

            glDeleteFramebuffersEXT(1, &lighting_fb);
            glDeleteFramebuffersEXT(1, &shadow_fb);


            glDeleteRenderbuffersEXT(1, &multiSample_pick);
            glDeleteRenderbuffersEXT(1, &multiSample_lightData);
            glDeleteRenderbuffersEXT(1, &multiSample_depth);
            glDeleteRenderbuffersEXT(1, &multiSample_color);
            glDeleteRenderbuffersEXT(1, &multiSample_db);
            glDeleteFramebuffersEXT(1, &multiSample_fb);

/*
    dSpaceDestroy(collisionSpace);
    dWorldDestroy(physicsWorld);
    dCloseODE();
*/
}

Renderer* Renderer::getInstance(){

    if (rendererInstance)
        return rendererInstance;
    else{
        rendererInstance=new Renderer;
        return rendererInstance;
        }
}

//************************************************************
//
//  Windowing stuff and screen setup
//
//************************************************************

void Renderer::registerProperties(){

    createMemberID("BDRAWLIGHTING",&bDrawLighting, this,true,"10BoolButton");
    createMemberID("BUSEBLENDING",&bUseBlending, this,true,"10BoolButton");
    createMemberID("BMULTISAMPLE",&bMultisample,this,false);
    createMemberID("BSSAO",&bSSAO,this,true,"10BoolButton");
    createMemberID("BDRAWCOLOR",&bDrawColor,this,true,"10BoolButton");
    createMemberID("BSHADOWPASS",&bShadowPass,this,true,"10BoolButton");
    createMemberID("BDOF",&bDOF,this,true,"10BoolButton");

    createMemberID("NEARCLIP",&nearClip,this);
    createMemberID("FARCLIP",&farClip,this);

    createMemberID("FOV",&fov,this);

    createMemberID("NORMALBLUR",&normalBlur,this);

    createMemberID("LIGHTING_SIZE",&lighting_size,this);
    createMemberID("SHADOW_SIZE",&shadow_size,this);

    createMemberID("SCENE_SIZE",&scene_size,this);

}

void Renderer::initWindow(int x, int y, string windowName){

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);

      char* gmString  = new char[64];
      // screenX screenY, 32bit pixel depth, 60Hz refresh rate
      sprintf(gmString," %ix%i:32@60",screenX,screenY);
      glutGameModeString( gmString );

    if (bFullscreen)
      {
      // start fullscreen game mode
      glutEnterGameMode();
      }
    else
      {
      glutInitWindowSize(windowX,windowY);
      glutInitWindowPosition(x,y);
      glutCreateWindow(windowName.c_str());
      }
}



void Renderer::reDrawScreen(int w, int h){

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

    sceneData->inspectorManager->closeAll();

    sceneData->menuBar->scale.x=w;

    cout << "redrawing... height:" << h << " width: "<<w << endl;

//	float ratio = 1.0* w / h;



    for (int i=0;i<(int)sceneData->layerList.size();i++){
        //sceneData->layerList[i]->scale=Vector3f(w,-h,1.0);
        //sceneData->layerList[i]->location.y=h;
        //sceneData->layerList[i]->setLocation(sceneData->layerList[i]->location);
    }

    Vector3f screenDelta=Vector3f(w-windowX,h-windowY,0);

    //update all inspectors and timeline too!
    for (int i=0;i<(int)sceneData->inspectorManager->inspectors.size();i++){
        Inspector* mI=sceneData->inspectorManager->inspectors[i];
        mI->addLocation(screenDelta);
        mI->initialLocation=mI->location;

        //update all list contents, backgrounds and inspectorButtons
        mI->backgroundButton->addLocation(screenDelta);
        //mI->scrollButton->addLocation(screenDelta);
        for (int j=0;j<(int)mI->inspectorButtons.size();j++){
            mI->inspectorButtons[j]->addLocation(screenDelta);
            mI->inspectorButtons[j]->initialLocation=mI->inspectorButtons[j]->location;
        }
        for (int j=0;j<(int) mI->listButton.size();j++){
            mI->listButton[j]->addLocation(screenDelta);
            mI->listButton[j]->initialLocation=mI->listButton[j]->location;
        }


    }

    windowX=w;
    windowY=h;


}


//************************************************************
//
//  Setting up and calling all Actors' update function
//
//************************************************************

void Renderer::setup(){

    input=Input::getInstance();
    sceneData=SceneData::getInstance();

	for (int i=0;i<FPSBUFFERSIZE;i++){
		fpsBuffer[i]=0.0;
	}

    verdana= new ofTrueTypeFont;
	verdana->loadFont("./resources/verdana.ttf",8);
	verdana->setLineHeight(20.0f);



    #ifdef TARGET_WIN32
	if (!GLEE_EXT_framebuffer_multisample){
		bMultisample=false;
		cout << "Multisampling not supported for FBOs, switching them off..." << endl;
	}
    #endif

    //setup picking texture!
    glGenTextures(1, &pickTexture);
    glBindTexture(GL_TEXTURE_2D, pickTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, depthPrecision,  1, 1, 0, GL_BGRA, dataType, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture (GL_TEXTURE_2D, 0);

    //check how many color buffers we have
	int maxColorBuffers;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorBuffers);
	cout << "max colorbuffers: " << maxColorBuffers << endl;


    //setup frameBufferObject
    setupFBOs();

    //enable Blending for everyone!
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    /*
     *  Enable Hardware Point Sprites throughout
     */

    //Setup Point sprite textures,
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
    glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
    //Enable Vertex Shader point-size control
    glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );

    //always enable (if disabling, re-enable afterwards!

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if (bMultisample)
        glEnable(GL_MULTISAMPLE);   //just in case we force multisampling

    glEnable(GL_NORMALIZE);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("glEnables Error check...");
    #endif

    //TODO: should sharedMem always be created on startup?
            //createEmptyTexture("sharedMemory",GL_RGBA, GL_UNSIGNED_BYTE,1024,512);


    //This was used for the promo video to get better framerates for kinect live-feed.
    //will make it in some time, but not now...
    //createKinectWall(640.0f);

}

void Renderer::physicsSetup(){

/*
    physicsWorld = dWorldCreate();          //create a default physics world
    collisionSpace = dHashSpaceCreate(0);   //create a default collision space
    dWorldSetGravity (physicsWorld,0,-9.81,0);

    dWorldSetCFM(physicsWorld, 1e-5);
    dWorldSetERP(physicsWorld, 0.2);
    dWorldSetContactMaxCorrectingVel(physicsWorld, 0.9);
    dWorldSetContactSurfaceLayer(physicsWorld, 0);
    dWorldSetAutoDisableFlag(physicsWorld, 1);

    jointGroup = dJointGroupCreate(0);
    groundPlane = dCreatePlane(collisionSpace,0,1,0,0);

    dInitODE();
    dWorldSetQuickStepNumIterations(physicsWorld,120);
*/
}


void Renderer::update(float deltaTime){

	if (bUpdatePhysics)
	  physicsUpdate();

	windowPosX=glutGet(GLUT_WINDOW_X);
	windowPosY=glutGet(GLUT_WINDOW_Y);

	//use depth of field when drawing and setting things up.
    focus=(input->mouse3D-sceneData->controller->controlledActor->location).length();

    glutPostRedisplay();
}

void Renderer::physicsUpdate(){

	// Detect collision
	//dSpaceCollide(collisionSpace,NULL,&Renderer::handleCollisionBetween);

	// Step world
	/*
	if (physicsTime<1)
	  physicsTime+=deltaTime * 0.001f;
    else
     {
    */
      //dWorldQuickStep(physicsWorld, 0.01);
      //super-accurate but sloooooow:
	  //dWorldStep(physicsWorld,0.01f);
	  // Remove all temporary collision joints now that the world has been stepped
	 // dJointGroupEmpty(jointGroup);
     // physicsTime=deltaTime;
	//  }

}

void Renderer::setupFBOs(){

    //frame buffer objects
    //always need them with layer system!

    //buffer to copy from for FSAA multisampling in FBOs
	createFBO(&multiSample_fb, NULL, &multiSample_db, screenX, screenY, false, "multisampleBuffer");

    //framebuffer and texture to store global lighting and shadow information
    createFBO(&lighting_fb, &lighting_tx, NULL, screenX/lighting_size, screenY/lighting_size, false, "lighting"); //uses scene_size because it's the final FBO in which we compute everything!
    createFBO(&normal_fb, &normal_tx, NULL, screenX/lighting_size, screenY/lighting_size, false, "normals"); //uses scene_size because it's the final FBO in which we compute everything!
    createFBO(&normalBlur_fb, &normalBlur_tx, NULL, screenX, screenY, false, "normalsBlurred"); //uses scene_size because it's the final FBO in which we compute everything!
    createFBO(&shadow_fb, &shadow_tx, NULL, screenX/shadow_size,screenY/shadow_size, false, "shadow");
    createFBO(&scene_fb, &scene_tx, NULL, screenX/scene_size, screenY/scene_size, false, "scene");

    #ifdef BDEBUGRENDERER
    checkOpenGLError("FBO Error check...");
    #endif
}

void Renderer::createFBO(GLuint* fbObject, GLuint* fbTexture, GLuint* fbDepth, int fbSizeX, int fbSizeY, bool bDepth, string name){
    //-------------------------------------------------------
    // framebuffer object


    //set up renderbuffer

    int maxsamples;
    glGetIntegerv(GL_MAX_SAMPLES_EXT,&maxsamples);

    //Multisample type - needs to be minimum 16 bit for picking and lighting calculations
	//GLenum sampleType=GL_RGBA16F_ARB;
	//GLenum sampleType=GL_RGBA;
	//GLenum sampleType=GL_RGBA32F_ARB;

    if (!bDepth){

        if (name=="multisampleBuffer"){

            glGenRenderbuffersEXT(1, &multiSample_color);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multiSample_color);

            if (bMultisample)
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, depthPrecision, fbSizeX, fbSizeY);
            else
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthPrecision, fbSizeX, fbSizeY);

            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

			//COLOR COMPONENTS

			glGenRenderbuffersEXT(1, &multiSample_depth);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multiSample_depth);
            if (bMultisample)
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, depthPrecision, fbSizeX, fbSizeY);
            else
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthPrecision, fbSizeX, fbSizeY);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

            glGenRenderbuffersEXT(1, &multiSample_pick);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multiSample_pick);
            if (bMultisample)
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, depthPrecision, fbSizeX, fbSizeY);
            else
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthPrecision, fbSizeX, fbSizeY);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

            glGenRenderbuffersEXT(1, &multiSample_lightData);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multiSample_lightData);
            if (bMultisample)
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, depthPrecision, fbSizeX, fbSizeY);
            else
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthPrecision, fbSizeX, fbSizeY);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

			//DEPTH COMPONENT

            glGenRenderbuffersEXT(1, &multiSample_db);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multiSample_db);
            if (bMultisample)
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, GL_DEPTH_COMPONENT, fbSizeX, fbSizeY);
            else
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, fbSizeX, fbSizeY);

            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

            glGenFramebuffersEXT (1, fbObject);
            glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, *fbObject);


			// attach renderbuffer
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, multiSample_color);
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_RENDERBUFFER_EXT, multiSample_depth);
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, multiSample_db);

		}

        else{
            glGenFramebuffersEXT (1, fbObject);
            glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, *fbObject);
        }
    }
    // create the texture we'll use for the shadow map

    if (name!="multisampleBuffer"){
        glGenTextures(1, fbTexture);
        glBindTexture(GL_TEXTURE_2D, *fbTexture);

        if (bDepth){

            glGenFramebuffersEXT (1, fbObject);
            glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, *fbObject);

            glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, fbSizeX, fbSizeY, 0, GL_DEPTH_COMPONENT, dataType, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, *fbTexture, 0);
        }
        else{
            cout << "no depth in FBO!" << name << endl;
            // attach colorBuffer to a texture
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glTexImage2D(GL_TEXTURE_2D, 0, depthPrecision,  fbSizeX, fbSizeY, 0, GL_RGBA, dataType, NULL);

            glGenerateMipmapEXT(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, texFilterMag);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, texFilterMin);

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

            glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *fbTexture, 0);

            if (fbDepth){
                glGenRenderbuffersEXT(1, fbDepth);
                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, *fbDepth);
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, fbSizeX, fbSizeY);

                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

                // attach renderbuffer
                glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, *fbDepth);
            }
        }

    }


    if (bDepth){
        glDrawBuffer (GL_NONE);
        glReadBuffer (GL_NONE);
    }

    // verify all is well and restore state
    checkFBOStatus();
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    glBindTexture (GL_TEXTURE_2D, 0);

    if (name=="multisampleBuffer")   return;

    sceneData->textureList[name]=new textureObject;
    sceneData->textureList[name]->texture=(uint)*fbTexture;
    sceneData->textureList[name]->nextTexture="NULL";
    sceneData->textureList[name]->frameRate=0;

    if (bDepth)
      sceneData->textureList[name]->bAlpha=true;
    else
      sceneData->textureList[name]->bAlpha=false;

    sceneData->textureList[name]->bWrap=false;
    sceneData->textureList[name]->texFilename="NULL";

    cout << "FBO texture name " << name << endl;

    //-------------------------------------------------------
    //end framebuffer object

}

void Renderer::checkFBOStatus(){

    GLenum status = glCheckFramebufferStatusEXT (GL_FRAMEBUFFER_EXT);
    switch (status){
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            cerr << "FBO configuration unsupported" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            cerr << "FBO DrawBuffer incomplete" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
            cerr << "FBO Multisample incomplete" << endl;
            break;


        default:
            cerr << "FBO programmer error" << endl;
            break;
    }
}

/*
void Renderer::handleCollisionBetween(void * data, dGeomID o0, dGeomID o1){


		// Create an array of dContact objects to hold the contact joints

		static const int MAX_CONTACTS = 10;
		dContact contact[MAX_CONTACTS];

		for (int i = 0; i < MAX_CONTACTS; i++)
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.mu2 = 0;
			//TODO: played around with Physics...
			//contact[i].surface.bounce = 0.8;

			contact[i].surface.bounce = 0.1;

			//contact[i].surface.bounce_vel = 0.1;

			contact[i].surface.bounce_vel = 0.01;

            //contact[i].surface.soft_cfm = 0.01;
			contact[i].surface.soft_cfm = 0.1;
		}
		if (int numc = dCollide(o0, o1, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
		{
			// Get the dynamics body for each geom
			dBodyID b1 = dGeomGetBody(o0);
			dBodyID b2 = dGeomGetBody(o1);
			// To add each contact point found to our joint group we call dJointCreateContact which is just one of the many
			// different joint types available.
			for (int i = 0; i < numc; i++)
			{
				// dJointCreateContact needs to know which world and joint group to work with as well as the dContact
				// object itself. It returns a new dJointID which we then use with dJointAttach to finally create the
				// temporary contact joint between the two geom bodies.
				dJointID c = dJointCreateContact(Renderer::rendererInstance->physicsWorld, Renderer::rendererInstance->jointGroup, contact + i);
				dJointAttach(c, b1, b2);
			}
		}

}
*/
//************************************************************
//
//  Drawing to the screen - Actors, Buttons and RenderToTexture
//
//************************************************************


void Renderer::setupCamera(bool bCalculateMatrices){

//setup Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//gluPerspective(fov,(screenY==0)?(1):((float)screenX/screenY),nearClip,farClip);
	//gluPerspective(fov,(screenY==0)?(1):((float)scene_size/scene_size),nearClip,farClip);
	//gluPerspective(fov,(screenY==0)?(1):((float)screenX/screenY),nearClip,farClip);
	gluPerspective(fov,((float)windowX)/((float)windowY),nearClip,farClip);


//setup camera
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  sceneData->controller->controlledActor->location.x, sceneData->controller->controlledActor->location.y, sceneData->controller->controlledActor->location.z,
                sceneData->controller->lookPoint.x, sceneData->controller->lookPoint.y, sceneData->controller->lookPoint.z,
                sceneData->controller->upPoint.x, sceneData->controller->upPoint.y,sceneData->controller->upPoint.z);

    if (bCalculateMatrices){
        glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
        glGetFloatv(GL_MODELVIEW_MATRIX,cameraMatrix);

        cameraRotationMatrix= cameraMatrix;
        cameraRotationMatrix.setTranslation(Vector3f(0,0,0));
        inverseCameraRotationMatrix=cameraRotationMatrix.inverse();
        inverseCameraMatrix=cameraMatrix.inverse();
        inverseProjectionMatrix=projectionMatrix.inverse();
    }

}


void Renderer::setupOrthoCamera(int width, int height){


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width,height,0,-10000,10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


///Actual Rendering


void Renderer::draw(){


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);


     setupCamera(true);



    //TODO:Stereo with Layers!
    /*
    if (bRenderStereo){
        drawStereoscopic();
    }
    */


	drawSceneTexture();



    #ifdef BDEBUGRENDERER
    checkOpenGLError("post-drawSceneTexture");
    #endif
	/////////////////////////////////////////////////////
    /// 2D Elements from here
    /////////////////////////////////////////////////////

    /*
	 *	Set Ortho viewing transformation
	 */
    setupOrthoCamera(screenX,screenY);

	/////////////////////////////////////////////////////
    /// LIGHTING PASS
    /////////////////////////////////////////////////////

    glClearColor(sceneData->backgroundColor.r,sceneData->backgroundColor.g,sceneData->backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("pre-Lighting");
    #endif

    for (int i=0;i<(int)sceneData->layerList.size();i++){

        //draw screen space normals into texture
        drawNormals(sceneData->layerList[i]);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("init lighting");
    #endif

        sceneData->layerList[i]->setTextureID(sceneData->layerList[i]->colorTextureID);

		if (bDrawLighting){
            drawDeferredLighting(sceneData->layerList[i]);  //draws lights into texture
        }
        else{
            sceneData->layerList[i]->sceneShaderID="buttonTexture";
        }

    #ifdef BDEBUGRENDERER
    checkOpenGLError("post-Lighting");
    #endif

	/////////////////////////////////////////////////////
    /// COMPOSITE PASS
    /////////////////////////////////////////////////////

        //set our shader to
        sceneData->layerList[i]->sceneShaderID="post";

        //bind depth, normals, lighting and generate mipmaps (if we haven't already)
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sceneData->layerList[i]->depthTex);


        //set shadowTexture (might not have one)
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, lighting_tx);
        glGenerateMipmapEXT(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normal_tx);
        glGenerateMipmapEXT(GL_TEXTURE_2D);


    #ifdef BDEBUGRENDERER
    checkOpenGLError("post-mipmapping FBOs");
    #endif


        //draw into FBO for post-production
        glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, sceneData->layerList[i]->sceneFBO);

        //Draw Background here
       drawBackground();

        //then, draw our composite
        drawButton(sceneData->layerList[i]);

        glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

        //generate mipmaps for scene Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sceneData->layerList[i]->sceneTex);
        glGenerateMipmapEXT(GL_TEXTURE_2D);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("post draw Final Frame");
    #endif



            /////////////////////////////////////////////////////
            /// Post-Production
            /////////////////////////////////////////////////////

            //for final composite, setup viewport to whole window
            glViewport (0, 0, windowX, windowY);

            sceneData->layerList[i]->setTextureID(sceneData->layerList[i]->sceneTextureID);

            if (bDOF)
                sceneData->layerList[i]->sceneShaderID="dof";
            else
                sceneData->layerList[i]->sceneShaderID="buttonTexture";


            //bind depth

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, sceneData->layerList[i]->depthTex);
            drawButton(sceneData->layerList[i]);


    }//end for loop through layers


   setupOrthoCamera(windowX,windowY);


    #ifdef BDEBUGRENDERER
    checkOpenGLError("post draw PostPro-Frame");
    #endif


    /*
    *   DisplayDebug
    */

    glDisable(GL_DEPTH_TEST);

    if (bDrawMenu){

        /*
         *	Draw all Buttons
         */

        draw2D();


        setupShading("font");
        displayDebug();

        #ifdef BDEBUGRENDERER
        checkOpenGLError("post-draw2D");
        #endif
    }
    glutSwapBuffers();
    deltaTime=glutGet(GLUT_ELAPSED_TIME)-currentTime;
    currentTime=glutGet(GLUT_ELAPSED_TIME);

    //cout << "called draw" << " in frame: "<< frames<< endl;



    frames++;
}

void Renderer::drawSceneTexture(){

    glPushAttrib(GL_VIEWPORT_BIT);

    glViewport (0, 0, screenX, screenY);

    glMatrixMode(GL_MODELVIEW);

	float draw3DTime=glutGet(GLUT_ELAPSED_TIME);

    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, multiSample_fb);

    glDrawBuffers(1,drawBuffers);

    //glClearColor( -1.0f, -1.0f, -1.0f, -1.0f );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    for (int i=0;i<(int)sceneData->layerList.size();i++){

        glClear( GL_COLOR_BUFFER_BIT |
                 GL_DEPTH_BUFFER_BIT );


        //disable blending for second buffer

        glDisableIndexedEXT(GL_BLEND,1);

        glActiveTexture(GL_TEXTURE0);

		 //drawbuffers are set up here!
        draw3D(sceneData->layerList[i]);


        //color blitting
        glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, multiSample_fb );
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->colorFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, screenX, screenY, 0, 0, screenX, screenY, GL_COLOR_BUFFER_BIT, GL_NEAREST );

        //meta blitting - zPos and ObjectID
        glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->depthFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, screenX, screenY, 0, 0, screenX, screenY, GL_COLOR_BUFFER_BIT, GL_NEAREST );

        //create mipmaps for color and depth
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sceneData->layerList[i]->colorTex);
        glGenerateMipmapEXT(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, sceneData->layerList[i]->depthTex);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
	}

    //cleanup
    glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 );
    glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, 0 );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

    if (!sceneData->controller->bRunning)
        pick(input->mouseX,input->mouseY);

	draw3DTime=glutGet(GLUT_ELAPSED_TIME) - draw3DTime;

    //TODO:draw brush here?

    glPopAttrib();
    //now draw the resulting image into a quad!
}



void Renderer::drawBackground(){

    glViewport (0, 0, screenX, screenY);
    setupOrthoCamera(screenX,screenY);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //only draw Background Texture if we're having one
    if (sceneData->backgroundTex=="NULL")
        return;

    setupShading("buttonTexture");

    //update objectID!
    shaderObject* myShader= sceneData->shaderList[currentShader];

    if (myShader->uniforms.find("objectID") != myShader->uniforms.end())
        glUniform1fARB(myShader->uniforms["objectID"], 0.0f);

    if (myShader->uniforms.find("bSelected") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["bSelected"], false);

   #ifdef BDEBUGRENDERER
    checkOpenGLError("post-drawBackground");
    #endif

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneData->textureList[sceneData->backgroundTex]->texture);

    glDepthMask(GL_FALSE);
    drawPlane(0,0,screenX,screenY);
    glDepthMask(GL_TRUE);

}

///Screen Space Normals

void Renderer::drawNormals(Layer* layer){


        glViewport (0, 0, screenX/lighting_size,screenY/lighting_size);
         glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, normal_fb);

        glDrawBuffers(1,drawBuffers);

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


        //set our textureID to lighting pass
        layer->setTextureID(layer->depthTextureID);
        //set our shader to
        layer->sceneShaderID="ssNormal";

        //draw using depthTextureID as base texture!
        drawButton(layer);

        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT,0);

        //generate MipMaps for Normals
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, normal_tx);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        for (int i=0;i<normalBlur;i++){
            performShader(layer,"normals","normalsBlurred",normalBlur_fb,"ssBlur");
            performShader(layer,"normalsBlurred","normals",normal_fb,"ssBlur");
        }
}

/// Lighting

void Renderer::drawDeferredLighting(Layer* layer){


        //don't draw lights when calculating light...
        if (!sceneData->controller->bRunning){
            for (int i=0;i<(int)sceneData->lightList.size(); i++){
                sceneData->lightList[i]->drawType=DRAW_NULL;
            }
        }

        bShadowPass=true;
        //preserve our unlit color content
        string oldTextureID=layer->textureID;


        //bind lighting base texture and clear it
        glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, lighting_fb);

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT,0);


        //set up viewport
        glViewport (0, 0, (float)screenX/lighting_size, (float)screenY/lighting_size);

        ///loop from here for every shadowed light!


        for (int i=0;i<(int)sceneData->lightList.size(); i++){


            float castShadow=(float)sceneData->lightList[i]->bCastShadows;
            //update light
            glLightfv(GL_LIGHT0,GL_POSITION,&sceneData->lightList[i]->location.x);
            glLightfv(GL_LIGHT0,GL_DIFFUSE,&sceneData->lightList[i]->color.r);
            glLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION,&sceneData->lightList[i]->lightDistance);
            glLightfv(GL_LIGHT0,GL_SPOT_CUTOFF,&castShadow);


            if (sceneData->lightList[i]->bCastShadows){
                drawShadows(sceneData->lightList[i]);
            }

            #ifdef BDEBUGRENDERER
            checkOpenGLError("post-drawShadow");
            #endif
            //setup 2D camera again!
            setupOrthoCamera(screenX,screenY);

           //bind depth
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, layer->depthTex);

            //set shadowTexture (might not have one) and generate MipMaps
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, shadow_tx);
            glGenerateMipmapEXT(GL_TEXTURE_2D);
            ///light&shadow rendering

            //render lighting pass into lighting FBO
            glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, lighting_fb);

            glDrawBuffers(1,drawBuffers);

            glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

            glClear( GL_DEPTH_BUFFER_BIT );

            //draw using lighting_tx as base texture!

            //set our textureID to lighting pass
            layer->setTextureID("lighting");
            //set our shader to
            layer->sceneShaderID="deferredLight";

            drawButton(layer);

            glBindFramebufferEXT( GL_FRAMEBUFFER_EXT,0);

        }             //repeat for every shadowed light!


        if (!sceneData->controller->bRunning){
            for (int i=0;i<(int)sceneData->lightList.size(); i++){
                sceneData->lightList[i]->drawType=DRAW_SPRITE;
            }
        }

        //set our textureID to lighting pass
        layer->setTextureID(oldTextureID);

        bShadowPass=false;
}


///Shadow Rendering

void Renderer::drawShadows(MsbLight* myLight){

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport (0, 0, screenX/shadow_size, screenY/shadow_size);

    //setup projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glPushMatrix();

    //set perspective

        gluPerspective(myLight->fov, 1.0f, nearClip, myLight->lightDistance);             //this sets the framing of the light!

        glGetFloatv(GL_PROJECTION_MATRIX,lightProjectionMatrix);
        lightProjectionMatrixInverse=lightProjectionMatrix.inverse();

        //setup camera
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();

            Vector3f lAxis = myLight->location+myLight->zAxis;
            //lAxis=myLight->zAxis;
            gluLookAt( myLight->location.x, myLight->location.y, myLight->location.z,
            lAxis.x, lAxis.y, lAxis.z,
            0.0f, 1.0f, 0.0f);

            glGetFloatv(GL_MODELVIEW_MATRIX,lightViewMatrix);
            lightViewMatrixInverse=lightViewMatrix.inverse();


    //glDisable(GL_BLEND);
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, multiSample_fb);
	glDrawBuffers(2,drawBuffers);
    //draw all objects of all layers for this light
    //TODO: this does a double loop through all layers!
    for (int i=0;i<(int)sceneData->layerList.size();i++){

        glClearColor( -1.0f, -1.0f, -1.0f, -1.0f );

        glClear( GL_COLOR_BUFFER_BIT |
                 GL_DEPTH_BUFFER_BIT );

        draw3D(sceneData->layerList[i]);


      glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, multiSample_fb );
      glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);

      glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, shadow_fb );
      glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

      glBlitFramebufferEXT( 0, 0, screenX/shadow_size, screenY/shadow_size, 0, 0, screenX/shadow_size, screenY/shadow_size, GL_COLOR_BUFFER_BIT, GL_LINEAR );

    }
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

        glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();

 }



///blur Pass

void Renderer::performShader(Layer* layer, string sourceTextureID, string destinationTextureID, GLuint renderFBO, string shaderName){


        glViewport (0, 0, screenX/lighting_size,screenY/lighting_size);
        glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, renderFBO);

        glDrawBuffers(1,drawBuffers);

        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //set our textureID to
        layer->setTextureID(sourceTextureID);

        //set our shader to
        layer->sceneShaderID=shaderName;

        //draw
        drawButton(layer);

        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT,0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sceneData->textureList[destinationTextureID]->texture);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

/*
        glViewport (0, 0, screenX/lighting_size,screenY/lighting_size);
         glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, normalBlur_fb);

        glDrawBuffers(1,drawBuffers);

        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


        //set our textureID to lighting pass
        layer->setTextureID(layer->depthTextureID);
        //set our shader to
        layer->sceneShaderID="ssNormal";

        //draw using depthTextureID as base texture!
        drawButton(layer);

        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT,0);

        //generate MipMaps for Normals
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, normalBlur_tx);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
*/
}



void Renderer::draw3D(Layer* currentLayer){

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDrawBuffers(2, drawBuffers);


    //draw color

    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
        //update matrices when hidden
        if(currentLayer->actorList[i]->bHidden){
            glPushMatrix();
            transformActorMatrix(currentLayer->actorList[i]);
            glPopMatrix();
        }
        else {
            //draw in all buffers for pickable actors
            if (currentLayer->actorList[i]->bPickable){
                drawActor(currentLayer->actorList[i]);
            }else{
            //only draw in color buffer for non-pickable actors
                glDrawBuffers(1, drawBuffers);
                drawActor(currentLayer->actorList[i]);
                glDrawBuffers(2, drawBuffers);
            }
            //don't draw in picking buffer for non-pickable actors
        }
    }



    #ifdef BDEBUGRENDERER
    checkOpenGLError("draw3D draw regular...");
    #endif


    //draw non-pickable actors afterwards!
    //used for drawings while drawing, so they're visible
    /*
    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
        if (!currentLayer->actorList[i]->bPickable){
            glDrawBuffers(1, drawBuffers);
            drawActor(currentLayer->actorList[i]);
            glDrawBuffers(2, drawBuffers);
        }
    }
    */
   //leave after this one if we're in shadow pass!
    if (bShadowPass){
        //reset texture Matrix transform
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        return;
    }
    #ifdef BDEBUGRENDERER
    checkOpenGLError("draw3D draw non-pickable...");
    #endif

	//draw helpers - brush, grid, actorGizmo, etc... if we're not running
    if (!sceneData->controller->bRunning){

        for (int i=0;i<(int)sceneData->helperList.size();i++){
            if (!sceneData->helperList[i]->bHidden){

                if (sceneData->helperList[i]->bPickable){
                    //no backface stuff for brush!
                    drawActor(sceneData->helperList[i]);
                    glDrawBuffers(2, drawBuffers);
                }
                else{
                    //don't draw in Z or draw normals if we're not pickable!
                    glDrawBuffers(1, drawBuffers);
                    drawActor(sceneData->helperList[i]);
                    glDrawBuffers(2, drawBuffers);
                }
            }
        }

    }

    #ifdef BDEBUGRENDERER
    checkOpenGLError("draw3D draw helpers...");
    #endif

    //reset texture Matrix transform
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    //this for xyz axis
    if (!sceneData->controller->bRunning){

        setupShading("color");

        for (int i=0;i<(int)currentLayer->actorList.size();i++){
                if (currentLayer->actorList[i]->name!="ground" && currentLayer->actorList[i]->name!="grid") {                //don't draw gizmos for ground and grid!
                    drawGizmos(currentLayer->actorList[i]);
                }
        }

    }

    #ifdef BDEBUGRENDERER
    checkOpenGLError("draw3D draw Orientation...");
    #endif

}

void Renderer::draw2D(){


    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);



    //colored Buttons  first
    for (unsigned int i=0;i<sceneData->buttonList.size();i++){
        if (    (!Control::bRunning || sceneData->buttonList[i]->bScreenOverlay)
                && !sceneData->buttonList[i]->bHidden){

                    Node* myNode=dynamic_cast<Node*>(sceneData->buttonList[i]);
                    NodeIO* myNodeIO=dynamic_cast<NodeIO*>(sceneData->buttonList[i]);
                    //if we disabled drawing nodes...
                    if ((myNode || myNodeIO) && bDrawNodes)
                        drawButton(sceneData->buttonList[i]);
                    //always draw everything else
                    if (!myNode && !myNodeIO)
                        drawButton(sceneData->buttonList[i]);
                }
    }

    //finally font rendering
    setupShading("font");


    for (unsigned int i=0;i<sceneData->buttonList.size();i++){
        if (    (!Control::bRunning || sceneData->buttonList[i]->bScreenOverlay)
                && !sceneData->buttonList[i]->bHidden){

                Node* myNode=dynamic_cast<Node*>(sceneData->buttonList[i]);
                NodeIO* myNodeIO=dynamic_cast<NodeIO*>(sceneData->buttonList[i]);

                if ((myNode || myNodeIO) && bDrawNodes)
                    sceneData->buttonList[i]->drawTooltip();

                if (!myNode && !myNodeIO)
                    sceneData->buttonList[i]->drawTooltip();
        }
    }
}

void Renderer::drawButton(BasicButton* b){

    //set Shader
    setupShading(b->sceneShaderID);

    b->updateShaders();

    //set Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneData->textureList[b->textureID]->texture);

    transformTextureMatrix(b);
    glPushMatrix();

    //regular buttons only translate
    if (b->drawType==DRAW_PLANE){
        glTranslatef(b->location.x,b->location.y,b->location.z);
        drawPlane(0.0, 0.0, b->scale.x, b->scale.y, b->color);
        b->drawPlane();
    }
   //3D buttons do much more!
    if (b->drawType==DRAW_TEA){
        transformActorMatrix(b);
        b->drawTeapot();
    }
    if (b->drawType==DRAW_CUBE){
        transformActorMatrix(b);   //3D buttons do much more!
        drawCube(1.0);
    }
    if (b->drawType==DRAW_VBOMESH){
        transformActorMatrix(b);   //3D buttons do much more!
        drawColladaMesh(b);
    }

    glPopMatrix();

}

void Renderer::drawActor(Actor* a){


    if (a->bTextured)
        setupTexturing(a->textureID, a);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("drawActor Texturing...");
    #endif

    //alpha blending
    glBlendFunc(a->blendModeOne,a->blendModeTwo);
    //glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

    //if (!bShadowPass)
    //    glBlendFuncSeparate(a->blendModeOne,a->blendModeOne,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //else
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("drawActor Blending Setup...");
    #endif

    //start translating
    glPushMatrix();

    //translate according to base
    transformActorMatrix(a);

    //shader
    setupShading(a->sceneShaderID);
    a->updateShaders();


    #ifdef BDEBUGRENDERER
    checkOpenGLError("drawActor Shader Update...");
    #endif

    if (!a->bZTest)  glDisable(GL_DEPTH_TEST);
    if (!a->bZWrite) glDepthMask(GL_FALSE);
    //set Color
    glColor4f(a->color.r,a->color.g,a->color.b,a->color.a);


    //Actual Drawing takes place here!
        if (a->drawType==DRAW_PLANE)            drawPlane(0.0,0.0,a->scale.x, -a->scale.y, a->color,true);
        else if (a->drawType==DRAW_VBOMESH)     drawColladaMesh(a);

        else if (a->drawType==DRAW_PARTICLES)   drawParticles(a);                 //Particles
        else if (a->drawType==DRAW_SPRITE)      a->drawSprite();
        else if (a->drawType==DRAW_CUBE)        drawCube(a->scale.x);                 //Mesh
        else if (a->drawType==DRAW_TEA)         a->drawTeapot();
        else if (a->drawType==DRAW_SPECIAL)     a->draw();
        //else if (a->drawType==DRAW_POINTPATCH)  drawPatch(a->scale.x,a->scale.x,a->particleScale);
        else if (a->drawType==DRAW_POINTPATCH){
                                                drawPatch(a->scale.x, a->scale.y,a->particleAngleScale);
                                               }
        else if (a->drawType==DRAW_KINECT){
                                                a->vboMeshID="kinectWall";
                                                drawColladaMesh(a);
                                               }


    if (!a->bZTest)  glEnable(GL_DEPTH_TEST);
    if (!a->bZWrite) glDepthMask(GL_TRUE);

    #ifdef BDEBUGRENDERER
    checkOpenGLError("drawActor actual draw...");
    #endif

    //end translation
    glPopMatrix();
}

void Renderer::drawGizmos(Actor* a){

    glPushMatrix();


    //draw line to base
    if (a->base){
        glMultMatrixf(a->base->baseMatrix);
        drawLine(Vector3f(0,0,0),(a->originalMatrix * a->transformMatrix).getTranslation(),Vector4f(1,1,1,1));
        glMultMatrixf(a->originalMatrix);
        glMultMatrixf(a->transformMatrix);
    }
    else{
        transformActorMatrix(a);
    }

    //save lighting information, but turn lighting off for now
    bool bComputeLight=a->bComputeLight;
    a->bComputeLight=false;

    a->updateShaders();

    glDrawBuffers(1,drawBuffers);

    if (a->bSelected && sceneData->controller->currentTool!=sceneData->navTool){
            if (a->drawType==DRAW_VBOMESH || a->drawType==DRAW_PARTICLES)
                drawBoundingBox(a->lowerLeftBack,a->upperRightFront,Vector4f(1,0,0,1));
            else
                drawBoundingBox(Vector3f(-a->scale.x,-a->scale.y,-a->scale.z)  ,Vector3f(a->scale.x,a->scale.y,a->scale.z),Vector4f(1,0,0,1));
    }

    else if (a->bHighlight && sceneData->controller->currentTool!=sceneData->navTool){
            if (a->drawType==DRAW_VBOMESH || a->drawType==DRAW_PARTICLES)
                drawBoundingBox(a->lowerLeftBack,a->upperRightFront,Vector4f(0.6,0.6,0.6,1));
            else
                drawBoundingBox(Vector3f(-a->scale.x,-a->scale.y,-a->scale.z)  ,Vector3f(a->scale.x,a->scale.y,a->scale.z),Vector4f(0.6,0.6,0.6,1));
    }

    glDrawBuffers(2,drawBuffers);


    //restore lighting information
    a->bComputeLight=bComputeLight;

    glPopMatrix();

    #ifdef BDEBUGRENDERER
    checkOpenGLError("individual Orientation...");
    #endif


}


void Renderer::draw3DOverlay(){

      //reset blending
        glDisable(GL_BLEND);

        //glViewport(0,0,screenX,screenY);
        glViewport(screenX,0,screenX,screenY);
        //TODO: should be drawSFX
        glActiveTexture(GL_TEXTURE2);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sceneData->textureList["leftEyeDepthTexture"]->texture);
        glActiveTexture(GL_TEXTURE0);


        glLoadIdentity();
//translate all the way back!
//glTranslatef(bkgOffset * offset,0,0.0);

setupShading("post");

    GLuint uniform_location=0;
    uniform_location = glGetUniformLocation(sceneData->shaderList["post"]->shader, "tex");
    glUniform1iARB(uniform_location, 0);

    uniform_location=0;
    uniform_location = glGetUniformLocation(sceneData->shaderList["post"]->shader, "depthTex");
    glUniform1iARB(uniform_location, 2);

    //zoom!
    float zoom3Dx=0.0, zoom3Dy=0.0;

    glTranslatef(640,512,0);
    glScalef(zoom3Dx,zoom3Dy,1.0);
    glTranslatef(-640,-512,0);

//do for all FBOs too!

    glBindTexture(GL_TEXTURE_2D, sceneData->textureList["leftEyeTexture"]->texture);

    glColor4f(1.0,1.0,1.0,1.0);

/*
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,1.0);
		glVertex3f(leftTopXLeftEye ,leftTopYLeftEye,0);
        glTexCoord2f(1.0,1.0);
		glVertex3f(rightTopXLeftEye,rightTopYLeftEye,0.0);
        glTexCoord2f(1.0,0.0);
		glVertex3f(rightBottomXLeftEye,rightBottomYLeftEye,0.0);
        glTexCoord2f(0.0,0.0);
		glVertex3f(leftBottomXLeftEye,leftBottomYLeftEye,0.0);
	glEnd();



 //       drawButton(leftEyeFBO);


        glViewport(2*screenX,0,screenX,screenY);
        //glViewport(screenX,0,screenX,screenY);
        //TODO: should be drawSFX

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, sceneData->textureList["rightEyeDepthTexture"]->texture);
        glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, sceneData->textureList["rightEyeTexture"]->texture);

    glColor4f(1.0,1.0,1.0,1.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0,1.0);
		glVertex3f(leftTopXRightEye ,leftTopYRightEye,0);
        glTexCoord2f(1.0,1.0);
		glVertex3f(rightTopXRightEye,rightTopYRightEye,0.0);
        glTexCoord2f(1.0,0.0);
		glVertex3f(rightBottomXRightEye,rightBottomYRightEye,0.0);
        glTexCoord2f(0.0,0.0);
		glVertex3f(leftBottomXRightEye ,leftBottomYRightEye,0.0);
	glEnd();
*/
//        drawButton(rightEyeFBO);

        //reset viewport
        glViewport(0,0,screenX,screenY);
        glLoadIdentity();
        //reset blending
        glEnable(GL_BLEND);
}



void Renderer::displayDebug(){

    double allDeltaTime=0.0;

    //shift buffer
    for (int i=FPSBUFFERSIZE-1;i>0;i--)
        fpsBuffer[i]=fpsBuffer[i-1];

    //update buffer
    fpsBuffer[0]=deltaTime;

    //calculate buffer
    for (int i=0;i<FPSBUFFERSIZE;i++)
      allDeltaTime+=fpsBuffer[i];

    allDeltaTime=allDeltaTime/FPSBUFFERSIZE;

    char writestring[30];

/*    for (int i=0;i<30;i++){
        writestring[i]=' ';
    }
*/
    sprintf(writestring,"FPS: %4.2f",1000.0/deltaTime);
    drawText(writestring,windowX-80*0.9,10 );


    //displaying the last couple of debug messages
    int listEnd=(int)sceneData->debugMessages.size()-1;

    for (int i=listEnd;i>0;i--)
        drawText(sceneData->debugMessages[i],windowX-300*0.9,100 - (listEnd-i)*10 );

    return;
}


/****************************************
*
*   transforms, textures and Shaders
*
*****************************************/


void Renderer::setupShading(string shaderName){

    if (shaderName!=lastShader && sceneData->shaderList[shaderName]){
        glUseProgram(sceneData->shaderList[shaderName]->shader);
        lastShader=shaderName;
        }
    if (!sceneData->shaderList[shaderName]){
        cout << "found bad shader: " << shaderName << endl;
        return;
    }
    currentShader=shaderName;

}

void Renderer::setupTexturing(string texName, Actor* a, GLenum texChannel){

    if (!a )
        return;

    if (sceneData->textureList[texName])
        glBindTexture(GL_TEXTURE_2D, sceneData->textureList[texName]->texture);
    else
        return;


    //texture animation
    if (sceneData->textureList[texName]->nextTexture!="NULL" && currentTime - a->textTimer > sceneData->textureList[texName]->frameRate ){
        a->textTimer += sceneData->textureList[texName]->frameRate;
        a->setTextureID(sceneData->textureList[texName]->nextTexture);
    }

    transformTextureMatrix(a);

}



void Renderer::transformActorMatrix(Actor* a){


    glMultMatrixf(a->baseMatrix);

    a->orientation=a->location+a->zAxis;
}

void Renderer::transformTextureMatrix(Actor* a){

    glActiveTexture(GL_TEXTURE0);
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();

        // make changes to the texture
        glTranslatef(a->texTranslation.x,a->texTranslation.y,a->texTranslation.z);

        glRotatef(a->texRotation.x,1,0,0);
        glRotatef(a->texRotation.y,0,1,0);
        glRotatef(a->texRotation.z,0,0,1);

        glScalef(a->texScale.x,a->texScale.y,a->texScale.z);

    glMatrixMode(GL_MODELVIEW);
}


/****************************************
*
*   Basic Shape drawing
*
*****************************************/

void Renderer::drawBone(float width, float height, float depth){


}

void Renderer::drawCube(float scale){

    glEnable(GL_DEPTH_TEST);


    //taken from: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
    GLfloat vertices[] = {scale,scale,scale,  -scale,scale,scale,  -scale,-scale,scale,  scale,-scale,scale,        // v0-v1-v2-v3
                          scale,scale,scale,  scale,-scale,scale,  scale,-scale,-scale,  scale,scale,-scale,        // v0-v3-v4-v5
                          scale,scale,scale,  scale,scale,-scale,  -scale,scale,-scale,  -scale,scale,scale,        // v0-v5-v6-v1
                          -scale,scale,scale,  -scale,scale,-scale,  -scale,-scale,-scale,  -scale,-scale,scale,    // v1-v6-v7-v2
                          -scale,-scale,-scale,  scale,-scale,-scale,  scale,-scale,scale,  -scale,-scale,scale,    // v7-v4-v3-v2
                          scale,-scale,-scale,  -scale,-scale,-scale,  -scale,scale,-scale,  scale,scale,-scale};   // v4-v7-v6-v5
    // color array

    GLfloat colors[] = {1,0,0,  1,0,0,  1,0,0,  1,0,0,              // v0-v1-v2-v3
                        0,1,0,  0,1,0,  0,1,0,  0,1,0,              // v0-v3-v4-v5
                        0,0,1,  0,0,1,  0,0,1,  0,0,1,              // v0-v5-v6-v1
                        1,1,0,  1,1,0,  1,1,0,  1,1,0,              // v1-v6-v7-v2
                        0,1,1,  0,1,1,  0,1,1,  0,1,1,              // v7-v4-v3-v2
                        1,0,1,  1,0,1,  1,0,1,  1,0,1};             // v4-v7-v6-v5

        glEnableClientState( GL_VERTEX_ARRAY );
//        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY);

//    	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
        glVertexPointer(3, GL_FLOAT, 0, vertices );
        glColorPointer(3,GL_FLOAT,0, colors);
        glDrawArrays( GL_QUADS, 0, 24 );

        glDisableClientState( GL_VERTEX_ARRAY );
//		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisableClientState( GL_COLOR_ARRAY);

    //    glutSolidCube( cubeSize / max(scale , 1.0f) );
}

void Renderer::drawBoundingBox(Vector3f lowerLeftBack,Vector3f upperRightFront, Vector4f color){

        GLfloat verts[] = { //lower rectangle
                            lowerLeftBack.x, lowerLeftBack.y, lowerLeftBack.z,          //1
                            upperRightFront.x, lowerLeftBack.y, lowerLeftBack.z,        //2
                            upperRightFront.x, lowerLeftBack.y, upperRightFront.z,      //3
                            lowerLeftBack.x, lowerLeftBack.y, upperRightFront.z,        //4
                            lowerLeftBack.x, lowerLeftBack.y, lowerLeftBack.z,          //5

                            //up
                            lowerLeftBack.x, upperRightFront.y, lowerLeftBack.z,        //6

                            //upper rectangle
                            lowerLeftBack.x, upperRightFront.y, upperRightFront.z,      //7
                            upperRightFront.x, upperRightFront.y, upperRightFront.z,    //8
                            upperRightFront.x, upperRightFront.y, lowerLeftBack.z,      //9
                            lowerLeftBack.x, upperRightFront.y, lowerLeftBack.z,        //10

                            //missing lines - some backtracking...
                            upperRightFront.x, upperRightFront.y, lowerLeftBack.z,      //11
                            upperRightFront.x, lowerLeftBack.y, lowerLeftBack.z,        //12
                            upperRightFront.x, lowerLeftBack.y, upperRightFront.z,      //13
                            upperRightFront.x, upperRightFront.y, upperRightFront.z,    //14
                            lowerLeftBack.x, upperRightFront.y, upperRightFront.z,      //15
                            lowerLeftBack.x, lowerLeftBack.y, upperRightFront.z,        //16

                            };

        glColor4f(color.r,color.g,color.b,color.a);

        glEnableClientState( GL_VERTEX_ARRAY );

		glVertexPointer(3, GL_FLOAT, 0, &verts[0] );

		glDrawArrays( GL_LINE_STRIP, 0, 16 );

        glDisableClientState( GL_VERTEX_ARRAY );

}


void Renderer::drawPlane(float x1,float  y1,float  x2,float  y2, Vector4f color, bool bCentered){



	//draw centered!
        float xOffset=0.0;
        float yOffset=0.0;

        if (bCentered){
            xOffset=(x2-x1)/2.0;
            yOffset=(y2-y1)/2.0;
        }

/*
                int h=10;
                int w=10;
                int step = 1;
                vector<GLfloat> vert;
                vector<GLfloat> tex;
                vector<GLfloat> col;

                for(int y = 0; y < h - step; y += step) {
                        for(int x = 0; x < w; x += step) {
                                tex.push_back(x);
                                tex.push_back(y);

                                vert.push_back(x);
                                vert.push_back(y);

                                col.push_back(color.r);
                                col.push_back(color.g);
                                col.push_back(color.b);
                                col.push_back(color.a);


                                tex.push_back(x);
                                tex.push_back(y + step);

                                vert.push_back(x);
                                vert.push_back(y + step);

                                col.push_back(color.r);
                                col.push_back(color.g);
                                col.push_back(color.b);
                                col.push_back(color.a);

                        }
                }

        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
     //   glEnableClientState( GL_NORMAL_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY);

    	glTexCoordPointer(2, GL_FLOAT, 0, &tex[0] );
		glVertexPointer(2, GL_FLOAT, 0, &vert[0] );
        glColorPointer(4,GL_FLOAT,0, &col[0]);

        cout << "vertices: " << vert.size() << endl;
		glDrawArrays( GL_TRIANGLE_FAN, 0, vert.size()/2.0 );

*/
        GLfloat verts[] = { x1-xOffset, y1-yOffset,
                            x2-xOffset, y1-yOffset,
                            x2-xOffset, y2-yOffset,
                            x1-xOffset, y2-yOffset };

		GLfloat tex_coords[] = { 0, 0,
                                 1, 0,
                                 1, 1,
                                 0, 1 };

        GLfloat vColor[] ={ color.r, color.g, color.b, color.a,
                            color.r, color.g, color.b, color.a,
                            color.r, color.g, color.b, color.a,
                            color.r, color.g, color.b, color.a };

        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
     //   glEnableClientState( GL_NORMAL_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY);

    	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glVertexPointer(2, GL_FLOAT, 0, verts );
        //glNormalPointer(GL_FLOAT, 0, normals );
        glColorPointer(4,GL_FLOAT,0, vColor);
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

        glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        //glDisableClientState( GL_NORMAL_ARRAY );
        glDisableClientState( GL_COLOR_ARRAY);

}

void Renderer::drawPatch(float width, float height, float resolution){

    //create a vertex array for a quad patch with "resolution" amount of vertices per side
    //lets do points for now...

    vector<Vector4f> vertices;
    vector<GLfloat> texCoords;

    for (int h=0;h<resolution;h++){

        //for every line...
        for (int l=0;l<resolution;l++){
            Vector4f myVertex;
            myVertex.x=float(l) * width/(resolution-1.0) - width/2.0f;            //x-coord
            myVertex.y=float(h) * height/(resolution-1.0) - height/2.0f;
            myVertex.z=0.0f;
            myVertex.w=0.01f;

            vertices.push_back(myVertex);


            texCoords.push_back( float(l) /(resolution-1.0) );        //x-texCoord
            texCoords.push_back( float(h) /(resolution-1.0) );        //y-texCoord
        }
    }


    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    GLfloat *verts=&vertices[0].x;

    glVertexPointer(4, GL_FLOAT, 0, verts);
    glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);

    // draw the patch as points
    glDrawArrays(GL_POINTS, 0, resolution* resolution );

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::drawLine(Vector3f start, Vector3f end, Vector4f startColor, Vector4f endColor){

    glLineWidth(4.0);

    glBegin(GL_LINES);
        glColor4f(startColor.r,startColor.g,startColor.b,startColor.a);
        glVertex3f(start.x,start.y,start.z);
        glColor4f(endColor.r,endColor.g,endColor.b,endColor.a);
        glVertex3f(end.x,end.y,end.z);
    glEnd();

}


void Renderer::drawParticles (Actor* a){

    MeshData* myMesh=sceneData->vboList[a->vboMeshID];

    if (!myMesh && myMesh->vData.size()==0)
        return;

    if (myMesh->meshType==MESH_POINTSPRITE)
        glEnable( GL_POINT_SPRITE_ARB );

    if (myMesh->vData.size()>0){

          GLfloat   *vertexIDs=&myMesh->vData[0].vertexID;

          GLfloat *verts=&myMesh->vData[0].location.x;
          //GLfloat *normals=&myMesh->vData[0].normal.x;
          GLfloat *colors=&myMesh->vData[0].color.r;
          //GLfloat *secondaryColors=&myMesh->vData[0].secondaryColor.r;
          GLfloat *vertexWeights=&myMesh->vData[0].vertexWeights.x;
          GLfloat *boneReferences=&myMesh->vData[0].boneReferences.x;



          glEnableClientState(GL_VERTEX_ARRAY);
          //glEnableClientState(GL_NORMAL_ARRAY);
          glEnableClientState(GL_COLOR_ARRAY);
          //glEnableClientState(GL_SECONDARY_COLOR_ARRAY);

		  glVertexPointer(4, GL_FLOAT,sizeof(myMesh->vData[0]),verts);
          //glNormalPointer(GL_FLOAT,sizeof(myMesh->vData[0]),normals);
		  glColorPointer(4, GL_FLOAT,sizeof(myMesh->vData[0]),colors);
		  //glSecondaryColorPointer(3, GL_FLOAT,sizeof(myMesh->vData[0]),secondaryColors);

          #ifdef BDEBUGRENDERER
          checkOpenGLError("drawParticles Array Pointers...");
          #endif


			//vertexID from here - if we're using a shader on a drawing that does not support vertexID
            //TODO: load attribs in shaders so no repeating lookup necessary!
            GLint indexThree;
            indexThree=glGetAttribLocation(sceneData->shaderList[currentShader]->shader,"vertexID");
            if (indexThree>-1){
                glEnableVertexAttribArray(indexThree);
                glVertexAttribPointer(indexThree,1,GL_FLOAT,false,sizeof(myMesh->vData[0]),vertexIDs);

                #ifdef BDEBUGRENDERER
                checkOpenGLError("drawParticles vertexID...");
                #endif
            }

            GLint indexOne,indexTwo;

            //skeletal Stuff from here

            if (myMesh->bIsSkeletal && sceneData->controller->tool==TOOL_SKIN && currentShader=="skinning"){
                indexOne=glGetAttribLocation(sceneData->shaderList["skinning"]->shader,"boneReferences");
                glEnableVertexAttribArray(indexOne);
                glVertexAttribPointer(indexOne,4,GL_FLOAT,false,sizeof(myMesh->vData[0]),boneReferences);
                indexTwo=glGetAttribLocation(sceneData->shaderList["skinning"]->shader,"vertexWeights");
                glEnableVertexAttribArray(indexTwo);
                glVertexAttribPointer(indexTwo,4,GL_FLOAT,false,sizeof(myMesh->vData[0]),vertexWeights);

                #ifdef BDEBUGRENDERER
                checkOpenGLError("drawParticles Skinning...");
                #endif
            }



          glDrawArrays(GL_POINTS,0,myMesh->vData.size());

          glDisableClientState(GL_VERTEX_ARRAY);
          //glDisableClientState(GL_NORMAL_ARRAY);
          glDisableClientState(GL_COLOR_ARRAY);
          //glDisableClientState(GL_SECONDARY_COLOR_ARRAY);

          if (indexThree>-1)
            glDisableVertexAttribArray(indexThree);

          if (myMesh->bIsSkeletal && sceneData->controller->tool==TOOL_SKIN){

                glDisableVertexAttribArray(indexOne);
                glDisableVertexAttribArray(indexTwo);
          }
      }

      if (myMesh->meshType==MESH_POINTSPRITE)
        glDisable(GL_POINT_SPRITE_ARB);
}

void Renderer::drawColladaMesh (Actor* a){

    MeshData* myMesh=sceneData->vboList[a->vboMeshID];

    if (!myMesh || myMesh->vertexBufferObject.size()==0)
       return;

	glPushMatrix();


        glEnableClientState(GL_VERTEX_ARRAY);
		//TODO: no more normals!
		//glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, myMesh->vertexBufferObject[0]);
        glVertexPointer(myMesh->verticesPerShapeCount, GL_FLOAT, 0, 0);


/*
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, myMesh->normalBufferObject[0]);
        glNormalPointer(GL_FLOAT, 0, 0);
*/

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, myMesh->texCoordBufferObject[0]);
        glTexCoordPointer(myMesh->texCoordPerVertexCount, GL_FLOAT, 0, 0);

        if (myMesh->colorBufferObject.size()>0){

            glEnableClientState(GL_COLOR_ARRAY);
            //TODO: no more secondary colors!

			//glEnableClientState(GL_SECONDARY_COLOR_ARRAY);

            glBindBufferARB(GL_ARRAY_BUFFER_ARB, myMesh->colorBufferObject[0]);
            glColorPointer(4, GL_FLOAT, 0, 0);

			//glBindBufferARB(GL_ARRAY_BUFFER_ARB, myMesh->secondaryColorBufferObject[0]);
			//glSecondaryColorPointer(3,GL_FLOAT, 0, 0);
        }



        //skinning from here!
        //use two index arrays for skinning
        GLint indexOne,indexTwo;

        if (myMesh->bIsSkeletal){

            indexOne=glGetAttribLocation(sceneData->shaderList["skeletal"]->shader,"boneReferences");
			glEnableVertexAttribArray(indexOne);
			glBindBufferARB(GL_ARRAY_BUFFER, myMesh->boneReferenceObject[0]);
			glVertexAttribPointer(indexOne,4,GL_FLOAT,false,0,0);

			indexTwo=glGetAttribLocation(sceneData->shaderList["skeletal"]->shader,"vertexWeights");
			glEnableVertexAttribArray(indexTwo);
			glBindBufferARB(GL_ARRAY_BUFFER, myMesh->vertexWeightsObject[0]);
			glVertexAttribPointer(indexTwo,4,GL_FLOAT,false,0,0);
		}




		glDrawArrays(myMesh->vertexInterpretation, 0, myMesh->vertexCount[0]);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

        glDisableClientState(GL_VERTEX_ARRAY);
        //glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        if (myMesh->bIsSkeletal){
        	glDisableVertexAttribArray(indexOne);
			glDisableVertexAttribArray(indexTwo);
            }

        if (myMesh->colorBufferObject.size()>0){
                glDisableClientState(GL_COLOR_ARRAY);
                //glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        }

    glPopMatrix();

}


void Renderer::drawSprite(){

    glBegin(GL_POINTS);
    glVertex3f(0,0,0);
    glEnd();
}

//Display and Textdrawing
void Renderer::drawText(string str, float x, float y){



    glColor4f(1.0,1.0,1.0,1.0);
    verdana->drawString(str, x, y);
    #ifdef BDEBUGRENDERER
    checkOpenGLError("post-Font:", false);
    #endif

}




void Renderer::createKinectWall( float resolution ){


    //delete(sceneData->vboList["kinectWall"]);
    //create a vertex array for a quad patch with "resolution" amount of vertices per side
    //lets do points for now...

    //4:3 for kinect...
    GLuint vertexBuffer=0;
    GLuint normalBuffer=0;
    GLuint texCoordBuffer=0;

    vector<Vector4f> vertices;
    vector<Vector3f> normals;
    vector<Vector3f> texCoords;

	sceneData->vboList["kinectWall"]=new MeshData;
    MeshData* myMesh=sceneData->vboList["kinectWall"];
    myMesh->bIsSkeletal=false;
    myMesh->bIsHead=false;
    myMesh->bVertexColor=false;
    myMesh->boneCount=0;
    myMesh->texCoordPerVertexCount=3;
    myMesh->verticesPerShapeCount=4;
    myMesh->vertexInterpretation=GL_POINTS;
    myMesh->meshType=MESH_VBO;

    float width=1.5;
    float height=1.5;

    for (int h=0;h<resolution;h++){

        //for every line...
        for (int l=0;l<resolution;l++){
            Vector4f myVertex;
            myVertex.x=float(l) * width/(resolution-1.0) - width/2.0f;            //x-coord
            myVertex.y=float(h) * height/(resolution-1.0) - height/2.0f;
            myVertex.z=0.0f;
            myVertex.w=0.01f;

            vertices.push_back(myVertex);
            normals.push_back(Vector3f(0,0,-1));
            texCoords.push_back(Vector3f( (float(l) /(resolution-1.0) ), ( float(h) /(resolution-1.0) ), 0.0 ));
        }
    }

    float vertexCount=vertices.size();

    cout << "setting up vertexBuffer" << endl;

	//vertex buffer
    glGenBuffersARB(1, &vertexBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector4f), &vertices[0].x , GL_STATIC_DRAW_ARB);


    sceneData->vboList["kinectWall"]->vertexBufferObject.push_back(vertexBuffer);
	sceneData->vboList["kinectWall"]->vertexCount.push_back(vertexCount);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    //creating vertexBuffer here!
    glGenBuffersARB(1, &texCoordBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &texCoords[0].x , GL_STATIC_DRAW_ARB);

    sceneData->vboList["kinectWall"]->texCoordBufferObject.push_back(texCoordBuffer);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    //normal Buffer
    glGenBuffersARB(1, &normalBuffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalBuffer);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(Vector3f), &normals[0].x , GL_STATIC_DRAW_ARB);

    sceneData->vboList["kinectWall"]->normalBufferObject.push_back(normalBuffer);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

/************************************************************
//
//Picking: determining what Actor the mouse points to
//         and mouse 3d coordinate
//
************************************************************/


//picking needs mouse coordinates
void Renderer::pick(int x, int y){

    //set worldTarget to NULL
    input->worldTarget=NULL;

    ///World Position and object ID
    //draw pickTex of current layer, just on mouse coordinate, one pixel wide
    //read pixel color at mouse coordinate

    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, sceneData->layerList[sceneData->currentLayer]->depthFBO);
    float mousePos[4];
    float centerInfo[4];

    //create small picking texture
    glBindTexture(GL_TEXTURE_2D,pickTexture);

    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,(int) (input->mouseX  * screenX/windowX ) ,(int) (screenY-(input->mouseY  * screenY/windowY)  ),1 ,1 );
    glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,dataType,&mousePos);

    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,(int) (screenX/2.0),(int) (screenY/2.0) ,1 ,1 );
    glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,dataType,&centerInfo);

    //Shader writes data as follows:
    //gl_FragData[1]=vec4(zPos,zPos,objectID,objectID);

    Vector2f vec=Vector2f(mousePos[2],mousePos[1]);
	float zPos= (vec.y + vec.x * 1.0/255.0) * farClip;

    Vector2f obj=Vector2f(mousePos[0],mousePos[3]);
	//float raw = ((obj.y + obj.x/255.0) * 1024.0 -100.0);
	int ob = floor( (obj.y + obj.x/255.0) * 1024.0 -100.0 +0.5);    //the + 0.5 at the end will make sure that we round!


    //cout << "objectID: " << raw << endl;

    ///Picking
    //get ObjectID and find worldTarget
    if (ob>=0){
        if ((int) sceneData->actorList.size() > ob)
            input->worldTarget=sceneData->actorList[ob];
    }
    else
        input->worldTarget=NULL;

    ///Picking Helpers
    //grid
    if ((int)ob==-2)
        input->worldTarget=sceneData->grid;

    //Actor Gizmo
    if (ob==-10)
        input->worldTarget=sceneData->aGizmo->xAxisGizmo;
    if (ob==-11)
        input->worldTarget=sceneData->aGizmo->yAxisGizmo;
    if (ob==-12)
        input->worldTarget=sceneData->aGizmo->zAxisGizmo;


    if (ob==-20)
        input->worldTarget=sceneData->aGizmo->xRotateGizmo;
    if (ob==-21)
        input->worldTarget=sceneData->aGizmo->yRotateGizmo;
    if (ob==-22)
        input->worldTarget=sceneData->aGizmo->zRotateGizmo;

    ///Mouse 3D Position


    float windowAspect = (float)windowX/(float)windowY;

    float mouseX =(float)input->mouseX * (float)screenX/(float)windowX;
	float mouseY =(float)input->mouseY * (float)screenY/(float)windowY;

    //Calculate mouse 3D position from zPos

    input->mouse3D= sceneData->controller->controlledActor->location;
    input->mouse3D+= sceneData->controller->controlledActor->zAxis * zPos;
    input->mouse3D-= sceneData->controller->controlledActor->xAxis * ((  mouseX/(float)screenX - 0.5 ) * zPos *   0.835 *  windowAspect);
    input->mouse3D+= sceneData->controller->controlledActor->yAxis * (((float)(screenY-mouseY)/(float)screenY - 0.5) * zPos *  0.835 ) ;


   ///Center 3D Position
    //Calculate center 3D position from zPos

    Vector2f cen=Vector2f(centerInfo[2],centerInfo[1]);
	zPos= (vec.y + vec.x * 1.0/255.0) * farClip;

    input->center3D= sceneData->controller->controlledActor->location;
    input->center3D+= sceneData->controller->controlledActor->zAxis * zPos;


    ///VertexID
    //TODO: implement vertexID
    //int vertexID = (int)ceil(mousePos[2] * 65536.0) + (int)ceil(mousePos[1]);

    //cout << "vertexID: " <<vertexID << endl;

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT,0);
    glBindTexture(GL_TEXTURE_2D,0);

}

//************************************************************
//
//       Texture and Shader Loading and Initialisation functions
//
//
//************************************************************

//generates a texture from a RAW file - needs implementation of sceneData->textureList!
GLuint Renderer::LoadTextureRAW( const char * filename,int size, int wrap ){

    GLuint texture;
    int width, height;
    BYTE * data;
    FILE * file;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;

    // allocate buffer
    width = size;
    height = size;
    data = (BYTE*) malloc( width * height * 3 );

    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );

    //allocate texture List
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     wrap ? GL_REPEAT : GL_CLAMP );

    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                       GL_RGB, GL_UNSIGNED_BYTE, data );

    // free buffer
    free( data );

    return texture;
}

bool Renderer::LoadTextureTGA( string filename, bool wrap, bool bAlpha, string texID ){

    GLuint texture;

    FIBITMAP * myBitmap = FreeImage_Load(FIF_TARGA,filename.c_str(),0);

    FreeImage_FlipVertical(myBitmap);
    //FreeImage_FlipHorizontal(myBitmap);


    if (FreeImage_GetBPP(myBitmap)<32)
        bAlpha=false;

    //allocate texture List
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,texTGAFilterMin );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texTGAFilterMag );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );

    // build our texture and mipmaps
    if (bAlpha)
      glTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, FreeImage_GetWidth(myBitmap), FreeImage_GetHeight(myBitmap), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(myBitmap) );
    else
      glTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, FreeImage_GetWidth(myBitmap), FreeImage_GetHeight(myBitmap), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(myBitmap) );

    glGenerateMipmapEXT(GL_TEXTURE_2D);
    FreeImage_Unload(myBitmap);

    sceneData->textureList[texID]=new textureObject;
    sceneData->textureList[texID]->texture=texture;
    sceneData->textureList[texID]->bAlpha=bAlpha;
    sceneData->textureList[texID]->bWrap=wrap;
    sceneData->textureList[texID]->texFilename=filename;
    sceneData->textureList[texID]->nextTexture="NULL";

    return true;
}

bool Renderer::createEmptyTexture( string texName, GLuint colorFormat, GLuint dataType, int width, int height){

    GLuint tex;

    int channels=0;
    if (colorFormat==GL_RGB)
        channels=3;
    else if (colorFormat==GL_RGBA)
        channels=4;
    else if (colorFormat==GL_LUMINANCE)
        channels=1;
    else{
        cout << "ERROR: format not supported, only GL_RGB, GL_RGBA and GL_LUMINANCE are supported right now!" << endl;
        return 0;
    }

    if (width>1024 || height> 1024){
        cout << "texture too big, not creating..." << endl;
    }

    //generate buffer up to 1024x1024x4
    static unsigned char texBuff[1024*1024*4];
    static float floatTexBuff[1024*1024*4];
    static short shortTexBuff[1024*1024*4];

    //create gradient
    for (int i=0;i<width*height*channels;i++){
        //texBuff[i]=(char)( (int) ( (float)(i*255.0) / (float)(width*height*channels) ));
        texBuff[i]=0;
        floatTexBuff[i]=0.0;
    }
    //create texture from raw data:
    glGenTextures( 1, &tex );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, tex );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    //the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

    //build the texture
    if (dataType==GL_UNSIGNED_BYTE)
        glTexImage2D( GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, &texBuff );
    else if (dataType==GL_FLOAT )
        glTexImage2D( GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_FLOAT, &floatTexBuff );
    else if (dataType==GL_SHORT)
        glTexImage2D( GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_SHORT, &shortTexBuff );
    else{
        cout << "ERROR: datatype not supported, must be GL_UNSIGNED_BYTE or GL_FLOAT or GL_SHORT" << endl;
        return 0;
    }

    sceneData->textureList[texName]= new textureObject;
    sceneData->textureList[texName]->texture=tex;
    sceneData->textureList[texName]->texFilename="memory";
    sceneData->textureList[texName]->nextTexture="NULL";

    cout << "created new empty texture with name:" << texName << " and number:" << tex << endl;
    return true;
}

bool Renderer::copyMemoryToTexture( void* originBuffer, string texName, float width, float height, bool bHighZRes){

        if (!originBuffer)
            return 0;

        glBindTexture(GL_TEXTURE_2D,sceneData->textureList[texName]->texture);
        //glPixelTransferf(GL_RED_SCALE,1.0/8192.0);
       // glTexSubImage2D(GL_TEXTURE_2D,0,(screenX - width)/2.0 ,(screenX - height)/2.0 ,width,height,GL_RGBA, GL_UNSIGNED_BYTE,(unsigned char*)originBuffer);
        if (bHighZRes)
            glTexSubImage2D(GL_TEXTURE_2D,0,(1024 - width)/2.0 ,(512 - height)/2.0 ,width,height,GL_RGBA, GL_FLOAT,(float*)originBuffer);
        else
            glTexSubImage2D(GL_TEXTURE_2D,0,(1024 - width)/2.0 ,(512 - height)/2.0 ,width,height,GL_RGBA, GL_UNSIGNED_BYTE,(unsigned char*)originBuffer);

        glBindTexture(GL_TEXTURE_2D,0);
        return 1;
}

bool Renderer::loadShader(string vertexShaderFileName, string fragmentShaderFileName, string shaderProgramName){

    GLuint fragmentShader;
    GLuint vertexShader;

    GLuint shaderProgram;


    char * vertexShaderFile;               //actually holds the whole file
    char * fragmentShaderFile;             //actually holds the whole file

    cout << "*************************************************************" << endl;

    //setup shader
    vertexShader=glCreateShader(GL_VERTEX_SHADER);
    fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);

    cout << "processing: " << vertexShaderFileName << "\n";
    cout << "processing: " << fragmentShaderFileName << "\n";

    vertexShaderFile=textFileRead((char*)vertexShaderFileName.c_str());
    fragmentShaderFile=textFileRead((char*)fragmentShaderFileName.c_str());

    const char* ptrV = vertexShaderFile;
    const char* ptrF = fragmentShaderFile;

    glShaderSource(vertexShader, 1, &ptrV,NULL);
    glShaderSource(fragmentShader, 1, &ptrF,NULL);

    glCompileShader(vertexShader);
    if (vertexShader==0){
      cout << "could not compile vertex shader " << vertexShaderFileName << endl;
      return false;
    }

    glCompileShader(fragmentShader);
    if (fragmentShader==0){
      cout << "could not compile fragment shader " << fragmentShaderFileName << endl;
      return false;
    }

    cout << "Info log for " << vertexShaderFileName << endl;
    printShaderInfoLog(vertexShader);

    cout << "Info log for " << fragmentShaderFileName << endl;
    printShaderInfoLog(fragmentShader);

    //Link shaders
    shaderProgram=glCreateProgram();
    if (shaderProgram==0){
      cout << "could not compile shader " << shaderProgramName << endl;
      return false;
    }

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);

    glLinkProgram(shaderProgram);

    cout << "Info log for " << shaderProgramName << endl;
    printProgramInfoLog(shaderProgram);


    //cleanUp
    free(vertexShaderFile);
    free(fragmentShaderFile);

    sceneData->shaderList[shaderProgramName]=new shaderObject;
    sceneData->shaderList[shaderProgramName]->shader=shaderProgram;
    sceneData->shaderList[shaderProgramName]->vertexShaderFilename=vertexShaderFileName;
    sceneData->shaderList[shaderProgramName]->fragmentShaderFilename=fragmentShaderFileName;
    cout << "registered program!" << shaderProgram << "\n";

    cout << "*************************************************************" << endl;

    cout << "building uniform lists for "<< shaderProgramName << endl;

	int maxLen=0;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen );
    char uniName[maxLen];

    GLint listSize=0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &listSize );

    int uniNameLength;
    int uniSize;
    GLenum uniType;
    for (int i=0;i<listSize;i++){
        glGetActiveUniform(shaderProgram, i , maxLen, &uniNameLength , &uniSize , &uniType , &uniName[0] );
        sceneData->shaderList[shaderProgramName]->uniforms[uniName]=glGetUniformLocation(shaderProgram,(const GLchar*) (&uniName)) ;
        cout << "has Uniform: " << uniName << " in Location " << sceneData->shaderList[shaderProgramName]->uniforms[uniName] << endl;
    }

    cout << "*************************************************************" << endl;

    #ifdef BDEBUGRENDERER
        checkOpenGLError("shader import ");
    #endif
    return true;
}

void Renderer::printShaderInfoLog(GLuint obj){
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void Renderer::printProgramInfoLog(GLuint obj){
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

bool Renderer::checkOpenGLError(string preText, bool bPrint){

    GLenum err=glGetError();

    switch(err){

        case GL_INVALID_ENUM:
            if (bPrint) cout << preText << " ERROR: invalid enum" << endl;
            return 1;

        case GL_INVALID_VALUE:
            if (bPrint) cout << preText << " ERROR: invalid value" << endl;
            return 1;

        case GL_INVALID_OPERATION:
            if (bPrint) cout << preText << " ERROR: invalid operation" << endl;
            return 1;

        case GL_STACK_OVERFLOW:
            if (bPrint) cout << preText << " ERROR: stack overflow" << endl;
            return 1;

        case GL_STACK_UNDERFLOW:
            if (bPrint) cout << preText << " ERROR: stack underflow" << endl;
            return 1;

        case GL_OUT_OF_MEMORY:
            if (bPrint) cout << preText << " ERROR: out of memory" << endl;
            return 1;

        default:
            //if (bPrint) cout << "No Error" << endl;
            return 0;
    }
}

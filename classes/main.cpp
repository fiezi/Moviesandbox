
#include "main.h"

#ifdef TARGET_MACOSX
#include <Cocoa/Cocoa.h>
#endif

/*

Actual Program

**********************/

using namespace std;

#ifdef WIN32

static bool SetPopUp(HWND hWnd)
{
   ::SetLastError(0);

   SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
   SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_COMPOSITED);
   SetMenu(hWnd,NULL);
   //this is to set the drawing region for the window - to get rid of black borders!
   HRGN hrgn = CreateRectRgn( 0, 38, 548, 580 );
   SetWindowRgn( hWnd, hrgn, TRUE );

   //NOTE: this is necessary for updating visuals!!!
   ::UpdateWindow(hWnd);
   ::ShowWindow(hWnd, SW_SHOW);    //SW_SHOWNORMAL, SW_SHOWNA
   ::SetFocus(hWnd);

   return (::GetLastError() == 0);
}
#endif


GLuint splashTex;

void draw(){renderManager->draw();}
void idle(){sceneDataManager->update(renderManager->deltaTime);}
void reDrawScreen(int w, int h){renderManager->reDrawScreen(w, h);}

void mouseButton(int button,int state,int x, int y){inputManager->pressedMouse(button,state,x,y);}
void dragMotion(int x, int y){inputManager->dragMouse(x,y);}
void mouseMotion(int x, int y){inputManager->moveMouse(x,y);}

void normalKey(unsigned char key, int x, int y){inputManager->normalKeyDown(key,x,y);}
void specialKey(int key, int x, int y){inputManager->specialKeyDown(key,x,y);}
void keyboardUp(unsigned char key,int x,int y){inputManager->keyUp(key,x,y);}
void specialKeyUp (int key,int x, int y){inputManager->specialKeyUp(key,x,y);}



void drawSplashScreen(){

	glDisable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

	splashTex=Renderer::LoadTextureRAW("splash.raw",512,1);

    glClear(GL_COLOR|GL_DEPTH);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,512,512,0,-1,1);

  	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, splashTex);

    glColor4f(1.0,1.0,1.0,1.0);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(0,0,1);

    glTexCoord2f(1,0);
    glVertex3f(512,0,1);

    glTexCoord2f(1,1);
    glVertex3f(512,512,1);

    glTexCoord2f(0,1);
    glVertex3f(0,512,1);

    glEnd();
    glutSwapBuffers();
}

/*
void redrawSplash(int w, int h){drawSplashScreen();}
void splashSpecial(int key,int x, int y){drawSplashScreen();}
void splashKey(unsigned char key,int x,int y){drawSplashScreen();}
void splashMouse(int button,int state,int x, int y){drawSplashScreen();}
*/

void createSplashScreen(){

    glutInitDisplayMode(GLUT_DOUBLE| GLUT_DEPTH | GLUT_RGBA );
	glutInitWindowSize(512,512);

   int windowX=1024;
   int windowY=768;

#ifdef WIN32

    // width
   windowX = GetSystemMetrics(SM_CXSCREEN);
   // height
   windowY = GetSystemMetrics(SM_CYSCREEN);


#endif

	glutInitWindowPosition(windowX/2-256,windowY/2-256);

	splashWindow=glutCreateWindow("loading");


#ifdef WIN32
    HWND hwnd = ::FindWindowA(0, "loading"); //NOTE, the windowtitle is crucial in order to find the handle, so you have to set it before!!!!
    if (hwnd != NULL)
    {
        SetPopUp(hwnd);
    }
#endif

}


void selectRenderer(bool bCompat){

	bool bGLCompatibilityShader=bCompat;


	//checking for extensions and shaders:
    if (!GLEE_ARB_point_sprite){
        cout << "GLee error:" << GLeeGetErrorString() << endl;
		std::cout << " not supporting point sprites \n";
		exit(0);
	}

    if (!GLEE_VERSION_2_0){
		std::cout << "OGL less than version 2.0" << endl;
        cout << "GLee error:" << GLeeGetErrorString() << endl;
		exit(0);
    }

    if (!(GLEE_ARB_vertex_shader && GLEE_ARB_fragment_shader)){
		std::cout << "GLSL unsupported \n";
        cout << "GLee error:" << GLeeGetErrorString() << endl;
		exit(0);
	}

    // and framebuffer  objects
	// verify FBOs are supported (otherwise we get FuBar'd Objects)

    if (!GLEE_EXT_draw_buffers2){
        cout << "oh no, cannot do blending right! Switching to GL 2.0 shader..." << endl;
        bGLCompatibilityShader=true;
    }

    if (!glutExtensionSupported ("GL_EXT_framebuffer_object") ){
        cerr << "regular FBO extension unsupported \n";
		exit(0);
	}

	if (!glutExtensionSupported ("GL_EXT_framebuffer_object") ){
        cerr << "regular FBO extension unsupported \n";
		exit(0);
	}

    if (!GLEE_ARB_texture_float){
		std::cout << " not supporting floating point textures\n";
		exit(0);
	}


	//set instance, since we already created our Renderer...
	if (bGLCompatibilityShader){
		renderManager=X1600Renderer::getInstance();
		cout << "################## Using GL 2.0 renderer ###############" << endl;
	}
	else{
		renderManager=Renderer::getInstance();
		cout << "################## Using GL 2.1 renderer ###############" << endl;
	}

}


void splashScreenLoop(){

	cout << "now loading..." << endl;

#ifdef TARGET_MACOSX
	//COCOA Code to get rid of GLUT Menu
	if (NSApp){
		NSMenu      *menu;
		NSMenuItem  *menuItem;

		[NSApp setMainMenu:[[NSMenu alloc] init]];

		menu = [[NSMenu alloc] initWithTitle:@""];
		[menu addItemWithTitle:@"About Moviesandbox" action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];

		[NSApp setDelegate:NSApp];
		[NSApp setAppleMenu:menu];
	}

#endif


	//creating objects
    sceneDataManager->setup();


    //loading preferences
    sceneDataManager->loadPreferences();

	//for some reason on Mac OSX, this is the earliest time we can call this for effect!
	drawSplashScreen();

    //init renderer
    renderManager->initWindow(0,0,"Moviesandbox");
    glutHideWindow();
    renderManager->setup();


    //load libraries and create scene
    sceneDataManager->createScene();

    //destroy splash screen
    glutDestroyWindow(splashWindow);

    //focus back on our window
    glutShowWindow();

	glutPostRedisplay();
	drawSplashScreen();

	glutIgnoreKeyRepeat(1);

    //window stuff
	glutIdleFunc(idle);
    glutDisplayFunc(draw);
	glutReshapeFunc(reDrawScreen);

    //Mouse
  	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mouseMotion);
	glutMotionFunc(dragMotion);

    //Keyboard
	glutKeyboardFunc(normalKey);
	glutSpecialFunc(specialKey);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialUpFunc(specialKeyUp);


	glutPostRedisplay();
	drawSplashScreen();

}


streambuf *psbuf, *backup;
ofstream filestr;


void startFileLog(){

    //setup cout to file

    cout << "starting file log..." << endl;

    filestr.open ("logfile.txt");

    backup = cout.rdbuf();     // back up cout's streambuf

    psbuf = filestr.rdbuf();   // get file's streambuf
    cout.rdbuf(psbuf);         // assign streambuf to cout

    cout << "Moviesandbox logfile ********************************* " << endl;

    //timestamp
    time_t rawtime;
    time ( &rawtime );
    cout << "Current local time is: " << ctime (&rawtime) << endl;

}

void endFileLog(){

    cout.rdbuf(backup);        // restore cout's original streambuf

    filestr.close();

}

int main(int argc, char* argv[]){


    //startFileLog();

	glutInit(&argc, argv);

#ifdef TARGET_MACOSX
	CGSetLocalEventsSuppressionInterval(0);
#endif

    createSplashScreen();

	//TODO: make proper command line parameters...
	if (argc>1)
		selectRenderer(true);
	else
		selectRenderer(false);


    sceneDataManager=SceneData::getInstance();
    inputManager=Input::getInstance();

#ifdef TARGET_WIN32

    TCHAR szEXEPath[MAX_PATH];
    GetModuleFileName(NULL,szEXEPath,MAX_PATH);
    sceneDataManager->exe_path= szEXEPath;

    sceneDataManager->exe_path = sceneDataManager->exe_path.substr(0, sceneDataManager->exe_path.find("Moviesandbox.exe")-1);  // Remove app name from string
    sceneDataManager->exe_path+= DIRECTORY_SEPARATION;

#endif

	glutDisplayFunc(drawSplashScreen);
    glutIdleFunc(splashScreenLoop);


    glutMainLoop();

    cout << "exiting..." << endl;

    delete(renderManager);

    return 0;
}



// load the named file and dump its structure to STDOUT


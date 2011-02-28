#include "main.h"

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
   HRGN hrgn = CreateRectRgn( 0, 36, 256, 296 );
   SetWindowRgn( hWnd, hrgn, TRUE );

   //NOTE: this is necessary for updating visuals!!!
   ::UpdateWindow(hWnd);
   ::ShowWindow(hWnd, SW_SHOW);    //SW_SHOWNORMAL, SW_SHOWNA
   ::SetFocus(hWnd);

   return (::GetLastError() == 0);
}
#endif



void draw(){renderManager->draw();}
void idle(){renderManager->update();}
void reDrawScreen(int w, int h){renderManager->reDrawScreen(renderManager->screenX, renderManager->screenY);}

void mouseButton(int button,int state,int x, int y){inputManager->pressedMouse(button,state,x,y);}
void dragMotion(int x, int y){inputManager->dragMouse(x,y);}
void mouseMotion(int x, int y){inputManager->moveMouse(x,y);}

void normalKey(unsigned char key, int x, int y){inputManager->normalKeyDown(key,x,y);}
void specialKey(int key, int x, int y){inputManager->specialKeyDown(key,x,y);}
void keyboardUp(unsigned char key,int x,int y){inputManager->keyUp(key,x,y);}
void specialKeyUp (int key,int x, int y){inputManager->specialKeyUp(key,x,y);}

void drawSplashScreen(){

    GLuint myTex=Renderer::LoadTextureRAW("splash.raw",512,1);

    glClear(GL_COLOR | GL_DEPTH);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,256,256,0,-1,1);

  	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTex);

    glColor3f(1.0,1.0,1.0);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex2f(0,0);

    glTexCoord2f(1,0);
    glVertex2f(256,0);

    glTexCoord2f(1,1);
    glVertex2f(256,256);

    glTexCoord2f(0,1);
    glVertex2f(0,256);

    glEnd();
    glutSwapBuffers();
    glDeleteTextures(1,&myTex);
    //delete(renderManager);

	glutPostRedisplay();
}


int main(int argc, char** argv)
{

#ifdef TARGET_MACOSX
	glutInit(&argc, argv);
	CGSetLocalEventsSuppressionInterval(0);
#endif

	bool bGLCompatibilityShader=false;

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(256,256);

   int screenX=1024;
   int screenY=768;

#ifdef WIN32

    // width
   screenX = GetSystemMetrics(SM_CXSCREEN);
   // height
   screenY = GetSystemMetrics(SM_CYSCREEN);

#endif

	glutInitWindowPosition(screenX/2-256,screenY/2-120);

	int myWindow=glutCreateWindow("loading");

#ifdef WIN32
    HWND hwnd = ::FindWindowA(0, "loading"); //NOTE, the windowtitle is crucial in order to find the handle, so you have to set it before!!!!
    if (hwnd != NULL)
    {
        SetPopUp(hwnd);
    }
    //for windows, we need to call this once!
    drawSplashScreen();
#endif


    glutDisplayFunc(drawSplashScreen);

	//checking for extensions and shaders:
    if (!GLEE_ARB_point_sprite){
		std::cout << " not supporting point sprites \n";
		exit(0);
	}
    if (!(GLEE_ARB_vertex_shader && GLEE_ARB_fragment_shader)){
		std::cout << "GLSL unsupported \n";
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


	inputManager=Input::getInstance();

    //loading...

    renderManager->loadPreferences();
    renderManager->initWindow(renderManager->windowX,renderManager->windowY,"Moviesandbox");
    glutHideWindow();
    renderManager->setup();

    //destroy splash screen
    glutDestroyWindow(myWindow);

    //focus back on our window
    glutShowWindow();

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

    glutMainLoop();

    delete(renderManager);

    return 0;
}



// load the named file and dump its structure to STDOUT


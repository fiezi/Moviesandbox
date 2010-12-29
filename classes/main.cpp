#include "main.h"

/*

Actual Program

**********************/

using namespace std;

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

void startUpDraw(){cout << "drawing startup..." <<endl;}

int main(int argc, char** argv)
{

#ifdef TARGET_MACOSX
	glutInit(&argc, argv);
	CGSetLocalEventsSuppressionInterval(0);
#endif




	bool bGLCompatibilityShader=false;

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(64,64);
	glutInitWindowPosition(256,256);

	int myWindow=glutCreateWindow("init...");

    //window stuff
	glutIdleFunc(startUpDraw);
    glutDisplayFunc(startUpDraw);


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

	glutDestroyWindow(myWindow);

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

    renderManager->loadPreferences();

    renderManager->initWindow(renderManager->windowX,renderManager->windowY,"Moviesandbox");


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

    //run program
    renderManager->setup();

    glutMainLoop();

    delete(renderManager);

    return 0;
}



// load the named file and dump its structure to STDOUT


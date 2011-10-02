

#include "viewportGizmo.h"
#include "renderer.h"
#include "input.h"
ViewportGizmo::ViewportGizmo(){

    textureID="viewportGizmo";
    bTextured=true;
    sceneShaderID="texture";
    viewport_fb = 0;

    //create a 100x100 texture
    renderer->createEmptyTexture("viewportGizmo",GL_RGBA,GL_FLOAT,100,100);

    //also, create an FBO
    renderer->createFBO(&viewport_fb,&(sceneData->textureList["viewportGizmo"]->texture),&myDepthBuffer,100,false, "viewportGizmo");


    registerProperties();
}

ViewportGizmo::~ViewportGizmo(){}


void ViewportGizmo::setup(){

    BasicButton::setup();
    scale.x=100;
    scale.y=80;

}

void ViewportGizmo::update(double deltaTime){

        //setup FBO

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,200,200);
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, viewport_fb);
    glDrawBuffers(1,renderer->drawBuffers);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    renderer->setupOrthoCamera();
    renderer->setupShading("colorAlphaID");
    updateShaders();

    //construct matrix
    Matrix4f myMatrix=sceneData->controller->controlledActor->baseMatrix;
    myMatrix.setTranslation(Vector3f(250,600,0));
    glMultMatrixf(myMatrix);

    renderer->drawCube(100.0);
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT,0);
    glPopAttrib();


}

void ViewportGizmo::mouseOver(){

    BasicButton::mouseOver();


       Vector4f myColor;

    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, viewport_fb);

    renderer->setupOrthoCamera();

    GLuint pickTexture;
    //setup picking texture!
    glGenTextures(1, &pickTexture);
    glBindTexture(GL_TEXTURE_2D, pickTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB,  1, 1, 0, GL_BGRA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //create small picking texture
    glBindTexture(GL_TEXTURE_2D,pickTexture);
//    float xRatio=(float)scene_size/(float)screenX;
//    float yRatio=(float)scene_size/(float)screenY;



    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,input->mouseX-800,input->mouseY-100,1 ,1 );
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,&myColor[0]);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
    cout << "Mx: " << input->mouseX << "My: " << input-> mouseY << endl;
    cout << myColor << endl;
}

void ViewportGizmo::mouseDrag(){}
void ViewportGizmo::finishDrag(){}
void ViewportGizmo::clickedLeft(){}

void ViewportGizmo::clickedRight(){}
void ViewportGizmo::focusClick(){

    BasicButton::focusClick();
}

void ViewportGizmo::deselect(int depth){

    BasicButton::deselect(depth);
}

void ViewportGizmo::create(){sceneData->addButton(this);}

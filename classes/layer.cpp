

#include "layer.h"
#include "renderer.h"
#include "input.h"
#include "msbLight.h"

Layer::Layer(){

	depthTextureID="NULL";
	pickTextureID="NULL";
	lightDataTextureID="NULL";

    sceneTex=0;
	depthTex=0;
	colorTex=0;
	pickTex=0;
	lightDataTex=0;

    sceneFBO=0;
	depthFBO=0;
	colorFBO=0;
	pickFBO=0;
	lightDataFBO=0;

    color=COLOR_WHITE;
    sceneShaderID="post";

	registerProperties();
}

Layer::~Layer(){}


void Layer::setup(){
    BasicButton::setup();
    location.x=0.0f;
    location.y=renderer->screenY;
    scale=Vector3f(renderer->windowX,-renderer->screenY,1.0);
    bScreenOverlay=true;

}

void Layer::update(double deltaTime){

}

void Layer::updateShaders(){

    shaderObject* myShader= sceneData->shaderList[sceneShaderID];

    if (myShader->uniforms.find("screenX") != myShader->uniforms.end()){
        GLint screenSize[4];
        glGetIntegerv(GL_VIEWPORT,(GLint*)&screenSize);
        glUniform1fARB(myShader->uniforms["screenX"], (float)screenSize[2]);
        glUniform1fARB(myShader->uniforms["screenY"], (float)screenSize[3]);
    }

   #ifdef BDEBUGRENDERER
    renderer->checkOpenGLError("ss-updateShaders...");
   #endif

    if (myShader->uniforms.find("scene_size") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["scene_size"], renderer->scene_size);
    }

    if (myShader->uniforms.find("lighting_size") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["lighting_size"], renderer->lighting_size);
    }

    if (myShader->uniforms.find("nearClip") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["nearClip"], renderer->nearClip);
    }
    if (myShader->uniforms.find("farClip") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["farClip"], renderer->farClip);
    }





    //needs if statements because the compiler throws out stuff if not used!
    if (myShader->uniforms.find("tex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["tex"], 0);

    //TODO: MAP is not the right way to store shader locations!
    if (myShader->uniforms.find("depthTex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["depthTex"], 1);

    if (myShader->uniforms.find("pickTex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["pickTex"], 2);

    if (myShader->uniforms.find("shadowTex") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["shadowTex"], 3);

    if (myShader->uniforms.find("fxTex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["fxTex"], 4);

    if (myShader->uniforms.find("multiTex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["multiTex"], 5);

    if (myShader->uniforms.find("lightViewMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightViewMatrix"],1,false,(GLfloat*)renderer->lightViewMatrix);

    if (myShader->uniforms.find("lightProjectionMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightProjectionMatrix"],1,false,(GLfloat*)renderer->lightProjectionMatrix);

    if (myShader->uniforms.find("cameraMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["cameraMatrix"],1,false,(GLfloat*)renderer->cameraMatrix);

    if (myShader->uniforms.find("cameraInverse") != myShader->uniforms.end())
        glUniformMatrix4fvARB(myShader->uniforms["cameraInverse"], 1,false, (GLfloat*)&renderer->inverseCameraMatrix);

    if (myShader->uniforms.find("projectionMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["projectionMatrix"],1,false,(GLfloat*)renderer->projectionMatrix);

    if (myShader->uniforms.find("projectionInverse") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["projectionInverse"],1,false,(GLfloat*)renderer->inverseProjectionMatrix);

    if (myShader->uniforms.find("numLights") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["numLights"],sceneData->lightList.size());

    if (myShader->uniforms.find("bLighting") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["bLighting"],renderer->bDrawLighting);

    if (myShader->uniforms.find("bSmudge") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["bSmudge"],renderer->bDrawSmudge);

    if (myShader->uniforms.find("bSSAO") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["bSSAO"],renderer->bSSAO);

    if (myShader->uniforms.find("bDrawColor") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["bDrawColor"],renderer->bDrawColor);

    if (myShader->uniforms.find("focus") != myShader->uniforms.end())
        glUniform1f(myShader->uniforms["focus"],renderer->focus);

   BasicButton::updateShaders();

}

void Layer::mouseOver(){

BasicButton::mouseOver();
}

void Layer::mouseDrag(){}
void Layer::finishDrag(){}
void Layer::clickedLeft(){}
void Layer::clickedRight(){}
void Layer::focusClick(){

BasicButton::focusClick();
}

void Layer::deselect(int depth){

BasicButton::deselect(depth);
}

void Layer::create(){sceneData->addButton(this);}

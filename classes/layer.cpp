

#include "layer.h"
#include "renderer.h"
#include "input.h"
#include "msbLight.h"

Layer::Layer(){

	depthTextureID="NULL";
	pickTextureID="NULL";
	lightDataTextureID="NULL";

	depthTex=0;
	colorTex=0;
	pickTex=0;
	lightDataTex=0;

	depthFBO=0;
	colorFBO=0;
	pickFBO=0;
	lightDataFBO=0;

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

    shaderObject* myShader= renderer->shaderList[renderer->currentShader];

    if (!myShader)
        return;

    BasicButton::updateShaders();


    glUniform1iARB(myShader->uniforms["tex"], 0);
    glUniform1iARB(myShader->uniforms["depthTex"], 1);
    glUniform1iARB(myShader->uniforms["pickTex"], 2);
    glUniform1iARB(myShader->uniforms["shadowTex"], 3);
    glUniform1iARB(myShader->uniforms["fxTex"], 4);
    glUniform1iARB(myShader->uniforms["bkgTex"], 5);

    if (myShader->uniforms["lightViewMatrix"])
        glUniformMatrix4fv(myShader->uniforms["lightViewMatrix"],1,false,(GLfloat*)renderer->lightViewMatrix);
    if (myShader->uniforms["lightProjectionMatrix"])
        glUniformMatrix4fv(myShader->uniforms["lightProjectionMatrix"],1,false,(GLfloat*)renderer->lightProjectionMatrix);
    if (myShader->uniforms["cameraMatrix"])
        glUniformMatrix4fv(myShader->uniforms["cameraMatrix"],1,false,(GLfloat*)renderer->cameraMatrix);
    if (myShader->uniforms["projectionMatrix"])
        glUniformMatrix4fv(myShader->uniforms["projectionMatrix"],1,false,(GLfloat*)renderer->projectionMatrix);
    if (myShader->uniforms["projectionInverse"])
        glUniformMatrix4fv(myShader->uniforms["projectionInverse"],1,false,(GLfloat*)renderer->inverseProjectionMatrix);
    if (myShader->uniforms["numLights"])
        glUniform1i(myShader->uniforms["numLights"],renderer->lightList.size());

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

void Layer::create(){renderer->addButton(this);}

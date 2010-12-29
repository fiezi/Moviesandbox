

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

    if (!renderer->shaderList[sceneShaderID] || !renderer->shaderList[sceneShaderID]->shader)
        return;

    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "tex"), 0);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "depthTex"), 1);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "pickTex"), 2);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "shadowTex"), 3);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "fxTex"), 4);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "bkgTex"), 5);

    GLuint uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "lightViewMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->lightViewMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "lightProjectionMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->lightProjectionMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "cameraMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->cameraMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "projectionMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->projectionMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "projectionInverse");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->inverseProjectionMatrix);

   uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "numLights");
   glUniform1i(uniform_location,renderer->lightList.size());

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

void Layer::create(){renderer->addButton(this);}

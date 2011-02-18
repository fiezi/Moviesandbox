

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

    shaderObject* myShader= renderer->shaderList[sceneShaderID];

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

    if (myShader->uniforms.find("lightViewMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightViewMatrix"],1,false,(GLfloat*)renderer->lightViewMatrix);

    if (myShader->uniforms.find("lightProjectionMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightProjectionMatrix"],1,false,(GLfloat*)renderer->lightProjectionMatrix);

    if (myShader->uniforms.find("cameraMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["cameraMatrix"],1,false,(GLfloat*)renderer->cameraMatrix);

    if (myShader->uniforms.find("projectionMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["projectionMatrix"],1,false,(GLfloat*)renderer->projectionMatrix);

    if (myShader->uniforms.find("projectionInverse") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["projectionInverse"],1,false,(GLfloat*)renderer->inverseProjectionMatrix);

    if (myShader->uniforms.find("numLights") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["numLights"],renderer->lightList.size());



/*
  glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "tex"), 0);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "depthTex"), 1);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "pickTex"), 2);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "shadowTex"), 3);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "fxTex"), 4);
    glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "bkgTex"), 5);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "lightViewMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->lightViewMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "lightProjectionMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->lightProjectionMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "projectionMatrix");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->projectionMatrix);

    uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "projectionInverse");
    glUniformMatrix4fv(uniform_location,1,false,(GLfloat*)renderer->inverseProjectionMatrix);

   uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "numLights");
   glUniform1i(uniform_location,renderer->lightList.size());

*/

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



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

    color=Vector4f(1.0,1.0,1.0,1.0);
    sceneShaderID="post";

	registerProperties();
}

Layer::~Layer(){}


void Layer::setup(){
    BasicButton::setup();
    location.x=0.0f;
    location.y=renderer->screenY;
    location.z=-10;
    scale=Vector3f(renderer->screenX,-renderer->screenY,1.0);
    bScreenOverlay=true;

}

void Layer::update(double deltaTime){

}

void Layer::updateShaders(){

    shaderObject* myShader= sceneData->shaderList[sceneShaderID];

    if (myShader->uniforms.find("screenX") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["screenX"], renderer->screenX);
    }

    if (myShader->uniforms.find("screenY") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["screenY"], renderer->screenY);
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



    if (myShader->uniforms.find("camLoc") != myShader->uniforms.end()){
        glUniform3fARB(myShader->uniforms["camLoc"], sceneData->controller->controlledActor->location.x,sceneData->controller->controlledActor->location.y,sceneData->controller->controlledActor->location.z);
    }
    if (myShader->uniforms.find("camX") != myShader->uniforms.end()){
        glUniform3fARB(myShader->uniforms["camX"], sceneData->controller->controlledActor->xAxis.x,sceneData->controller->controlledActor->xAxis.y,sceneData->controller->controlledActor->xAxis.z);
    }
    if (myShader->uniforms.find("camY") != myShader->uniforms.end()){
        glUniform3fARB(myShader->uniforms["camY"], sceneData->controller->controlledActor->yAxis.x,sceneData->controller->controlledActor->yAxis.y,sceneData->controller->controlledActor->yAxis.z);
    }
    if (myShader->uniforms.find("camZ") != myShader->uniforms.end()){
        glUniform3fARB(myShader->uniforms["camZ"], sceneData->controller->controlledActor->zAxis.x,sceneData->controller->controlledActor->zAxis.y,sceneData->controller->controlledActor->zAxis.z);
    }




    //needs if statements because the compiler throws out stuff if not used!
    if (myShader->uniforms.find("tex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["tex"], 0);

    if (myShader->uniforms.find("depthTex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["depthTex"], 1);

    if (myShader->uniforms.find("shadowTex") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["shadowTex"], 2);

    if (myShader->uniforms.find("normalTex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["normalTex"], 3);




    if (myShader->uniforms.find("lightViewMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightViewMatrix"],1,false,(GLfloat*)renderer->lightViewMatrix);

    if (myShader->uniforms.find("lightViewMatrixInverse") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightViewMatrixInverse"],1,false,(GLfloat*)renderer->lightViewMatrixInverse);

    if (myShader->uniforms.find("lightProjectionMatrixInverse") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightProjectionMatrixInverse"],1,false,(GLfloat*)renderer->lightProjectionMatrixInverse);

    if (myShader->uniforms.find("lightProjectionMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["lightProjectionMatrix"],1,false,(GLfloat*)renderer->lightProjectionMatrix);

    if (myShader->uniforms.find("cameraMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["cameraMatrix"],1,false,(GLfloat*)renderer->cameraMatrix);


    if (myShader->uniforms.find("cameraRotationMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["cameraRotationMatrix"],1,false,(GLfloat*)renderer->cameraRotationMatrix);

    if (myShader->uniforms.find("inverseCameraRotationMatrix") != myShader->uniforms.end())
        glUniformMatrix4fv(myShader->uniforms["inverseCameraRotationMatrix"],1,false,(GLfloat*)renderer->inverseCameraRotationMatrix);


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

    if (myShader->uniforms.find("bDrawNormals") != myShader->uniforms.end())
        glUniform1i(myShader->uniforms["bDrawNormals"],renderer->bDrawNormals);

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

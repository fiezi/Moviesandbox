

#include "msbLight.h"
#include "renderer.h"

MsbLight::MsbLight(){

    bLight=true;
    name="Light";
    bCastShadows=true;
    bComputeLight=false;
    bTextured=true;
    textureID="sprite_light";
    sceneShaderID="texture";
    drawType=DRAW_SPRITE;
    scale=Vector3f(1.0,1.0,1.0);
    particleScale=20.0;
    shadow_fb=0;
    shadow_tx=0;
    shadow_size=renderer->shadow_size;

    fov=110;
    lightDistance = 32;

    registerProperties();
}

MsbLight::~MsbLight(){

    glDeleteFramebuffersEXT(1, &shadow_fb);

}

void MsbLight::registerProperties(){

createMemberID("BCASTSHADOWS",&bCastShadows,this,true,"10BoolButton");
createMemberID("FOV",&fov,this);
createMemberID("LIGHTDISTANCE",&lightDistance, this);
Actor::registerProperties();
}

void MsbLight::setup(){

    Actor::setup();
    sceneData->lightList.push_back(this);
}

void MsbLight::trigger(MsbObject* other){}

void MsbLight::update(double deltaTime){
    Actor::update(deltaTime);

//maybe at some point, use this instead of the openGL Light stuff...

/*
    //fill lightProperties
    lightProperties[0]=location.x;
    lightProperties[1]=location.y;
    lightProperties[2]=location.z;
    lightProperties[3]=1.0; //could be distance falloff

    lightProperties[4]=zAxis.x;
    lightProperties[5]=zAxis.y;
    lightProperties[6]=zAxis.z;
    lightProperties[7]=1.0; //could be opening radius

    lightProperties[8]=color.r;
    lightProperties[9]=color.g;
    lightProperties[10]=color.b;
    lightProperties[11]=1.0; //could be something else
*/
}

void MsbLight::start(){

    Actor::start();
    drawType=DRAW_NULL;
}

void MsbLight::stop(){

    Actor::stop();
    drawType=DRAW_SPRITE;
}


void MsbLight::remove(){

    for (int i=0;i<(int)sceneData->lightList.size();i++){
        if (sceneData->lightList[i]==this)
            sceneData->lightList.erase(sceneData->lightList.begin()+i);
    }
    Actor::remove();
}
void MsbLight::create(){sceneData->addActor(this);}

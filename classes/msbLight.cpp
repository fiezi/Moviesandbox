

#include "msbLight.h"
#include "renderer.h"

MsbLight::MsbLight(){

bLight=true;
name="Light";
bCastShadows=true;
drawType=DRAW_CUBE;
scale=Vector3f(0.25,0.25,0.25);
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

createMemberID("BCASTSHADOWS",&bCastShadows,this);
createMemberID("FOV",&fov,this);
createMemberID("LIGHTDISTANCE",&lightDistance, this);
Actor::registerProperties();
}

void MsbLight::setup(){

    Actor::setup();
    renderer->lightList.push_back(this);
}

void MsbLight::trigger(Actor * other){}

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
    drawType=DRAW_CUBE;
}


void MsbLight::remove(){

    for (int i=0;i<(int)renderer->lightList.size();i++){
        if (renderer->lightList[i]==this)
            renderer->lightList.erase(renderer->lightList.begin()+i);
    }
    Actor::remove();
}
void MsbLight::create(){renderer->addActor(this);}

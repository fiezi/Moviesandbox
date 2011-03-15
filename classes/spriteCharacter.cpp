

#include "spriteCharacter.h"
#include "renderer.h"
#include "input.h"


SpriteCharacter::SpriteCharacter(){

displayTextureIDs.push_back("cindy");
displayTextureIDs.push_back("icon_move");
displayTextureIDs.push_back("icon_rotate");
displayTextureIDs.push_back("icon_flat");

drawType=DRAW_SPRITE;
drawType=DRAW_PARTICLES;
sceneShaderID="projectedTex";
textureID="cindy";

bUseShader=true;
bTextured=true;

name="SpriteCharacter";
scale=Vector3f(1,1,1);

registerProperties();
}

SpriteCharacter::~SpriteCharacter(){}

void SpriteCharacter::registerProperties(){

ParticleSystem::registerProperties();
}

void SpriteCharacter::setup(){

ParticleSystem::setup();

addRandomParticles(50000,1);
}

void SpriteCharacter::trigger(MsbObject* other){}

void SpriteCharacter::update(double deltaTime){

ParticleSystem::update(deltaTime);

//here we need to project our camera orientation vector onto the orientation of our character
//and depending on the camera position, we switch between the different texture states

//project orientation of camera

Vector3f camOrient=sceneData->controller->controlledActor->orientation;

float dotProduct= camOrient.dotProduct(orientation);

//swap textures accordingly

if (dotProduct <-0.5)
    textureID=displayTextureIDs[BACKTEXTURE];
if (dotProduct<0.5 && dotProduct> -0.5)
    textureID=displayTextureIDs[LEFTTEXTURE];
if (dotProduct>0.5)
    textureID=displayTextureIDs[FRONTTEXTURE];
}


void SpriteCharacter::create(){sceneData->addActor(this);}

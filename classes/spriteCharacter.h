#ifndef H_SPRITECHARACTER
#define H_SPRITECHARACTER

#include "particleSystem.h"

#define FRONTTEXTURE 0
#define LEFTTEXTURE  1
#define RIGHTTEXTURE 2
#define BACKTEXTURE  3



class SpriteCharacter: public ParticleSystem{

public:

    std::vector<std::string>    displayTextureIDs;              //we store the textures to switch from in a vector

    SpriteCharacter();
    virtual ~SpriteCharacter();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // SPRITECHARACTER

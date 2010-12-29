#ifndef H_MSBLIGHT
#define H_MSBLIGHT

#include "actor.h"

class MsbLight: public Actor{

public:

    Matrix4f lightProperties;
    bool bCastShadows;

    float fov;
    float lightDistance;

    GLuint shadow_tx;                    // the shadow texture
    GLuint shadow_fb;                           // the framebuffer object to render to that texture
    int shadow_size;                           // the shadow map size
    GLuint shadow_db;

    MsbLight();
    virtual ~MsbLight();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(Actor * other);

    virtual void update(double deltaTime);

    virtual void start();
    virtual void stop();

    virtual void remove();

    virtual void create();
};
#endif // MSBLIGHT

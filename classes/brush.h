#ifndef H_BRUSH
#define H_BRUSH

#include "actor.h"
//#include "ragDoll.h"


class ParticleSystem;
class SkeletalActor;

#define BRUSH_NULL 1
#define BRUSH_DRAW 2
#define BRUSH_SKIN 3
#define BRUSH_BONE 4
#define BRUSH_SELECT 5

#define NORMAL_UP 0
#define NORMAL_FRONT 1
#define NORMAL_WORLD 2



class MeshData;

class Brush: public Actor{

public:

    int mode;                           //the way we draw things
    int normalMode;
    bool bNormalFollowDrawing;

    float intensity;                    //for skinning and drawing (soon...)

    SkeletalActor* drawing;				//the particleSystem we draw into

    bool bMouseControlled;              //controlled by a mouse, as opposed to some UDP stuff
    Vector3f calcLoc;                   //the location of the drawn Particle
    Vector3f pNormal;                   //the particle's normal

    vector<int> selectedData;
    vector<Vector4f>  selectedOldColors;


    Brush();
    virtual ~Brush();

    virtual void update(double deltaTime);

    virtual void setColor(Vector4f newColor);


    virtual void create();

};
#endif

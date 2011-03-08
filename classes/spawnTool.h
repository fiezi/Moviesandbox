#ifndef H_SPAWNTOOL
#define H_SPAWNTOOL

#include "skinTool.h"

#define BRUSH_ABSOLUTE   0
#define BRUSH_NORMAL     1
#define BRUSH_ROTATE     2
#define BRUSH_MOVE       3

#define BRUSH_X          0
#define BRUSH_Y          1
#define BRUSH_Z          2

class SpawnTool: public MsbTool{

public:

    int brushMove,
        brushRot;

    bool bKeepSelection;

    Vector3f brushX,brushY,brushZ;

    string  spawnType;
    Brush* brush;

    SpawnTool();
    virtual ~SpawnTool();

    virtual void start();
    virtual void stop();
    virtual void update(double deltaTime);

    virtual void selectActors(int btn, Actor* other);

    virtual void keyReleased(int key);
    virtual void mouseReleased(int btn);
    virtual void postSpawn(Actor* myActor);
};
#endif // SPAWNTOOL

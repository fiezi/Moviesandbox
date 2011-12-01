#ifndef H_BONETOOL
#define H_BONETOOL

#include "spawnTool.h"

class Brush;

class BoneTool: public SpawnTool{

public:

    BoneTool();
    virtual ~BoneTool();

    virtual void start();
    virtual void stop();
    virtual void mouseReleased(int btn);
    virtual void keyReleased(int key);


    virtual void postSpawn(Actor* myActor);
    virtual void save();
};
#endif // BONETOOL

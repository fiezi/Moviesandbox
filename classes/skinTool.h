#ifndef H_SKINTOOL
#define H_SKINTOOL

#include "drawTool.h"



class SkinTool: public DrawTool{

public:

    SkinTool();
    virtual ~SkinTool();

    virtual void start();
    virtual void stop();

    virtual void keyReleased(int key);
    virtual void mouseReleased(int btn);

    virtual void selectActors(int btn, Actor* other);
    virtual void paint();
    virtual void erase();
    virtual void singleSkin(int pID,int boneID, Vector3f distance);
    virtual void eraseSingleSkin(int pID,int boneID, Vector3f distance);

};
#endif // SKINTOOL

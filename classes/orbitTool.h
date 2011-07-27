#ifndef H_ORBITTOOL
#define H_ORBITTOOL

#include "navTool.h"

class OrbitTool: public NavTool{

public:

    float amountX;
    float amountY;
    float distance;
    Vector3f lookPoint;

    OrbitTool();
    virtual ~OrbitTool();

    virtual void start();
    virtual void update(double deltaTime);
    virtual void processMove(double deltaTime);
    virtual void processRotation(double deltaTime);
};
#endif // ORBITTOOL

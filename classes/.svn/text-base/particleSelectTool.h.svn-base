#ifndef H_PARTICLESELECTTOOL
#define H_PARTICLESELECTTOOL

#include "drawTool.h"

class ParticleSelectTool: public DrawTool{

public:

    vector<int> selectedData;
    vector<Vector4f>  selectedOldColors;


    ParticleSelectTool();
    virtual ~ParticleSelectTool();

    virtual void setup();

    virtual void start();
    virtual void stop();

    virtual void keyReleased(int key);

    virtual void paint();
    virtual void erase();

};
#endif // PARTICLESELECTTOOL

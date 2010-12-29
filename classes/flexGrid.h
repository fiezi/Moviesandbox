#ifndef H_FLEXGRID
#define H_FLEXGRID

#include "ParticleSystem.h"

class FlexGrid: public Actor{

public:

    vector<Vector3f> vertLocations;

    float gridHeight;

    FlexGrid();

    void setup();

    void trigger(Actor * other);

    void makeGridFromParticles(ParticleSystem * p);
    void drawQuadstrip();
    void drawPlane();
    void draw();


};
#endif

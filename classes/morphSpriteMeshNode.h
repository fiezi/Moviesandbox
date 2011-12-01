#ifndef H_MORPHSPRITEMESHNODE
#define H_MORPHSPRITEMESHNODE

#include "node.h"

class MorphSpriteMeshNode: public Node{

public:

    Actor* morphOne;
    Actor* morphTwo;

    float particleScaleOne;
    float particleScaleTwo;

    float particleAngleScaleOne;
    float particleAngleScaleTwo;

    float morphRate;
    float morphAngleRate;

    MorphSpriteMeshNode();
    virtual ~MorphSpriteMeshNode();

    virtual void registerProperties();

    virtual void start();
    virtual void stop();
    virtual void execute();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // MORPHSPRITEMESHNODE

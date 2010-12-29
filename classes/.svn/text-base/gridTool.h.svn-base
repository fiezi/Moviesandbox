#ifndef H_GRIDTOOL
#define H_GRIDTOOL

#include "msbTool.h"

#define GRID_ABSOLUTE   0
#define GRID_NORMAL     1
#define GRID_ROTATE     2
#define GRID_MOVE       3

#define GRID_X          0
#define GRID_Y          1
#define GRID_Z          2

class GridTool: public MsbTool{

public:

    int gridMove,
        gridRot;

    Vector3f gridX,gridY,gridZ;


    GridTool();
    virtual ~GridTool();

    virtual void setup();

    virtual void start();
    virtual void stop();

    virtual void keyPressed(int key);
    virtual void keyReleased(int key);

    virtual void mousePressed(int btn);
    virtual void mouseReleased(int btn);
    virtual void mouseDragged(int btn);

    virtual void update(double deltaTime);
    virtual void rotateGrid(double deltaTime);
};
#endif // GRIDTOOL

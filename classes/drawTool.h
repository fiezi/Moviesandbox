#ifndef H_DRAWTOOL
#define H_DRAWTOOL

#include "msbTool.h"
#include "brush.h"
#include "meshData.h"
#include "brushFilter.h"


#define BRUSHBUFFER 4

class DrawTool: public MsbTool{

public:

    Brush*  brush;
    Vector3f calcLoc;

    bool bMouseControlled;
    bool bDrawing;
	bool bStartStroke;
	bool bPaintMesh;

    Vector3f brushLocBuffer[16];

    vector<BrushFilter*> filters;

    DrawTool();
    virtual ~DrawTool();

    virtual void setup();

    virtual void start();
    virtual void stop();

    virtual void keyPressed(int key);
    virtual void keyReleased(int key);

    virtual void mousePressed(int btn);
    virtual void mouseReleased(int btn);
    virtual void mouseDragged(int btn);

    virtual void selectActors(int btn, Actor* other);

    virtual void update(double deltaTime);

    virtual void paint();
    virtual void erase();
    virtual void selectParticles();
    virtual void save();
    virtual void deselectAllParticles();
    virtual void scaleZ(float factor);
    virtual void mergeDrawings();
    virtual void splitDrawing();
    virtual void flipNormals();
    virtual void clearDrawing();

    virtual void createNewDrawing(bool bUnnamed=false);

    virtual void calcLocation();
};
#endif // DRAWTOOL

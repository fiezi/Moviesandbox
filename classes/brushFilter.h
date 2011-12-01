#ifndef H_BRUSHFILTER
#define H_BRUSHFILTER

#include "msbObject.h"
#include "brush.h"
#include "meshData.h"

class DrawTool;

class BrushFilter: public MsbObject{

public:

    DrawTool*   parent;
    Brush*      brush;

	Vector3f	oldMouse3D;

    BrushFilter();
    BrushFilter(DrawTool* myTool);

    virtual ~BrushFilter();

    virtual void setup();
    virtual void filter(vertexData* myVData);
    virtual void reset();
};
#endif // BRUSHFILTER

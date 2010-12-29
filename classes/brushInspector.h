#ifndef H_BRUSHINSPECTOR
#define H_BRUSHINSPECTOR

#include "inspector.h"
#include "assignButton.h"

class BrushInspector: public Inspector{

public:

    vector<AssignButton*>   colorSwatches;
    int                     currentSwatch;
    int                     numSwatches;

    BrushInspector();
    virtual ~BrushInspector();

    virtual void setup();

    virtual void refreshList();
    virtual void assembleList();
    virtual void trigger(Actor* other);

    virtual void create();
};
#endif // BRUSHINSPECTOR

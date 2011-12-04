#ifndef H_BRUSHINSPECTOR
#define H_BRUSHINSPECTOR

#include "inspector.h"
#include "assignButton.h"

class BrushInspector: public Inspector{

public:

    BasicButton*                            brushViz;
    vector<AssignButton*>      colorSwatches;
    int                                             currentSwatch;
    int                                             numSwatches;



    BrushInspector();
    virtual ~BrushInspector();

    virtual void setup();

    virtual void createInspectorButtons();

    virtual void refreshList();
    virtual void assembleList();
    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // BRUSHINSPECTOR

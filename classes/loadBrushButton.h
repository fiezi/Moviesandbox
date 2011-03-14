#ifndef H_LOADBRUSHBUTTON
#define H_LOADBRUSHBUTTON

#include "loadButton.h"

class LoadBrushButton: public LoadButton{

public:

    LoadBrushButton();
    virtual ~LoadBrushButton();

    virtual void clickedLeft();
    virtual void assembleLoadList();
    virtual void trigger(MsbObject*other);
    virtual void loadFile(string filename);
    virtual void placeScrollBar();

    virtual void create();
};
#endif // LOADBRUSHBUTTON

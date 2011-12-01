#ifndef H_LOADDRAWINGBUTTON
#define H_LOADDRAWINGBUTTON

#include "loadButton.h"

class LoadDrawingButton: public LoadButton{

public:

    LoadDrawingButton();
    virtual ~LoadDrawingButton();

    virtual void clickedLeft();

    virtual void assembleLoadList();

    virtual void placeScrollBar();

    virtual void trigger(MsbObject* other);

    virtual void loadFile(string filename);

    virtual void create();
};
#endif // LOADDRAWINGBUTTON

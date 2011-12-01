#ifndef H_LOADDRAWSKELETONBUTTON
#define H_LOADDRAWSKELETONBUTTON

#include "loadDrawingButton.h"

class LoadDrawSkeletonButton: public LoadDrawingButton{

public:

    LoadDrawSkeletonButton();
    virtual ~LoadDrawSkeletonButton();

    virtual void clickedLeft();

    virtual void assembleLoadList();

    virtual void trigger(MsbObject* other);

    virtual void placeScrollBar();

    virtual void create();
};
#endif // LOADDRAWSKELETONBUTTON

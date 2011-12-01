#ifndef H_SAVEDRAWINGBUTTON
#define H_SAVEDRAWINGBUTTON
#include <saveButton.h>

class SaveDrawingButton: public SaveButton{

public:

        SaveDrawingButton();
        virtual ~SaveDrawingButton();

        virtual void clickedLeft();
        virtual void assembleSaveList();
        virtual void placeScrollBar();

        virtual void trigger(Actor * other);
        virtual void saveFile(string filename);
        virtual void create();
};
#endif

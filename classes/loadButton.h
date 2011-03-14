#ifndef H_LOADBUTTON
#define H_LOADBUTTON

#include <listButton.h>

class LoadButton: public ListButton{

public:

    LoadButton();
    virtual ~LoadButton();

    virtual void clickedLeft();
    virtual void assembleLoadList();
    virtual void trigger(MsbObject* other);
    virtual void loadFile(string filename);
    virtual void create();
};
#endif // LOADBUTTON

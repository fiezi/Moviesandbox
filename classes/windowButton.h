#ifndef H_WINDOWBUTTON
#define H_WINDOWBUTTON

#include "listButton.h"

class WindowButton: public ListButton{

public:

    vector<string> tabNames;                   //tab names of all tabs for this window

    int currentTab;                                 //currently selected tab

    WindowButton();
    virtual ~WindowButton();

    virtual void setup();

    virtual void assembleList();
/*
    virtual void closeWidget();
*/
    virtual void deselect(int depth);

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // WINDOWBUTTON

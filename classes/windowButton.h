#ifndef H_WINDOWBUTTON
#define H_WINDOWBUTTON

#include "widget.h"

class WindowButton: public Widget{

public:

    BasicButton* backgroundButton;

    vector<string> tabNames;                   //tab names of all tabs for this window

    int currentTab;                                 //currently selected tab

    WindowButton();
    virtual ~WindowButton();

    virtual void setup();

    virtual void assembleList();

    virtual void closeWidget();

    virtual void deselect(int depth);

    virtual void create();
};
#endif // WINDOWBUTTON

#ifndef H_INSPECTOR
#define H_INSPECTOR

#include "tabbedListButton.h"

class InspectorManager;

class Inspector: public TabbedListButton{

public:

    class MyTab: public Tab{

        public:
        MyTab(){};
        MyTab(TabbedListButton* m){mine=m;};
        virtual ~MyTab(){};
        virtual void assembleList(){cout << "assemblin'..." << endl;};

    };

    bool bOpen;
    bool bToggleInspector;                      //close inspector when new one opens?

    float tabWidth;
    float tabHeight;

    vector<BasicButton*> inspectorButtons;      //special inspector buttons, like "import", etc...

    InspectorManager* myManager;


    Inspector();
    virtual ~Inspector();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void createInspectorButtons();

    virtual void drawPlane();

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void finishDrag();

    virtual void clickedLeft();

    virtual void refreshList();
    virtual void assembleList();

    virtual void placeButton(int buttonNumber, int drawPosition);
    virtual void placeScrollBar();

    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);

    virtual void create();
};
#endif // INSPECTOR

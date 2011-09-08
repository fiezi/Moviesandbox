#ifndef H_TABBEDLISTBUTTON
#define H_TABBEDLISTBUTTON

#include "listButton.h"
#include "sliderButton.h"

class TabbedListButton: public ListButton{


public:

    class Tab: public MsbObject{

        public:

        TabbedListButton* mine;

        Tab(){};
        Tab(TabbedListButton* m){mine=m;};
        virtual ~Tab(){};
        virtual void assembleList();

    };


    vector<Tab*>   tabs;   //pointers to functions that should be called as "assembleList"

    int currentTab;                             //which tab do we have open?

    /*** Functions ***/


    TabbedListButton();
    virtual ~TabbedListButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void clickedLeft();                    //calls assembleList() and placeButton()

    virtual void assembleList();                                    //creates Buttons from the listType vector and puts them in listButton vector as Button references
    virtual void trigger(MsbObject* other);                             //callback from Scrollbar

    virtual void deselect(int depth);


    virtual void create();
};
#endif

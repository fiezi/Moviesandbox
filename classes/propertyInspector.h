#ifndef H_PROPERTYINSPECTOR
#define H_PROPERTYINSPECTOR

#include "inspector.h"

class PropertyInspector: public Inspector{

public:

    class propTabOne: public tab{

        public:

        propTabOne(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };

    class propTabTwo: public tab{

        public:

        propTabTwo(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };

    MsbObject*  oldParent;

    PropertyInspector();
    virtual ~PropertyInspector();

    virtual void setup();

    virtual void createInspectorButtons();
    virtual void refreshList();

    virtual void assembleList();
    virtual void assembleListTabTwo();

    virtual void trigger(MsbObject* other);


    virtual void create();
};
#endif // PROPERTYINSPECTOR

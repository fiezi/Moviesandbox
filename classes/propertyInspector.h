#ifndef H_PROPERTYINSPECTOR
#define H_PROPERTYINSPECTOR

#include "inspector.h"

class PropertyInspector: public Inspector{

public:

    class PropTabOne: public Tab{

        public:

        PropTabOne(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };

    class PropTabTwo: public Tab{

        public:

        PropTabTwo(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };

    MsbObject*  oldParent;

    PropertyInspector();
    virtual ~PropertyInspector();

    virtual void setup();

    virtual void createInspectorButtons();
    virtual void refreshList();

    virtual void trigger(MsbObject* other);


    virtual void create();
};
#endif // PROPERTYINSPECTOR

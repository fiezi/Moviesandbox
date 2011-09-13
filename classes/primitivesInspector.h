#ifndef H_PRIMITIVESINSPECTOR
#define H_PRIMITIVESINSPECTOR

#include "inspector.h"


class PrimitivesInspector: public Inspector{

public:


   class PrimitivesTab: public Tab{

        public:

        PrimitivesTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

    class NodeTab: public Tab{

        public:

        NodeTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };



    PrimitivesInspector();
    virtual ~PrimitivesInspector();

    virtual void setup();

    virtual void createInspectorButtons();

    virtual void refreshList();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // PrimitivesInspector

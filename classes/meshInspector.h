#ifndef H_MESHINSPECTOR
#define H_MESHINSPECTOR

#include "inspector.h"
#include "meshData.h"

class MeshInspector: public Inspector{

public:


   class MeshTab: public Tab{

        public:

        MeshTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

    class TextureTab: public Tab{

        public:

        TextureTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };

    class ActionTab: public Tab{

        public:

        ActionTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };

    class PrefabTab: public Tab{

        public:

        PrefabTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
    };


    MeshInspector();
    virtual ~MeshInspector();

    virtual void setup();

    virtual void createInspectorButtons();

    virtual void refreshList();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // MeshInspector

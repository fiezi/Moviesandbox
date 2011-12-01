#ifndef H_LAYERINSPECTOR
#define H_LAYERINSPECTOR

#include "inspector.h"
#include "assignButton.h"

class LayerInspector: public Inspector{

public:

   class ActorListTab: public Tab{

        public:

        ActorListTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

   class HelpersTab: public Tab{

        public:

        HelpersTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

    AssignButton* newLayer;

    vector<AssignButton*> layerButtons;
    vector<AssignButton*> actorButtons;
    vector<Actor*> actorReferences;         //used for name updating, reordering, layer resorting, zWrite toggling, etc.

    LayerInspector();
    virtual ~LayerInspector();

    virtual void registerProperties();
//    virtual void update(double deltaTime);

    virtual void setup();
    virtual void update(double deltaTime);
    virtual void createInspectorButtons();

    virtual void refreshList();
    virtual void clearLists();
    virtual void assembleList();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // LAYERWIDGET

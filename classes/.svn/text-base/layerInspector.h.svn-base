#ifndef H_LAYERINSPECTOR
#define H_LAYERINSPECTOR

#include "inspector.h"
#include "assignButton.h"

class LayerInspector: public Inspector{

public:

    AssignButton* newLayer;

    vector<AssignButton*> layerButtons;
    vector<AssignButton*> actorButtons;
    vector<Actor*> actorReferences;         //used for name updating, reordering, layer resorting, zWrite toggling, etc.

    LayerInspector();
    virtual ~LayerInspector();

    virtual void registerProperties();
//    virtual void update(double deltaTime);

    virtual void createInspectorButtons();

    virtual void refreshList();
    virtual void assembleList();

    virtual void trigger(Actor* other);

    virtual void create();
};
#endif // LAYERWIDGET

#ifndef H_PREFABINSPECTOR
#define H_PREFABINSPECTOR

#include "inspector.h"

class PrefabInspector: public Inspector{

public:

    PrefabInspector();
    virtual ~PrefabInspector();

    virtual void createInspectorButtons();

    virtual void refreshList();
    virtual void assembleList();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // PREFABINSPECTOR

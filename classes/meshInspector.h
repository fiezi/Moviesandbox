#ifndef H_MESHINSPECTOR
#define H_MESHINSPECTOR

#include "inspector.h"
#include "meshData.h"

class MeshInspector: public Inspector{

public:

    MeshInspector();
    virtual ~MeshInspector();

    virtual void createInspectorButtons();

    virtual void refreshList();
    virtual void assembleList();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // MeshInspector

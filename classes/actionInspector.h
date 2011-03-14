#ifndef H_ACTIONINSPECTOR
#define H_ACTIONINSPECTOR

#include "inspector.h"

class ActionInspector: public Inspector{

public:

    ActionInspector();
    virtual ~ActionInspector();

    virtual void createInspectorButtons();
    virtual void refreshList();
    virtual void assembleList();

    virtual void trigger(MsbObject* other);

    virtual void create();
};
#endif // ACTIONINSPECTOR

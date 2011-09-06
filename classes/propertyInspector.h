#ifndef H_PROPERTYINSPECTOR
#define H_PROPERTYINSPECTOR

#include "inspector.h"

class PropertyInspector: public Inspector{

public:

    MsbObject*  oldParent;

    PropertyInspector();
    virtual ~PropertyInspector();

    virtual void createInspectorButtons();
    virtual void refreshList();

    virtual void assembleList();
    virtual void assembleListTabTwo();

    virtual void trigger(MsbObject* other);


    virtual void create();
};
#endif // PROPERTYINSPECTOR

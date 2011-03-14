#ifndef H_PROPERTYINSPECTOR
#define H_PROPERTYINSPECTOR

#include "inspector.h"

class PropertyInspector: public Inspector{

public:

    MsbObject*  oldParent;

    PropertyInspector();
    virtual ~PropertyInspector();

    virtual void refreshList();
    virtual void assembleList();

    virtual void create();
};
#endif // PROPERTYINSPECTOR

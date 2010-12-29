#ifndef H_PROPERTYINSPECTOR
#define H_PROPERTYINSPECTOR

#include "inspector.h"

class PropertyInspector: public Inspector{

public:

    Actor*  oldParent;

    PropertyInspector();
    virtual ~PropertyInspector();

    virtual void refreshList();
    virtual void assembleList();

    virtual void create();
};
#endif // PROPERTYINSPECTOR

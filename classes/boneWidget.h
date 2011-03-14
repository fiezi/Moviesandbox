#ifndef H_BONEWIDGET
#define H_BONEWIDGET

#include "widget.h"

class BoneWidget: public Widget{

public:

    Vector4f    highlightColor;

    BoneWidget();
    virtual ~BoneWidget();

    virtual void openWidget();
    virtual void closeWidget();
    virtual void trigger(MsbObject* other);

    virtual void highlight(Actor* other);

    virtual void create();
};
#endif // BONEWIDGET

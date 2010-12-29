#ifndef H_DRAWINGWIDGET
#define H_DRAWINGWIDGET

#include "widget.h"
#include "particleSystem.h"
#include "brush.h"

class DrawTool;

class DrawingWidget: public Widget
{
public:

        DrawTool* drawTool;

        float brushScale;

        DrawingWidget();
        ~DrawingWidget();

        virtual void setup();

        virtual void openWidget();
        virtual void closeWidget();

        virtual void trigger(Actor * other);

        virtual void create();
};
#endif

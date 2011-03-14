#ifndef H_DRAWINGWIDGET
#define H_DRAWINGWIDGET

#include "widget.h"
#include "particleSystem.h"
#include "brush.h"
#include "windows.h"

class DrawTool;

class DrawingWidget: public Widget
{
public:

        DrawTool* drawTool;

        float brushScale;

        bool bKinectToolOpen;

        #ifdef TARGET_WIN32
                PROCESS_INFORMATION processId;
        #else
                pid_t processId;
        #endif

        DrawingWidget();
        ~DrawingWidget();

        virtual void setup();

        virtual void openWidget();
        virtual void closeWidget();

        virtual void trigger(MsbObject* other);

        virtual void openKinectTool();
        virtual void closeKinectTool();
        virtual void importKinect();

        virtual void create();
};
#endif

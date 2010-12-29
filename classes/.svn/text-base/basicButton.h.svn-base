#ifndef H_BASICBUTTON
#define H_BASICBUTTON

#include "actor.h"

#define TOOLTIP_RIGHT 1
#define TOOLTIP_LEFT  2
#define TOOLTIP_ABOVE 3
#define TOOLTIP_BELOW 4

class Input;

class BasicButton: public Actor{
public:

        bool bOver;                             //is set from Input - mouse hovers over button
        bool bDrawName;                         //draws the button name onto it
        bool bAlwaysUpdate;                     //updates this button even if mouse is not over and is not in focus
        bool bPermanent;                        //don't delete when deselecting - also pushes Buttons to "saveableButtonList" when true on default...
        bool bScreenOverlay;                    //gets drawn when running the scene
        bool bMessageWindow;                    //this button is used as an on-screen message.

        bool bDragable;                         //this button can be dragged around...
        bool bConfineDragX;                     //can only be dragged around horizontal
        bool bConfineDragY;                     //can only be dragged around vertical
        bool bResetAfterDrag;                   //sets button back to initial location after finished dragging
        bool bTriggerWhileDragging;             //triggers parent while dragging

        string tooltip;                         //a tooltip to show when mouse hovers over button
        Vector2f tooltipOffset;                 //where to draw the tooltip

        string buttonProperty;                  //stores a string - often used to set the property of some Actor


        int level;                              // used for submenus

        BasicButton();
        virtual ~BasicButton();

        //setup functions
        virtual void registerProperties();

        virtual void setup();
        virtual void update(double deltaTime);
        virtual void drawTooltip();
        virtual void drawPlane();
        virtual void mouseOver();
        virtual void mouseDrag();
        virtual void finishDrag();
        virtual void clickedLeft();
        virtual void clickedRight();
        virtual void focusClick();
        virtual void deselect(int depth);

        virtual void remove();
        virtual void create();

        virtual TiXmlElement* save(TiXmlElement *root);

};
#endif

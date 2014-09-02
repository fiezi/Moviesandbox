#ifndef _INPUT_H_
#define _INPUT_H_

#include "msbObject.h"
#include "x1600Renderer.h"


///******************************
/// #DEFINES
///******************************

#define MOUSEBTNLEFT 0
#define MOUSEBTNMIDDLE 1
#define MOUSEBTNRIGHT 2

#define MOUSEBTNPRESSED 0
#define MOUSEBTNRELEASED 1



class Input: public MsbObject{

public:

///******************************
/// OBJECTS
///******************************

    Renderer*        renderer;


    BasicButton      *focusButton,     //the button that gets all our attention
                     *hudTarget,
                     *dragButton;      // the button we're currently hovering on

    Actor*           worldTarget;     //the actor we clicked on


    static Input*    inputInstance;

///******************************
/// VARIABLES
///******************************


    char        lastKey;                      //last key pressed

    int         mouseX,                       //x Position of Mouse
                mouseY,                       //y Position of Mouse

                startPressLeftBtn,
                startPressRightBtn;


    bool    bWarpMouse;             //configurable input states

    bool        pressedLeft,             //special internal mouse and Keyboard Keys (non-configurable)
                pressedRight,
                pressedMiddle,
                bShiftDown,
                bCtrlDown,
                bAltDown,
                bModifierPressed,
                bTextInput,
                bKeepSelection,
                bPressedMovementKeys,
                bConfineMouse;


    std::string tooltip;
    std::string inputText;

    Vector3f    keyVector,          //delta!!!!
                mouseVector,        //delta!!!!
                mouse3D,
                center3D,
                lastMouse3D;            //mouse position in 3D coordinates (uses picking)


    Vector3f    worldNormal;

    float*      upDown;
    float*      leftRight;

    //TODO: quadwarping should become a tool!
    bool        bQuadWarp;
    int         qwCurrentPoint;


///******************************
/// FUNCTIONS
///******************************

       Input();
       virtual ~Input();

       virtual void registerProperties();

       static Input* getInstance();

       //setup
       void setup();
       void update(double deltaTime);
       void resetInputVectors();

       //mouse
       void pressedMouse(int button,int state,int x, int y);
       void moveMouse(int x, int y);
       void dragMouse(int x, int y);

       //keys
       void normalKeyDown(unsigned char key, int x, int y);
       void specialKeyDown(int key, int x, int y);

       void keyUp(unsigned char key,int x,int y);
       void specialKeyUp (int key,int x, int y);

       //selection and Grouping
       void selectActor(int button);
       void deselectButtons(int depth);
       void deselectActors();

       void confineMouse();

};
#endif

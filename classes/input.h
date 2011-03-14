#ifndef _INPUT_H_
#define _INPUT_H_

#include "msbObject.h"
#include "x1600Renderer.h"
#include "control.h"
#include "inspectorManager.h"
#include "listButton.h"
#include "ofTrueTypeFont.h"
#include "console.h"


///******************************
/// #DEFINES
///******************************


#define FPSBUFFERSIZE 10

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

    Renderer        *renderer;

    Control         *controller;
    Console         *console;

    InspectorManager *inspectorManager;

    BasicButton     *focusButton,     //the button that gets all our attention
                    *hudTarget,
                    *dragButton,      // the button we're currently hovering on
                    *staticButton;    //using a button from the list created in Renderer

    ListButton      *actorMenu;     //holds buttonlist for an Actor we clicked on

    Actor           *worldTarget;     //the actor we clicked on

    vector<Actor*>  selectedActors;   //the last Actor we clicked on/selected - gets NULL when clicked on ground or world
    vector<Actor*>  selectionMarkers;

    Actor           *specialSelected;

    static Input    *inputInstance;

///******************************
/// VARIABLES
///******************************


    char        lastKey;                      //last key pressed

    int         mouseX,                       //x Position of Mouse
                mouseY,                       //y Position of Mouse

                screenX,
                screenY,

                windowX,
                windowY,

                invertMouse,

                startPressLeftBtn,
                startPressRightBtn,

                numParticles;                //for debugging


    bool        pressedLeft,             //special mouse and Keyboard Keys
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

    std::string savedDrawingDirName;
    std::string savedSceneDirName;
    std::string savedActionsDirName;

    std::vector<string> savedDrawings;
    std::vector<string> savedCharacters;
    std::vector<string> savedScenes;
    std::vector<string> userImages;
    std::vector<string> userBrushes;
    std::vector<string> prefabs;


    std::vector<std::string>  eventTrigger;

    Vector3f    keyVector,          //delta!!!!
                mouseVector,        //delta!!!!
                mouse3D,
                lastMouse3D;            //mouse position in 3D coordinates (uses picking)

    float       gridSize;           //size of the general grid


//debug info
    double      fpsBuffer[FPSBUFFERSIZE];
    double		updateTime;
    double		draw3DTime;
    double		drawTime;

    Vector3f    worldNormal;

    string      debugText;         //fps counter and particleCounter, needs to get a bit more elaborate...

    ofTrueTypeFont   verdana;


    float*      upDown;
    float*      leftRight;


///******************************
/// FUNCTIONS
///******************************

       Input();
       virtual ~Input();
       static Input* getInstance();

       //setup
       void setup();
       void setupFonts();
       void update(double deltaTime);

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
       void createNewSelectionMarker(Actor* selectedActor);
       void createActorMenu();

       void makeUserPopUp(string text, Actor* parent);
       void makeWarningPopUp(string message, Actor* parent);

       void confineMouse();

       //text rendering
       void displayDebug();
       void drawText(string str, float x, float y);
       void drawText(string str, float x, float y, Vector4f col, float fontSize);

       //saving and loading
       void saveAll(std::string filename);
       void loadAll(std::string filename, bool bCleanup=true);

       void loadMeshes(std::string fileName);
       void loadPrefab(std::string fileName);
       void loadAction(std::string fileName);           //loads individual action
       void loadActionList(std::string fileName);       //loads list of actions from config file (and later, .asset file)
       void loadTextures(std::string filename);
       void loadShaders(std::string filename);

       void addToLibrary(TiXmlElement* myElement);

       //directory stuff
       void getAllDrawings();
       void getAllScenes();
       void getAllImages();
       void getAllBrushes();
       void getAllPrefabs();

       // 3D and general helper functions
       float convertToGrid(float nonGrid);
       float setToRange(float min, float max, float value);

       //File IO
       string openFileDialog();

};
#endif

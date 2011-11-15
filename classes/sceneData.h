#ifndef H_SCENEDATA
#define H_SCENEDATA

#include "msbObject.h"
#include "content.h"
#include "brush.h"
#include "layer.h"
#include "brush.h"
#include "particleSystem.h"
#include "basicButton.h"
#include "node.h"
#include "action.h"
#include "textfile.h"

#include "msbLight.h"

#include "actorGizmo.h"

#include "control.h"
#include "inspectorManager.h"
#include "listButton.h"
#include "console.h"


using namespace std;

class MeshData;
class ColladaLoader;
class SpriteMeshLoader;
class SpriteMeshLoaderXML;

class SceneData: public MsbObject{

public:


        //*****************************************
        //Template for creating Actor IDs
        //*****************************************
        template<typename myType>
        void createActorID(myType *ref ){
            actorID *act;
            act=new actorID;
            //Actor
            act->actorReference=ref;
            act->actorType=&(typeid(*ref));
            act->actorSize=sizeof(*ref);
            actorInfo[typeid(*ref).name()]=*act;
            std::cout << "created a reference for " << typeid(*ref).name() << "\n";
            std::cout << "created a reference for " << act->actorType->name() << "\n";
        }

        //*****************************************
        //Template for creating an Actor
        //*****************************************
        template <typename myType>
        void addActor (myType *a){

            a = new myType;
            a->renderLayer=currentLayer;
            actorList.push_back(a);
            char newName[8];
            sprintf(newName,"%d",(int)actorList.size()-1);
            a->name+=newName;
            layerList[currentLayer]->actorList.push_back(a);
        }

        //*****************************************
        //Template for creating an Actor
        //*****************************************
        template <typename myType>
        void addButton (myType *b){
            Actor *ac;
            ac = new myType;
            BasicButton *basePtr;
            basePtr = dynamic_cast<BasicButton*>(ac);

            buttonList.push_back(basePtr);

            //add to save list if we are a saveable button!
            if (basePtr->bPermanent)
                saveableButtonList.push_back(basePtr);
        }

        //*****************************************
        //Template for creating a Node
        // Nodes are also represented in the buttonList!
        //*****************************************
        template <typename myType>
        void addNode (myType *n){
            Actor *ac;
            ac = new myType;
            Node *basePtr;
            basePtr = (Node*)ac;
            nodeList.push_back(basePtr);
            buttonList.push_back(basePtr);
        }

        //*****************************************
        //Template for creating an Action
        // Actions are also represented in the buttonList!
        //*****************************************
        template <typename myType>
        void addAction (myType *b){
            Actor *ac;
            ac = new myType;
            Action *basePtr;
            basePtr = dynamic_cast<Action*>(ac);
            buttonList.push_back(basePtr);

        }


        static SceneData* sceneDataInstance;

        Renderer*        renderer;
        Input*           input;

        map <string, actorID> actorInfo;    //for RTTY - all types of actors this program has
                                           //and type/size information for them

        std::string  startSceneFilename;    //scene to load at startup
        std::string  startProject;          //project to load at startup
        std::string  library;               //basic content xml location - meshes, shaders, textures, etc...

        std::string  currentScene;         //our currently loaded scene

        std::string  exe_path;              //directory of the executable

        //lists
        vector <Actor*> actorList;          //all created actors go here
        vector <Actor*> helperList;         //brush and grid go here
        vector <MsbLight*> lightList;         //brush and grid go here

        vector <BasicButton*> buttonList;        //all created buttons go here
        vector <BasicButton*> saveableButtonList;    //all saveable Buttons go here
        vector <Node*> nodeList;            //all created nodes go here
        vector <Layer*> layerList;              //all Layers

        //debug message list
        vector <string> debugMessages;

        //data maps
        map <string, Action*> actionList;    //all actions
        map <string, externalInputData*> externalInputList;    //all external programs for data input
        map <string, MeshData*> vboList;  //new Vertex Buffer Object List
        map <string, textureObject*> textureList;   //all loaded textures go here
        map <string, shaderObject*> shaderList;    //all loaded shaders go here

        int currentLayer;                    //currently selected renderLayer
        string currentShader;                //currently bound shader

        string backgroundTex;
        Vector4f backgroundColor;

        int frames;                         //amount of frames rendered

        double currentTime,                 //time since program start,
              deltaTime,                   //time since last frame
              physicsTime;                 //time for physics step (sometimes deltaTime is too small to advance physics...)

        //Control stuff
        float mouseSensitivity;             //mouse sensitivity and general turn speed
        float moveSpeed;                    //general move speed

        Content *content;                   //the list of things we create at program start
        ColladaLoader* colladaLoader;       //helper object for loading meshes and animations
        SpriteMeshLoader* spriteMeshLoader; //helper Object for loading sprite meshes from Base64
        SpriteMeshLoaderXML* spriteMeshLoaderXML; //helper Object for loading old sprite meshes from XML


       //Interface related - refactored from Input

        Actor* grid;                        //direct pointer to Grid
        Brush* brush;                       //direct pointer to Brush
        ActorGizmo* aGizmo;                 //direct pointer to actorGizmo

        NavTool*            navTool;        //pointer to navTool for convenience
        DrawTool*           drawTool;       //pointer to drawTool for convenience
        GridTool*           gridTool;       //pointer to gridTool for convenience

        Control*            controller;
        Console*            console;

        InspectorManager*   inspectorManager;
        ListButton*         actorMenu;     //holds buttonlist for an Actor we clicked on

        BasicButton*        staticButton;    //using a button from the list created in Renderer

        vector<Actor*>      selectedActors;   //the last Actor we clicked on/selected - gets NULL when clicked on ground or world

        std::vector<string> savedDrawings;
        std::vector<string> savedCharacters;
        std::vector<string> savedScenes;
        std::vector<string> userImages;
        std::vector<string> userBrushes;
        std::vector<string> prefabs;


        std::vector<std::string>  eventTrigger;

        float       gridSize;                   //size of the general grid

        string      debugText;                  //fps counter and particleCounter, needs to get a bit more elaborate...
        int         numParticles;               //for debugging

        int         invertMouse;

        int         numberOfUntitledDrawings;   //number of untitled drawings in project

    ////////////////////////////////////////////////////////////////////////
    // FUNCTIONS
    ////////////////////////////////////////////////////////////////////////

        SceneData();
        virtual ~SceneData();

        //singleton-ish
        static SceneData* getInstance();

        virtual void setup();

        virtual void fillGlobalLists();                  //for RTTY - generate actorInfo

        virtual void loadPreferences();                  //first thing called when program starts

        virtual void createScene();


        virtual void update(float deltaTime);

        virtual void addLayer(string layerName);
        virtual void addGrid();
        virtual void addBrush();
        virtual void addGizmos();

        virtual int readSharedMemory();

        virtual void updateView();

        //Interface related
        virtual void createActorMenu();

        virtual void makeUserPopUp(string text, Actor* parent);
        virtual void makeWarningPopUp(string message, Actor* parent);

        //saving and loading
        void saveAll(std::string filename);
        void loadAll(std::string fileName, bool bCleanup=true);

        void loadMeshes(std::string path, std::string fileName);
        void loadPrefab(std::string path, std::string fileName);
        void loadAction(std::string path, std::string fileName);           //loads individual action
        void loadActionList(std::string path, std::string fileName);       //loads list of actions from config file (and later, .asset file)
        void loadTextures(std::string path, std::string filename);
        void loadShaders(std::string path, std::string filename);

        void loadScene(string sceneName, bool bStart=false);
        void loadProject(string projectName, bool bStart=false);

        void saveScene(std::string sceneName, bool bStart=false);

        void newScene();
        void newProject(std::string projectName);

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
        string openFileDialog(string ext="*");

        string saveFileDialog(string ext="*");


};


#endif

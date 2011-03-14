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
#include "flexGrid.h"

#include "msbLight.h"


#define BUFSIZE 512



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

        //verion debug


       static SceneData* sceneDataInstance;
       Renderer*        renderer;
       Input*           input;

       map <string, actorID> actorInfo;    //for RTTY - all types of actors this program has
                                           //and type/size information for them

       std::string  startSceneFilename;    //basic scene
       vector<std::string>  library;       //all libraries
       //lists
       vector <Actor*> actorList;          //all created actors go here
       vector <Actor*> helperList;         //brush and grid go here
       vector <MsbLight*> lightList;         //brush and grid go here

       vector <BasicButton*> buttonList;        //all created buttons go here
       vector <BasicButton*> saveableButtonList;    //all saveable Buttons go here
       vector <Node*> nodeList;            //all created nodes go here
       vector <Layer*> layerList;              //all Layers
       map <string, Action*> actionList;    //all actions
       map <string, externalInputData*> externalInputList;    //all external programs for data input

       //map <string, ObjFile> meshList;     //old Mesh List
       map <string, MeshData*> vboList;  //new Vertex Buffer Object List
       map <string, textureObject*> textureList;   //all loaded textures go here
       map <string, shaderObject*> shaderList;    //all loaded shaders go here

       int currentLayer;                    //currently selected renderLayer
       string currentShader;                //currently bound shader

       string backgroundTex;
       Vector4f backgroundColor;

       Actor* grid;                        //direct pointer to Grid
       Brush* brush;                       //direct pointer to Brush

       int screenX,screenY;                //rendered screen width, height - maybe rename..
       int windowX,windowY;                //window width and height

       int frames;                         //amount of frames rendered
       double currentTime,                 //time since program start,
              deltaTime,                   //time since last frame
              physicsTime;                 //time for physics step (sometimes deltaTime is too small to advance physics...)

       //Control stuff
       float mouseSensitivity;             //mouse sensitivity and general turn speed
       float moveSpeed;                    //general move speed

       float fov;                          //field of view

       Content *content;                   //the list of things we create at program start
       ColladaLoader* colladaLoader;       //helper object for loading meshes and animations
       SpriteMeshLoader* spriteMeshLoader; //helper Object for loading sprite meshes from Base64
       SpriteMeshLoaderXML* spriteMeshLoaderXML; //helper Object for loading old sprite meshes from XML

       SceneData();
       virtual ~SceneData();

       //singleton-ish
       static SceneData* getInstance();

       //***************************************
       //this is stuff we can update easily
       //***************************************
       virtual void fillGlobalLists();                  //for RTTY - generate actorInfo

       virtual void setup();                      //first thing called when program starts
       virtual void loadPreferences();

       virtual void update(float deltaTime);

       virtual void addLayer(string layerName);
       virtual void addGrid();
       virtual void addBrush();

       virtual int readSharedMemory();

};


#endif

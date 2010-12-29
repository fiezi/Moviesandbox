#ifndef H_ACTOR
#define H_ACTOR

#include "includes.h"


#define DRAW_PLANE          0
#define DRAW_MESH           1
#define DRAW_TEA            2
#define DRAW_CUBE           3
#define DRAW_PARTICLES      4
#define DRAW_PARTICLES_LINE 5
#define DRAW_PARTICLES_PATH 6
#define DRAW_SPRITE         7
#define DRAW_VBOMESH        8
#define DRAW_SPECIAL        9

#define DRAW_NULL           100


//*****************************************
//Template for assigning memberIDs
//*****************************************
template <typename myType>
bool setPropertyTo (memberID* mID, myType value){
    const std::type_info* mType;
    const std::type_info* strType;
    std::string s;

    mType=&(typeid(value));
    strType=&(typeid(s));

    if (mID->memberType->name()==mType->name())
        {
        memcpy(mID->memberReference,&value,sizeof(value));
        return true;
        }
    else
        return false;
}

template <typename myType>
bool setVecPropertyTo (memberID* mID, vector<myType> vec){

   if (mID->memberType->name()==typeid(vec).name()){
        vector<myType> * myVec = (vector<myType> *) (mID->memberReference);
        myVec->clear();
        for (int i=0;i<(int)vec.size();i++){
            myVec->push_back(vec[i]);
        }
        return true;
    }
    return false;

}

//*****************************************
//Template for creating Member IDs
//*****************************************

template<typename MyType, typename ActorClass>
void createMemberID(const char* memberName, MyType *memberRef, ActorClass* base){
    memberID mID;
    //Actor
    mID.memberReference=memberRef;
    mID.memberType=&(typeid(*memberRef));
    mID.memberSize=sizeof(*memberRef);
    mID.memberName=(char*)memberName;
    base->property[memberName]=mID;
}

class Control;
class Input;
class Renderer;
class InterpolationHelper;
class Node;
class Action;

typedef int ListID;                             //convenient way of making clear what those mean...
typedef unsigned int uint;                      //to get rid of compiler warnings and not type "unsigned" all the time...

class Actor{

        /***********************************************************************************

        Variables

        ************************************************************************************/

public:

       Renderer* renderer;                          //connections to renderer and input classes...
       Input* input;

       int renderLayer;                             //layer in which this Actor is being rendered

       std::map <std::string, memberID> property;   //actor variables as string representations for easy property passing

       Vector3f location,                           //space properties
                rotation,
                scale,
                orientation,                        //orientation of actor in world space
                xAxis,                              //three axis that make up the rotation of the actor
                yAxis,
                zAxis;

       // Animation blending
       float transitionTime;                        //time to interpolate between different movers
       vector<float> currentTransitions;            //current Time in transition
       int numberOfMovers;                          //number of Movers in last frame

       vector<Vector3f> moverLocations;             //buffers for different movers
       vector<Vector3f> moverRotations;
       vector<Matrix4f> moverMatrices;

       float collisionCubeSize;                     //the size of the pickable cube

       float particleScale;                         //size multiplier for particle Mesh
       float particleAngleScale;                    //angle size attenuation multiplier for particle Mesh (to fill holes)

       Vector4f color;

       Vector3f pivotLocation;                      //for rotation around a different point - is relative to location!!!

       Matrix4f originalMatrix;                     //original transformation - makes transformmatrix relative to this one - used for bones!
       Matrix4f transformMatrix;                    //our individual transform, without base transformations
       Matrix4f scaleMatrix;                        //our scaling
       bool     bUseTransformMatrix;                //should we use the matrix?


       Matrix4f initialTransform;                   //for resetting after scene finished playing
       Vector3f initialLocation;                    //mainly used for buttons!
       Vector3f initialRotation;

       Actor    *base;                              //for forward kinematics and hirarchical animation
       Matrix4f  baseMatrix;                        //stores the final actor matrix (to be used as base for the next child)

       Actor    *parent;                            //actor is depending on this one - not rotation/translation wise, but maybe hirarchical?
       std::string      groupID;                            //which group this actor belongs to.

       Node     *baseNode;                          //the node controlling this actor

       std::string name;                            //actor name - not unique yet!
       std::string sceneShaderID;                   //shader identifier

       std::string vboMeshID;                       //new COLLADA mesh identifier

       std::string textureID;                         //texture identifier for texture instancing

       int drawType;                                //which draw function to call: e.g. DRAW_CUBE, DRAW_MESH, DRAW_SPRITE, etc...

       double elapsedTime;                          //time since scene started
       double birth;                                //time when this actor was created

       bool    bInit;                               //is false before loading

       bool bHidden;                                //is rendered
       bool bPickable;                              //can be clicked on (mouse)
       bool bRemoveable;                            //cannot be removed...
       bool bSelected;                              //is in input->selectedActors

       //Render options
       bool bTextured,                              //uses Texture Map in TextureUnit 0
            bShadowTextured,                                //uses ShadowMap in TextureUnit 1
            bZTest,                                 //checks Z-Buffer before drawing
            bZWrite,                                //writes to Depth Buffer
            bUseShader,                             //uses Shader for drawing
            bComputeLight,                            //lighting disabled
            bDrawOrientation;                       //draws RGB Lines for XYZ Orientation

       bool bLight;                                 //counts as light

       GLuint   blendModeOne;                       //first Blend mode
       GLuint   blendModeTwo;                       //second Blend mode

       Control *controller;                         //somewhat undefined, takes control when scene is playing
       vector<InterpolationHelper*> movers;                  //moves the actor - will be destroyed when keyframe reached

       //for radial menu
       std::vector <std::string> menuType;          //menu list for actor (when mouse is clicked on actor
       std::vector <std::string> menuProp;
       std::vector <std::string> menuName;
       std::vector <std::string> menuIcon;
       //end radial menu

        // timer for texture animation
        float   textTimer;

        // texture properties
        Vector3f    texScale,
                    texRotation,
                    texTranslation;

        //for actor loading
        int actorOffset;
        int nodeOffset;

        //for actor picking
        float objectID;


        /***********************************************************************************

        Functions

        ************************************************************************************/

       Actor();
       virtual ~Actor();

       //setup functions
       virtual void registerProperties();           //fill property map with all user-changeable variables as string properties

       virtual void postLoad();                     //gets called immediately after all variables have been loaded, but before setup
       virtual void setup();
       virtual void trigger(Actor * other);         //general trigger function

       //runtime looping functions
       virtual void update(double deltaTime);       //handles interpolation
       virtual void updateShaders();                // uniform updates
       virtual void draw();                         // sets up rendering flags and transformations

       //drawType functions - the actual glBegin() stuff is in here
       virtual void drawMesh();
       virtual void drawSprite();
       virtual void drawPlane();
       virtual void drawParticles();
       virtual void drawTeapot();
       virtual void drawCube();
       virtual void drawVBOMesh();
       virtual void drawSkeletal();

       //get back to initial state when scene started
       virtual void reset();

       virtual void start();                            //gets called from controller when running scene
       virtual void stop();                             //gets called from controller when running scene

       virtual void setBase(Actor* newBase);            //set Base and keep actor's position on screen

       //transformation functions - please always call!
       virtual void setLocation(Vector3f loc);          //set translation
       virtual void setAbsoluteLocation(Vector3f loc);          //set translation
       virtual void setRotation(Vector3f rot);          //set rotation
       virtual void setRotation(Vector3f xA, Vector3f yA, Vector3f zA); //set rotation from 3 axis
       virtual void addRotation(float amount,Vector3f axis);          //add to rotation - used for mouse rotation when bUseTransformmatrix
       virtual void setScale(Vector3f newScale);

       virtual void getAxis(Vector3f* xA, Vector3f* yA, Vector3f* zA, Matrix4f myMatrix);
       virtual void setRotationFromAxis( Vector3f xA, Vector3f yA, Vector3f zA);
       virtual Matrix4f calcMatrix(Actor* myActor);     //recursive function to determine absolute transformMatrix
       virtual Vector3f calcEuler(Matrix4f myMatrix);   //Rotation Matrix to Euler Angles
       virtual void matrixToVectors();              //generates location vector, axis and rotation in euler angles

       virtual TiXmlElement* save(TiXmlElement *root);  //saves all properties as defined in registerProperties
       virtual TiXmlElement* saveAsPrefab(TiXmlElement *root);  //saves all properties as defined in registerProperties
       virtual void load(TiXmlElement *myInfo);         //loads all properties as defined in registerProperties

       //property assigning
       virtual string memberToString(memberID *mID);                //reads a member from a string
       virtual void memberFromString(memberID *mID,string value);   //sets a member as defined in registerProperties to a value given as a string

       virtual bool setStringPropertyTo(memberID *mID,string s);
       virtual bool setActorPropertyTo(memberID *mID,Actor* a);
       virtual bool setNodePropertyTo(memberID * mID,Node* n);

       // XML parsing functions for loading:
       virtual string writeMatrix3f(memberID* mID);
       virtual string writeMatrix4f(memberID* mID);
       virtual string writeVector3f(memberID* mID);
       virtual string writeVector4f(memberID* mID);
       virtual string writeInt(memberID* mID);
       virtual string writeFloat(memberID* mID);
       virtual string writeDouble(memberID* mID);
       virtual string writeBool(memberID* mID);
       virtual string writeGLuint(memberID* mID);
       virtual string writeString(memberID* mID);

       virtual string writeNode(memberID* mID);
       virtual string writeActor(memberID* mID);

       virtual string writeVecActor(memberID* mID);
       virtual string writeVecNode(memberID* mID);
       virtual string writeVecString(memberID* mID);
       virtual string writeVecVector3f(memberID* mID);
       virtual string writeVecVector4f(memberID* mID);


       virtual Vector3f readVector3f(char* cValue);
       virtual Vector4f readVector4f(char* cValue);
       virtual Matrix3f readMatrix3f(char* cValue);
       virtual Matrix4f readMatrix4f(char* cValue);
       virtual bool readBool(char* cValue);
       virtual GLuint readGLuint(char* cValue);
       virtual int readInt(char* cValue);
       virtual Actor* readActor(char* cValue);                         //identical to readInt, but parses the actor* tag in the string!
       virtual Node* readNode(char* cValue);                          //identical to readInt, but parses the node* tag in the string!
       virtual float readFloat(char* cValue);
       virtual double readDouble(char* cValue);
       virtual string readString(char* cValue);

       virtual vector<string> readVecString(char* cValue);
       virtual vector<Actor*> readVecActor(char* cValue);
       virtual vector<Node*> readVecNode(char* cValue);
       virtual vector<Vector3f> readVecVector3f(char* cValue);
       virtual vector<Vector4f> readVecVector4f(char* cValue);

       virtual void remove();                           //removes the actor from actorList and frees memory.

       bool isA(string className);                      //check against specific classname

       bool createNewActor(string actorIDName);         //runtime Actor spawning through a string

       Actor* spawn(string actorIDName);

       virtual void create();                           //runtime Actor creation
};



#endif

#ifndef H_MSBOBJECT
#define H_MSBOBJECT

#include "includes.h"


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

class Input;
class Renderer;
class SceneData;
class Node;

typedef int ListID;                             //convenient way of making clear what those mean...
typedef unsigned int uint;                      //to get rid of compiler warnings and not type "unsigned" all the time...

class MsbObject{

        /***********************************************************************************

        Variables

        ************************************************************************************/

public:

       Renderer* renderer;                          //connections to renderer and input classes...
       Input* input;
       SceneData* sceneData;

       std::map <std::string, memberID> property;   //actor variables as string representations for easy property passing



       MsbObject    *parent;                            //actor is depending on this one - not rotation/translation wise, but maybe hirarchical?

       std::string name;                            //actor name - not unique yet!

        Vector4f color;                             //yes, objects can have color. Why not?

       double elapsedTime;                          //time since scene started


        /***********************************************************************************

        Functions

        ************************************************************************************/

       MsbObject();
       virtual ~MsbObject();

       //setup functions
       virtual void registerProperties();           //fill property map with all user-changeable variables as string properties

       virtual void setup();
       virtual void trigger(MsbObject * other);         //general trigger function

       //runtime looping functions
       virtual void update(double deltaTime);       //handles interpolation

       //get back to initial state when scene started
       virtual void reset();

       virtual void start();                            //gets called from controller when running scene
       virtual void stop();                             //gets called from controller when running scene


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
};



#endif

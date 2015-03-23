#include <sstream>

#include "msbObject.h"
#include "sceneData.h"
#include "node.h"
#include "action.h"
#include "meshData.h"

MsbObject::MsbObject(){

parent=NULL;
color=COLOR_WHITE;
elapsedTime=0.0;

name="MsbObject";


registerProperties();
}

MsbObject::~MsbObject(){
}

void MsbObject::registerProperties(){

createMemberID("NAME",&name,this);
}


void MsbObject::setup(){

}

void MsbObject::trigger(MsbObject * other){



}

void MsbObject::update(double deltaTime){


    elapsedTime+=deltaTime;

}



void MsbObject::reset(){}

/* control start/stop */

void MsbObject::start(){}
void MsbObject::stop(){}


/* saving/loading stuff */

//saves all properties as defined in registerProperties
TiXmlElement* MsbObject::save(TiXmlElement *root){

TiXmlElement * element = new TiXmlElement( "Actor" );
element->SetAttribute("name", name);
TiXmlText * text = new TiXmlText( typeid(*this).name() );
element->LinkEndChild( text );

std::map <std::string, memberID>::iterator it;

for ( it=property.begin() ; it != property.end(); it++ )
  {
  TiXmlElement * property=new TiXmlElement(it->first);
  string value = memberToString(&it->second);
  property->LinkEndChild ( new TiXmlText( value));
  element->LinkEndChild( property );
  }

return element;
}

//msbObjects don't belong in prefabs!
TiXmlElement* MsbObject::saveAsPrefab(TiXmlElement *root){

    return NULL;
}

//loads all properties as defined in registerProperties
void MsbObject::load(TiXmlElement *myInfo){

    std::map <std::string, memberID>::iterator it;

    for ( it=property.begin() ; it != property.end(); it++ ){

        TiXmlElement * xmlProperty=NULL;                     //always initialise if you want to check against NULL!
        xmlProperty=myInfo->FirstChildElement(it->first);
        if (xmlProperty && xmlProperty->GetText()!=NULL){        //check if the saved file has this element

            string myValue=xmlProperty->Value();

            memberFromString(&(it->second),xmlProperty->GetText());
        }
        else
            cout << "unknown property..." << endl;
    }

}

//reads out a property value as a string
string MsbObject::memberToString(memberID *mID, bool bPart, int part){

    //Vector4
    if (mID->memberType->name()==typeid(Vector4f).name()){
        if (!bPart)
            return writeVector4f(mID);
        else{
            if (part==0)
                return writeVector4fdotX(mID);
            if (part==1)
                return writeVector4fdotY(mID);
            if (part==2)
                return writeVector4fdotZ(mID);
            if (part==3)
                return writeVector4fdotW(mID);
        }
    }

    //Vector3f
    if (mID->memberType->name()==typeid(Vector3f).name() ){
        if (!bPart)
            return writeVector3f(mID);
        else{
            if (part==0)
                return writeVector3fdotX(mID);
            if (part==1)
                return writeVector3fdotY(mID);
            if (part==2)
                return writeVector3fdotZ(mID);
        }
    }

    //float
    if (mID->memberType->name()==typeid(float).name())
        return writeFloat(mID);

    //bool
    if (mID->memberType->name()==typeid(bool).name())
       return writeBool(mID);

    //gluint
    if (mID->memberType->name()==typeid(GLuint).name())
        return writeGLuint(mID);

    //Matrix4f
    if (mID->memberType->name()==typeid(Matrix4f).name())
        return writeMatrix4f(mID);

    //Matrix3f
    if (mID->memberType->name()==typeid(Matrix3f).name())
        return writeMatrix3f(mID);

    //int
    if (mID->memberType->name()==typeid(int).name())
        return writeInt(mID);

    //double
    if (mID->memberType->name()==typeid(double).name())
        return writeDouble(mID);

    //string
    if (mID->memberType->name()==typeid(string).name())
        return writeString(mID);

    //Actor*
    if (mID->memberType->name()==typeid(Actor*).name())
        return writeActor(mID);

    //Node Pointer
    if (mID->memberType->name()==typeid(Node*).name())
        return writeNode(mID);

    ///vectors

    //Vector Actor*
    if (mID->memberType->name()==typeid(vector<Actor*>).name())
        return writeVecActor(mID);

    //Vector Node*
    if (mID->memberType->name()==typeid(vector<Node*>).name())
        return writeVecNode(mID);

    //Vector String*
    if (mID->memberType->name()==typeid(vector<string>).name())
        return writeVecString(mID);

    //Vector Vec3f*
    if (mID->memberType->name()==typeid(vector<Vector3f>).name())
        return writeVecVector3f(mID);

    //Vector Vec4f*
    if (mID->memberType->name()==typeid(vector<Vector4f>).name())
        return writeVecVector4f(mID);


    return "NULL";
}

//sets a property as defined through registerProperties to a value passed in a string
void MsbObject::memberFromString(memberID *mID,string value){

    char * cValue;
    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());


    //allow individual components of vectors to be changed
    //for this, in order to not mix up vector components and full vectors, let's check first

    if (testForVector3Component(cValue)){
        if (setVector3PropertyTo (mID, readVector3fComponent(cValue))){
            free(cValue);
            return;
        }
    }


    if (testForVector4Component(cValue)){
        if (setVector4PropertyTo (mID, readVector4fComponent(cValue))){
            free(cValue);
            return;
        }
    }

    //give special priority to locations and other vectors!
    if (setPropertyTo (mID, readVector3f(cValue))){
        free(cValue);
        return;
    }
    if (setPropertyTo (mID, readVector4f(cValue))){
        free(cValue);
        return;
    }
    if (setPropertyTo (mID, readMatrix4f(cValue))){
        free(cValue);
        return;
    }
    if (setPropertyTo (mID, readBool(cValue))){
        free(cValue);
        return;
    }
    if (setPropertyTo (mID, readGLuint(cValue))){
        free(cValue);
        return;
    }
    if (setPropertyTo (mID, readInt(cValue))){
        free(cValue);
        return;
    }
    if (setPropertyTo (mID, readFloat(cValue))){
        free(cValue);
        return;
    }

    if (setStringPropertyTo(mID, readString(cValue))){
        free(cValue);
        return;
    }

    if (setNodePropertyTo(mID, readNode(cValue))){
        free(cValue);
        return;
    }
    if (setActorPropertyTo(mID, readActor(cValue))){
        free(cValue);
        return;
    }

    ///vectors


    if (setVecPropertyTo (mID,readVecString(cValue))){
        free(cValue);
        return;
    }

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecActor(cValue))){
        free(cValue);
        return;
    }

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecNode(cValue))){
        free(cValue);
        return;
    }

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecVector3f(cValue))){
        free(cValue);
        return;
    }
    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecVector4f(cValue))){
        free(cValue);
        return;
    }
    free(cValue);
}

//TODO: I am sure this can be done much nicer...
bool MsbObject::testForVector3Component(char* cValue){

    if( strncmp("vec3fx ",cValue,7) == 0 )
        return true;
    if( strncmp("vec3fy ",cValue,7) == 0 )
        return true;
    if( strncmp("vec3fz ",cValue,7) == 0 )
        return true;

    return false;
}

bool MsbObject::testForVector4Component(char* cValue){

    if( strncmp("vec4fx ",cValue,7) == 0 )
        return true;
    if( strncmp("vec4fy ",cValue,7) == 0 )
        return true;
    if( strncmp("vec4fz ",cValue,7) == 0 )
        return true;
    if( strncmp("vec4fw ",cValue,7) == 0 )
        return true;

    return false;
}

bool MsbObject::setVector4PropertyTo(memberID * mID,Vector4f v){

    if (mID->memberType->name()==typeid(v).name())
      {
      Vector4f * myVec=(Vector4f*)mID->memberReference;
      if (v.x!=-65536.0)
        myVec->x=v.x;
      if (v.y!=-65536.0)
        myVec->y=v.y;
      if (v.z!=-65536.0)
        myVec->z=v.z;
      if (v.w!=-65536.0)
        myVec->w=v.w;
      return true;
      }
    return false;
}

bool MsbObject::setVector3PropertyTo(memberID * mID,Vector3f v){

    if (mID->memberType->name()==typeid(v).name())
      {
      Vector3f * myVec=(Vector3f*)mID->memberReference;
      if (v.x!=-65536.0)
        myVec->x=v.x;
      if (v.y!=-65536.0)
        myVec->y=v.y;
      if (v.z!=-65536.0)
        myVec->z=v.z;
      return true;
      }
    return false;
}

//gets called from memberFromString! Don't use as standalone function!
bool MsbObject::setStringPropertyTo(memberID * mID,string s){

    if (mID->memberType->name()==typeid(s).name())
      {
      string * theString=(string*)mID->memberReference;
      *theString=s;
      return true;
      }
    return false;
}


//gets called from memberFromString! Don't use as standalone function!
bool MsbObject::setActorPropertyTo(memberID * mID,Actor* a){


    const std::type_info* mType;
    mType=&(typeid(a));

    if (mID->memberType->name()==mType->name() )
      {
      //double referencing ftw!!
      //get adress of pointer
      Actor** pa= (Actor**)mID->memberReference;
      //switch content of adress of pointer (wow!)
      if (a)
        *pa=a;
      else
        *pa=NULL;
      return true;
      }
    return false;
}

bool MsbObject::setNodePropertyTo(memberID * mID,Node* n){


    const std::type_info* mType;
    mType=&(typeid(n));

    if (mID->memberType->name()==mType->name() && n )
      {
      cout << "loading a Node* property!" << endl;
      //double referencing ftw!!
      //get adress of pointer
      Node** pa= (Node**)mID->memberReference;
      //switch content of adress of pointer (wow!)
      *pa=n;
      cout << "referenced Node name: " << (*pa)->name << endl;
      return true;
      }
    return false;
}


/* xml parsing */

string MsbObject::writeVector3f(memberID* mID){
    char value[512];
    Vector3f * p;
    p=(Vector3f*)mID->memberReference;
    sprintf(value,"vec3f %f %f %f",p->x, p->y, p->z);
    return value;
}

string MsbObject::writeVector3fdotX(memberID* mID){
    char value[512];
    Vector3f * p;
    p=(Vector3f*)mID->memberReference;
    sprintf(value,"vec3fx %f",p->x);
    return value;
}

string MsbObject::writeVector3fdotY(memberID* mID){
    char value[512];
    Vector3f * p;
    p=(Vector3f*)mID->memberReference;
    sprintf(value,"vec3fy %f",p->y);
    return value;
}

string MsbObject::writeVector3fdotZ(memberID* mID){
    char value[512];
    Vector3f * p;
    p=(Vector3f*)mID->memberReference;
    sprintf(value,"vec3fz %f",p->z);
    return value;
}

string MsbObject::writeFloat(memberID* mID){
    char value[512];
    float * p;
    p=(float*)mID->memberReference;
    sprintf(value,"float %f",*p);
    return value;
}

string MsbObject::writeDouble(memberID* mID){
    char value[512];
    double * p;
    p=(double*)mID->memberReference;
    sprintf(value,"double %f",*p);
    return value;
}

string MsbObject::writeVector4f(memberID* mID){
    char value[512];
    Vector4f * p;
    p=(Vector4f*)mID->memberReference;
    sprintf(value,"vec4f %f %f %f %f",p->r, p->g, p->b, p->a);
    return value;
}

string MsbObject::writeVector4fdotX(memberID* mID){
    char value[512];
    Vector4f * p;
    p=(Vector4f*)mID->memberReference;
    sprintf(value,"vec4fx %f",p->r);
    return value;
}

string MsbObject::writeVector4fdotY(memberID* mID){
    char value[512];
    Vector4f * p;
    p=(Vector4f*)mID->memberReference;
    sprintf(value,"vec4fy %f",p->g);
    return value;
}

string MsbObject::writeVector4fdotZ(memberID* mID){
    char value[512];
    Vector4f * p;
    p=(Vector4f*)mID->memberReference;
    sprintf(value,"vec4fz %f",p->b);
    return value;
}

string MsbObject::writeVector4fdotW(memberID* mID){
    char value[512];
    Vector4f * p;
    p=(Vector4f*)mID->memberReference;
    sprintf(value,"vec4fw %f",p->a);
    return value;
}

string MsbObject::writeBool(memberID* mID){
    char value[512];
    bool * p;
    p=(bool*)mID->memberReference;
    sprintf(value,"bool %i",*p);
    return value;
}

string MsbObject::writeGLuint(memberID* mID){
    char value[512];
    GLuint * p;
    p=(GLuint*)mID->memberReference;
    sprintf(value,"GLuint %i",*p);
    return value;
}

string MsbObject::writeMatrix4f(memberID* mID){
    char value[512];
    Matrix4f mat4f;
    Matrix4f * p;
    p=(Matrix4f*)mID->memberReference;
    mat4f=*p;
    sprintf(value,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
                    ,mat4f.data[0], mat4f.data[1], mat4f.data[2], mat4f.data[3],
                     mat4f.data[4], mat4f.data[5], mat4f.data[6], mat4f.data[7],
                     mat4f.data[8], mat4f.data[9], mat4f.data[10],mat4f.data[11],
                     mat4f.data[12],mat4f.data[13],mat4f.data[14],mat4f.data[15]);
    return value;
}

string MsbObject::writeInt(memberID* mID){
    char value[512];
    int * p;
    p=(int*)mID->memberReference;
    sprintf(value,"int %i",*p);
    return value;
}

string MsbObject::writeMatrix3f(memberID* mID){
       char value[512];
       Matrix3f * p;
       Matrix3f mat3f;
       p=(Matrix3f*)mID->memberReference;
       mat3f=*p;
       sprintf(value,"mat3f %f %f %f %f %f %f %f %f %f"
                    ,mat3f.data[0], mat3f.data[1], mat3f.data[2],
                     mat3f.data[3], mat3f.data[4], mat3f.data[5],
                     mat3f.data[6], mat3f.data[7], mat3f.data[8]);
       return value;
}

string MsbObject::writeString(memberID* mID){
    string* me=(string*) mID->memberReference;
    string s="string ";
    if (me)
        s+= (*me);
    return s;
}

string MsbObject::writeNode(memberID* mID){

    char value[512];
    Node* n;
    Node** p;                           //the ugliness that is **!
    p=(Node**)(mID->memberReference);   //We're actually holding the ADRESS of a NODE* in the memberReference
    n=*p;                                //so we need to dereference twice!

    if (n){                                   //save actor position in actorList!
        for (uint i=0; i<sceneData->nodeList.size();i++){
            if (sceneData->nodeList[i]==n) sprintf(value,"node* %i",i);
        }
    } else sprintf(value,"node* -1");

    return value;
}

string MsbObject::writeActor(memberID* mID){

    char value[512];
    Actor* a=NULL;
    Actor** p=NULL;                           //the ugliness that is **!
    p=(Actor**)(mID->memberReference);   //We're actually holding the ADRESS of an ACTOR* in the memberReference
    if (p)
        a=*p;                                //so we need to dereference twice!

    if (a){                                   //save actor position in actorList!
        for (uint i=0; i<sceneData->actorList.size();i++){
            if (sceneData->actorList[i]==a) sprintf(value,"actor* %i",i);
        }
    } else sprintf(value,"actor* -1");

    return value;
}


string MsbObject::writeVecActor(memberID* mID){

        string fullString="vector";
        vector<Actor*> * myVec = (vector<Actor*> *)mID->memberReference;


        //cout << "size of saveable Vector: "<< myVec->size() << endl;
        for (int i=0; i< (int)myVec->size(); i++){
            //create temporary mID
            createMemberID("temp",&((*myVec)[i]), this);
            //add to our string
            fullString+="|" + writeActor(&property["temp"]);
            property.erase("temp");
            }
        return fullString;
}

string MsbObject::writeVecNode(memberID* mID){

        string fullString="vector";
        vector<Node*> * myVec = (vector<Node*> *)mID->memberReference;
        for (int i=0; i< (int)myVec->size(); i++){
            //create temporary mID
            createMemberID("temp",&((*myVec)[i]), this);
            //add to our string
            fullString+="|" + writeNode(&property["temp"]);
            property.erase("temp");
            }
        return fullString;
}

string MsbObject::writeVecString(memberID* mID){
       string fullString="vector";
        vector<string> * myVec = (vector<string> *)mID->memberReference;
        for (int i=0; i< (int)myVec->size(); i++){
            //create temporary mID
            createMemberID("temp",&((*myVec)[i]), this);
            //add to our string
            fullString+="|" + writeString(&property["temp"]);
            property.erase("temp");
            }
        return fullString;
}

string MsbObject::writeVecVector3f(memberID* mID){
       string fullString="vector";
        vector<Vector3f> * myVec = (vector<Vector3f> *)mID->memberReference;
        for (int i=0; i< (int)myVec->size(); i++){
            //create temporary mID
            createMemberID("temp",&((*myVec)[i]), this);
            //add to our string
            fullString+="|" + writeVector3f(&property["temp"]);
            property.erase("temp");
            }
        return fullString;
}

string MsbObject::writeVecVector4f(memberID* mID){
       string fullString="vector";
        vector<Vector4f> * myVec = (vector<Vector4f> *)mID->memberReference;
        for (int i=0; i< (int)myVec->size(); i++){
            //create temporary mID
            createMemberID("temp",&((*myVec)[i]), this);
            //add to our string
            fullString+="|" + writeVector4f(&property["temp"]);
            property.erase("temp");
            }
        return fullString;
}



Vector3f MsbObject::readVector3f(char* cValue){
Vector3f vec3f;
if( strncmp("vec3f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f",&vec3f.x,&vec3f.y,&vec3f.z);
return vec3f;
}

Vector3f MsbObject::readVector3fComponent(char* cValue){

    //this is done so we can check if anything changed
    Vector3f vec3f=Vector3f(-65536.0,-65536.0,-65536.0);

    if( strncmp("vec3fx ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec3f.x);
    if( strncmp("vec3fy ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec3f.y);
    if( strncmp("vec3fz ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec3f.z);
    return vec3f;
}

Vector4f MsbObject::readVector4f(char* cValue){
Vector4f vec4f;
if( strncmp("vec4f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f%f",&vec4f.r,&vec4f.g,&vec4f.b,&vec4f.a);
return vec4f;
}

Vector4f MsbObject::readVector4fComponent(char* cValue){

    //this is done so we can check if anything changed
    Vector4f vec4f=Vector4f(-65536.0,-65536.0,-65536.0,-65536.0);

    if( strncmp("vec4fx ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec4f.x);
    if( strncmp("vec4fy ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec4f.y);
    if( strncmp("vec4fz ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec4f.z);
    if( strncmp("vec4fw ",cValue,7) == 0 )
        sscanf((cValue+7),"%f",&vec4f.w);
    return vec4f;
}

Matrix4f MsbObject::readMatrix4f(char* cValue){
Matrix4f mat4f;
if( strncmp("mat4f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",&mat4f.data[0],&mat4f.data[1],&mat4f.data[2],&mat4f.data[3]
                                                        ,&mat4f.data[4],&mat4f.data[5],&mat4f.data[6],&mat4f.data[7]
                                                        ,&mat4f.data[8],&mat4f.data[9],&mat4f.data[10],&mat4f.data[11]
                                                        ,&mat4f.data[12],&mat4f.data[13],&mat4f.data[14],&mat4f.data[15]);
return mat4f;
}

Matrix3f MsbObject::readMatrix3f(char* cValue){
Matrix3f mat3f;
if( strncmp("mat3f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f%f%f%f%f%f%f", &mat3f.data[0],&mat3f.data[1],&mat3f.data[2],
                                            &mat3f.data[3],&mat3f.data[4],&mat3f.data[5],
                                            &mat3f.data[6],&mat3f.data[7],&mat3f.data[8]);
return mat3f;
}

bool MsbObject::readBool(char* cValue){
bool b=false;
int convBuffer;
if( strncmp("bool ",cValue,5) == 0 )
    sscanf((cValue+5),"%i",&convBuffer);
if (convBuffer>0)
 b=true;
return (bool)b;
}

GLuint MsbObject::readGLuint(char* cValue){
GLuint glui;
if( strncmp("GLuint ",cValue,7) == 0 )
    sscanf((cValue+7),"%u",&glui);
return glui;
}

int MsbObject::readInt(char* cValue){
int i;
if( strncmp("int ",cValue,4) == 0 )
    sscanf((cValue+4),"%i",&i);
return i;
}

Actor* MsbObject::readActor(char* cValue){

    int i=-65536;
    if( strncmp("actor* ",cValue,7) == 0 )
        sscanf((cValue+7),"%i",&i);

    //no actorOffset, as we won't be doing prefab stuff here!
    if (i >= 0 && i < (int)sceneData->actorList.size()){
        return sceneData->actorList[i];
        }

    if (i<0 && -i<(int)sceneData->helperList.size()+1){
        return sceneData->helperList[(-i)-1];
    }
    return NULL;
}

Node* MsbObject::readNode(char* cValue){

    int i=-1;
    if( strncmp("node* ",cValue,6) == 0 )
        sscanf((cValue+6),"%i",&i);

    //no nodeOffset, as we won't be doing prefeb stuff here!
    if (i >= 0 && i < (int)sceneData->nodeList.size()){
        cout << "found a node list reference!" << endl;
        return sceneData->nodeList[i];
        }

    return NULL;
}

float MsbObject::readFloat(char* cValue){
float f;
if( strncmp("float ",cValue,6) == 0 )
    sscanf((cValue+6),"%f",&f);
return f;
}

//sscanf only reads float!
double MsbObject::readDouble(char* cValue){
double d;
if( strncmp("double ",cValue,7) == 0 )
    sscanf((cValue+7),"%lf",&d);
return d;
}

string MsbObject::readString(char* cValue){
string s="NULL";
if( strncmp("string ",cValue,7) == 0 )
    {
    char * myString = new char[64];
    sscanf((cValue+7),"%s",myString);
    s= myString;
    }
return s;
}


vector<string> MsbObject::readVecString(char* cValue){

    vector<string> myVec;

    if( strncmp("vector",cValue,6) == 0 ){

        //split vector part away...
        char* vectorPart = strtok(cValue, "|");

        //have to do one more time - first is a blank...
        vectorPart = strtok(NULL, "|"); // Splits spaces between words in st


        //push back all
        while(vectorPart!=NULL){
            myVec.push_back(readString(vectorPart));
            vectorPart = strtok(NULL, "|"); // Splits spaces between words in st
        }
    }
    return myVec;
}

vector<Actor*> MsbObject::readVecActor(char* cValue){

    vector<Actor*> myVec;

    if( strncmp("vector",cValue,6) == 0 ){

        //split vector part away...
        char* vectorPart = strtok(cValue, "|");

        //have to do one more time - first is a blank...
        vectorPart = strtok(NULL, "|"); // Splits spaces between words in st
        //push back all
        while(vectorPart!=NULL){
            myVec.push_back(readActor(vectorPart));
            vectorPart = strtok(NULL, "|"); // Splits spaces between words in st
        }
    }
    return myVec;
}

vector<Node*> MsbObject::readVecNode(char* cValue){

    vector<Node*> myVec;

    if( strncmp("vector",cValue,6) == 0 ){

        //split vector part away...
        char* vectorPart = strtok(cValue, "|");

        //have to do one more time - first is a blank...
        vectorPart = strtok(NULL, "|"); // Splits spaces between words in st

        //push back all
        while(vectorPart!=NULL){
            myVec.push_back(readNode(vectorPart));
            vectorPart = strtok(NULL, "|"); // Splits spaces between words in st
        }
    }
    return myVec;
}

vector<Vector4f> MsbObject::readVecVector4f(char* cValue){

    vector<Vector4f> myVec;

    if( strncmp("vector",cValue,6) == 0 ){

        //split vector part away...
        char* vectorPart = strtok(cValue, "|");

        //have to do one more time - first is a blank...
        vectorPart = strtok(NULL, "|"); // Splits spaces between words in st

        //push back all
        while(vectorPart!=NULL){
            myVec.push_back(readVector4f(vectorPart));
            vectorPart = strtok(NULL, "|"); // Splits spaces between words in st
        }
    }
    return myVec;
}

vector<Vector3f> MsbObject::readVecVector3f(char* cValue){

    vector<Vector3f> myVec;

    if( strncmp("vector",cValue,6) == 0 ){

        //split vector part away...
        char* vectorPart = strtok(cValue, "|");

        //have to do one more time - first is a blank...
        vectorPart = strtok(NULL, "|"); // Splits spaces between words in st

        //push back all
        while(vectorPart!=NULL){
            myVec.push_back(readVector3f(vectorPart));
            vectorPart = strtok(NULL, "|"); // Splits spaces between words in st
        }
    }
    return myVec;
}



/*****************************************
    Life or Death Functions
****************************************/

void MsbObject::remove(){

    delete(this);
}

bool MsbObject::isA(string className){

//create internal class name
int nameCount=className.length();
std::string s;
std::stringstream out;
out << nameCount;
s = out.str();
className=s.append(className);

//transform to upper case
std::transform(className.begin(), className.end(),className.begin(), ::toupper);

std::string myName=typeid(*this).name();

//TODO: isA is compiler specific!
//TODO: isA is not taking into account if we're a child of anything!

//transform to upper case
std::transform(myName.begin(), myName.end(),myName.begin(), ::toupper);

if (myName==className)
  return true;
else
  return false;
}



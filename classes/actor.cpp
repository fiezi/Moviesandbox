#include <sstream>

#include "actor.h"
#include "control.h"
#include "input.h"
#include "interpolationHelper.h"
#include "renderer.h"
#include "node.h"
#include "action.h"
#include "brush.h"
#include "meshData.h"

Actor::Actor(){


renderer=Renderer::getInstance();
input=Input::getInstance();

renderLayer=0;

location=Vector3f(0,0,0);
rotation=Vector3f(0,0,0);
scale=Vector3f(1,1,1);
orientation=Vector3f(0,0,1);

xAxis=Vector3f(0,0,0);
yAxis=Vector3f(0,0,0);
zAxis=Vector3f(0,0,0);


pivotLocation=Vector3f(0,0,0);

transformMatrix.identity();
originalMatrix.identity();
bUseTransformMatrix=true;

collisionCubeSize=1.0f;

particleScale=2.0f;
particleAngleScale = 0.0f;

transitionTime=0.5f;

base=NULL;
baseMatrix.identity();
baseNode=NULL;

parent=NULL;
groupID="NULL";

initialLocation=Vector3f(0,0,0);
initialRotation=Vector3f(0,0,0);

color=Vector4f(1,1,1,1);

sceneShaderID="color";

//soundFileName="NULL";

elapsedTime=0.0;
birth=0.0;

name="Actor";

bInit=false;

bPickable=true;
bHidden=false;
bRemoveable=true;
bSelected=false;

drawType=DRAW_TEA;                     //teapot

blendModeOne=GL_SRC_ALPHA;
blendModeTwo=GL_ONE_MINUS_SRC_ALPHA;

vboMeshID="NULL";
textureID="NULL";

controller=NULL;

registerProperties();

//Actor Menu
menuType.push_back("10MoveButton");
menuType.push_back("12RotateButton");
menuType.push_back("13SetBaseButton");

menuProp.push_back("NULL");
menuProp.push_back("NULL");
menuProp.push_back("NULL");

menuIcon.push_back("icon_move");
menuIcon.push_back("icon_rotate");
menuIcon.push_back("icon_base");

menuName.push_back("LOCATION");
menuName.push_back("ROTATION");
menuName.push_back("Set Base");

//Render properties
bTextured=false;
bShadowTextured=false;
bZTest=true;
bZWrite=true;
bUseShader=true;
bComputeLight=true;
bDrawOrientation=true;

bLight=false;

textTimer=renderer->currentTime;

texScale        = Vector3f(1,1,1);
texRotation     = Vector3f(0,0,0);
texTranslation  = Vector3f(0,0,0);

//for actor loading
actorOffset=0;
nodeOffset=0;

//for actor picking
objectID=-1.0;
}

Actor::~Actor(){
}

void Actor::registerProperties(){
//register all properties for saving
createMemberID("RENDERLAYER",&renderLayer,this);

createMemberID("NAME",&name,this);

createMemberID("LOCATION", &location, this);
createMemberID("ROTATION", &rotation, this);
createMemberID("SCALE", &scale, this);
createMemberID("ORIENTATION", &orientation, this);

createMemberID("TRANSFORMMATRIX",&transformMatrix,this);
createMemberID("SCALEMATRIX",&scaleMatrix,this);

createMemberID("BASE",&base,this);
createMemberID("BASEMATRIX",&baseMatrix,this);
createMemberID("PIVOTLOCATION", &pivotLocation, this);
createMemberID("BASENODE",&baseNode, this);

createMemberID("GROUPID",&groupID, this);

createMemberID("CUBESIZE",&collisionCubeSize, this);
createMemberID("PARTICLESCALE",&particleScale, this);
createMemberID("PARTICLEANGLESCALE",&particleAngleScale, this);

createMemberID("COLOR", &color, this);

createMemberID("VBOMESHID", &vboMeshID, this);

createMemberID("TEXTUREID", &textureID, this);
createMemberID("TEXTRANSLATION",&texTranslation,this);
createMemberID("TEXROTATION",&texRotation,this);
createMemberID("TEXSCALE",&texScale,this);

createMemberID("DRAWTYPE",&drawType,this);
createMemberID("BLENDMODEONE",&blendModeOne,this);
createMemberID("BLENDMODETWO",&blendModeTwo,this);

createMemberID("SCENESHADERID",&sceneShaderID,this);

createMemberID("BHIDDEN",&bHidden,this);
createMemberID("BUSETRANSFORMMATRIX",&bUseTransformMatrix,this);

//createMemberID("SOUNDFILENAME",&soundFileName,this);

createMemberID("BSHADOWTEXTURED",&bShadowTextured,this);
createMemberID("BCOMPUTELIGHT",&bComputeLight,this);

createMemberID("BTEXTURED",&bTextured,this);
createMemberID("BZTEST",&bZTest,this);
createMemberID("BZWRITE",&bZWrite,this);
createMemberID("BUSESHADER",&bUseShader,this);
createMemberID("BPICKABLE",&bPickable,this);

createMemberID("BLIGHT",&bLight,this);


createMemberID("TEXTIMER",&textTimer,this);



createMemberID("MENUTYPE", &menuType, this);

}


void Actor::postLoad(){

    baseMatrix=calcMatrix(this);
    matrixToVectors();
    bInit=true;
}

void Actor::setup(){

 //   baseMatrix=calcMatrix(this);
 //   matrixToVectors();
}

void Actor::trigger(Actor * other){



}

void Actor::setLocation(Vector3f loc){

    transformMatrix.setTranslation(loc);
    baseMatrix=calcMatrix(this);
    matrixToVectors();

}

void Actor::setRotation(Vector3f rot){

    Vector3f relTranslation=transformMatrix.getTranslation();
    transformMatrix=Matrix4f::createRotationAroundAxis(rot.x*M_PI/180,rot.y*M_PI/180,rot.z*M_PI/180);
    transformMatrix.setTranslation(relTranslation);

    baseMatrix=calcMatrix(this);
    matrixToVectors();

}

void Actor::setRotation(Vector3f xA, Vector3f yA, Vector3f zA){

    Matrix3f rotationMatrix;

    rotationMatrix[0]=xA.x;
    rotationMatrix[1]=xA.y;
    rotationMatrix[2]=xA.z;

    rotationMatrix[3]=yA.x;
    rotationMatrix[4]=yA.y;
    rotationMatrix[5]=yA.z;

    rotationMatrix[6]=zA.x;
    rotationMatrix[7]=zA.y;
    rotationMatrix[8]=zA.z;

    transformMatrix.setRotation(rotationMatrix);

    baseMatrix=calcMatrix(this);
    matrixToVectors();

}

void Actor::addRotation(float amount, Vector3f Axis){

    //using OpenGL to generate rotationMatrix
    Matrix4f fullMat;

    //push Matrix in before old transform.
            Matrix4f oldRotation=transformMatrix;
            Matrix4f thisMat;

            //construct new full rotation Matrix
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            //glMultMatrixf(baseMatrix);
            glRotatef(amount,Axis.x,Axis.y,Axis.z);
            glGetFloatv(GL_MODELVIEW_MATRIX,thisMat.data);
            glPopMatrix();

            thisMat.setTranslation(oldRotation.getTranslation());
            oldRotation.setTranslation(Vector3f(0,0,0));

            transformMatrix= thisMat * oldRotation;

    baseMatrix=calcMatrix(this);
    matrixToVectors();

}

void Actor::setScale(Vector3f newScale){

    scale=newScale;

    scaleMatrix.identity();
    scaleMatrix[0]=scale.x;
    scaleMatrix[5]=scale.y;
    scaleMatrix[10]=scale.z;

    setRotationFromAxis(xAxis,yAxis,zAxis);
}

void Actor::setAbsoluteLocation(Vector3f loc){

    if (base){
        //find relative location
        Vector3f locDiff = loc - base->baseMatrix.getTranslation();
        //project on base's axis
        Vector3f xA,yA,zA;
        getAxis(&xA,&yA,&zA, base->baseMatrix);

        Vector3f relLoc;
        relLoc.x= locDiff.dotProduct(xA);
        relLoc.y= locDiff.dotProduct(yA);
        relLoc.z= locDiff.dotProduct(zA);
        setLocation(relLoc);
    }
    else
        setLocation(loc);
}

void Actor::getAxis(Vector3f* xA, Vector3f* yA, Vector3f* zA, Matrix4f myMatrix){

    xA->x=myMatrix[0];
    xA->y=myMatrix[1];
    xA->z=myMatrix[2];

    yA->x=myMatrix[4];
    yA->y=myMatrix[5];
    yA->z=myMatrix[6];

    zA->x=myMatrix[8];
    zA->y=myMatrix[9];
    zA->z=myMatrix[10];

}
void Actor::setRotationFromAxis(Vector3f xA, Vector3f yA, Vector3f zA){

    xA.normalize();
    yA.normalize();
    zA.normalize();

/*
    xA=xA* scale.x;
    yA=yA* scale.y;
    zA=zA* scale.z;
*/
    transformMatrix[0]=xA.x;
    transformMatrix[1]=xA.y;
    transformMatrix[2]=xA.z;

    transformMatrix[4]=yA.x;
    transformMatrix[5]=yA.y;
    transformMatrix[6]=yA.z;

    transformMatrix[8]=zA.x;
    transformMatrix[9]=zA.y;
    transformMatrix[10]=zA.z;



   // finalMatrix.setTranslation(translation);

    baseMatrix=calcMatrix(this);
    matrixToVectors();


}

Matrix4f Actor::calcMatrix(Actor* myActor){

    Matrix4f myMatrix;
    if (myActor->base){
        myMatrix=  calcMatrix(myActor->base) * myActor->originalMatrix * myActor->transformMatrix * myActor->scaleMatrix;
        return myMatrix;
    }
    else
        return myMatrix * myActor->originalMatrix * myActor->transformMatrix * myActor->scaleMatrix;
}

Vector3f Actor::calcEuler(Matrix4f myMatrix){

//calculate rotation in euler angles - maybe put this into a separate function?
// from MATRIX and QUATERNION FAQ - http://www.flipcode.com/documents/matrfaq.html#Q37

    float trx, trY, angle_y, angle_x, angle_z, C;

    #define RADIANS 180/M_PI

    angle_y =  -asin( myMatrix[2]);        /* Calculate Y-axis angle */
    C           =  cos( angle_y );
    angle_y    *= RADIANS;



    if ( fabs( C ) > 0.005 )             /* Gimball lock? */
      {
      trx      =  myMatrix[10] / C;           /* No, so get X-axis angle */
      trY      = -myMatrix[6]  / C;

      angle_x  = atan2( trY, trx ) * RADIANS;

      trx      =  myMatrix[0] / C;            /* Get Z-axis angle */
      trY      = -myMatrix[1] / C;

      angle_z  = atan2( trY, trx ) * RADIANS;
      }
    else                                 /* Gimball lock has occurred */
      {
      angle_x  = 0;                      /* Set X-axis angle to zero */

      trx      = myMatrix[5];                 /* And calculate Z-axis angle */
      trY      = myMatrix[4];

      angle_z  = atan2( trY, trx ) * RADIANS;
      }

    return(Vector3f(angle_x,-angle_y,-angle_z));
}

void Actor::matrixToVectors(){

    //updating axis
    xAxis=Vector3f(baseMatrix[0],baseMatrix[1],baseMatrix[2]);
    yAxis=Vector3f(baseMatrix[4],baseMatrix[5],baseMatrix[6]);
    zAxis=Vector3f(baseMatrix[8],baseMatrix[9],baseMatrix[10]);

    //update location
    location=transformMatrix.getTranslation();

    //update rotation
    rotation=calcEuler(transformMatrix);

}

/* update and draw */


void Actor::update(double deltaTime){

elapsedTime+=deltaTime;

    for (int i=0;i<(int)movers.size();i++){
        if (movers[i]!=NULL && !movers[i]->bFinished){  //if we have a mover and it hasn't finished moving yet...
            //update the mover
            movers[i]->interpolate();
            //update our mover buffers
        }
    }

    for (int i=0;i<(int)movers.size();i++){
    if (movers[i]->bFinished){
            delete(movers[i]);
            movers.erase(movers.begin()+i);
        }
    }

   baseMatrix=calcMatrix(this);
   matrixToVectors();
}

// updates shader uniforms
void Actor::updateShaders(){

     if (!bHidden){
		GLuint uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "time");
        glUniform1fARB(uniform_location, renderer->currentTime);

		uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "cameraInverse");
        glUniformMatrix4fvARB(uniform_location, 1,false, (GLfloat*)&renderer->inverseCameraMatrix[0]);

		uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "bComputeLight");
        glUniform1i(uniform_location, (GLuint)bComputeLight);

		uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "bSelected");
        glUniform1i(uniform_location, (GLuint)bSelected);

        uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "objectID");
        glUniform1f(uniform_location, objectID);

    }

    //THESE bind to texture units it seems?
/*
    if (renderer->shaderList[sceneShaderID]->shader){
        glUniform1iARB(glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "tex"), 0);
    }
*/

    if ( ( (drawType==DRAW_VBOMESH) && renderer->vboList[vboMeshID] && renderer->vboList[vboMeshID]->vertexInterpretation==GL_POINTS )
            || drawType==DRAW_PARTICLES){

        GLint uniform_location=0;
        uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "particleMultiplier");
        glUniform1fARB(uniform_location, particleScale);

        uniform_location=0;
        uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "particleAngleScale");
        glUniform1fARB(uniform_location, particleAngleScale);

        uniform_location=0;
        uniform_location = glGetUniformLocation(renderer->shaderList[sceneShaderID]->shader, "postColor");
        glUniform4fARB(uniform_location, color.r, color.g, color.b, color.a );
    }

}

//translates, rotates, sets up shaders and textures, then calls a more specifc draw function depending on the drawtype
void Actor::draw(){
}


void Actor::drawMesh(){

/*
glScalef(scale.x,scale.y,scale.z);
if (renderer->meshList[mesh])
    DrawOBJ(renderer->meshList[mesh]);

*/
}

void Actor::drawSprite(){

    if (bTextured){
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_POINT_SPRITE_ARB);
        }
    glBegin(GL_POINTS);
        glVertex4f(0,0,0,scale.x*0.1);
    glEnd();
    if (bTextured){
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_POINT_SPRITE_ARB);
        }

}

//Plane is aligned at Top Left corner
void Actor::drawPlane(){
}

void Actor::drawParticles(){}

void Actor::drawTeapot(){

glColor4f(color.r,color.g,color.b,color.a);
glFrontFace(GL_CW);
glutSolidTeapot(scale.x/2);
glFrontFace(GL_CCW);
}

void Actor::drawCube(){

glColor4f(color.r,color.g,color.b,color.a);
glutSolidCube(1/scale.x);
}

void Actor::drawVBOMesh(){

if (renderer->vboList[vboMeshID]){
    renderer->drawColladaMesh(renderer->vboList[vboMeshID]);

    if (renderer->vboList[vboMeshID]->vertexInterpretation==GL_POINTS && !input->controller->bRunning){
        renderer->setupShading("color");
        glutSolidCube(1/scale.x);
        }
    }
}

void Actor::drawSkeletal(){
}


void Actor::reset(){}

/* control start/stop */

void Actor::start(){}
void Actor::stop(){

    for (int i=0;i<(int)movers.size();i++){
                delete(movers[i]);
                movers.erase(movers.begin()+i);
    }
}


void Actor::setBase(Actor* newBase){

    Vector3f absLoc=baseMatrix.getTranslation();
    Vector3f newBaseAbsLoc=newBase->baseMatrix.getTranslation();
    Vector3f newLocation=absLoc-newBaseAbsLoc;
    base=newBase;
    setLocation(newLocation);
}

/* saving/loading stuff */

//saves all properties as defined in registerProperties
TiXmlElement* Actor::save(TiXmlElement *root){

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

TiXmlElement* Actor::saveAsPrefab(TiXmlElement *root){

    TiXmlElement* regularSave = save(root);

    //find base
    TiXmlElement* baseElement = regularSave->FirstChildElement("BASE");
    baseElement->Clear();
    char newBaseText[50];

    for (int i=0;i<(int)input->selectedActors.size();i++){
        if (base == input->selectedActors[i]){
            sprintf(newBaseText,"actor* %i",i);
            break;
        }else{
            sprintf(newBaseText,"actor* -1");
        }

    }
    //sprintf
    baseElement->Clear();
    baseElement->LinkEndChild ( new TiXmlText( newBaseText));
    //baseElement->LinkEndChild ( new TiXmlText( value));
    return regularSave;
    //make it relative to selectActors and not actorList!
}

//loads all properties as defined in registerProperties
void Actor::load(TiXmlElement *myInfo){

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
string Actor::memberToString(memberID *mID){

    //Vector3f
    if (mID->memberType->name()==typeid(Vector3f).name() )
       return writeVector3f(mID);

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

    //Vector4
    if (mID->memberType->name()==typeid(Vector4f).name())
        return writeVector4f(mID);

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
void Actor::memberFromString(memberID *mID,string value){

    char * cValue;
    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());



    //give special priority to locations and other vectors!
    if (setPropertyTo (mID, readVector3f(cValue)))
        return;
    if (setPropertyTo (mID, readVector4f(cValue)))
        return;
    if (setPropertyTo (mID, readMatrix4f(cValue)))
        return;
    if (setPropertyTo (mID, readBool(cValue)))
        return;
    if (setPropertyTo (mID, readGLuint(cValue)))
        return;
    if (setPropertyTo (mID, readInt(cValue)))
        return;
    if (setPropertyTo (mID, readFloat(cValue)))
        return;
    if (setPropertyTo (mID, readDouble(cValue)))
        return;
    if (setStringPropertyTo(mID, readString(cValue)))
        return;

    if (setNodePropertyTo(mID, readNode(cValue)))
        return;

    if (setActorPropertyTo(mID, readActor(cValue)))
        return;

    ///vectors


    if (setVecPropertyTo (mID,readVecString(cValue)))
        return;

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecActor(cValue)))
        return;

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecNode(cValue)))
        return;

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecVector3f(cValue)))
        return;

    cValue = new char [value.size()+1];
    strcpy (cValue, value.c_str());

    if (setVecPropertyTo (mID,readVecVector4f(cValue)))
        return;

    delete(cValue);
}

//gets called from memberFromString! Don't use as standalone function!
bool Actor::setStringPropertyTo(memberID * mID,string s){

    if (mID->memberType->name()==typeid(s).name())
      {
      string * theString=(string*)mID->memberReference;
      *theString=s;
      return true;
      }
    return false;
}


//gets called from memberFromString! Don't use as standalone function!
bool Actor::setActorPropertyTo(memberID * mID,Actor* a){


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

bool Actor::setNodePropertyTo(memberID * mID,Node* n){


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

string Actor::writeVector3f(memberID* mID){
    char value[512];
    Vector3f * p;
    p=(Vector3f*)mID->memberReference;
    sprintf(value,"vec3f %f %f %f",p->x, p->y, p->z);
    return value;
}

string Actor::writeFloat(memberID* mID){
    char value[512];
    float * p;
    p=(float*)mID->memberReference;
    sprintf(value,"float %f",*p);
    return value;
}

string Actor::writeDouble(memberID* mID){
    char value[512];
    double * p;
    p=(double*)mID->memberReference;
    sprintf(value,"double %f",*p);
    return value;
}

string Actor::writeVector4f(memberID* mID){
    char value[512];
    Vector4f * p;
    p=(Vector4f*)mID->memberReference;
    sprintf(value,"vec4f %f %f %f %f",p->r, p->g, p->b, p->a);
    return value;
}

string Actor::writeBool(memberID* mID){
    char value[512];
    bool * p;
    p=(bool*)mID->memberReference;
    sprintf(value,"bool %i",*p);
    return value;
}

string Actor::writeGLuint(memberID* mID){
    char value[512];
    GLuint * p;
    p=(GLuint*)mID->memberReference;
    sprintf(value,"GLuint %i",*p);
    return value;
}

string Actor::writeMatrix4f(memberID* mID){
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

string Actor::writeInt(memberID* mID){
    char value[512];
    int * p;
    p=(int*)mID->memberReference;
    sprintf(value,"int %i",*p);
    return value;
}

string Actor::writeMatrix3f(memberID* mID){
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

string Actor::writeString(memberID* mID){
    string* me=(string*) mID->memberReference;
    string s="string ";
    if (me)
        s+= (*me);
    return s;
}

string Actor::writeNode(memberID* mID){

    char value[512];
    Node* n;
    Node** p;                           //the ugliness that is **!
    p=(Node**)(mID->memberReference);   //We're actually holding the ADRESS of a NODE* in the memberReference
    n=*p;                                //so we need to dereference twice!

    if (n){                                   //save actor position in actorList!
        for (uint i=0; i<renderer->nodeList.size();i++){
            if (renderer->nodeList[i]==n) sprintf(value,"node* %i",i);
        }
    } else sprintf(value,"node* -1");

    return value;
}

string Actor::writeActor(memberID* mID){

    char value[512];
    Actor* a=NULL;
    Actor** p=NULL;                           //the ugliness that is **!
    p=(Actor**)(mID->memberReference);   //We're actually holding the ADRESS of an ACTOR* in the memberReference
    if (p)
        a=*p;                                //so we need to dereference twice!

    if (a){                                   //save actor position in actorList!
        for (uint i=0; i<renderer->actorList.size();i++){
            if (renderer->actorList[i]==a) sprintf(value,"actor* %i",i);
        }
    } else sprintf(value,"actor* -1");

    return value;
}


string Actor::writeVecActor(memberID* mID){

        string fullString="vector";
        vector<Actor*> * myVec = (vector<Actor*> *)mID->memberReference;


        cout << "size of saveable Vector: "<< myVec->size() << endl;
        for (int i=0; i< (int)myVec->size(); i++){
            //create temporary mID
            createMemberID("temp",&((*myVec)[i]), this);
            //add to our string
            fullString+="|" + writeActor(&property["temp"]);
            property.erase("temp");
            }
        return fullString;
}

string Actor::writeVecNode(memberID* mID){

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

string Actor::writeVecString(memberID* mID){
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

string Actor::writeVecVector3f(memberID* mID){
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

string Actor::writeVecVector4f(memberID* mID){
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



Vector3f Actor::readVector3f(char* cValue){
Vector3f vec3f;
if( strncmp("vec3f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f",&vec3f.x,&vec3f.y,&vec3f.z);
return vec3f;
}

Vector4f Actor::readVector4f(char* cValue){
Vector4f vec4f;
if( strncmp("vec4f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f%f",&vec4f.r,&vec4f.g,&vec4f.b,&vec4f.a);
return vec4f;
}

Matrix4f Actor::readMatrix4f(char* cValue){
Matrix4f mat4f;
if( strncmp("mat4f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",&mat4f.data[0],&mat4f.data[1],&mat4f.data[2],&mat4f.data[3]
                                                        ,&mat4f.data[4],&mat4f.data[5],&mat4f.data[6],&mat4f.data[7]
                                                        ,&mat4f.data[8],&mat4f.data[9],&mat4f.data[10],&mat4f.data[11]
                                                        ,&mat4f.data[12],&mat4f.data[13],&mat4f.data[14],&mat4f.data[15]);
return mat4f;
}

Matrix3f Actor::readMatrix3f(char* cValue){
Matrix3f mat3f;
if( strncmp("mat3f ",cValue,6) == 0 )
    sscanf((cValue+6),"%f%f%f%f%f%f%f%f%f", &mat3f.data[0],&mat3f.data[1],&mat3f.data[2],
                                            &mat3f.data[3],&mat3f.data[4],&mat3f.data[5],
                                            &mat3f.data[6],&mat3f.data[7],&mat3f.data[8]);
return mat3f;
}

bool Actor::readBool(char* cValue){
bool b=false;
int convBuffer;
if( strncmp("bool ",cValue,5) == 0 )
    sscanf((cValue+5),"%i",&convBuffer);
if (convBuffer>0)
 b=true;
return (bool)b;
}

GLuint Actor::readGLuint(char* cValue){
GLuint glui;
if( strncmp("GLuint ",cValue,7) == 0 )
    sscanf((cValue+7),"%u",&glui);
return glui;
}

int Actor::readInt(char* cValue){
int i;
if( strncmp("int ",cValue,4) == 0 )
    sscanf((cValue+4),"%i",&i);
return i;
}

Actor* Actor::readActor(char* cValue){

    int i=-1;
    if( strncmp("actor* ",cValue,7) == 0 )
        sscanf((cValue+7),"%i",&i);

    if (i >= 0 && i < (int)renderer->actorList.size()){
        return renderer->actorList[i+actorOffset];
        }

    return NULL;
}

Node* Actor::readNode(char* cValue){

    int i=-1;
    if( strncmp("node* ",cValue,6) == 0 )
        sscanf((cValue+6),"%i",&i);

    if (i >= 0 && i < (int)renderer->nodeList.size()){
        cout << "found a node list reference!" << endl;
        return renderer->nodeList[i+nodeOffset];
        }

    return NULL;
}

float Actor::readFloat(char* cValue){
float f;
if( strncmp("float ",cValue,6) == 0 )
    sscanf((cValue+6),"%f",&f);
return f;
}

//sscanf only reads float!
double Actor::readDouble(char* cValue){
double d;
if( strncmp("double ",cValue,7) == 0 )
    sscanf((cValue+7),"%lf",&d);
return d;
}

string Actor::readString(char* cValue){
string s="NULL";
if( strncmp("string ",cValue,7) == 0 )
    {
    char * myString = new char[64];
    sscanf((cValue+7),"%s",myString);
    s= myString;
    }
return s;
}


vector<string> Actor::readVecString(char* cValue){

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

vector<Actor*> Actor::readVecActor(char* cValue){

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

vector<Node*> Actor::readVecNode(char* cValue){

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

vector<Vector4f> Actor::readVecVector4f(char* cValue){

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

vector<Vector3f> Actor::readVecVector3f(char* cValue){

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

void Actor::remove(){

    //remove references in other actors
    //go through all actors
    for (int i=0;i<(int)renderer->actorList.size();i++){
        Actor* myActor=renderer->actorList[i];
        //go through all properties
        std::map <std::string, memberID>::iterator it;
        for ( it=myActor->property.begin() ; it != myActor->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(Actor*));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                Actor** pa=(Actor**)mID.memberReference;
                //see if it references the Actor we want to delete!
                if (*pa==this){
                    *pa=NULL;   //and set the reference to NULL!
                    }
                }
            }
        }

    //do the same for actor vectors!
    for (int i=0;i<(int)renderer->actorList.size();i++){
        Actor* myActor=renderer->actorList[i];
        //go through all properties
        std::map <std::string, memberID>::iterator it;
        for ( it=myActor->property.begin() ; it != myActor->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(vector<Actor*>));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                vector<Actor*>* pa=(vector<Actor*>*)mID.memberReference;
                //see if it references the Actor we want to delete!
                for (int j=0;j<(int)(*pa).size(); j++){
                    if ((*pa)[j]==this)
                        (*pa)[j]=NULL;   //and set the reference to NULL!
                }
            }
        }
    }
    //remove references in buttons
    //go through all buttons
    for (unsigned int i=0;i<renderer->buttonList.size();i++){
        BasicButton* myButton=renderer->buttonList[i];

        //go through all properties
        std::map <std::string, memberID>::iterator it;

        for ( it=myButton->property.begin() ; it != myButton->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(Actor*));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                Actor** pa=(Actor**)mID.memberReference;
                //see if it references the Actor we want to delete!
                if (*pa==this){
                    *pa=NULL;   //and set the reference to NULL!
                    }
                }
            }
        }
    //and the same in all buttons with Actor Vectors
    for (int i=0;i<(int)renderer->buttonList.size();i++){
        Actor* myActor=renderer->buttonList[i];
        //go through all properties
        std::map <std::string, memberID>::iterator it;
        for ( it=myActor->property.begin() ; it != myActor->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(vector<Actor*>));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                vector<Actor*>* pa=(vector<Actor*>*)mID.memberReference;
                //see if it references the Actor we want to delete!
                for (int j=0;j<(int)(*pa).size(); j++){
                    if ((*pa)[j]==this)
                        (*pa)[j]=NULL;   //and set the reference to NULL!
                }
            }
        }
    }

    //remove references in nodes
    //go through all nodes
    for (unsigned int i=0;i<renderer->nodeList.size();i++){
        Node* myNode=renderer->nodeList[i];
        //go through all properties
        std::map <std::string, memberID>::iterator it;
        for ( it=myNode->property.begin() ; it != myNode->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(Actor*));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                Actor** pa=(Actor**)mID.memberReference;
                //see if it references the Actor we want to delete!
                if (*pa==this){
                    *pa=NULL;   //and set the reference to NULL!
                    }
                }
            }
        }

    //remove us from selectedActors
    for (int i=0;i<(int)input->selectedActors.size();i++){
        if(input->selectedActors[i]==this){
            //input->selectedActors[i]->remove();
            input->selectedActors.erase(input->selectedActors.begin()+i);
            }
    }

    //check if we're the current drawing!
    if (renderer->brush->drawing && (Actor*)renderer->brush->drawing == this)
        renderer->brush->drawing=NULL;

    //remove from renderLayer list
    for (int i=0;i<(int)renderer->layerList[renderLayer]->actorList.size();i++){
        if (renderer->layerList[renderLayer]->actorList[i]==this)
          renderer->layerList[renderLayer]->actorList.erase(renderer->layerList[renderLayer]->actorList.begin()+i);
    }
    //remove from actorList
    for (int i=0;i<(int)renderer->actorList.size();i++){
        if (renderer->actorList[i]==this)
          renderer->actorList.erase(renderer->actorList.begin()+i);
    }

    delete(this);
}

//for runtime Actor creation
void Actor::create(){ renderer->addActor(this); }

bool Actor::createNewActor(string actorIDName){

if (renderer->actorInfo[actorIDName].actorReference)
  {
  renderer->actorInfo[actorIDName].actorReference->create();
  return true;
  }
else
  return false;
}

Actor* Actor::spawn(string actorIDName){

if (renderer->actorInfo[actorIDName].actorReference)
  {
  renderer->actorInfo[actorIDName].actorReference->create();
  return renderer->actorList.back();
  }
else
  return NULL;

}

bool Actor::isA(string className){

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



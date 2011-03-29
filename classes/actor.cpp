#include <sstream>

#include "actor.h"
#include "input.h"
#include "renderer.h"
#include "sceneData.h"
#include "control.h"
#include "interpolationHelper.h"
#include "node.h"
#include "action.h"
#include "brush.h"
#include "meshData.h"

Actor::Actor(){


renderer=Renderer::getInstance();
input=Input::getInstance();
sceneData=SceneData::getInstance();

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
bDebug=false;

drawType=DRAW_TEA;                     //teapot

blendModeOne=GL_SRC_ALPHA;
blendModeTwo=GL_ONE_MINUS_SRC_ALPHA;

vboMeshID="NULL";
textureID="NULL";

controller=NULL;

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

registerProperties();
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


createMemberID("BCOMPUTELIGHT",&bComputeLight,this);

createMemberID("BTEXTURED",&bTextured,this);
createMemberID("BZTEST",&bZTest,this);
createMemberID("BZWRITE",&bZWrite,this);
createMemberID("BUSESHADER",&bUseShader,this);
createMemberID("BPICKABLE",&bPickable,this);
createMemberID("BDEBUG",&bDebug,this);

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

void Actor::trigger(MsbObject* other){



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

    if (bDebug)
        cout << "setting Mover stuff now..." << renderer->frames <<endl;

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


    if (bDebug)
        cout << "after Mover stuff..." << renderer->frames <<endl;

    baseMatrix=calcMatrix(this);
    matrixToVectors();

    if (bDebug)
        cout << "after updating bases..." << renderer->frames <<endl;

}

// updates shader uniforms
void Actor::updateShaders(){

    shaderObject* myShader= sceneData->shaderList[renderer->currentShader];

    //these uniforms should always exist, but let's check maybe?

    if (myShader->uniforms.find("time") != myShader->uniforms.end())
        glUniform1fARB(myShader->uniforms["time"], renderer->currentTime);

    if (myShader->uniforms.find("cameraInverse") != myShader->uniforms.end())
        glUniformMatrix4fvARB(myShader->uniforms["cameraInverse"], 1,false, (GLfloat*)&renderer->inverseCameraMatrix);

    if (myShader->uniforms.find("bComputeLight") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["bComputeLight"], (GLuint)bComputeLight);

    if (myShader->uniforms.find("bSelected") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["bSelected"], (GLuint)bSelected);

    if (myShader->uniforms.find("objectID") != myShader->uniforms.end())
        glUniform1fARB(myShader->uniforms["objectID"], objectID);

    #ifdef BDEBUGRENDERER
    renderer->checkOpenGLError("half-updateShaders...");
    #endif


    if (myShader->uniforms.find("tex") != myShader->uniforms.end())
        glUniform1iARB(myShader->uniforms["tex"], 0);

    if (myShader->uniforms.find("particleMultiplier") != myShader->uniforms.end())
        glUniform1fARB(myShader->uniforms["particleMultiplier"], particleScale);

    if (myShader->uniforms.find("particleAngleScale") != myShader->uniforms.end())
        glUniform1fARB(myShader->uniforms["particleAngleScale"], particleAngleScale);

    if (myShader->uniforms.find("postColor") != myShader->uniforms.end())
        glUniform4fARB(myShader->uniforms["postColor"], color.r, color.g, color.b, color.a );

    //transmit scene width/height
    if (myShader->uniforms.find("screensize") != myShader->uniforms.end()){
        GLint screenSize[4];
        glGetIntegerv(GL_VIEWPORT,(GLint*)&screenSize);
        glUniform1fARB(myShader->uniforms["screensize"], (float)screenSize[2]);
    }

    if (myShader->uniforms.find("scene_size") != myShader->uniforms.end()){
        glUniform1fARB(myShader->uniforms["scene_size"], (float)renderer->scene_size);
    }
	
    //for shadow pass
    if (renderer->bShadowPass){
        if (myShader->uniforms.find("lighting_size") != myShader->uniforms.end()){
            glUniform1fARB(myShader->uniforms["lighting_size"], (float)renderer->lighting_size);
        }
        if (myShader->uniforms.find("shadow_size") != myShader->uniforms.end()){
            glUniform1fARB(myShader->uniforms["shadow_size"], renderer->shadow_size);
        }
    }

    #ifdef BDEBUGRENDERER
    renderer->checkOpenGLError("post-updateShaders...");
    #endif

}

//translates, rotates, sets up shaders and textures, then calls a more specifc draw function depending on the drawtype
void Actor::draw(){
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


    if (!newBase || newBase->name=="ground" || newBase->name=="grid"){
        Vector3f absLoc=baseMatrix.getTranslation();
        base=NULL;
        setLocation(absLoc);
    }else{  //removing base connection
        Vector3f absLoc=baseMatrix.getTranslation();
        Vector3f newBaseAbsLoc=newBase->baseMatrix.getTranslation();
        Vector3f newLocation=absLoc-newBaseAbsLoc;
        base=newBase;
        setLocation(newLocation);
    }
}

/* saving/loading stuff */
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

    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
        if (base == sceneData->selectedActors[i]){
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


Actor* Actor::readActor(char* cValue){

    int i=-1;
    if( strncmp("actor* ",cValue,7) == 0 )
        sscanf((cValue+7),"%i",&i);

    if (i >= 0 && i < (int)sceneData->actorList.size()){
        return sceneData->actorList[i+actorOffset];
        }

    return NULL;
}

Node* Actor::readNode(char* cValue){

    int i=-1;
    if( strncmp("node* ",cValue,6) == 0 )
        sscanf((cValue+6),"%i",&i);

    if (i >= 0 && i < (int)sceneData->nodeList.size()){
        cout << "found a node list reference!" << endl;
        return sceneData->nodeList[i+nodeOffset];
        }

    return NULL;
}


/*****************************************
    Life or Death Functions
****************************************/

void Actor::remove(){

    //remove references in other actors
    //go through all actors
    for (int i=0;i<(int)sceneData->actorList.size();i++){
        Actor* myActor=sceneData->actorList[i];

        //remove base without location change
        if (myActor->base==this)
            myActor->setBase(NULL);
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
    for (int i=0;i<(int)sceneData->actorList.size();i++){
        Actor* myActor=sceneData->actorList[i];
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
    for (unsigned int i=0;i<sceneData->buttonList.size();i++){
        BasicButton* myButton=sceneData->buttonList[i];

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
    for (int i=0;i<(int)sceneData->buttonList.size();i++){
        Actor* myActor=sceneData->buttonList[i];
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
        //remove from timeline
        TimelineInspector* tiInsp=dynamic_cast<TimelineInspector*>(sceneData->buttonList[i]);
        if (tiInsp){
            for (int t=0;t<(int)tiInsp->timelineActors.size();t++){
                if (tiInsp->timelineActors[t]==this)
                    tiInsp->removeTimeline(t);
            }
        }
    }

    //remove references in nodes
    //go through all nodes
    for (unsigned int i=0;i<sceneData->nodeList.size();i++){
        Node* myNode=sceneData->nodeList[i];
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
    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
        if(sceneData->selectedActors[i]==this){
            //sceneData->selectedActors[i]->remove();
            sceneData->selectedActors.erase(sceneData->selectedActors.begin()+i);
            }
    }

    //check if we're the current drawing!
    if (sceneData->brush->drawing && (Actor*)sceneData->brush->drawing == this)
        sceneData->brush->drawing=NULL;

    //check if we're the currently specialSelected Actor
    if (sceneData->specialSelected==this)
        sceneData->specialSelected=NULL;


    //remove from renderLayer list
    for (int i=0;i<(int)sceneData->layerList[renderLayer]->actorList.size();i++){
        if (sceneData->layerList[renderLayer]->actorList[i]==this)
          sceneData->layerList[renderLayer]->actorList.erase(sceneData->layerList[renderLayer]->actorList.begin()+i);
    }
    //remove from actorList
    for (int i=0;i<(int)sceneData->actorList.size();i++){
        if (sceneData->actorList[i]==this)
          sceneData->actorList.erase(sceneData->actorList.begin()+i);
    }

    delete(this);
}

//for runtime Actor creation
void Actor::create(){ sceneData->addActor(this); }

bool Actor::createNewActor(string actorIDName){

if (sceneData->actorInfo[actorIDName].actorReference)
  {
  sceneData->actorInfo[actorIDName].actorReference->create();
  return true;
  }
else
  return false;
}

Actor* Actor::spawn(string actorIDName){

if (sceneData->actorInfo[actorIDName].actorReference)
  {
  sceneData->actorInfo[actorIDName].actorReference->create();
  return sceneData->actorList.back();
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



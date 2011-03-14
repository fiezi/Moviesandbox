#include "ragDoll.h"
#include "renderer.h"
#include "input.h"

RagDoll::RagDoll(){

name="ragDoll";
drawType=DRAW_CUBE;

bComputeLight=true;

//standard ragdoll...
UPPER_ARM_LEN = 0.30;
FORE_ARM_LEN = 0.25;
HAND_LEN = 0.13; // wrist to mid-fingers only
FOOT_LEN = 0.18; // ankles to base of ball of foot only
HEEL_LEN = 0.05;

BROW_H = 1.68;
MOUTH_H = 1.53;
NECK_H = 1.50;
SHOULDER_H = 1.37;
CHEST_H = 1.35;
HIP_H = 0.86;
KNEE_H = 0.48;
ANKLE_H = 0.08;

SHOULDER_W = 0.41;
CHEST_W = 0.36; // actually wider, but we want narrower than shoulders (esp. with large radius)
LEG_W = 0.28; // between middles of upper legs
PELVIS_W = 0.25; // actually wider, but we want smaller than hip width

activeFoot="NULL";
lftHandRel=Vector3f(0,0,0);             //relative to the Ragdoll's body - can this be abstracted?
rtHandRel=Vector3f(0,0,0);
lftFootRel=Vector3f(0,0,0);
rtFootRel=Vector3f(0,0,0);

lftFootRelToLast=Vector3f(0,0,0);       //relative to the last step
rtFootRelToLast=Vector3f(0,0,0);
lftFootLastAbsolute=Vector3f(0,0,0);    //last absolute foot position
rtFootLastAbsolute=Vector3f(0,0,0);
loffset=Vector3f(0,0,0);                //current left foot offset from the ragdoll's center in x-Axis
roffset=Vector3f(0,0,0);                //current right foot offset from the ragdoll's center in x-Axis
walkDistance=Vector3f(0,0,0);           //distance from the initial RagDoll Position
spineAbsolute=Vector3f(0,1.55,0);             //the absolute position of the ragdoll's spine


bFlipFeet=true;              //flips control direction of feet
bUDPControlled=true;

baseOffset=0.02f;
//actionSpaceMultiplier=7;
actionSpaceMultiplier=7;

totalMass=0.0f;

registerProperties();
}


//TODO:register properties for all the strings!!!

RagDoll::~RagDoll(){

//clean up all our stuff!
map<string,dJoint*>::iterator it;
map<string, PhysicsActor*>::iterator p;

for ( it=joints.begin() ; it != joints.end(); it++ )
    dJointDestroy((*it).second->id());

for ( p=parts.begin() ; p != parts.end(); p++ )
    dBodyDestroy((*p).second->body);
}

void RagDoll::registerProperties(){

createMemberID("LFTHANDREL", &lftHandRel, this);
createMemberID("LFTFOOTREL", &lftFootRel, this);
createMemberID("RTHANDREL", &rtHandRel, this);
createMemberID("RTFOOTREL", &rtFootRel, this);
}


void RagDoll::setup(){
Actor::setup();

cout << "setting up..." << endl;
createRagDoll();
}

void RagDoll::createRagDoll(){

R_SHOULDER_POS = Vector3f(-SHOULDER_W * 0.5, SHOULDER_H, 0.0);
L_SHOULDER_POS = Vector3f(SHOULDER_W * 0.5, SHOULDER_H, 0.0);
R_ELBOW_POS = R_SHOULDER_POS - Vector3f(UPPER_ARM_LEN, 0.0, 0.0);
L_ELBOW_POS = L_SHOULDER_POS + Vector3f(UPPER_ARM_LEN, 0.0, 0.0);
R_WRIST_POS = R_ELBOW_POS - Vector3f(FORE_ARM_LEN, 0.0, 0.0);
L_WRIST_POS = L_ELBOW_POS + Vector3f(FORE_ARM_LEN, 0.0, 0.0);
R_FINGERS_POS = R_WRIST_POS - Vector3f(HAND_LEN, 0.0, 0.0);
L_FINGERS_POS = L_WRIST_POS + Vector3f(HAND_LEN, 0.0, 0.0);

R_HIP_POS = Vector3f(-LEG_W * 0.5, HIP_H, 0.0);
L_HIP_POS = Vector3f(LEG_W * 0.5, HIP_H, 0.0);
R_KNEE_POS = Vector3f(-LEG_W * 0.5, KNEE_H, 0.0);
L_KNEE_POS = Vector3f(LEG_W * 0.5, KNEE_H, 0.0);
R_ANKLE_POS = Vector3f(-LEG_W * 0.5, ANKLE_H, 0.0);
L_ANKLE_POS = Vector3f(LEG_W * 0.5, ANKLE_H, 0.0);

R_HEEL_POS = R_ANKLE_POS - Vector3f(0.0, 0.0, HEEL_LEN);
L_HEEL_POS = L_ANKLE_POS - Vector3f(0.0, 0.0, HEEL_LEN);
R_TOES_POS = R_ANKLE_POS + Vector3f(0.0, 0.0, FOOT_LEN);
L_TOES_POS = L_ANKLE_POS + Vector3f(0.0, 0.0, FOOT_LEN);


Vector3f rightAxis = Vector3f(1.0, 0.0, 0.0);
Vector3f leftAxis = Vector3f(-1.0, 0.0, 0.0);
Vector3f upAxis = Vector3f(0.0, 1.0, 0.0);
Vector3f downAxis = Vector3f(0.0, -1.0, 0.0);
Vector3f bkwdAxis = Vector3f(0.0, 0.0, 1.0);
Vector3f fwdAxis = Vector3f(0.0, 0.0, -1.0);

//create and setup parts

addBody(Vector3f(-CHEST_W * 0.5, CHEST_H, 0.0),Vector3f(CHEST_W * 0.5, CHEST_H, 0.0), 0.13, "chest");
addBody(Vector3f(0.0, CHEST_H - 0.1, 0.0), Vector3f(0.0, HIP_H + 0.1, 0.0), 0.125, "belly");

addBody(Vector3f(-PELVIS_W * 0.5, HIP_H, 0.0),Vector3f(PELVIS_W * 0.5, HIP_H, 0.0), 0.125,"pelvis");
addBody(Vector3f(0.0, BROW_H, 0.0), Vector3f(0.0, MOUTH_H, 0.0), 0.11,"head");

//addBallJoint(parts["chest"]->body, parts["head"]->body, Vector3f(0.0, NECK_H, 0.0), "neck");
addFixedJoint(parts["chest"]->body, parts["head"]->body,"neck");

//addHingeJoint(parts["belly"]->body, parts["pelvis"]->body, parts["chest"]->location, leftAxis, "lowSpine", 0.0, M_PI * 0.75);
addFixedJoint(parts["belly"]->body, parts["pelvis"]->body,"lowSpine");

//addHingeJoint(parts["rightUpperLeg"]->body, parts["rightLowerLeg"]->body, R_KNEE_POS, leftAxis, "rightKnee", 0.0, M_PI * 0.75);
addFixedJoint(parts["chest"]->body, parts["belly"]->body, "midSpine");



addBody(R_HIP_POS, R_KNEE_POS, 0.11,"rightUpperLeg");
addUniversalJoint(parts["pelvis"]->body, parts["rightUpperLeg"]->body,R_HIP_POS, fwdAxis, rightAxis,"rightHip", -0.1 * M_PI, 0.3 * M_PI, -0.15 * M_PI, 0.75 * M_PI);
//addBallJoint(parts["pelvis"]->body, parts["rightUpperLeg"]->body,R_HIP_POS,"rightHip");


addBody(L_HIP_POS, L_KNEE_POS, 0.11,"leftUpperLeg");
addUniversalJoint(parts["pelvis"]->body, parts["leftUpperLeg"]->body,L_HIP_POS, bkwdAxis, rightAxis,"leftHip", -0.1 * M_PI, 0.3 * M_PI, -0.15 * M_PI,0.75 * M_PI);
//addBallJoint(parts["pelvis"]->body, parts["leftUpperLeg"]->body,L_HIP_POS,"leftHip");

addBody(R_KNEE_POS, R_ANKLE_POS, 0.09, "rightLowerLeg");
addHingeJoint(parts["rightUpperLeg"]->body, parts["rightLowerLeg"]->body, R_KNEE_POS, leftAxis, "rightKnee", 0.0, M_PI * 0.75);
//addBallJoint(parts["rightUpperLeg"]->body, parts["rightLowerLeg"]->body, R_KNEE_POS,"rightKnee");

addBody(L_KNEE_POS, L_ANKLE_POS, 0.09, "leftLowerLeg");
addHingeJoint(parts["leftUpperLeg"]->body, parts["leftLowerLeg"]->body, L_KNEE_POS, leftAxis, "leftKnee", 0.0, M_PI * 0.75);
//addBallJoint(parts["leftUpperLeg"]->body, parts["leftLowerLeg"]->body, L_KNEE_POS,"leftKnee");


addBody(R_HEEL_POS, R_TOES_POS, 0.09,"rightFoot");
addHingeJoint(parts["rightLowerLeg"]->body, parts["rightFoot"]->body, R_ANKLE_POS, rightAxis, "rightAnkle", -0.1 * M_PI, 0.05 * M_PI);
//addBallJoint(parts["rightLowerLeg"]->body, parts["rightFoot"]->body, R_ANKLE_POS,"rightAnkle");

addBody(L_HEEL_POS, L_TOES_POS, 0.09,"leftFoot");
addHingeJoint(parts["leftLowerLeg"]->body, parts["leftFoot"]->body, L_ANKLE_POS, rightAxis, "leftAnkle", -0.1 * M_PI, 0.05 * M_PI);
//addBallJoint(parts["leftLowerLeg"]->body, parts["leftFoot"]->body, L_ANKLE_POS,"leftAnkle");

addBody(R_SHOULDER_POS, R_ELBOW_POS, 0.08,"rightUpperArm");
addBallJoint(parts["chest"]->body, parts["rightUpperArm"]->body, R_SHOULDER_POS,"rightShoulder");

addBody(R_ELBOW_POS, R_WRIST_POS, 0.075,"rightForeArm");
addHingeJoint(parts["rightUpperArm"]->body,	parts["rightForeArm"]->body, R_ELBOW_POS, downAxis,"rightElbow", 0.0, 0.6 * M_PI);
//addBallJoint(parts["rightUpperArm"]->body,	parts["rightForeArm"]->body, R_ELBOW_POS,"rightElbow");

addBody(R_WRIST_POS, R_FINGERS_POS, 0.075,"rightHand");
addHingeJoint(parts["rightForeArm"]->body, parts["rightHand"]->body, R_WRIST_POS,fwdAxis,"rightWrist", -0.1 * M_PI, 0.2 * M_PI);
//addBallJoint(parts["rightForeArm"]->body, parts["rightHand"]->body, R_WRIST_POS,"rightWrist");


addBody(L_SHOULDER_POS, L_ELBOW_POS, 0.08, "leftUpperArm");
addBallJoint(parts["chest"]->body, parts["leftUpperArm"]->body,L_SHOULDER_POS,"leftShoulder");

addBody(L_ELBOW_POS, L_WRIST_POS, 0.075,"leftForeArm");
addHingeJoint(parts["leftUpperArm"]->body, parts["leftForeArm"]->body, L_ELBOW_POS, upAxis, "leftElbow",0.0, 0.6 * M_PI);
//addBallJoint(parts["leftUpperArm"]->body, parts["leftForeArm"]->body, L_ELBOW_POS,"leftElbow");

addBody(L_WRIST_POS, L_FINGERS_POS, 0.075,"leftHand");  //original!
addHingeJoint(parts["leftForeArm"]->body, parts["leftHand"]->body, L_WRIST_POS, fwdAxis, "leftWrist", -0.1 * M_PI, 0.2 * M_PI);
//addBallJoint(parts["leftForeArm"]->body, parts["leftHand"]->body, L_WRIST_POS,"leftWrist");




//*****************************************************************************************************

//     S T R I N G S

//*****************************************************************************************************

// these have to be individual Actors (with a place of their own in sceneData->actorList!!!
//left Hand
addString(L_FINGERS_POS+Vector3f(0,0.15,0), L_FINGERS_POS+Vector3f(0,0.3,0), 0.015,"leftString");  //original!
addBallJoint(parts["leftHand"]->body, myStrings["leftString"]->body,L_FINGERS_POS+Vector3f(0,0.1,0),"leftStringConnect");
//myStrings["leftString"]=parts["leftHand"];
createStringJoint("leftString");


//right Hand
addString(R_FINGERS_POS+Vector3f(0,0.15,0), R_FINGERS_POS+Vector3f(0,0.3,0), 0.015,"rightString");  //original!
//addFixedJoint(parts["rightHand"]->body, myStrings["rightString"]->body,"rightStringConnect");
addBallJoint(parts["rightHand"]->body, myStrings["rightString"]->body,R_FINGERS_POS+Vector3f(0,0.1,0),"rightStringConnect");
//myStrings["rightString"]=parts["rightHand"];
createStringJoint("rightString");

//left Foot
addString(L_TOES_POS+Vector3f(0,0.15,0), L_TOES_POS+Vector3f(0,0.3,0), 0.015,"leftFootString");  //original!
addBallJoint(parts["leftFoot"]->body, myStrings["leftFootString"]->body,L_TOES_POS+Vector3f(0,0.1,0),"leftFootStringConnect");
//myStrings["leftFootString"]=parts["leftFoot"];
createStringJoint("leftFootString");

//right Foot
addString(R_TOES_POS+Vector3f(0,0.15,0), R_TOES_POS+Vector3f(0,0.3,0), 0.015,"rightFootString");  //original!
addBallJoint(parts["rightFoot"]->body, myStrings["rightFootString"]->body,R_TOES_POS+Vector3f(0,0.1,0),"rightFootStringConnect");
//myStrings["rightFootString"]=parts["rightFoot"];
createStringJoint("rightFootString");


//spine
Vector3f spinePos=Vector3f(0.0, CHEST_H, 0.0);

addString(spinePos+Vector3f(0,0,-0.15), spinePos+Vector3f(0,0,-0.3), 0.015,"spineString");  //original!
addFixedJoint(parts["chest"]->body, myStrings["spineString"]->body,"spineStringConnect");

//addBallJoint(parts["chest"]->body, myStrings["spineString"]->body,spinePos+Vector3f(0,0,0.15),"spineStringConnect");
//myStrings["spineString"]=parts["chest"];
createStringJoint("spineString");

Vector3f hipPos=Vector3f(0.0, HIP_H, 0.0);

//addString(hipPos+Vector3f(0,0,-2), hipPos+Vector3f(0,0,-4), 0.015,"hipString");  //original!
addString(hipPos+Vector3f(0,0,-0.15), hipPos+Vector3f(0,0,-0.3), 0.015,"hipString");  //original!
addFixedJoint(parts["pelvis"]->body, myStrings["hipString"]->body,"hipStringConnect");
//addBallJoint(parts["pelvis"]->body, myStrings["hipString"]->body,hipPos+Vector3f(0,0,0.15),"hipStringConnect");
//myStrings["hipString"]=parts["pelvis"];
createStringJoint("hipString");
}


void RagDoll::update(double deltaTime){

if (renderer->bUpdatePhysics)
    updateMove();

//read back Limb locations
/*
if (!bUDPControlled){
    Vector3f spineAbsolute=Vector3f(0,0,0);
    Vector3f initialOffset=Vector3f(0,0,-0.235);

    //update Spine all the time

    Vector3f leftFootLoc=myStrings["leftFootString"]->location + initialOffset;
    Vector3f rightFootLoc=myStrings["rightFootString"]->location + initialOffset;

    spineAbsolute=(leftFootLoc+rightFootLoc)*0.5;
    //take care of spine
    //maybe add a spine offset if we move the spine?
    //Spine
    //according to the distance between the two feet
    //spineAbsolute=(lftFootLastAbsolute+rtFootLastAbsolute)/2;
    spineAbsolute.y= CHEST_H;
    //center String should always be in between both feet
    updateString("spineString",spineAbsolute);
    spineAbsolute.y= HIP_H;
    updateString("hipString",spineAbsolute);
    }
*/
}

void RagDoll::drawCube(){

    glPushMatrix();

    //don't need to, because all the individual parts are doing this!!!
    //glTranslatef(location.x,location.y,location.z);

    //don't need to, because all the individual parts are doing this!!!
    //glRotatef(rotation.x,1,0,0);
    //glRotatef(rotation.y,0,1,0);
    //glRotatef(rotation.z,0,0,1);

    std::map <std::string, PhysicsActor*>::iterator it;


    for ( it=parts.begin() ; it != parts.end(); it++ )
        {
          PhysicsActor * pA = it->second;
          pA->color=color;
          pA->draw();
        }
    glPopMatrix();
}

void RagDoll::drawCollision(){

    glPushMatrix();

    if (base)
        glLoadMatrixf(base->baseMatrix);

    //don't need to, because all the individual parts are doing this!!!
    if (bUseTransformMatrix)
        glMultMatrixf(transformMatrix);
    else{
        glTranslatef(location.x,location.y,location.z);
        glRotatef(rotation.x,1,0,0);
        glRotatef(rotation.y,0,1,0);
        glRotatef(rotation.z,0,0,1);
        }

    std::map <std::string, PhysicsActor*>::iterator it;

    //added for picking, so we know about sub-objects
    for ( it=parts.begin() ; it != parts.end(); it++ )
        {
          PhysicsActor * pA = it->second;
          pA->color=color;
          pA->draw();
        }

//these are helpers now...
/*
    for ( it=myStrings.begin() ; it != myStrings.end(); it++ )
        {
          PhysicsActor * pA = it->second;
          pA->color=color;
          glPushName(STRINGS);
          glPushName(pickName++);
          pA->draw();
          glPopName();
          glPopName();
        }
*/
    glPopMatrix();
}


void RagDoll::addBody(Vector3f p1, Vector3f p2, float radius, string partName){

		PhysicsActor * newPart = new PhysicsActor();

        float length=(p2-p1).length();

		newPart->shape.x=radius;
		newPart->shape.y=length;

        newPart->setup();
        newPart->name=partName;

		Vector3f myLoc;
        myLoc=(p1+p2)*0.5;

		newPart->setLocation(myLoc+location);
		newPart->setRotationODE(p2-p1);

		totalMass += newPart->mass;

        newPart->parent=this;

        parts[partName]=newPart;
}


void RagDoll::addString(Vector3f p1, Vector3f p2, float radius, string partName){

		PhysicsActor * newPart = new PhysicsActor();

        Vector3f myLoc=p1;

        float length=(p2-p1).length();

		newPart->shape.x=radius;
		newPart->shape.y=length;

        newPart->setup();
        newPart->name=partName;

		newPart->setLocation(myLoc+location);

		newPart->setRotationODE(p2-p1);

        newPart->parent=this;

        myStrings[partName]=newPart;

        newPart->base=this;

        cout << "Adress of new string is: " << newPart << endl;
}


void RagDoll::addFixedJoint(dBodyID body1,dBodyID body2, string name){

        dFixedJoint * joint = new dFixedJoint(renderer->physicsWorld);

		joint->attach(body1, body2);
		joint->set();

		joints[name]=joint;
}

void RagDoll::addBallJoint(dBodyID body1, dBodyID body2, Vector3f anchor, string name){

        anchor+=location;
        // create the joint
		dBallJoint * joint = new dBallJoint(renderer->physicsWorld);

		joint->attach(body1, body2);
		joint->setAnchor(anchor.x,anchor.y,anchor.z);

        joints[name]=joint;
}

void RagDoll::addHingeJoint( dBodyID body1, dBodyID body2, Vector3f anchor,Vector3f axis, string name, float loStop, float hiStop){
		anchor +=location;

		dHingeJoint * joint = new dHingeJoint(renderer->physicsWorld);

		joint->attach(body1, body2);
		joint->setAnchor(anchor.x,anchor.y,anchor.z);
		joint->setAxis(axis.x,axis.y,axis.z);

        joint->setParam(dParamLoStop, loStop);
		joint->setParam(dParamHiStop, hiStop);
        joints[name]=joint;
}

void RagDoll::addUniversalJoint(dBodyID body1,dBodyID body2,Vector3f anchor,Vector3f axis1,Vector3f axis2, string name,float loStop1, float hiStop1, float loStop2, float hiStop2){

		anchor +=location;

		dUniversalJoint * joint = new dUniversalJoint(renderer->physicsWorld);
		joint->attach(body1, body2);
		joint->setAnchor(anchor.x,anchor.y,anchor.z);
		joint->setAxis1(axis1.x,axis1.y,axis1.z);
		joint->setAxis2(axis2.x,axis2.y,axis2.z);
		joint->setParam(dParamLoStop, loStop1);
		joint->setParam(dParamHiStop, hiStop1);
		joint->setParam(dParamLoStop2, loStop2);
		joint->setParam(dParamHiStop2, hiStop2);

		joints[name]=joint;
}




void RagDoll::trigger(MsbObject* other){

}


void RagDoll::createStringJoint(string stringName){

//let's do this manually...
		dFixedJoint * joint = new dFixedJoint(renderer->physicsWorld);

		joint->attach(0,myStrings[stringName]->body);
		joint->set();

        joints[stringName]=joint;
}

void RagDoll::updateString(string stringName, Vector3f loc){

dJointDestroy(joints[stringName]->id());
dBodySetPosition(myStrings[stringName]->body,loc.x,loc.y,loc.z);
myStrings[stringName]->location=loc;
createStringJoint(stringName);
}


void RagDoll::updateMove(){

// how does this work?
// we get positions from the gametrak (or any other input device from that matter) that are relative to the ragdoll's center.
// inorder to make the ragdoll walk, the positions of the feet have to be relative to the last foot position,
// otherwise it will be very hard for the puppeteer to not make the foot "jump" when lifting it up again.
//
// walking works like this:
// 1.   the first foot that gets lifted above a certain threshhold becomes the "active" foot.
//      (both feet can be lifted at the same time, but only one can be the active foot!)
//
// 2.   once the active foot is being put down again, we calculate the distance vector between
//      the two feet and set the ragdoll's torso to half that vector (disregarding the y-Axis obviously)


//get foot and hand positions from helper object location

    //cout << myStrings["leftFootString"]->location + lftFootLastAbsolute +Vector3f(0,0,-0.235) << endl;
    //lftFootRel= myStrings["leftFootString"]->location + lftFootLastAbsolute +Vector3f(0,0,-0.235);

bool bSteppedLeft=false;
bool bSteppedRight=false;


//left Foot - check for step
    lftFootRel.y-=0.15;                                                     //everything below -0.15 is considered a grounded foot
    if (lftFootRel.y<0)                                                     //this foot is grounded
      {
      lftFootRel.y=0;                                                       //clip to zero
      if (lftFootRelToLast.y>0 && activeFoot=="left")                        //if we were the active foot and our footPlace was above ground, we just stepped down!
        {
        activeFoot="NULL";                                                //deactivate left Foot
        bSteppedLeft=true;                                                //we just made a step
        }
      }

//right Foot - check for step
    rtFootRel.y-=0.15;
    if (rtFootRel.y<0)
      {
      rtFootRel.y=0;
      if (rtFootRelToLast.y>0 && activeFoot=="right")                       //we just stepped down
        {
        bSteppedRight=true;
        activeFoot="NULL";
        }
      }


//flip feet - for gametrak positioning reasons - optional!
    if (bFlipFeet)
      {
        rtFootRel.x=-rtFootRel.x;
        rtFootRel.z=-rtFootRel.z;
        lftFootRel.x=-lftFootRel.x;
        lftFootRel.z=-lftFootRel.z;
      }


//feet movement
//only valid if above certain threshhold

//Left Foot
    if (lftFootRel.y>0)                                           //if we lift the left foot
      {
      activeFoot="left";                                        //consider this the one we are looking at for the next step
      lftFootRelToLast=(lftFootRel+Vector3f(baseOffset,0,0))*actionSpaceMultiplier -loffset;    //calculating the leftFoot Position relative to the last Step
      updateString("leftFootString",lftFootRelToLast+ lftFootLastAbsolute);
      }
    else                                                        //we calculate the relative position of our foot so we can start from our actual foot position, no matter where input is!
      {
      loffset=(lftFootRel+Vector3f(baseOffset,0,0))*actionSpaceMultiplier;
      }

//Right Foot
    if (rtFootRel.y>0)
      {
      activeFoot="right";
      rtFootRelToLast=(rtFootRel+Vector3f(-baseOffset,0,0))*actionSpaceMultiplier - roffset;
      updateString("rightFootString",rtFootRelToLast + rtFootLastAbsolute);
      }
    else
      {
      roffset=(rtFootRel+Vector3f(-baseOffset,0,0))*actionSpaceMultiplier;
      }

//Spine
//according to the distance between the two feet
  spineAbsolute=(lftFootLastAbsolute+rtFootLastAbsolute)/2;
  spineAbsolute.y= CHEST_H;
//center String should always be in between both feet
  updateString("spineString",spineAbsolute);
  spineAbsolute.y= HIP_H;
  updateString("hipString",spineAbsolute);

//Hands
//relative to the spine location, only if we are controlled by a pilot?
  spineAbsolute.y=0;
//TODO: only if not mousePilot!!!
  if (bUDPControlled){
      updateString("leftString",(lftHandRel*Vector3f(0.1,1,0.1)+Vector3f(baseOffset,0,0))*actionSpaceMultiplier + spineAbsolute);
      updateString("rightString",(rtHandRel*Vector3f(0.1,1,0.1)-Vector3f(baseOffset,0,0))*actionSpaceMultiplier + spineAbsolute);
      }
//Update if we actually stepped!
//LeftFoot
    if (bSteppedLeft)
      {
      lftFootLastAbsolute+=lftFootRelToLast;
      lftFootLastAbsolute.y=0;
      cout << "steppedLeft: " <<bSteppedLeft << endl;
      }
//RightFoot
    if (bSteppedRight)
      {
      rtFootLastAbsolute+=rtFootRelToLast;
      rtFootLastAbsolute.y=0;
      cout << " steppedRight: " << bSteppedRight << endl;
      }

}

void RagDoll::reset(){


cout << "resetting...";
lftFootRelToLast=Vector3f(0,0,0);
rtFootRelToLast=Vector3f(0,0,0);
spineAbsolute=Vector3f(0.0,  1.35, 0.0);
activeFoot="NULL";

cout << "done!" << endl;

  updateString("leftFootString",lftFootRelToLast+ lftFootLastAbsolute);
  updateString("rightFootString",rtFootRelToLast+ rtFootLastAbsolute);
}

void RagDoll::create(){
renderer->addActor(this);
std::cout << "creating a ragdoll! \n";
}


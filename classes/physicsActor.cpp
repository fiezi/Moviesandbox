#include "physicsActor.h"
#include "renderer.h"
#include "input.h"
#include "ragDoll.h"

PhysicsActor::PhysicsActor(){

name="PhysicsActor";
drawType=DRAW_CUBE;

world=renderer->physicsWorld;
space=renderer->collisionSpace;

body = dBodyCreate(renderer->physicsWorld);
geom=0;
joint=NULL;
oldBase=NULL;

shape=Vector3f(0.1,0.3,0.1);
mass=1.0;
type=CAPSULESHAPE;
//drawType=DRAW_SPECIAL;
bJointedToBase=false;
bSkeletalPart=false;
bFixToWorld=false;
bInit=false;

angleDamp=0.59;
linearDamp=0.004;

registerProperties();



menuType.push_back("12AssignButton");
menuProp.push_back("NULL");
menuIcon.push_back("icon_base");
menuName.push_back("make joint");
}


PhysicsActor::PhysicsActor(Vector3f myShape,dReal myMass, int myType){

world=renderer->physicsWorld;
space=renderer->collisionSpace;

shape=myShape;
type=myType;
mass=myMass;

}


void PhysicsActor::registerProperties(){

Actor::registerProperties();
createMemberID("BFIXTOWORLD",&bFixToWorld,this);
createMemberID("BJOINTEDTOBASE",&bJointedToBase,this);
createMemberID("ANGLEDAMP",&angleDamp,this);
createMemberID("LINEARDAMP",&linearDamp,this);
}

PhysicsActor::~PhysicsActor(){

if (joint>0)
    dJointDestroy(joint);
if (geom>0)
    dGeomDestroy(geom);
if (body>0)
    dBodyDestroy(body);

}

void PhysicsActor::postLoad(){

	dMass m;

    if (type==CUBESHAPE)
      {
	  geom = dCreateBox(space,shape.x,shape.y,shape.z);
      dMassSetBox(&m,1.0f,shape.x,shape.y,shape.z);
      }
	if (type==CAPSULESHAPE)
	  {
	  geom = dCreateCapsule(space,shape.x,shape.y);
      dMassSetCapsule(&m, shape.z, 3, shape.x, shape.y);      //the '3' means align on z-axis  and density is shape.z
	  generateCapsuleList();
	  }
	dMassAdjust(&m,mass);
	dBodySetMass(body,&m);


	dGeomSetBody(geom,body);

    //initialise position
    if (base){
        Matrix4f bGlobal= base->baseMatrix * renderer->inverseCameraMatrix;
        dBodySetPosition(body,bGlobal.data[12] + originalMatrix.data[12] + transformMatrix.data[12],bGlobal.data[13] + originalMatrix.data[13] + transformMatrix.data[13],bGlobal.data[14] + originalMatrix.data[14] + transformMatrix.data[14]);
    }else{
        dBodySetPosition(body,originalMatrix.data[12] + transformMatrix.data[12],originalMatrix.data[13] + transformMatrix.data[13],originalMatrix.data[14] + transformMatrix.data[14]);
    }

   dBodySetDamping(body, linearDamp, angleDamp);

   bInit=true;

}

void PhysicsActor::setup(){

    if (!bInit)
        postLoad();

    if (bJointedToBase){
        makeJoint();
    }

    angleDamp=0.9;
    linearDamp=0.2;

}

void PhysicsActor::update(double deltaTime){

    if (drawType==DRAW_CUBE){
        if (sceneData->controller->bRunning)
            drawType=DRAW_NULL;
        else
            drawType=DRAW_CUBE;
    }


	const dReal *p = dBodyGetPosition(body);
	//const dReal *p = dGeomGetPosition(geom);
	const dReal *r = dBodyGetRotation(body);
	float m[16];

	m[ 0] = r[ 0];m[ 1] = r[ 4];m[ 2] = r[ 8];m[ 3] = 0;
	m[ 4] = r[ 1];m[ 5] = r[ 5];m[ 6] = r[ 9];m[ 7] = 0;
	m[ 8] = r[ 2];m[ 9] = r[ 6];m[10] = r[10];m[11] = 0;
	m[12] = p[ 0];m[13] = p[ 1];m[14] = p[ 2];m[15] = 1;



    //update physics from drawing when not live
    // make sure we assign our base again if we had one when simulating

    Matrix4f bGlobal;
    bGlobal.identity();

    if (!renderer->bUpdatePhysics){

        if (oldBase && !base){
            base=oldBase;
            oldBase=NULL;
        }
        if (base){
               bGlobal= renderer->inverseCameraMatrix * baseMatrix;// * originalMatrix;
               dBodySetPosition(body,bGlobal.data[12],bGlobal.data[13],bGlobal.data[14]);
               //dBodySetPosition(body,bGlobal.data[12] + transformMatrix.data[12],bGlobal.data[13] + transformMatrix.data[13],bGlobal.data[14] + transformMatrix.data[14]);
        }else{
            //dBodySetPosition(body,originalMatrix.data[12] + transformMatrix.data[12],originalMatrix.data[13] + transformMatrix.data[13],originalMatrix.data[14] + transformMatrix.data[14]);
            //calculate viewpoint into absolute position!
            dBodySetPosition(body,bGlobal.data[12] + transformMatrix.data[12],bGlobal.data[13] + transformMatrix.data[13], bGlobal.data[14] +transformMatrix.data[14]);
        }
    //update drawing from physis when live
    //also get rid of base actors as they screw up the calculation
    //originalMatrix.identity();
    }else{

        if (!bFixToWorld){
            if (base && !oldBase){
              oldBase=base;
              base=NULL;
            }
            transformMatrix=m;
        }else{
            dBodySetLinearVel(body,0,0,0);
            if (base){
               bGlobal= (renderer->inverseCameraMatrix * baseMatrix);// * originalMatrix;
               //dBodySetPosition(body,bGlobal.data[12] + transformMatrix.data[12],bGlobal.data[13] + transformMatrix.data[13],bGlobal.data[14] + transformMatrix.data[14]);
               dBodySetPosition(body,bGlobal.data[12],bGlobal.data[13],bGlobal.data[14]);
            }else{
                dBodySetPosition(body,bGlobal.data[12] + transformMatrix.data[12],bGlobal.data[13] + transformMatrix.data[13], bGlobal.data[14] +transformMatrix.data[14]);
            }
        }

    }
}


void PhysicsActor::trigger(MsbObject* other){

    Actor::trigger(other);

    if (other->name=="make joint"){
        makeJoint();
    }

//    dBodyDisable(body);
//    dBodySetDamping(body, linearDamp, angleDamp);
//    dBodyEnable(body);
}
#define CAPSULE_SLICES 16
#define CAPSULE_STACKS 12

void PhysicsActor::drawCollisionBox() const {

	if (type==CUBESHAPE)
	  {
	  glScalef(shape.x,shape.y,shape.z);
      glColor4fv(color);
      glutSolidCube(0.5);
	  }

    if (type==CAPSULESHAPE)
	  {
	  glCallList(capsuleList);
	  }
    //TODO: for painting purposes
    if (type==FLATSHAPE)
      {
      glScalef(shape.x,shape.y,shape.z);
      glColor4fv(color);
      glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3f(0,0,0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(scale.x,0,0.0);
        glTexCoord2f(1.0,1.0);
		glVertex3f(scale.x,scale.y,0.0);
        glTexCoord2f(0.0,1.0);
		glVertex3f(0,scale.y,0.0);
      glEnd();
      }
}


void PhysicsActor::generateCapsuleList(){

capsuleList=glGenLists(1);

glNewList(capsuleList,GL_COMPILE);

	  glColor4fv(color);
	  float cylHalfHeight = shape.y / 2.0;
	  glBegin(GL_QUAD_STRIP);
		for (int i=0; i<(CAPSULE_SLICES + 1);i++)
		  {
			float angle = float(i) / float(CAPSULE_SLICES) * 2.0 * M_PI;
			float ca = cos(angle);
			float sa = sin(angle);
			glNormal3f(ca, sa, 0);
			glVertex3f(shape.x * ca, shape.x * sa, cylHalfHeight);
			glVertex3f(shape.x * ca, shape.x * sa, -cylHalfHeight);
		  }

		glEnd();
		glTranslatef(0, 0, cylHalfHeight);
		glutSolidSphere(shape.x, CAPSULE_SLICES, CAPSULE_STACKS);
		glTranslatef(0, 0, -2.0 * cylHalfHeight);
		glutSolidSphere(shape.x, CAPSULE_SLICES, CAPSULE_STACKS);
// endList
glEndList();

}

void PhysicsActor::setRotationODE(Vector3f vec){

/*
# define body rotation automatically from body axis
		za = norm3(sub3(p2, p1))
		if (abs(dot3(za, (1.0, 0.0, 0.0))) < 0.7): xa = (1.0, 0.0, 0.0)
		else: xa = (0.0, 1.0, 0.0)
		ya = cross(za, xa)
		xa = norm3(cross(ya, za))
		ya = cross(za, xa)
		rot = (xa[0], ya[0], za[0], xa[1], ya[1], za[1], xa[2], ya[2], za[2])

*/

        Vector3f xAxis;
		Vector3f yAxis;
		Vector3f zAxis;

		zAxis = vec;
		zAxis.normalize();

		//check forward/backward facing ?
		if (abs(zAxis.dotProduct(Vector3f(1.0, 0.0, 0.0))) < 0.7)
		  xAxis = Vector3f(1.0, 0.0, 0.0);
		else
		  xAxis = Vector3f(0.0, 1.0, 0.0);

		yAxis = zAxis.crossProduct(xAxis);

		xAxis = yAxis.crossProduct(zAxis);
		xAxis.normalize();

		yAxis = zAxis.crossProduct(xAxis);

		dMatrix3 rotMatrix;
		rotMatrix[0] = xAxis[0]; rotMatrix[1] = yAxis[0]; rotMatrix[2] = zAxis[0];
        rotMatrix[4] = xAxis[1]; rotMatrix[5] = yAxis[1]; rotMatrix[6] = zAxis[1];
        rotMatrix[8] = xAxis[2]; rotMatrix[9] = yAxis[2]; rotMatrix[10] = zAxis[2];

        dBodySetRotation(body,rotMatrix);
}

//creates ball joint on base
void PhysicsActor::makeJoint(){

        if (!base)
            return;

        PhysicsActor* physBase=dynamic_cast<PhysicsActor*>(base);

        if (!physBase)
            return;

        cout << "creating joint..." << endl;

        const dReal* baseLoc=dBodyGetPosition(physBase->body);
        // create the joint
        joint = dJointCreateBall(renderer->physicsWorld,0);

		//joint->setAnchor((baseLoc[0] + myLoc[0])*0.5 ,(baseLoc[1] + myLoc[1]) * 0.5 ,(baseLoc[2] + myLoc[2]) * 0.5 );
		dJointAttach(joint, body, physBase->body);

        // And lastly we tell ODE where exactly the two objects are joined in world coordinates.
        dJointSetBallAnchor(joint, baseLoc[0], baseLoc[1], baseLoc[2]);

        bJointedToBase=true;
}


void PhysicsActor::create(){sceneData->addActor(this);}


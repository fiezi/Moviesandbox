#ifndef H_RAGDOLL
#define H_RAGDOLL

#include "actor.h"
#include "physicsActor.h"

class RagDoll: public Actor
{
    public:
    //TODO: we don't move the physicsActors in Physics coordinates - so they interact with each other even though they shouldn't!
            //this comes from the pilot and is indeed always relative!

            string      activeFoot;

            Vector3f    lftHandRel,             //relative to the Ragdoll's body - can this be abstracted?
                        rtHandRel,
                        lftFootRel,
                        rtFootRel;

            Vector3f    lftFootRelToLast,       //relative to the last step
                        rtFootRelToLast,
                        lftFootLastAbsolute,    //last absolute foot position
                        rtFootLastAbsolute,
                        loffset,                //current left foot offset from the ragdoll's center in x-Axis
                        roffset,                //current right foot offset from the ragdoll's center in x-Axis
                        walkDistance,           //distance from the initial RagDoll Position
                        spineAbsolute;          //the absolute position of the ragdoll's spine

            bool        bFlipFeet,              //flips control direction of feet
                        bUDPControlled;         //determines if  ragdoll is controlled through UDP or mouse

            float       baseOffset,             //initial distance between feet and center -- has something to do with Leg_W
                        actionSpaceMultiplier;  //our action space - how far do we want out hands to go?


        std::map<string, PhysicsActor*> parts;          //all the ragdoll's physical bodies
		std::map<string, dJoint*> joints;               //all the ragdoll's joints
        std::map<string, PhysicsActor*> myStrings;      //all the control points (they are joints too, but more special!)

        Vector3f R_SHOULDER_POS,                        //physical definition of ragdoll - should come from an XML soon
                 L_SHOULDER_POS,

                 R_ELBOW_POS,
                 L_ELBOW_POS,

                 R_WRIST_POS,
                 L_WRIST_POS,

                 R_FINGERS_POS,
                 L_FINGERS_POS,

                 R_HIP_POS,
                 L_HIP_POS,

                 R_KNEE_POS,
                 L_KNEE_POS,

                 R_ANKLE_POS,
                 L_ANKLE_POS,

                 R_HEEL_POS,
                 L_HEEL_POS,

                 R_TOES_POS,
                 L_TOES_POS;

        float   UPPER_ARM_LEN,
                FORE_ARM_LEN,
                HAND_LEN,
                FOOT_LEN,
                HEEL_LEN,

                BROW_H,
                MOUTH_H,
                NECK_H,
                SHOULDER_H,
                CHEST_H,
                HIP_H,
                KNEE_H,
                ANKLE_H,

                SHOULDER_W,
                CHEST_W,
                LEG_W,
                PELVIS_W;


		float totalMass;                    //gets calculated

         RagDoll();
        ~RagDoll();

        void registerProperties();
        void createRagDoll();               //called in setup - so we can apply different values and create our Ragdoll from there...
        void setup();
        void update(double deltaTime);
        void drawCube();
        virtual void drawCollision();

        void reset();

        //Ragdoll construction functions
        void addBody(Vector3f p1, Vector3f p2, float radius, string partName);
        void addFixedJoint(dBodyID body1, dBodyID body2, string partName);
        void addBallJoint(dBodyID body1, dBodyID body2, Vector3f anchor, string name);
        void addHingeJoint(dBodyID body1, dBodyID body2, Vector3f anchor,Vector3f axis,string name, float loStop = dInfinity, float hiStop = dInfinity);
        void addUniversalJoint(dBodyID body1, dBodyID body2, Vector3f anchor, Vector3f axis1, Vector3f axis2, string name, float loStop1 = dInfinity, float hiStop1 = dInfinity, float loStop2 = dInfinity, float hiStop2 = dInfinity);

        //controllable joints!
        void addString(Vector3f p1, Vector3f p2, float radius, string name);
        void createStringJoint(string stringName);

        void trigger(Actor * other);

        //moving the controllable joints
        virtual void updateString(string stringName, Vector3f loc);
        virtual void updateMove();

        virtual void create();
};
#endif





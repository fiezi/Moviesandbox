

#include "hitBox.h"
#include "renderer.h"
#include "input.h"
#include "skeletalActor.h"



HitBox::HitBox(){

    name="hitBox";
    collisionActor=NULL;
    currentLoc=0;

    drawType=DRAW_TEA;

    //Actor Menu
    menuType.push_back("12AssignButton");
    menuName.push_back("saveLocation");
    menuProp.push_back("NULL");
    menuIcon.push_back("icon_base");


    menuType.push_back("15PickWorldButton");
    menuName.push_back("collisionActor");
    menuProp.push_back("NULL");
    menuIcon.push_back("icon_character");

    boxLocations.push_back(Vector3f(1.53,5.11,1.37));
    boxLocations.push_back(Vector3f(-0.95,4.51,1.57));
    boxLocations.push_back(Vector3f(-2.34,6.21,1.37));
    boxLocations.push_back(Vector3f(-0.42,2.31,1.97));
    boxLocations.push_back(Vector3f(-2.52,4.5,0.24));
    boxLocations.push_back(Vector3f(0.08,5.9,0.86));

    registerProperties();
}

HitBox::~HitBox(){


       doc.SaveFile( savename);
}

void HitBox::registerProperties(){

Actor::registerProperties();

createMemberID("COLLISIONACTOR",&collisionActor,this);
createMemberID("BOXLOCATIONS",&boxLocations,this);
}

void HitBox::setup(){
Actor::setup();

    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    root = new TiXmlElement( "Box Game" );
    doc.LinkEndChild( root );

/*
             time_t rawtime;
                struct tm * timeinfo;

                time ( &rawtime );
                timeinfo = localtime ( &rawtime );


        savename="exp_";
        savename+=asctime (timeinfo);
        //get rid of terminating and line break
        savename.erase(savename.end()-2,savename.end());

        int pos=1;

        while (pos!=(int)string::npos){
            pos=savename.find(':', pos+1);
            if (pos!=(int)string::npos)
                savename.replace(pos,1,"-");
        }

        savename+=".xml";

*/
}

void HitBox::trigger(MsbObject* other){

    if (other->name=="collisionActor"){
        collisionActor=input->worldTarget;
        cout << "set Collision Actor to:" << collisionActor->name << endl;
        }
    if (other->name=="saveLocation"){
        boxLocations.push_back(location);
        }
        //here we assign a new location - just like a keyframe or something...
}

void HitBox::update(double deltaTime){

    if (collisionActor && collisionActor->isA("SkeletalActor")){
        SkeletalActor* skel=dynamic_cast<SkeletalActor*>(collisionActor);

        for (uint i=0; i<skel->bones.size();i++){
            Actor* bone=skel->bones[i];
            Vector3f boneLoc=(renderer->inverseCameraMatrix * bone->baseMatrix ).getTranslation();
            Vector3f distance = boneLoc-location;
            if (distance.length()< 0.5){
              if ((uint)currentLoc<boxLocations.size())
                setLocation(boxLocations[currentLoc++]);
              else{
                currentLoc=0;
                setLocation(boxLocations[currentLoc]);
                }
            cout << "hit!" << endl;


/*

            //save controller location and rotation
            TiXmlElement * hitBoxElement=new TiXmlElement("Hit Box");

                time_t rawtime;
                struct tm * timeinfo;

                time ( &rawtime );
                timeinfo = localtime ( &rawtime );


                hitBoxElement->SetAttribute("Time",asctime (timeinfo));
                hitBoxElement->SetAttribute("Box Number",currentLoc);

            root->LinkEndChild(hitBoxElement);
            doc.SaveFile(savename);
*/
            }
        }
    }

}


void HitBox::create(){sceneData->addActor(this);}

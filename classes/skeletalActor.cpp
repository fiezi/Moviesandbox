

#include "skeletalActor.h"
#include "renderer.h"
#include "input.h"
#include "boneActor.h"
#include "physicsActor.h"
#include "meshData.h"

SkeletalActor::SkeletalActor(){

drawType=DRAW_VBOMESH;
sceneShaderID="color";
bUseShader=true;

vboMeshID="teapot";

boneTransforms=NULL;
invBoneTransforms=NULL;
skeletonName="NULL";

bPhysicsBones=false;
bDelayedConvert=false;

bInit=false;

/*
menuType.push_back("12AssignButton");
menuProp.push_back("NULL");
menuIcon.push_back("icon_base");
menuName.push_back("make Physics");
*/

menuType.push_back("12AssignButton");
menuProp.push_back("NULL");
menuIcon.push_back("icon_base");
menuName.push_back("reset");

registerProperties();
}

SkeletalActor::~SkeletalActor(){}

void SkeletalActor::registerProperties(){

createMemberID("BONES",&bones,this);
createMemberID("BPHYSICSBONES",&bPhysicsBones,this);
createMemberID("SKELETONNAME",&skeletonName,this);
ParticleSystem::registerProperties();
}


void SkeletalActor::postLoad(){

    //TODO:problem with actorList referencing!
    if (bPhysicsBones){          //remove geoms and bodys so we don't create ODE errors
        if (bones.size()>0){
            for (int i=0; i<(int)bones.size();i++){
                if (bones[i]){
                        ((PhysicsActor*)bones[i])->geom=0;
                        ((PhysicsActor*)bones[i])->body=0;
                        ((PhysicsActor*)bones[i])->joint=0;
                    }
                    bones[i]->remove();
            }
        }
        bones.clear();
    }


    //always remove bones here if we have any!
    //NO DON'T!! It will kill all references!
    /*
    for (int i=0;i<(int)bones.size();i++){
        bones[i]->remove();
    }
    bones.clear();
    */
    cout << "SkeletalActor Name: " << name << endl;
    cout << "SkeletalActor vboID: " << vboMeshID << endl;

	if (renderer->vboList[vboMeshID]->boneCount>0){
		boneTransforms=new Matrix4f[renderer->vboList[vboMeshID]->boneCount];
		invBoneTransforms=new Matrix4f[renderer->vboList[vboMeshID]->boneCount];
	}
    int myPos=0;

    //find my position in actorList
    for (int i=0;i<(int)renderer->actorList.size();i++){
        if (renderer->actorList[i]==this){
            myPos=i;
        }
    }


    //spawn bones
    for (int i=0; i< renderer->vboList[vboMeshID]->boneCount; i++){

        boneTransforms[i].identity();
        invBoneTransforms[i]=*renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix;
        if ((int)bones.size()==i){
            //insert before character?
            //renderer->actorList.insert(renderer->actorList.begin()+ myPos+i, bones[i]);
                bones.push_back(spawn("9BoneActor"));
                bones[i]->name=renderer->vboList[vboMeshID]->bones[i]->name;
            bones[i]->bRemoveable=false;
            bones[i]->drawType=DRAW_CUBE;
            //bones[i]->scale=bones[i]->scale*0.25;
            bones[i]->base=this;

            }

        bones[i]->originalMatrix=*renderer->vboList[vboMeshID]->bones[i]->boneMatrix;

        //cout << "created new bone: " << bones[i]->name << endl;

        }

//connect parents...
    for (int i=0; i< renderer->vboList[vboMeshID]->boneCount; i++){
        for (int j=0;j<renderer->vboList[vboMeshID]->boneCount;j++){
            if (renderer->vboList[vboMeshID]->bones[i]->parentBone){
                if (renderer->vboList[vboMeshID]->bones[j]==renderer->vboList[vboMeshID]->bones[i]->parentBone){
                    cout << "connecting " << bones[i]->name << "  with " << bones[j]->name <<endl;
                    bones[i]->base=bones[j];
                    j=100;
                }
            }
            else{
              bones[i]->base=this;
              //bones[i]->baseMatrix=bones[i]->originalMatrix*renderer->inverseCameraMatrix;
            }
        }
    }

    //setup all bones
    for (int i=0; i<(int)bones.size();i++){
        bones[i]->baseMatrix=calcMatrix(bones[i]);
        bones[i]->matrixToVectors();
        bones[i]->parent=this;
        //((BoneActor*)bones[i])->bDoIK=true;
    }

    if (bPhysicsBones){
        bDelayedConvert=true;
    }

    Actor::postLoad();
}

void SkeletalActor::setup(){

    ParticleSystem::setup();

    if (!bInit)
        postLoad();


}


void SkeletalActor::trigger(Actor * other){

    if (other && other->name=="make Physics")
        convertToPhysicsBones();

    if (other && other->name=="reset")
        reset();
}

void SkeletalActor::update(double deltaTime){

    Actor::update(deltaTime);

        if (bDelayedConvert && bPhysicsBones){
            Vector3f oldRotation= input->controller->controlledActor->rotation;
            input->console->setZero("NULL");
            bDelayedConvert=false;
            renderer->update();                 //TODO:skip one whole frame
            convertToPhysicsBones();
            input->controller->controlledActor->setRotation(oldRotation);
            //input->controller->navBtn->processMove(0);
        }

}

void SkeletalActor::updateShaders(){

    ParticleSystem::updateShaders();

    shaderObject* myShader= renderer->shaderList[renderer->currentShader];

    //if we're being drawn into
    if (drawType==DRAW_PARTICLES){

        if (renderer->currentShader=="skinning"){
            int boneIndices[]={-1,-1,-1,-1};
            int current=0;

            //determine the bones we've got selected
            for (int i=0;i<(int)input->selectedActors.size();i++){
                for (int j=0;j<(int)bones.size();j++){
                    if (input->selectedActors[i]==bones[j]){
                        if (current<4){
                            boneIndices[current]=j;
                            current++;
                        }else cout << "too many bones selected!" << endl;
                    }
                }
            }
        if (myShader->uniforms.find("boneIndices") != myShader->uniforms.end())
            glUniform1iv(myShader->uniforms["boneIndices"],4,(GLint*)&boneIndices);
        }
        return;
    }

    //remove our own transformations from the bone transforms!
    Matrix4f initialMatrix = baseMatrix.inverse();

    if (bPhysicsBones){
        for (int i=0;i<(int)bones.size();i++){

            PhysicsActor* phys=(PhysicsActor*)bones[i];
            if (!renderer->bUpdatePhysics){
                boneTransforms[i]= initialMatrix * bones[i]->baseMatrix * *renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix * *renderer->vboList[vboMeshID]->bindShapeMatrix;
            }else if (phys->bFixToWorld)
                boneTransforms[i]= initialMatrix * bones[i]->baseMatrix * *renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix * *renderer->vboList[vboMeshID]->bindShapeMatrix;
            else{
                boneTransforms[i]= (renderer->cameraMatrix * bones[i]->transformMatrix) * *renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix;
            }
        }
    }else{
        for (int i=0;i<(int)bones.size();i++){
            boneTransforms[i]=  initialMatrix * bones[i]->baseMatrix * *renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix * *renderer->vboList[vboMeshID]->bindShapeMatrix;
        }
    }


    //This depends on driver implementation
    if (myShader->uniforms.find("boneTransforms") != myShader->uniforms.end() || myShader->uniforms.find("boneTransforms[0]") != myShader->uniforms.end()){
        glUniformMatrix4fv(myShader->uniforms["boneTransforms"],renderer->vboList[vboMeshID]->boneCount,false,(GLfloat*)boneTransforms[0]);
	}
}


void SkeletalActor::start(){

    for (int i=0;i<(int)bones.size();i++){
        bones[i]->bHidden=true;
    }
}

void SkeletalActor::stop(){

    Actor::stop();


    for (int i=0;i<(int)bones.size();i++){
        bones[i]->bHidden=false;
    }

    if (!bPhysicsBones)
      return;

        if (bones.size()>0){
            for (int i=0; i<(int)bones.size();i++){
                if (bones[i]){
                        ((PhysicsActor*)bones[i])->geom=0;
                        ((PhysicsActor*)bones[i])->body=0;
                        ((PhysicsActor*)bones[i])->joint=0;
                    }
                    bones[i]->remove();
            }
        }
        bones.clear();

    boneTransforms=new Matrix4f[renderer->vboList[vboMeshID]->boneCount];
    invBoneTransforms=new Matrix4f[renderer->vboList[vboMeshID]->boneCount];
    int myPos=0;

    for (int i=0;i<(int)renderer->actorList.size();i++){
        if (renderer->actorList[i]==this){
            myPos=i;
        }
    }


    for (int i=0; i< renderer->vboList[vboMeshID]->boneCount; i++){

        boneTransforms[i].identity();
        invBoneTransforms[i]=*renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix;
        if ((int)bones.size()==i){
            //insert before character?
            //renderer->actorList.insert(renderer->actorList.begin()+ myPos+i, bones[i]);
                bones.push_back(new BoneActor);
                bones[i]->name=renderer->vboList[vboMeshID]->bones[i]->name;
                renderer->actorList.push_back(bones[i]);
            bones[i]->bUseTransformMatrix=true;
            bones[i]->bRemoveable=false;
            bones[i]->drawType=DRAW_CUBE;
            bones[i]->scale=bones[i]->scale*0.25;
            bones[i]->base=this;
            }

        bones[i]->originalMatrix=*renderer->vboList[vboMeshID]->bones[i]->boneMatrix;

        //cout << "created new bone: " << bones[i]->name << endl;

        }

//connect parents...
    for (int i=0; i< renderer->vboList[vboMeshID]->boneCount; i++){
        for (int j=0;j<renderer->vboList[vboMeshID]->boneCount;j++){
            if (renderer->vboList[vboMeshID]->bones[i]->parentBone){
                if (renderer->vboList[vboMeshID]->bones[j]==renderer->vboList[vboMeshID]->bones[i]->parentBone){
                cout << "connecting " << bones[i]->name << "  with " << bones[j]->name <<endl;
                bones[i]->base=bones[j];
                j=100;
                }
            else{
              bones[i]->base=this;
              bones[i]->baseMatrix=bones[i]->originalMatrix*renderer->inverseCameraMatrix;
              }
            }
        }
    }

    bDelayedConvert=true;
}

void SkeletalActor::reset(){

for (int i=0;i<(int)bones.size();i++)
    bones[i]->remove();

bones.clear();
bInit=false;
//bPhysicsBones=false;
setup();
}

void SkeletalActor::remove(){

    //are now removed by itself!


    for (int i=(int)bones.size()-1;i>=0;i--){
        if (bones[i])
            bones[i]->remove();
        }

    bones.clear();

    Actor::remove();
}


void SkeletalActor::convertToPhysicsBones(){


    vector<Actor*> oldBones;

    bones.swap(oldBones);

    //create Physics Bones

    boneTransforms=new Matrix4f[renderer->vboList[vboMeshID]->boneCount];
    invBoneTransforms=new Matrix4f[renderer->vboList[vboMeshID]->boneCount];

    int myPos=0;

    for (int i=0;i<(int)renderer->actorList.size();i++){
        if (renderer->actorList[i]==this){
            myPos=i;
        }
    }

    for (int i=0; i< renderer->vboList[vboMeshID]->boneCount; i++){

        boneTransforms[i].identity();
        invBoneTransforms[i]=*renderer->vboList[vboMeshID]->bones[i]->invBoneMatrix;
        if ((int)bones.size()==i){
            bones.push_back(new PhysicsActor);
            bones[i]->name=renderer->vboList[vboMeshID]->bones[i]->name;
            //renderer->actorList.insert(renderer->actorList.begin()+ myPos+i, bones[i]);
            renderer->actorList.push_back(bones[i]);
            bones[i]->bUseTransformMatrix=true;
            bones[i]->bRemoveable=false;
            bones[i]->drawType=DRAW_CUBE;
            bones[i]->scale=bones[i]->scale*0.25;
            bones[i]->base=this;
            }
        //bones[i]->originalMatrix=*renderer->vboList[vboMeshID]->bones[i]->boneMatrix;
        bones[i]->baseMatrix=oldBones[i]->baseMatrix;
        bones[i]->transformMatrix=*renderer->vboList[vboMeshID]->bones[i]->boneMatrix;

        //cout << "created new bone: " << bones[i]->name << endl;

        }

//connect parents...
    for (int i=0; i< renderer->vboList[vboMeshID]->boneCount; i++){
        for (int j=0;j<renderer->vboList[vboMeshID]->boneCount;j++){
            if (renderer->vboList[vboMeshID]->bones[i]->parentBone){

                if (renderer->vboList[vboMeshID]->bones[j]==renderer->vboList[vboMeshID]->bones[i]->parentBone){
                    cout << "connecting " << bones[i]->name << "  with " << bones[j]->name <<endl;
                    bones[i]->base=bones[j];
                    j=100;
                    ((PhysicsActor*)bones[i])->bJointedToBase=true;
                    ((PhysicsActor*)bones[i])->bFixToWorld=false;
                }

            }
            else{
                  cout << "this is our hip bone!" << endl;
                  cout << bones[i]->name << endl;
                  bones[i]->base=this;
                  ((PhysicsActor*)bones[i])->bFixToWorld=true;
                  ((PhysicsActor*)bones[i])->bJointedToBase=false;
                  //bones[i]->baseMatrix=bones[i]->transformMatrix;
                }
        }
    }


    for (int i=0;i<(int)bones.size();i++){
        bones[i]->postLoad();

    }
    for (int i=0;i<(int)bones.size();i++){
        bones[i]->setup();
    }

    for (int i=0;i<(int)bones.size();i++){
        oldBones[i]->remove();
    }

    bPhysicsBones=true;
}

void SkeletalActor::create(){renderer->addActor(this);}

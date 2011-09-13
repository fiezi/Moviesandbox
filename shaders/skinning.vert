attribute float vertexID;

attribute vec4 boneReferences;
attribute vec4 vertexWeights;
uniform mat4 boneTransforms[48];

uniform int boneIndices[4];
uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;

uniform mat4 cameraInverse;
uniform float objectID;
uniform float screensize;
uniform float scene_size;
uniform float fov;

varying float zPos;
varying float vID;

int bones[4];
float weights[4];

/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w ;
  particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
  return ( (particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );

}

void seperateBoneInfluences(){

bones[0]=int(boneReferences.x);
bones[1]=int(boneReferences.y);
bones[2]=int(boneReferences.z);
bones[3]=int(boneReferences.w);

weights[0]=vertexWeights.x;
weights[1]=vertexWeights.y;
weights[2]=vertexWeights.z;
weights[3]=vertexWeights.w;
}

void main(){

    seperateBoneInfluences();

    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;
    mat4 myMat=mat4(0.0);

    gl_FrontColor=vec4(0.0);// * 0.5;

    float myWeight=0.0;
    int myBone=-1;
    int howManyBones=0;
    //if we have a bone selected
    for (int i=0;i<4;i++){
        if (boneIndices[0]==bones[i]){
            myBone=i;
        }
    }
    if (myBone>=0){
        myWeight=weights[myBone];
    }

    //if this bone has a weight assigned to this vertex, see how many weights we have
        for (int i=0;i<4;i++){
            if (weights[i]>0.0){
                howManyBones++;
            }
        }

        //if we have 1 weight, color red, with the amount of weight
        if (howManyBones==1)
            gl_FrontColor.r=myWeight;
        //if we have 2 weights, color green
        if (howManyBones==2)
            gl_FrontColor.g=myWeight;
        //if we have 3 weights, color blue
        if (howManyBones==3)
            gl_FrontColor.b=myWeight;

/*
    gl_FrontColor.r=float(boneIndices[0]);
    gl_FrontColor.g=0.0;
    gl_FrontColor.b=0.0;
*/
    gl_FrontColor.a=1.0;

    //apply bone transformations
    gl_Position=gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
    gl_PointSize= pointSize() * screensize/scene_size * (45.0*45.0)/(fov*fov);


    zPos=gl_Position.z;

    vID=vertexID;
}





uniform float screensize;
uniform float scene_size;

attribute float vertexID;


attribute vec4 boneReferences;
attribute vec4 vertexWeights;
uniform mat4 boneTransforms[48];

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform mat4 cameraInverse;
uniform float objectID;

varying float zPos;
varying float pSize;
varying vec4 pixelPos;

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

    //check for bad data
      for (int i=0;i<4;i++){
        if (bones[i]<0){
            gl_FrontColor=vec4(1.0,1.0,0,1);
            weights[i]=0.0;
        }
      }
}

/*
*   Main
*/

void main(){

    //Vertex transformation


    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;
    mat4 myMat=mat4(0.0);

    seperateBoneInfluences();

    //apply bone transformations
    if (bones[0]<0)
        myMat=mat4(1.0);


    for (int i=0; i< 4; i++){
            myMat+= boneTransforms[bones[i]] * weights[i];
        }



    vec4 world=myMat * myVertex;
    world/=world.w;

    vec4 myPosition=gl_ProjectionMatrix * gl_ModelViewMatrix * world;
    gl_Position=myPosition;

    //Point Size
    gl_PointSize=pointSize() * screensize/scene_size;

    //make unskinned particles go away!
    //if (bones[0]<0)
    //  gl_PointSize=0.0;

    //3D positions



    zPos=gl_Position.z;


    gl_FrontColor=gl_Color;

	pixelPos= myPosition;
	pixelPos /= pixelPos.w;
	pixelPos= pixelPos * 0.5 + 0.5;

	pSize=gl_PointSize;

    vID=vertexID;
}





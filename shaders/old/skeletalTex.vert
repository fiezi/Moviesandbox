varying vec3 N, L;
uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
varying float zPos;


attribute vec4 boneReferences;
attribute vec4 vertexWeights;

uniform float distortion;


uniform mat4 boneTransforms[48];

int bones[4];
float weights[4];

void distort(){

  gl_Position.x+=sin( sin(time)) * distortion;
  gl_FrontColor+=sin( sin(time)) * distortion;
}

float pointSize(vec4 loc){
  
  float ptSize = length(loc);
  ptSize= (10000.0/ptSize);
  
  float particleScale=1.0;
  particleScale=gl_Vertex.w * particleMultiplier * gl_Position.w ;
  vec3 myNormal=gl_Normal;
  float sideNess= 1.0 / max(abs(myNormal.z),0.02);
  if (particleScale>0.0)
      particleScale+= particleAngleScale * sideNess* sideNess;  
  ptSize*=particleScale;
  
  return ptSize; 
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
    if (bones[i]<=0)
      weights[i]=0.0;
  }
}

void main(){
gl_FrontColor=vec4(gl_MultiTexCoord0.x, gl_MultiTexCoord0.y, gl_MultiTexCoord0.z,1.0);

seperateBoneInfluences();

vec4 myVertex=gl_Vertex;
myVertex.w=1.0;
mat4 myMat=mat4(0.0);



//apply bone transformations
for (int i=0; i< 4; i++){
        myMat+= boneTransforms[bones[i]] *  weights[i];
    }

//combine with ModelView Matrix, etc...
vec4 world=myMat * myVertex;

gl_Position=gl_ProjectionMatrix * world; 
gl_PointSize=pointSize(gl_Position);

//lighting
  vec4 V = gl_ModelViewMatrix * myVertex;
  vec4 norm=vec4(gl_Normal.x,gl_Normal.y,gl_Normal.z,1.0);
  N = gl_Normal; 
  L = vec3(1.0,1.0,1.0) - V.xyz;

zPos=gl_Position.z;

distort();
}





uniform float time;
varying float zPos;
uniform float particleMultiplier;
uniform float particleAngleScale;

uniform mat4 boneTransforms[48];

int bones[4];
float weights[4];

float pointSize(vec4 loc){
  
  float ptSize = length(loc);
  ptSize= (10000.0/ptSize);
  
  float particleScale=1.0;
  particleScale=gl_Vertex.w * particleMultiplier * gl_Position.w ;
  vec3 myNormal=gl_Normal;
  float sideNess= 1.0 / max(abs(myNormal.z),0.02);
  if (particleScale>0)
      particleScale+= particleAngleScale * sideNess* sideNess;  
  ptSize*=particleScale;
  
  return ptSize; 
}

void seperateBoneInfluences(){

bones[0]=int(gl_Color.x);
bones[1]=int(gl_Color.y);
bones[2]=int(gl_Color.z);
bones[3]=int(gl_Color.w);

weights[0]=gl_SecondaryColor.x;
weights[1]=gl_SecondaryColor.y;
weights[2]=gl_SecondaryColor.z;
weights[3]=gl_SecondaryColor.w;
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
vec4 world=myMat* myVertex;
gl_Position=gl_ProjectionMatrix * world;

gl_PointSize=pointSize(world);

zPos=gl_Position.z;
}





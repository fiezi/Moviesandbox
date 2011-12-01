uniform float time;
varying float zPos;
uniform float particleMultiplier;
uniform float particleAngleScale;

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


void main(){
  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;
  
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
  
  gl_PointSize= pointSize(gl_ModelViewMatrix * myVertex);
  
  gl_FrontColor=gl_Color;
  
  zPos=gl_Position.z;
}





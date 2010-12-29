varying vec3 N, L;
vec3 lightPos;
uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
varying float zPos;

uniform float distortion;

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
  float sideNess= 1.0 / max(abs(myNormal.z),0.00);
  if (particleScale>0.0)
      particleScale+= particleAngleScale * sideNess;  
  ptSize*=particleScale;
  
  return ptSize; 
}



void main()
{ 

  lightPos=vec3(25.0* sin(time * 0.001) * 1.0,1.0,1.0);
  //lightPos=vec3(1.0,1.0,1.0);

  

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;
 
  myVertex.x+=(0.1* sin(time * cos(gl_Normal.z * 0.1) * 0.001) * 1.0);
  myVertex.x-=(0.3* sin(time * cos(myVertex.z * 0.1) * 0.001) * 1.0);
  myVertex.y+=(0.3* sin(time * cos(myVertex.x * 0.1) * 0.001) * 1.0);
  myVertex.z+=(0.4* sin(time * cos(gl_Normal.y * 0.3) * 0.01) * 1.0);
 
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
  
  gl_PointSize= pointSize(gl_ModelViewMatrix * myVertex);
  
  vec4 V = gl_ModelViewMatrix * myVertex;
  N = gl_NormalMatrix * gl_Normal;
  L = gl_LightSource[0].position.xyz - V.xyz;
  L = lightPos - V.xyz;


  gl_FrontColor=clamp(gl_Color, 0.0, 1.0);

  zPos=gl_Position.z;
  
  distort();
}





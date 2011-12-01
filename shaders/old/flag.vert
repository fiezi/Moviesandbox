varying vec3 N, L;
vec3 lightPos;
uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
varying float zPos;

float pointSize(vec4 loc){
  
  float ptSize = length(loc);
  ptSize= (10000.0/ptSize);
  
  float particleScale=1.0;
  particleScale=gl_Vertex.w * particleMultiplier * gl_Position.w ;
  vec3 myNormal=gl_Normal;
  float sideNess= 1.0 / max(abs(myNormal.z),0.00);
  if (particleScale>0.0)
      particleScale+= particleAngleScale * sideNess* sideNess;  
  ptSize*=particleScale;
  
  return ptSize; 
}



void main()
{ 

  lightPos=vec3(25.0* sin(time * 0.001) * 1.0,1.0,1.0);
  lightPos=vec3(1.0,1.0,1.0); 

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;
 
  myVertex.z+=sin( cos(myVertex.y * 0.5+ time * 0.00051) + myVertex.x + time * 0.0051);
 
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize(gl_ModelViewMatrix * myVertex);
  vec3 myNormal=gl_Normal;
  myNormal+=sin( cos(myVertex.y * 0.5+ time * 0.00051) + myVertex.x + time * 0.0051);
  vec4 V = gl_ModelViewMatrix * myVertex;
  N = gl_NormalMatrix * myNormal;
  L = gl_LightSource[0].position.xyz - V.xyz;
  L = lightPos - V.xyz;


  gl_FrontColor=clamp(gl_Color, 0.0, 1.0);

  zPos=gl_Position.z;
}





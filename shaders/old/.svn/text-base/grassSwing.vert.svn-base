varying vec3 N, L;
uniform float time;
varying float zPos;

float pointSize(vec4 loc)
{
float ptSize = length(loc);
return (10000.0/ptSize);
}



void main()
{
gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
gl_FrontColor=gl_Color;
float particleScale=1.0;

particleScale=gl_Vertex.w*1.5;

//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;

myVertex.x +=0.5* sin(time* 0.0006 + gl_Vertex.x + gl_Vertex.z)* gl_Vertex.y;
myVertex.z +=0.0* cos(time* 0.0004 + gl_Vertex.z + gl_Vertex.y)* gl_Vertex.y;

gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
gl_PointSize= pointSize(gl_Position)*particleScale;


  vec4 V = gl_ModelViewMatrix * myVertex;
  N = gl_NormalMatrix * vec3(0.0,0.0,1.0);
  N.x +=0.5* sin(time* 0.0006 + gl_Vertex.x + gl_Vertex.z)* gl_Vertex.y;
  N.z +=0.2* cos(time* 0.0004 + gl_Vertex.z + gl_Vertex.y)* gl_Vertex.y;

  L = gl_LightSource[0].position.xyz - V.xyz;
    
  gl_FrontColor=clamp(gl_Color, 0.0, 1.0);

  zPos=gl_Position.z;
}





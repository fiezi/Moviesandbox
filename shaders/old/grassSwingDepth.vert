uniform float time;
varying float zPos;

float pointSize(vec4 loc)
{
float ptSize = length(loc);
return (10000.0/ptSize);
}



void main()
{
gl_FrontColor=gl_Color;
float particleScale=1.0;

particleScale=gl_Vertex.w*1.0;

//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;

myVertex.x +=0.5* sin(time* 0.0006 + gl_Vertex.x + gl_Vertex.z)* gl_Vertex.y;
myVertex.z +=0.0* cos(time* 0.0004 + gl_Vertex.z + gl_Vertex.y)* gl_Vertex.y;

gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
gl_PointSize= pointSize(gl_Position)*particleScale;

zPos=gl_Position.z;
}





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

particleScale=gl_Vertex.w*3.0;

//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;

gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
gl_PointSize= pointSize(gl_Position)*particleScale;
zPos=gl_Position.z;
}

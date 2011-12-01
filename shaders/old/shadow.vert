varying vec4 sCoord;
uniform float time;

float pointSize(vec4 loc)
{
float ptSize = length(loc);
return (10000.0/ptSize);
}


void main()
{
gl_FrontColor=gl_Color;

//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;

gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;


vec4 texCoord = gl_TextureMatrix[1] * gl_ModelViewMatrix * myVertex;
sCoord=texCoord;
gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

float particleScale=gl_Vertex.w * 1.0;
gl_PointSize= pointSize(gl_Position)*particleScale;
}

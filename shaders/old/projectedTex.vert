uniform sampler2D tex;
uniform float time;

float pointSize(vec4 loc)
{
float ptSize = length(loc);
return (10000.0/ptSize);
}



void main()
{
//standard texture stuff
float particleScale=1.0;

particleScale=gl_Vertex.w*1.5;

//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;

float factor=1.0;
vec4 texCoordOffset = gl_TextureMatrix[0] * vec4(myVertex.x*factor, myVertex.y*factor,0.0,1.0);
//color lookup in texture per vertex
vec4 color = texture2D(tex,(gl_MultiTexCoord0.st * 0.015)+texCoordOffset.xy);

gl_FrontColor=color;

gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
gl_PointSize= pointSize(gl_Position)*particleScale;

gl_PointSize=gl_PointSize*color.a;
}





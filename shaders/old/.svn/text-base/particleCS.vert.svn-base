uniform float time;

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

particleScale=gl_Vertex.w*0.15;

//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;

myVertex.y +=.025*sin(time*.00025*cos(.00001*time+gl_Vertex.z));
myVertex.z +=.05*sin(time*.0002+gl_Vertex.y);
myVertex.x +=.02*sin(time*.0002*sin(gl_Vertex.z));

gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
gl_PointSize= pointSize(gl_Position)*particleScale;
}





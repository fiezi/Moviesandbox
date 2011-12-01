uniform float time;
uniform vec4 camPos;

float pSize=32.0;
float pSizeMax=1024.0;

int particleLine=0;

void main()
{
//distance attenuation as seen in point sprites...
//derivedSize = clamp (size * sqrt (1 / (a + b * d + c * d^2)))

float dist = distance (camPos, gl_Vertex);
gl_PointSize=clamp(pSize * sqrt(1.0/(1.0+0.0*dist+0.01*pow(dist,2.0))),0.0,pSizeMax);


gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
} 

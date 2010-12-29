uniform float time;
uniform vec4 click;
uniform vec4 iniPos;
uniform vec4 camPos;

varying vec4 clickColor;

float pSize=32.0;
float pSizeMax=1024.0;

void main()
{
vec4 v = vec4(gl_Vertex);	

	if (distance(gl_Vertex+iniPos,click)<5.0)
	  {
      clickColor.r=1.0;
      v.y = sin(v.x+time*0.01);
      }

//distance attenuation as seen in point sprites...
//derivedSize = clamp (size * sqrt (1 / (a + b * d + c * d^2)))

float dist = distance (camPos, gl_Vertex);
gl_PointSize=clamp(pSize * sqrt(1.0/(1.0+0.0*dist+0.01*pow(dist,2.0))),0.0,pSizeMax);


gl_Position = gl_ModelViewProjectionMatrix * v;
} 

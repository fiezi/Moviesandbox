varying vec2 texCoord;
uniform float time;
float pSize=1024.0;

float pointSize(vec4 loc)
{
float len = length(loc);
float myPSize=pSize/len;

if (myPSize<30.0)
  myPSize=30.0;
return (myPSize);
}

void main(void) {
	gl_Position = ftransform();
	texCoord=gl_MultiTexCoord0.xy;
	gl_FrontColor = gl_Color;
}
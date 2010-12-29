varying vec3 normal, lightDir;
varying float zPos;
varying float yPos;
varying float vertTime;
varying float vertBirth;

uniform sampler2D tex;

void ammi()
{
vec4 yColor,fullColor,verlauf;
int bias=0;

yColor = vec4(0.3,0.01,1.0,1.0);
verlauf = vec4(0.3,0.2,0.1,1.0);
fullColor = vec4(1.0,1.0,1.0,1.0);

bias=int(gl_FragCoord.z*10.0);
if (int(gl_FragCoord.y*10.0)%bias< bias/4 && int(gl_FragCoord.x*10.0)%bias < bias/4)
  gl_FragColor=yColor*yPos/7.0 + (fullColor-verlauf*gl_FragCoord.y/150.0);
else 
  gl_FragColor=yColor*yPos/9.0 + (fullColor-verlauf*gl_FragCoord.y/200.0);

gl_FragColor.a=((gl_FragCoord.y/200.0)*0.1*vertTime);
}

void main()
{
ammi();
}


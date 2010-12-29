varying vec3 normal, lightDir;

varying float zPos;
varying float yPos;
varying float vertTime;
varying float vertBirth;

uniform sampler2D tex;



void ammi()
{
vec4 zColor, yColor, fullColor;

vec4 light=vec4(lightDir.x,lightDir.y,lightDir.z,1.0);
yColor = vec4(0.3,0.3,0.5,1.0);
zColor = vec4(0.1,0.2,0.3,1.0);
fullColor = vec4(1.0,1.0,1.0,1.0);

gl_FragColor=zColor;

if (normal.x+gl_FragCoord.y/20<2)
  gl_FragColor=yColor;

if (int(gl_FragCoord.y)%12>3)
  gl_FragColor=yColor;

if (normal.x+gl_FragCoord.y/20>2 && int(gl_FragCoord.y)%12<3)
  gl_FragColor=zColor;

if (normal.x+gl_FragCoord.y/20<2 && int(gl_FragCoord.y)%12>3)
  gl_FragColor=zColor;

gl_FragColor.a=1.0;
}

void main()
{
ammi();
}

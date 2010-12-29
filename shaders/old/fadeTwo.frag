varying vec3 normal, lightDir;
varying float zPos;
varying float yPos;
varying float vertTime;
varying float vertBirth;

uniform sampler2D tex;

void ammi()
{
vec4 yColor,fullColor,verlauf;
yColor = vec4(0.0,0.1,0.3,1.0);
verlauf = vec4(0.0,0.1,0.1,1.0);

//if (int(gl_FragCoord.x)%2< 1)// && int(gl_FragCoord.z)%6 < 0)
if (int(gl_FragCoord.y)%4< 2 && int(gl_FragCoord.x)%6 < 3)
  fullColor=yColor*(yPos+6.0) + (verlauf*(gl_FragCoord.y+150.0)/100.0); 
else
  fullColor=yColor*(yPos+2.0) + (verlauf*(gl_FragCoord.y+150.0)/100.0); 

gl_FragColor=fullColor;

gl_FragColor.a=((gl_FragCoord.y/200.0)*(vertTime+gl_FragCoord.y/100.0-7));
}

void main()
{
ammi();
}

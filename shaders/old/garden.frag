varying vec3 normal, lightDir;
varying float zPos;
varying float yPos;
uniform sampler2D tex;



void ammi()
{
vec4 zColor, yColor,fullColor,verlauf;
zColor = vec4(0.5,0.5,0.5,1.0);
yColor = vec4(0.0,0.5,0.1,1.0);
verlauf = vec4(0.0,0.3,0.1,1.0);

//if (int(gl_FragCoord.x)%2< 1)// && int(gl_FragCoord.z)%6 < 0)
if (int(gl_FragCoord.y)%4< 2 && int(gl_FragCoord.x)%6 < 3)
  fullColor=yColor*(yPos+6.0) + (verlauf*(gl_FragCoord.y+150.0)/100.0); 
else
  fullColor=yColor*(yPos+2.0) + (verlauf*(gl_FragCoord.y+150.0)/100.0); 
gl_FragColor=fullColor;
}

void main()
{
ammi();
}

varying float zPos;
varying float yPos;

void ammi()
{
vec4 yColor = vec4(0.0,0.8,0.6,1.0);
vec4 verlauf = vec4(0.0,0.3,0.5,1.0);
vec4 fullColor = vec4(1.0,1.0,1.0,1.0);

int bias=0;

bias=int(gl_FragCoord.z*10.0);

if (int(gl_FragCoord.y*10.0)%bias< bias/4 && int(gl_FragCoord.x*10.0)%bias < bias/4)
  gl_FragColor=yColor*yPos/7.0 + (fullColor-verlauf*gl_FragCoord.y/150.0);
else 
  gl_FragColor=yColor*yPos/9.0 + (fullColor-verlauf*gl_FragCoord.y/200.0);

gl_FragColor.a=1.0;
}

void main()
{
ammi();
}



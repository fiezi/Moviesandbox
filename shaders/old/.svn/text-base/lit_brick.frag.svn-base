varying vec3 normal, lightDir;

void main()
{
	float intensity;
	vec3 n;
	vec4 lightColor;
	vec4 brickColor;
	
	n = normalize(normal);
	intensity = max(dot(lightDir,n),0.0); 
	lightColor = vec4(intensity,intensity,intensity,1.0);
	
	if (sin(intensity)>0.8)
	  lightColor = vec4(1,1,1,1.0);	
	
	brickColor=vec4(1,0,0,1);

	if (mod(gl_FragCoord[1],5)==0.5)
          brickColor=vec4(1,1,1,1);
	if (mod(gl_FragCoord[0],20)==0.5)
	  brickColor=vec4(1,1,1,1);
	
	gl_FragColor = vec4(lightColor * brickColor);
}

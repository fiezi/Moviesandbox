varying vec3 normal, lightDir;

uniform sampler2D tex;

void main()
{
	float intensity;
	vec3 n;
	vec4 lightColor;
	vec4 texColor;

	n = normalize(normal);
	intensity = max(dot(lightDir,n),0.0); 
	lightColor = vec4(intensity,intensity,intensity,1.0);
	
	if (sin(intensity)>0.8)
	  lightColor = vec4(1,1,1,1.0);	
	
	texColor = texture2D(tex,gl_TexCoord[0].st);
	gl_FragColor = vec4(lightColor * texColor);

}

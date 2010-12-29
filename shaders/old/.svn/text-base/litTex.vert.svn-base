varying vec3 normal, lightDir;

uniform float time;


void main()
{

	lightDir=normalize(vec3(gl_LightSource[0].position));
	normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_TexCoord[0] = gl_MultiTexCoord0;	
	

	gl_Position = ftransform();
	//gl_Position[0] = sin(time);
}
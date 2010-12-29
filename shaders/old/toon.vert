// simple toon vertex shader
// www.lighthouse3d.com


attribute vec4 gl_Color;
varying vec3 normal, lightDir;
varying float i;

void main()
{	
	gl_Position = ftransform();

	i=gl_Color[0];
	gl_Position[0] += sin(gl_Vertex[0]+i);
	gl_Position[1] += sin(gl_Vertex[1]+i);
	gl_Position[2] += sin(gl_Vertex[2]+i);
	gl_Position[3] += sin(gl_Vertex[3]+i);

	lightDir = normalize(vec3(gl_LightSource[0].position));
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_FrontColor = gl_Color;

}

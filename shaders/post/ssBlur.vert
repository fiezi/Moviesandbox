uniform float time;
uniform float screenX;


varying vec2 texCoord;

//light position stuff


void main(){

	gl_Position = ftransform();
	texCoord=gl_MultiTexCoord0.xy;
	gl_FrontColor = gl_Color;

}

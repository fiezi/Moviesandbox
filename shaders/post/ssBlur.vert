uniform float time;

uniform float nearClip;
uniform float farClip;

uniform mat4 cameraMatrix;
uniform mat4 cameraInverse;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

varying vec2 texCoord;

//light position stuff


void main(){

	gl_Position = ftransform();
	texCoord=gl_MultiTexCoord0.xy;
	gl_FrontColor = gl_Color;

}

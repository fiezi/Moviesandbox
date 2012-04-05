uniform float time;

uniform float nearClip;
uniform float farClip;

uniform mat4 projectionMatrix;
uniform mat4 projectionInverse;
uniform mat4 cameraMatrix;
uniform mat4 cameraInverse;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

varying vec2 texCoord;

//light position stuff
varying vec3 lightColor;
varying vec4 lightPos;
varying mat4 lightSpaceMat;

void main(){

	gl_Position = ftransform();
	texCoord=gl_MultiTexCoord0.xy;

    lightSpaceMat= lightProjectionMatrix * lightViewMatrix *  cameraInverse ;
   lightColor= gl_LightSource[0].diffuse.rgb;

    //light in world space
    lightPos=gl_LightSource[0].position;
    lightPos.w=1.0;
    //light in Eye Space
    lightPos = cameraMatrix * lightPos;

    float lightZScreen=farClip/ (farClip - lightPos.z * (farClip- nearClip));
    lightPos.z= 1.0/lightZScreen;



	gl_FrontColor = gl_Color;

}

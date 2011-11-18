uniform float time;

uniform float nearClip;
uniform float farClip;

uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;

varying vec2 texCoord;

//light position stuff

varying vec3 lightColor;
varying vec4 lightPos;
varying float lightZScreen;

void main(){

	gl_Position = ftransform();
	texCoord=gl_MultiTexCoord0.xy;


   lightColor= gl_LightSource[0].diffuse.rgb;

    //light in world space
    lightPos=gl_LightSource[0].position;
    lightPos.w=1.0;
    //light in Eye Space
    lightPos = cameraMatrix * lightPos;

    lightZScreen=farClip/ (farClip - lightPos.z * (farClip- nearClip));

    lightPos.xy/=lightPos.z;
    lightPos/=(lightZScreen);

	gl_FrontColor = gl_Color;

}

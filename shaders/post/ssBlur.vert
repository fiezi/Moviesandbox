uniform float time;
uniform float screenX;


varying vec2 texCoord;

//light position stuff


void main(){

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;	gl_FrontColor = gl_Color;
	gl_Position = ftransform();

}

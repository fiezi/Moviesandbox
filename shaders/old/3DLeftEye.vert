uniform float time;

void main()
{
//standard texture stuff
gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
gl_FrontColor=gl_Color;
gl_Position = ftransform();
}




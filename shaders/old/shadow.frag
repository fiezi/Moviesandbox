uniform sampler2D tex;
uniform sampler2DShadow shadowTex;
varying vec4 sCoord;

void main()
{


vec4 pColor= shadow2DProj(shadowTex, sCoord).r * gl_Color;

//if we don't have a shadow linked, set to black!
pColor.a=1.0;

vec4 color = texture2D(tex,gl_TexCoord[0].st);
gl_FragColor =pColor * color;
}

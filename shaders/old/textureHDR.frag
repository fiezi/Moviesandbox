uniform sampler2D tex;

void main()
{
vec4 color = texture2D(tex,gl_TexCoord[0].st);
gl_FragColor.rgb=color / (color.rgb + 1.0);
gl_FragColor.a=1.0;
}

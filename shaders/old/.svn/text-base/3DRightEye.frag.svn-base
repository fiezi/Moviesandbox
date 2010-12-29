uniform sampler2D tex;

void main()
{
vec4 color = texture2D(tex,gl_TexCoord[0].st);
gl_FragColor=gl_Color * color * vec4(1.0 , 0.0 , 0.0 , 1.0);
//gl_FragDepth=gl_FragCoord.z+(1.0-gl_FragColor.a);
}



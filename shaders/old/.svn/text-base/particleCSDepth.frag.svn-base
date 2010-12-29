uniform sampler2D tex;
varying float zPos;

void main()
{
vec4 color = texture2D(tex,gl_TexCoord[0].st);
gl_FragColor=gl_Color * color;
gl_FragDepth=gl_FragCoord.z+(1.0-gl_FragColor.a);
gl_FragColor=vec4(zPos ,zPos , zPos,gl_FragColor.a);
}



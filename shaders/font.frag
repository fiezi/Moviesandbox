uniform sampler2D tex;
varying float zPos;

void main(){

    vec2 texCoords =gl_TexCoord[0].st;
    vec4 color = texture2D(tex,gl_TexCoord[0].st);
    texCoords.s-=0.04;
    texCoords.t-=0.04;

    vec4 offsetColor = texture2D(tex,texCoords.st);
    gl_FragData[0]=vec4(0.3 ,0.3 ,0.3 ,offsetColor.a*1.0);
    gl_FragData[0]+=gl_Color * vec4(color.a, color.a, color.a, color.a);
    gl_FragData[1]=vec4(zPos ,zPos , zPos,1.0);

}



uniform sampler2D tex;
uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;

varying vec3 N;
varying float backSide;

varying float zPos;
varying vec4 picking;


void main(){

    vec4 color = texture2D(tex,gl_TexCoord[0].st);

    //black if not facing light
    gl_FragData[0]=color * gl_Color;//* postColor;

    if (gl_FragData[0].a<0.5)
        gl_FragData[0].r=1.0;

    //transparency...
    if (gl_FragData[0].a < 0.1){
        discard;
    }
    else{
        gl_FragDepth=gl_FragCoord.z;
        //gl_FragData[1]=vec4(N.x ,N.y ,N.z, zPos );
        //gl_FragData[1]=vec4(dFdx(gl_FragCoord.z)*65536.0 ,dFdy(gl_FragCoord.z)*65536.0 , 0.0,zPos);
    }

    gl_FragData[1]=vec4( zPos, objectID, 0.0, 0.0 );
}

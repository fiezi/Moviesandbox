uniform sampler2D tex;
uniform vec4 postColor;
uniform bool bComputeLight;
uniform int bSelected;
uniform float objectID;

varying vec3 N;

varying float zPos;
varying vec4 picking;


void main(){

    vec4 color = texture2D(tex,gl_TexCoord[0].st);

    gl_FragData[0]=color * gl_Color;//* postColor;

    if (bSelected==1){
        gl_FragData[0]*=0.5;
        //don't brighten if transparent!
        if (gl_FragData[0].a>0.1)
            gl_FragData[0]+=vec4(0.5,0.0,0.0,0.5);

    }

    //transparency...
    if (gl_FragData[0].a < 0.1){
        discard;
    }
    else{
        gl_FragDepth=gl_FragCoord.z;
        gl_FragData[1]=vec4(N.x ,N.y ,N.z, zPos );
    }

    if (!bComputeLight)
        gl_FragData[1]=vec4(-100.0 ,0.0 ,0.0, zPos );

    gl_FragData[2]=picking;

    gl_FragData[3]=vec4(0.0,0.0,0.0,1.0);

}

uniform sampler2D tex;
//uniform sampler2D displacementTex;
//uniform sampler2D normalTex;


uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;

varying vec3 N;

varying float zPos;
varying vec4 picking;


void main(){

    gl_FragData[0]=gl_Color * postColor;
    //gl_FragData[0]=vec4(1.0);
    gl_FragData[0].a=1.0;

    //transparency...
    if (gl_FragData[0].a < 0.1){
        gl_FragDepth=1.0;
        gl_FragData[1]=vec4(65536.0);
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

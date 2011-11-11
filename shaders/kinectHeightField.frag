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

    gl_FragData[0]=vec4(0.5,0.5,0.5,1.0);
    gl_FragData[0].a=1.0;

    gl_FragData[1]=vec4( zPos, objectID, 0.0, 0.0 );

}

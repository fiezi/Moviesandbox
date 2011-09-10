uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;
varying float zPos;
varying float vID;

void main(){

    gl_FragData[0]=gl_Color;
    int vIDOneInt=int(vID/65536.0);
    float vIDOne=float(vIDOneInt);

    float vIDTwo=mod(vID,65536.0);

    gl_FragData[1]=vec4( zPos, objectID, 0.0, 0.0 );

}



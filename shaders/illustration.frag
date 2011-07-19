uniform vec4 postColor;
uniform float objectID;
uniform bool bComputeLight;

varying float zPos;
varying float vID;


/*
*   Main
*/

void main(){

    gl_FragData[0]=gl_Color * postColor;

/*
    if (!bComputeLight)
        gl_FragData[1]=vec4(-100.0 ,0.0 ,0.0, zPos );
*/

   //per-vertex ID
        int vIDOneInt=int(vID/65536.0);
        float vIDOne=float(vIDOneInt);

        float vIDTwo=mod(vID,65536.0);

    gl_FragData[1]=vec4(zPos ,objectID ,0.0,0.0 );
}



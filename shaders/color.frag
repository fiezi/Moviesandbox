uniform vec4 postColor;
uniform bool bComputeLight;

varying vec3 N;
varying float zPos;
varying vec4 picking;
varying float vID;
varying vec3 smudge;


/*
*   Main
*/

void main(){

    gl_FragData[0]=gl_Color;// * postColor;
    //gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

	gl_FragData[1]=vec4(N.x ,N.y , N.z,zPos);

    if (!bComputeLight)
        gl_FragData[1]=vec4(-100.0 ,0.0 ,0.0, zPos );

	gl_FragData[2]=picking;


   //per-vertex ID
        int vIDOneInt=int(vID/65536.0);
        float vIDOne=float(vIDOneInt);

        float vIDTwo=mod(vID,65536.0);

    gl_FragData[3]=vec4(smudge.x,smudge.y,vIDOne,vIDTwo);

}



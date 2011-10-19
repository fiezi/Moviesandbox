uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;

varying float zPos;
varying vec4 picking;
varying float vID;



/*
*   Main
*/

void main(){

    gl_FragData[0]=gl_Color;// * postColor;
    //gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

/*
    if (!bComputeLight)
        gl_FragData[1]=vec4(-100.0 ,0.0 ,0.0, zPos );
*/
   //per-vertex ID
        int vIDOneInt=int(vID/65536.0);
        float vIDOne=float(vIDOneInt);

        float vIDTwo=mod(vID,65536.0);

//    gl_FragData[1]=vec4(zPos/256.0 ,mod(zPos,256.0),objectID, 0.0 );
    gl_FragData[1]=vec4( zPos, objectID, 0.0, 0.0 );

    //front
    if (gl_Color.r==1.0 && gl_Color.g==0.0 && gl_Color.b==0.0)
        gl_FragData[0]=vec4(0.85,0.85,0.85,1.0);

    //right
    if (gl_Color.r==0.0 && gl_Color.g==1.0 && gl_Color.b==0.0)
        gl_FragData[0]=vec4(0.75,0.75,0.75,2.0);
    //top
    if (gl_Color.r==0.0 && gl_Color.g==0.0 && gl_Color.b==1.0)
        gl_FragData[0]=vec4(0.7,0.7,0.7,3.0);

    //bottom
    if (gl_Color.r==0.0 && gl_Color.g==1.0 && gl_Color.b==1.0)
        gl_FragData[0]=vec4(0.65,0.65,0.65,4.0);

    //left
    if (gl_Color.r==1.0 && gl_Color.g==1.0 && gl_Color.b==0.0)
        gl_FragData[0]=vec4(0.75,0.75,0.75,5.0);

    //back
    if (gl_Color.r==1.0 && gl_Color.g==0.0 && gl_Color.b==1.0)
        gl_FragData[0]=vec4(0.8,0.8,0.8,6.0);

}



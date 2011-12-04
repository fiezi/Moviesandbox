uniform float objectID;

/*
*   Main
*/

void main(){

    ///we're not doing any 16bit to 8bit conversion here, as we only need 6 values for our object ID and no Z - info

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
        gl_FragData[0]=vec4(0.55,0.55,0.55,5.0);

    //back
    if (gl_Color.r==1.0 && gl_Color.g==0.0 && gl_Color.b==1.0)
        gl_FragData[0]=vec4(0.9,0.9,0.9,6.0);

}






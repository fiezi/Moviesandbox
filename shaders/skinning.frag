varying vec4 picking;
varying float zPos;
varying float vID;

void main(){

    gl_FragData[0]=gl_Color;
    gl_FragData[1]=vec4(1.0,1.0,1.0,zPos);
    gl_FragData[2]=picking;

    //per-vertex ID
        int vIDOneInt=int(vID/65536.0);
        float vIDOne=float(vIDOneInt);

        float vIDTwo=mod(vID,65536.0);
		gl_FragData[3]=vec4(0.0,0.0,vIDOne,vIDTwo);

}



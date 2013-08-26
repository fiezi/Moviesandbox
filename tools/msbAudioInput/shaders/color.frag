uniform vec4 postColor;
uniform int bComputeLight;
uniform int bSelected;

varying vec3 N;
varying float zPos;
varying vec4 picking;
varying float vID;



/*
*   Main
*/

void main(){

    gl_FragData[0]=gl_Color;
    //gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

    if (bSelected==1){
        gl_FragData[0]*=0.5;
        gl_FragData[0]+=vec4(0.5,0.0,0.0,0.5);
    }


	gl_FragData[1]=vec4(N.x ,N.y , N.z,zPos);
	gl_FragData[2]=picking;
    gl_FragData[3]=vec4(0.0,0.0,0.0,1.0);

}



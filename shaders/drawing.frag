uniform vec4 postColor;
uniform float objectID;
uniform bool bComputeLight;
uniform float screensize;

varying float zPos;
varying vec4 pixelPos;
varying float pSize;

varying float vID;

float PI = 3.14159265358979323846264;


/*
*   Main
*/

void main(){

	vec2 myPixelPos=pixelPos.xy * screensize;
	vec2 pixelDist=myPixelPos-gl_FragCoord.xy;
	pixelDist=pixelDist/(pSize);

	float myDist=length(pixelDist * 2.0);

	if (myDist>1.0){
		discard;
		}
	else{
		gl_FragDepth=gl_FragCoord.z;
	}
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



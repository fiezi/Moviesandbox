uniform sampler2D sceneTex;

uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;
varying float zPos;
varying float pSize;
varying vec4 pixelPos;

varying float vID;

float PI = 3.14159265358979323846264;

/*
*   Main
*/

void main(){

    gl_FragData[0]=gl_Color;

/*
    if (!bComputeLight)
        gl_FragData[1]=vec4(-100.0 ,0.0 ,0.0, zPos );
*/

/*
	vec2 myPixelPos=pixelPos.xy * 768.0;
	vec2 pixelDist=myPixelPos-gl_FragCoord.xy;
	pixelDist=pixelDist/(pSize);

	float myDist=length(pixelDist * 2.0);


	if (myDist>1.0){
		gl_FragDepth=1.0;
		}
	else{
		gl_FragDepth=gl_FragCoord.z;
	}

    gl_FragData[0]=gl_Color;


*/
    gl_FragData[1]=vec4(zPos ,objectID ,0.0,0.0 );
}


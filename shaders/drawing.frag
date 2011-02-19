uniform vec4 postColor;
uniform int bComputeLight;
uniform int bSelected;

varying vec3 N;
varying vec3 smudge;
varying float zPos;
varying vec4 picking;
varying vec4 pixelPos;
varying float pSize;
varying float bTubeNormal;

varying float vID;

float PI = 3.14159265358979323846264;


/*
*   Main
*/

void main(){

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


    if (bSelected==1){
        gl_FragData[0]*=0.5;
        gl_FragData[0]+=vec4(0.5,0.0,0.0,0.5);
    }

	//Normal calculation, as in a tube...
	//normal on top of our tube points up -> is our N
	//normal to the right points to N cross smudge
	//normal to the left points to negative N cross smudge
	//normal downwards points negative N

	//also: depending on smudge going in or out, it's all different!


	if (bTubeNormal>0.5){
		vec3 biNormal= smudge;
		vec3 NcrossS = cross( biNormal, N );

		vec3 tubeNormal=(sin(pixelDist.x * PI/4.0) *  -NcrossS + sin(pixelDist.y * PI/4.0) * -N);
		gl_FragData[1]=vec4(tubeNormal.x ,tubeNormal.y , tubeNormal.z,zPos);
		}
	else
		gl_FragData[1]=vec4(N.x ,N.y , N.z,zPos);

	gl_FragData[2]=picking;

    //per-vertex ID
        int vIDOneInt=int(vID/65536.0);
        float vIDOne=float(vIDOneInt);

        float vIDTwo=mod(vID,65536.0);
		gl_FragData[3]=vec4(smudge.x,smudge.y,vIDOne,vIDTwo);

}



uniform sampler2D sceneTex;

uniform vec4 postColor;
uniform int bSelected;
uniform bool bComputeLight;
varying vec3 N;
varying float zPos;
varying vec4 picking;
varying vec3 smudge;
varying float pSize;
varying vec4 pixelPos;
varying float bTubeNormal;

varying float vID;

float PI = 3.14159265358979323846264;

/*
*   Main
*/

void main(){

    gl_FragData[0]= gl_Color * postColor;
    //gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

    if (bSelected==1){
        gl_FragData[0]*=0.5;
        gl_FragData[0]+=vec4(0.5,0.0,0.0,0.5);
    }
	gl_FragData[1]=vec4(N.x ,N.y , N.z,zPos);

    if (!bComputeLight)
        gl_FragData[1]=vec4(-100.0 ,0.0 ,0.0, zPos );

    if (mod(gl_FragCoord.x,10.0)>5.0 + 2.0*sin(zPos) && mod(gl_FragCoord.y,8.0)>4.0+ 2.0*sin(zPos)){
        gl_FragData[1].y=0.0;
        //gl_FragData[0].xyz=gl_FragData[0].xyz * gl_FragData[0].xyz;
    }

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
/*
	vec3 biNormal= smudge;
	vec3 NcrossS = cross( -N, biNormal );

	float flip = 1.0;

	if (NcrossS.x>0.0)
			flip = -1.0;

	vec3 tubeNormal= flip * (sin(pixelDist.x * PI/4.0) * NcrossS + sin(pixelDist.y * PI/4.0)  * -N);

	//gl_FragData[0].rgb=tubeNormal;
	if (bTubeNormal>0.0)
		gl_FragData[1]=vec4(tubeNormal.x ,tubeNormal.y , tubeNormal.z,zPos);
	else {
		gl_FragData[1]=vec4(1.0 ,0.0 , 0.0,zPos);
	}
*/

    gl_FragData[2]=picking;

    gl_FragData[3]=vec4(smudge.x,smudge.y,0.0,vID);
    //gl_FragData[3]=vec4(0.0,0.0,0.0,vID);


}

